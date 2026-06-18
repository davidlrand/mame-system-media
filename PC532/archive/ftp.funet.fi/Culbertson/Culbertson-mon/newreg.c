/* NSC 32000 ROM debugger.
 * Bruce Culbertson  Bob Krause
 *
 * Register table.  Code to lookup and print registers.
 */

#include "debugger.h"
#include "machine.h"

#define CAPIFSET(val,bit,let) ((val & bit)? let-'a'+'A': let)

static long v1, v2;		/* handy user variables */

CONST
struct regTable regTable[] = {
  {"r0",	(char *)(&machState.r[7]), T_LONG},   /* General Register 0 */
  {"r1",	(char *)(&machState.r[6]), T_LONG},   /* General Register 1 */
  {"r2",	(char *)(&machState.r[5]), T_LONG},   /* General Register 2 */
  {"r3",	(char *)(&machState.r[4]), T_LONG},   /* General Register 3 */
  {"r4",	(char *)(&machState.r[3]), T_LONG},   /* General Register 4 */
  {"r5",	(char *)(&machState.r[2]), T_LONG},   /* General Register 5 */
  {"r6",	(char *)(&machState.r[1]), T_LONG},   /* General Register 6 */
  {"r7",	(char *)(&machState.r[0]), T_LONG},   /* General Register 7 */
  {"f0",	(char *)(&machState.f.l[0]), T_LONG}, /* Floating Point  0 */
  {"f1",	(char *)(&machState.f.l[1]), T_LONG}, /* Floating Point  1 */
  {"f2",	(char *)(&machState.f.l[2]), T_LONG}, /* Floating Point  2 */
  {"f3",	(char *)(&machState.f.l[3]), T_LONG}, /* Floating Point  3 */
  {"f4",	(char *)(&machState.f.l[4]), T_LONG}, /* Floating Point  4 */
  {"f5",	(char *)(&machState.f.l[5]), T_LONG}, /* Floating Point  5 */
  {"f6",	(char *)(&machState.f.l[6]), T_LONG}, /* Floating Point  6 */
  {"f7",	(char *)(&machState.f.l[7]), T_LONG}, /* Floating Point  7 */
  {"l1l",	(char *)(&machState.f.l[8]), T_LONG}, /* Floating Point L1 */
  {"l1h",	(char *)(&machState.f.l[9]), T_LONG}, /* Floating Point L1 */
  {"l3l",	(char *)(&machState.f.l[10]), T_LONG},/* Floating Point L3 */
  {"l3h",	(char *)(&machState.f.l[11]), T_LONG},/* Floating Point L3 */
  {"l5l",	(char *)(&machState.f.l[12]), T_LONG},/* Floating Point L5 */
  {"l5h",	(char *)(&machState.f.l[13]), T_LONG},/* Floating Point L5 */
  {"l7l",	(char *)(&machState.f.l[14]), T_LONG},/* Floating Point L7 */
  {"l7h",	(char *)(&machState.f.l[15]), T_LONG},/* Floating Point L7 */
  {"l0",        NULL, 0},
  {"l1",        NULL, 0},
  {"l2",        NULL, 0},
  {"l3",        NULL, 0},
  {"l4",        NULL, 0},
  {"l5",        NULL, 0},
  {"l6",        NULL, 0},
  {"l7",        NULL, 0},
  {"pc",	(char *)(&machState.pc), T_LONG},     /* Program counter */
  {"usp",	(char *)(&machState.usp), T_LONG},    /* 532 */
  {"isp",	(char *)(&machState.isp), T_LONG},    /* 532 */
  {"fp",	(char *)(&machState.fp), T_LONG},     /* Frame Pointer */
  {"sb",	(char *)(&machState.sb), T_LONG},     /* Static Base */
  {"intbase",	(char *)(&machState.intbase), T_LONG},/* Interrupt Base */
  {"mod",	(char *)(&machState.mod), T_SHORT},   /* Module Register */
  {"psr",	(char *)(&machState.psr), T_SHORT|T_PSR}, /* Processor Status */
  {"upsr",	NULL, 0},                             /* Processor Status */
  {"dcr",	(char *)(&machState.dcr), T_LONG},    /* 532 */
  {"dsr",	(char *)(&machState.dsr), T_LONG},    /* 532 */
  {"car",	(char *)(&machState.car), T_LONG},    /* 532 */
  {"bpc",	(char *)(&machState.bpc), T_LONG},    /* 532 */
  {"cfg",	(char *)(&machState.cfg), T_LONG},    /* 532 */
  {"ptb0",	(char *)(&machState.ptb0), T_LONG},   /* Page Table Base 0 */
  {"ptb1",	(char *)(&machState.ptb1), T_LONG},   /* Page Table Base 1 */
  {"ivar0",	NULL, 0},			      /* 532 */
  {"ivar1",	NULL, 0},			      /* 532 */
  {"tear",	(char *)(&machState.tear), T_LONG},   /* 532 */
  {"mcr",	(char *)(&machState.mcr), T_LONG},    /* 532 */
  {"msr",	(char *)(&machState.msr), T_LONG},    /* Memory Management Status */
  {"fsr",	(char *)(&machState.fsr), T_LONG},    /* Floating Point Status */
  {"sp",	NULL, 0},                             /* x(x(sp)) adr mode */
  {"???",       NULL, 0},			      /* unknown reg */
  {".",		(char *)(&Dot), T_LONG},
  {"v1",	(char *)(&v1), T_LONG},		      /* user variable */
  {"v2",	(char *)(&v2), T_LONG},		      /* user variable */
  {"bkpt0",	(char *)(&bkpt[0].adr), T_LONG},      /* software breakpoint */
  {"bkpt1",	(char *)(&bkpt[1].adr), T_LONG},      /* software breakpoint */
  {"bkpt2",	(char *)(&bkpt[2].adr), T_LONG},      /* software breakpoint */
  {"bkpt3",	(char *)(&bkpt[3].adr), T_LONG},      /* software breakpoint */
  {"bkpt4",	(char *)(&bkpt[4].adr), T_LONG},      /* software breakpoint */
  {"bkpt5",	(char *)(&bkpt[5].adr), T_LONG},      /* software breakpoint */
  {"bkpt6",	(char *)(&bkpt[6].adr), T_LONG},      /* software breakpoint */
  {"bkpt7",	(char *)(&bkpt[7].adr), T_LONG},      /* software breakpoint */
  {"radix",	(char *)(&defaultBase), T_LONG|T_DECI},   /* radix */
  {"debug",	(char *)(&debug), T_LONG|T_DECI},         /* debug flag */
  {"scrlen",	(char *)(&screenLength), T_LONG|T_DECI},  /* screen length */
# if !STANDALONE
  {"offset",	(char *)(&fileBase), T_LONG},         /* file buffer */
#else
  {"scsi_adr",	(char *)(&scsiAdr), T_LONG},	      /* default SCSI bus adr */
  {"scsi_lun",	(char *)(&scsiLun), T_LONG},	      /* default SCSI bus adr */
# endif
};

