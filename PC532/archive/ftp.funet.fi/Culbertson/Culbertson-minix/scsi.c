/****************************************************************************
 * NS32K Minix SCSI driver
 * Bruce Culbertson
 * 18 Jun 1989
 *
 * Modifications
 *   10 Oct 1989 WBC
 *     Added kludge to prevent a crash caused by a race in the SCSI/DMA
 *     hardware handshake.  Create a watch dog timer with clock task.
 *     When time runs out, set error flag and read/write SCSI data port
 *     which un-hangs handshake.
 ****************************************************************************/
#include "../h/const.h"
#include "../h/com.h"
#include "../h/error.h"
#include "../h/type.h"
#include "../h/32k.h"
#include "glo.h"

#undef	PRIVATE
#define	PRIVATE

/* SCSI bus phases
 */
#define PH_ODATA	0
#define PH_IDATA	1
#define PH_CMD		2
#define PH_STAT		3
#define PH_IMSG		7
#define PH_NONE		8
#define PH_IN(phase)	((phase) & 1)

/* NCR5380 SCSI controller registers
 */
#define SC_CTL		0xfff600	/* base for control registers */
#define SC_DMA		0xfff800	/* base for data registers */
#define SC_CURDATA	SC_CTL+0
#define SC_OUTDATA	SC_CTL+0
#define SC_ICMD		SC_CTL+2
#define SC_MODE		SC_CTL+4
#define SC_TCMD		SC_CTL+6
#define SC_STAT1	SC_CTL+8
#define SC_STAT2	SC_CTL+10
#define SC_START_SEND	SC_CTL+10
#define SC_INDATA	SC_CTL+12
#define SC_RESETIP	SC_CTL+14
#define SC_START_RCV	SC_CTL+14

/* Bits in NCR5380 registers
 */
#define SC_A_RST	0x80
#define SC_A_SEL	0x04
#define SC_S_SEL	0x02
#define SC_S_REQ	0x20
#define SC_S_BSY	0x40
#define SC_S_BSYERR	0x04
#define SC_S_PHASE	0x08
#define SC_S_IRQ	0x10
#define SC_M_DMA	0x02
#define SC_M_BSY	0x04
#define SC_ENABLE_DB	0x01

/* Status of interrupt routine, returned in m1_i1 field of message.
 */
#define ISR_NOTDONE	0
#define ISR_OK		1
#define ISR_BSYERR	2
#define ISR_RSTERR	3
#define ISR_BADPHASE	4

/* Miscellaneous
 */
#define MAX_WAIT	500000
#define IMASK_MSB	22
#define SC_ARM_INT() 	(RD_ADR(ICU_ADR + IMASK_MSB) &= ~(1<<(IR_SCSI-8)))
#define SC_DISARM_INT() (RD_ADR(ICU_ADR + IMASK_MSB) |= (1<<(IR_SCSI-8)))
#define printf		printk
#define SC_LOG_LEN	32

PRIVATE struct scsi_args	*sc_ptrs;
PRIVATE char			sc_reset_done;
PRIVATE char			sc_have_msg;
PRIVATE unsigned char		sc_cur_phase;
PRIVATE char			sc_accept_int;

#ifdef DEBUG
struct sc_log {
  unsigned char stat1, stat2;
}				sc_log [SC_LOG_LEN],
				*sc_log_head = sc_log;
int				sc_spurious_int;
#endif
unsigned char
	watchdog_count,			/* watch dog counter */
	watchdog_error;

/*===========================================================================*
 *				execute_scsi_cmd			     * 
 *===========================================================================*/
