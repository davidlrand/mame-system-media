/* NSC 32000 ROM debugger.
 * Bruce Culbertson  Bob Krause
 *
 * Routines which setup the machine on boot, create tables.  Also
 * run and breakpoint command handlers.
 */

#include "debugger.h"
#include "dasm.h"

/* Linker seeks up to the next multiple of this, after text, before
 * writing data.
 */
#define DATARND		0x400

/* Where to put mod and interrupt tables.
 */
#define TABLE_START	0x20
#define MODTAB_ADR	TABLE_START
#define INTTAB_ADR	(MODTAB_ADR + 0x20)

/* Where interrupt and user stacks are initially.  (Minix continues
 * to use ISP.)
 */
#define ISP		0xc00
#define USP		0x1000

#define TRACE_VEC	9
#define TRACE_FLAG	2
#define FAKE_RET	11
#define PSR_USR		0x100
#define PSR_USP		0x200
#define UPSR		(PSR_USR | PSR_USP)	/* default user psr */

char *trap_string [] = {
  "nonvectored interrupt",
  "nonmaskable interrupt",
  "abort",
  "slave",
  "illegal instruction",
  "service call",
  "divide by 0",
  "flag",
  "breakpoint",
  "trace",
  "undefined instruction",
  "restartable bus error",
  "non-restartable bus error",
  "overflow",
  "debug",
  "return",
};

struct bkpt bkpt[NUM_SW_BKPT];

struct mod_entry {
  long sb;
  long link;
  long pc_base;
  long dummy;
};

#if STANDALONE
extern unsigned short cfg_init;

int	btext(),
	isr_nvi(),
	isr_nmi(),
	isr_abt(),
	isr_slv(),
	isr_ill(),
	isr_svc(),
	isr_dvz(),
	isr_flg(),
	isr_bpt(),
	isr_trc(),
	isr_und(),
	isr_rbe(),
	isr_nbe(),
	isr_ovf(),
	isr_dbg(),
	ret_save();
	
/* Table for initializing interrupt vector table.
 */
int ((*isrInitTab[])()) = {
	isr_nvi,	/* 0 */
	isr_nmi,
	isr_abt,
	isr_slv,
	isr_ill,	/* 4 */
	isr_svc,
	isr_dvz,
	isr_flg,
	isr_bpt,	/* 8 */
	isr_trc,
	isr_und,
	isr_rbe,
	isr_nbe,	/* 12 */
	isr_ovf,
	isr_dbg,
};
#define ISRTABSZ (sizeof (isrInitTab)/sizeof (int ((*)())))
#endif

/* Create a safe environment for user.
 */
init_machState()
{
#if STANDALONE
  modtab_init();
  inttab_init();
/* Put return addresses on stack in case user code falls through.
 */
  *((long *)(USP-4)) = (long)ret_save;
  *((long *)(ISP-4)) = (long)ret_save;
  machState.cfg = cfg_init;
#endif
  machState.sb = 0;
  machState.usp = USP - 4;
  machState.isp = ISP - 4;
  machState.intbase = INTTAB_ADR;
  machState.mod = MODTAB_ADR;
  /* machState.psr = UPSR; 0 is more convenient */
}

#if STANDALONE
/* Setup default mod table.
 */
modtab_init()
{
  struct mod_entry *p;

  p = (struct mod_entry *)MODTAB_ADR;
  p->sb = 0;
  p->pc_base = (long)btext;
}

/* Setup default interrupt table.
 */
inttab_init()
{
  int ((**fn)());
  long *q;

  q = (long *) INTTAB_ADR;
  for (fn = isrInitTab; fn < isrInitTab + ISRTABSZ; ++fn)
    *q++ = MODTAB_ADR | ((long)(*fn - btext)) << 16;
}
#endif

/* Single step one instrution.  Then set breakpoints and set user going.
 * On return from user, clear breakpoints and print reason for return.
 */
run(p)
char *p;
{
  int ret;
  long adr;

  ret = getIntScan (&p, &adr);
  if (ret == BAD_NUM) {
    myPrintf ("Bad address\n");
    return;
  } else if (ret == GOT_NUM) machState.pc = adr;
  machState.psr |= TRACE_FLAG;
  if ((ret = resume()) != TRACE_VEC) {
    machState.psr &= ~TRACE_FLAG;
    print_return_info (ret);
    return;
  }
  machState.psr &= ~TRACE_FLAG;
  setBreaks();
  ret = resume();
  clrBreaks();
  print_return_info (ret);
}

/* Execute specified number of instructions (default 1).
 */
single_step(p)
char *p;
{
  int ret, cnt;

  switch (getIntScan (&p, &cnt)) {
    case NO_NUM:
      cnt = 1;
      break;
    case BAD_NUM:
      printf ("Bad count\n");
      return;
    default:;
  }
  do {
    machState.psr |= TRACE_FLAG;
    ret = resume();
  } while (--cnt > 0 && ret == TRACE_VEC);
  machState.psr &= ~TRACE_FLAG;
  print_return_info (ret);
}

#define MAX_INSN_LEN	20
print_return_info (type)
int type;
{
  struct insn insn;
  char text [50], buf [MAX_INSN_LEN], *bufp;
  int i;
  long adr, translateVaddr(), getCurrentPtb(), ptb;

  ptb = getCurrentPtb();
  for (bufp = buf, i = 0; i < MAX_INSN_LEN; ++bufp, ++i) {
    if (GOT_NUM != translateVaddr (machState.pc + BASE + i, ptb, &adr)) {
      printf ("Bad virtual address\n");
      return;
    }
    *bufp = *((char *)adr);
  }
  initInsn (&insn);
  dasm_ns32k (&insn, buf);
  formatAsm (&insn, text);
  printf ("%6lx%c\t%s\t(%s)\n",
    machState.pc,
    (ptb == 0)? ' ': 'V',
    text,
    (type < 0 || type > FAKE_RET)?
    "unknown trap or interrupt": trap_string [type]);
}

#if STANDALONE
/* Zero out bss segment.
 */
extern char etext, bdata, edata, end;
zero_bss()
{
  register char *p;

  for (p = &edata; p < &end; ++p) *p = 0;
}

/* So code can be put in ROM, copy data segment from end of text
 * to actual data segment.
 */
copy_dataseg()
{
  char *src, *dst;

  /* linker starts data at next page after text so round up */
  src = (char *)((((long)&etext + DATARND - 1) / DATARND) * DATARND);
  for (dst = &bdata; dst < &edata;)
    *dst++ = *src++;
}
#endif

/* For all non-null breakpoints, save instruction at address and insert
 * breakpoint instruction.
 */
setBreaks()
{
  struct bkpt *p;

  for (p = bkpt; p < bkpt + NUM_SW_BKPT; ++p)
    if (p->adr != NULL) {
      p->insn = *(p->adr);
      *(p->adr) = INSN_BPT;
    }
}

/* For all non-null breakpoints, restore original instruction at address.
 */
clrBreaks()
{
  struct bkpt *p;

  for (p = bkpt; p < bkpt + NUM_SW_BKPT; ++p)
    if (p->adr != NULL)
      *(p->adr) = p->insn;
}

/* Initialize breakpoints to NULL.
 */
initBreaks()
{
  struct bkpt *p;

  for (p = bkpt; p < bkpt + NUM_SW_BKPT; ++p)
    p->adr = NULL;
}

#if !STANDALONE
resume()
{
}
#endif