#define REGTABLESZ ((sizeof regTable) / (sizeof (struct regTable)))

/* Search regTable for 1) a register matching what the user typed or 2)
 * a unique register which is a superstring of what the user typed.
 */
CONST struct regTable *
find_reg (p)
char *p;
{
  CONST struct regTable *q, *substr;
  int multisub = 0;

  for (q = regTable; q < regTable + REGTABLESZ; ++q)
    switch (myStrCmp (p, q->name)) {
      case CMP_MATCH:
        return q;
      case CMP_SUBSTR:
        ++multisub;
        substr = q;
        break;
      case CMP_NOMATCH:
        break;
    }
  return (multisub == 1)? substr: NULL;
}

/* Scan a register.
 */
scanreg (p, regbuf)
char **p, *regbuf;
{
  while (REGCHAR (**p))
    *regbuf++ = tolower (*(*p)++);
  *regbuf = '\0';
}

/* Set a value in regTable.
 */
set (p)
char *p;
{
  char regname [80];
  CONST struct regTable *r;
  long val;

  scan (&p);
  scanreg (&p, regname);
  if (NULL == (r = find_reg (regname))) {
    myPrintf ("Bad register name\n");
    return;
  }
  if (GOT_NUM != getIntScan (&p, &val)) {
    myPrintf ("Bad value\n");
    return;
  }
  switch (r->type & T_LEN) {
    case T_CHAR:	CHAR_STAR (r->ptr) = val; break;
    case T_SHORT:	SHORT_STAR (r->ptr) = val; break;
    case T_LONG:	LONG_STAR (r->ptr) = val; break;
  }
}