PUBLIC
int
execute_scsi_cmd (args, scsi_adr)
struct scsi_args *args;
int scsi_adr;
{
  unsigned char dummy;
  message msg;

  sc_ptrs = args;			/* make pointers globally accessible */
  if (!sc_reset_done) sc_reset();
  /* TCMD has some undocumented behavior in initiator mode.  I think the
   * data bus cannot be enabled if i/o is asserted.
   */
  WR_ADR (SC_TCMD, 0);
  if (OK != sc_wait_bus_free ()) {	/* bus-free phase */
    printf ("SCSI: bus not free\n");
    return NOT_OK;
  }
  sc_cur_phase = PH_NONE;
  sc_have_msg = 0;
  if (OK != sc_select (scsi_adr))	/* select phase */
    return NOT_OK;
  watchdog_error = 0;			/* watchdog timer */
  watchdog_count = HZ;
  receive (HARDWARE, &msg);		/* isr does the rest */
  if (watchdog_error) {
    sc_reset();
    /*printf ("SCSI timeout\n");*/
    return NOT_OK;
  }
  switch (msg.m1_i1) {
    case ISR_OK:
      return OK;
    case ISR_BSYERR:
      sc_reset();
      printf ("SCSI: busy error\n");
      return NOT_OK;
    case ISR_RSTERR:
      sc_reset();
      printf ("SCSI: reset error\n");
      return NOT_OK;
    case ISR_BADPHASE:
      sc_reset();
      printf ("SCSI: NULL pointer for current phase\n");
      return NOT_OK;
    default:
      sc_reset();
      printf ("SCSI: unknown interrupt status\n");
      return NOT_OK;
  }
}

/*===========================================================================*
 *				sc_reset				     * 
 *===========================================================================*/
/*
 * Reset SCSI bus.
 */
PRIVATE
sc_reset()
{
  int i, old_int;

  WR_ADR (SC_MODE, 0);			/* get into harmless state */
  WR_ADR (SC_OUTDATA, 0);
  old_int = lock();
  WR_ADR (SC_ICMD, SC_A_RST);		/* assert RST on SCSI bus */
  i = 50;				/* wait 25 usec */
  while (i--);
  WR_ADR (SC_ICMD, 0);			/* deassert RST, get off bus */
  restore (old_int);
  sc_reset_done = 1;
}

/*===========================================================================*
 *				sc_wait_bus_free			     * 
 *===========================================================================*/
PRIVATE int
sc_wait_bus_free()
{
  int i = MAX_WAIT;

  while (i--) {
    /* Must be clear for 2 usec, so read twice */
    if (RD_ADR (SC_STAT1) & (SC_S_BSY | SC_S_SEL)) continue;
    if (RD_ADR (SC_STAT1) & (SC_S_BSY | SC_S_SEL)) continue;
    return OK;
  }
  sc_reset_done = 0;
  return NOT_OK;
}

/*===========================================================================*
 *				sc_select				     * 
 *===========================================================================*/
/* This duplicates much of the work that the interrupt routine would do on a
 * phase mismatch and, in fact, the original plan was to just do the select,
 * let a phase mismatch occur, and let the interrupt routine do the rest.
 * That didn't work because the 5380 did not reliably generate the phase
 * mismatch interrupt after selection.
 */
PRIVATE int
sc_select(adr)
int adr;
{
  int i, stat1;
  long new_ptr;

  WR_ADR (SC_OUTDATA, adr);		/* SCSI bus address */
  WR_ADR (SC_ICMD, SC_A_SEL | SC_ENABLE_DB);
  for (i = 0;; ++i) {			/* wait for target to assert SEL */
    stat1 = RD_ADR (SC_STAT1);
    if (stat1 & SC_S_BSY) break;	/* select successful */
    if (i > MAX_WAIT) {			/* timeout */
      printf ("sc_select: SEL timeout\n");
      sc_reset();
      return NOT_OK;
    }
  }
  WR_ADR (SC_ICMD, 0);			/* clear SEL, disable data out */
  WR_ADR (SC_OUTDATA, 0);
  for (i = 0;; ++i) {			/* wait for target to assert REQ */
    if (stat1 & SC_S_REQ) break;	/* target requesting transfer */
    if (i > MAX_WAIT) {			/* timeout */
      printf ("sc_select: REQ timeout\n");
      sc_reset();
      return NOT_OK;
    }
    stat1 = RD_ADR (SC_STAT1);
  }
  sc_cur_phase = (stat1 >> 2) & 7;	/* get new phase from controller */
  if (sc_cur_phase != PH_CMD) {
    printf ("sc_select: bad phase = %d\n", sc_cur_phase);
    sc_reset();
    return NOT_OK;
  }
  new_ptr = sc_ptrs->ptr[PH_CMD];
  if (new_ptr == 0) {
    printf ("sc_select: NULL command pointer\n");
    sc_reset();
    return NOT_OK;
  }
  sc_accept_int = 1;
  dma_setup (0, DMA_SCSI_CH, new_ptr, SC_DMA, 0xffff);
  WR_ADR (SC_TCMD, PH_CMD);
  WR_ADR (SC_ICMD, SC_ENABLE_DB);
  WR_ADR (SC_MODE, SC_M_BSY | SC_M_DMA);
  WR_ADR (SC_START_SEND, 0);
  return OK;
}

