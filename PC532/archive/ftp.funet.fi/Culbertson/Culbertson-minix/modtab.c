From owner-pc532%daver@mips.com Wed Jan 24 00:57:35 1990
Flags: 000000000001
Reply-To: pc532@daver.UU.NET
Date: Tue, 23 Jan 90 11:01:04 pst
From: Bruce Culbertson <culberts%hplwbc@hplabs.hp.com>
To: pc532@daver.UU.NET
Subject: Re:  byte ordering
Status: O

> would some kind soul help me I am trying to hand craft a module table
> and exception table for a 32016 (ie non 532 with direct exceptions)
> The data sheets have lovely little digrams showing memory ascending from
> the left to right as a byte stream, but I believe that the displacements
> in  the opcode are bit reversed

I have used the 32016 a fair amount and can understand your frustration.
National decided not to take sides in the little vs. big endian
(byte ordering) crusades -- they made the 32000 both-endian.  I think
they generally are in the lsb-comes-first (LCF) camp.  However, they
were unwilling to give up several features which could not be
implemented with LCF, so some msb-comes-first (MCF) crept into the
architecture.

I have to admit that I am a LCF zealot, myself.  If your CPU's bus
is too small to transfer a whole address, then you want to transfer
the least significant bytes first.  That way, you can start an
address calculation (which may generate a carry) while you fetch
the most significant bytes.  This dictates LCF if you want to
fetch the instruction stream in simple ascending order.  This
actually was not an issue on the 32016, but was one of the original
reasons for the popularity of LCF.  Further, LCF considerably
simplifies the compiler.  If, for example, you decide to read
an integer as a byte (e.g. due to a C cast), the address does
not change if you are using LCF; it does change if you are using MCF.

The 32000 architecture includes displacements which may be 1, 2,
or 4 bytes long.  The length is encoded in the most significant
1 or 2 bits of the displacement.  You need to be able to get
the length from the first byte you fetch since the first byte
might be the only byte.  Hence, displacements are MCF.  As I
recall, all data and addresses in the instruction stream, whether
or not they are displacements, are MCF; everything else is LCF.
Can anyone think of an exception to this rule?  In particular,
the module and interrupt tables are not in the instruction stream
and are LCF.

Below is the code which builds the module and interrupt tables in
my version of Minix.

Bruce Culbertson
--------------------------------------------------------------------

struct mod_entry {			/* 32k module table entry */
  long sb;
  long link;
  long pc_base;
  long dummy;
};

/* Interrupt handlers
 */
int	isr_abt(), isr_ill(), isr_svc(), isr_dvz(), isr_flg(),
	isr_bpt(), isr_trc(), isr_slave(),
	isr_und(), isr_clk(), isr_scsi(), isr_fdc(), isr_tty1(), isr_tty2();

extern char btext, etext, bdata, edata, end;

struct int_vec {
	short	number;
	int	(*isr)();
} isrInitTab[] = {
	{VEC_ABT,	isr_abt},
	{VEC_SLAVE,	isr_slave},
	{VEC_ILL,	isr_ill},
	{VEC_SVC,	isr_svc},
	{VEC_DVZ,	isr_dvz},
	{VEC_FLG,	isr_flg},
	{VEC_UND,	isr_und},
	{VEC_CLK,	isr_clk},
	{VEC_SCSI,	isr_scsi},
	{VEC_FDC,	isr_fdc},
	{VEC_TTY2,	isr_tty2},
	{VEC_TTY1,	isr_tty1},
#ifndef DEBUG
	/* Normally, you want the OS to catch these and send a signal to
	 * the proc which executed them.  For debugging with the ROM
	 * debugger, we need these to get us back into the debugger.
	 */
	{VEC_BPT,	isr_bpt},
	{VEC_TRC,	isr_trc},
#endif
};
#define ISRTABSZ (sizeof (isrInitTab)/sizeof (struct int_vec))

/*===========================================================================*
 *				modtab_init
 *===========================================================================*/
/* Setup mod table.
 */
modtab_init()
{
  struct mod_entry *p;

  p = ((struct mod_entry *)MODTAB_ADR) + MODTAB_MINIX;
  p->sb = 0;
  p->pc_base = (long)(&btext);
}

/*===========================================================================*
 *				inttab_init
 *===========================================================================*/
/* Setup interrupt table.
 */
inttab_init()
{
  struct int_vec *p;
  long *q;

  q = (long *) INTTAB_ADR;
  for (p = isrInitTab; p < isrInitTab + ISRTABSZ; ++p)
    *(q + p->number) = 
	(unsigned)(((struct mod_entry *)MODTAB_ADR) + MODTAB_MINIX) |
	((long)(p->isr) - (long)(&btext)) << 16;
}