printreg (r)
struct regTable *r;
{
  long val;


  switch (r->type & T_LEN) {
    case T_CHAR:	val = CHAR_STAR (r->ptr); break;
    case T_SHORT:	val = SHORT_STAR (r->ptr); break;
    case T_LONG:	val = LONG_STAR (r->ptr); break;
  }
  if (r->type & T_PSR)
    myPrintf ("%8s=%c%c%c%c%c%c%c%c%c%c%c",
      r->name,
      CAPIFSET (val, PSR_I, 'i'),
      CAPIFSET (val, PSR_P, 'p'),
      CAPIFSET (val, PSR_S, 's'),
      CAPIFSET (val, PSR_U, 'u'),
      CAPIFSET (val, PSR_N, 'n'),
      CAPIFSET (val, PSR_Z, 'z'),
      CAPIFSET (val, PSR_F, 'f'),
      CAPIFSET (val, PSR_V, 'v'),
      CAPIFSET (val, PSR_L, 'l'),
      CAPIFSET (val, PSR_T, 't'),
      CAPIFSET (val, PSR_C, 'c'));
  else if (r->type & T_DECI)
    myPrintf ("%8s=d'%-6ld ", r->name, val);
  else myPrintf ("%8s=%-8lx ", r->name, val);
}

/* Print range of registers.
 */
showregs(first, last)
int first, last;
{
  CONST struct regTable *r;
  int num, i;

  num = 0;
  r = regTable + first;
  for (i = first; i <= last; ++i, ++r)
    if (NULL != r->ptr) {
      if (num && 0 == (num & 3)) myPrintf ("\n");
      ++num;
      printreg (r);
    }
  myPrintf ("\n");
}

/* Print all registers.
 */
show()
{
  showregs (REG_R0, LAST_MODE);
}

/* Print R0-R7
 */
gpr()
{
  showregs (REG_R0, REG_R7);
}

/* Print MMU registers
 */
mmu()
{
  showregs (REG_FIRST_MMU, REG_LAST_MMU);
}

/* Print FPU registers
 */
fpu()
{
  showregs (REG_F0, REG_F15);
  showregs (REG_FSR, REG_FSR);
}

/* Print software breakpoints.
 */
printBkpt()
{
  showregs (REG_BKPT0, REG_BKPT7);
}

/* Print CPU registers
 */
cpu()
{
  showregs (REG_FIRST_CPU, REG_LAST_CPU);

#if 0
  printreg (&regTable [REG_FP]);
  printreg (&regTable [REG_USP]);
  printreg (&regTable [REG_PC]);
  printreg (&regTable [REG_SB]);
  myPrintf ("\n");
  printreg (&regTable [REG_INTBASE]);
  printreg (&regTable [REG_ISP]);
  printreg (&regTable [REG_MOD]);
  printreg (&regTable [REG_PSR]);
  myPrintf ("\n");
#endif
}

/* Print mode registers.
 */
mode()
{
  showregs (FIRST_MODE, LAST_MODE);
}