/*===========================================================================*
 *				scsi_interrupt				     *
 *===========================================================================*/
/* SCSI interrupt handler.
 */
PUBLIC
scsi_interrupt()
{
  unsigned char stat2, dummy;
  long new_ptr;
  int ret = ISR_NOTDONE;

  stat2 = RD_ADR (SC_STAT2);		/* get status before clearing request */

# ifdef DEBUG				/* debugging log of interrupts */
  sc_log_head->stat1 = RD_ADR (SC_STAT1);
  sc_log_head->stat2 = stat2;
  if (++sc_log_head >= sc_log + SC_LOG_LEN) sc_log_head = sc_log;
  sc_log_head->stat1 = sc_log_head->stat2 = 0xff;
# endif

  for (;;) {
    dummy = RD_ADR (SC_RESETIP);	/* clear interrupt request */
    if (!sc_accept_int ||		/* return if spurious interrupt */
        ((stat2 & SC_S_BSYERR) == 0 && (stat2 & SC_S_PHASE) == 1))
    {
#     ifdef DEBUG
        ++sc_spurious_int;
#     endif
      return;
    }
    RD_ADR (SC_MODE) &= ~SC_M_DMA;	/* clear DMA mode */
    WR_ADR (DMA_ADR + 			/* stop DMA controller */
      DMA_SCSI_CH * DMA_CH_LEN, 0);
    WR_ADR (SC_ICMD, 0);		/* disable data bus */
    if (sc_cur_phase != PH_NONE) {	/* if did DMA, save the new pointer */
      if (PH_IN (sc_cur_phase))		/* fetch new pointer from DMA cntlr */
        new_ptr = *((long *)(DMA_ADR + DMA_SCSI_CH * DMA_CH_LEN + DMA_DST));
      else new_ptr = *((long *)(DMA_ADR + DMA_SCSI_CH * DMA_CH_LEN + DMA_SRC));
      new_ptr &= 0xffffff;		/* high byte is garbage */
      if (sc_cur_phase == PH_IMSG &&	/* have message? */
        new_ptr != sc_ptrs->ptr[PH_IMSG]) sc_have_msg = 1;
      sc_ptrs->ptr[sc_cur_phase] =	/* save pointer */
        new_ptr;
    }
    if (stat2 & SC_S_BSYERR) {		/* target deasserted BSY? */
      if (sc_have_msg) ret = ISR_OK;
      else ret = ISR_BSYERR;
    } else if (!(stat2 & SC_S_PHASE)) {	/* if phase mismatch, setup new phase */
      sc_cur_phase = 			/* get new phase from controller */
        (RD_ADR (SC_STAT1) >> 2) & 7;
      new_ptr = sc_ptrs->ptr[sc_cur_phase];
      if (new_ptr == 0) ret = ISR_BADPHASE;
      else {
        WR_ADR (SC_TCMD, sc_cur_phase);	/* write new phase into TCMD */
        if (PH_IN (sc_cur_phase)) {	/* set DMA controller */
          dma_setup (1, DMA_SCSI_CH, SC_DMA, new_ptr, 0xffff);
          RD_ADR (SC_MODE) |= SC_M_DMA;
          WR_ADR (SC_START_RCV, 0);	/* tell SCSI to start DMA */
	} else {
          dma_setup (0, DMA_SCSI_CH, new_ptr, SC_DMA, 0xffff);
	  RD_ADR (SC_MODE) |= SC_M_DMA;
	  WR_ADR (SC_ICMD, SC_ENABLE_DB);
	  WR_ADR (SC_START_SEND, 0);
	}
      }
    } else ret = ISR_RSTERR;
    if (ret != ISR_NOTDONE) {		/* if done, send message to task */
      WR_ADR (SC_MODE, 0);		/* clear monbsy, dma */
      scsi_int_mess.m_type = DISKINT;
      scsi_int_mess.m1_i1 = ret;
      interrupt (SCSI, &scsi_int_mess);
      sc_accept_int = 0;
      break;				/* reti re-enables ints */
    }
    if (0 == ((stat2 =			/* check for another interrupt */
      RD_ADR (SC_STAT2)) & SC_S_IRQ)) 
    {
      break;
    }
  }
}
