/* NS32K Minix SCSI hard disk driver
 * Bruce Culbertson
 * 18 June 1989
 */

/* The driver supports two operations: read a block and
 * write a block.  It accepts two messages, one for reading and one for
 * writing, both using message format m2 and with the same parameters:
 *
 *    m_type      DEVICE    PROC_NR     COUNT    POSITION  ADDRESS
 * ----------------------------------------------------------------
 * |  DISK_READ | device  | proc nr |  bytes  |  offset | buf ptr |
 * |------------+---------+---------+---------+---------+---------|
 * | DISK_WRITE | device  | proc nr |  bytes  |  offset | buf ptr |
 * ----------------------------------------------------------------
 *
 */

#include "../h/const.h"
#include "../h/type.h"
#include "../h/callnr.h"
#include "../h/com.h"
#include "../h/error.h"
#include "const.h"
#include "type.h"
#include "glo.h"
#include "proc.h"
#include "../h/32k.h"

/* These parameters are for a Seagate ST277N
 */
#define MAX_SCSI_RETRIES	6
#define	SEC_PER_CYL	155
#define NR_SEC		126790
#define NR_BLK		((NR_SEC*BYT_PER_SEC)/BLOCK_SIZE)
#define NR_CYL		(NR_SEC/SEC_PER_CYL)
#define SEC_PER_TRK	26
#define BYT_PER_SEC	512
#define SEC_PER_BLK	(BLOCK_SIZE/BYT_PER_SEC)
#define BLK_PER_TRK	(SEC_PER_TRK/SEC_PER_BLK)
#define BIG_PART_CYL	406
#define BIG_PART	/* large partition, 31465 blocks */\
	(BIG_PART_CYL*SEC_PER_CYL*BYT_PER_SEC/BLOCK_SIZE)
#define BOOT_PART_CYL	/* 6 cyl */\
	(NR_CYL - 2*BIG_PART_CYL)
#define BOOT_PART	/* boot partition, 465 blocks */\
	(BOOT_PART_CYL*SEC_PER_CYL*BYT_PER_SEC/BLOCK_SIZE)

PRIVATE message sc_msg;

#define U8	unsigned char

#define CMD_IX		2
#define CMD_LEN		12		/* longest SCSI command */
#define CMD_SENSE	0x03
#define CMD_READ	0x28
#define CMD_WRITE	0x2a
PRIVATE U8		cmd_buf[CMD_LEN];

#define SENSE_LEN 	24		/* extended sense length */
#define SENSE_KEY	2
#define NO_SENSE	0
#define RECOVERY_ERR	1
#define UNIT_ATTN	6
#define ADD_SENSE_CODE	12
#define SENSE_RST	0x29
PRIVATE	U8		sense_buf[SENSE_LEN];

#define CHECK_CONDITION	2
#define STAT_IX		3
PRIVATE U8		stat_buf[1];
#define IMSG_IX		7
PRIVATE U8		msg_buf[1];

#define ODATA_IX	0
#define IDATA_IX	1
PRIVATE struct scsi_args scsi_args;

/* Partition table.  This is not completely general in that the interleave
 * funtion does not work unless there are an even number of blocks per
 * track and the interleave is relatively prime to the number of blocks
 * per track.  However, you should be able to find an acceptable combination
 * for any drive.  Also, might be better to be MS-DOS compatible.
 */
struct partition_tbl {			/* partitions */
  long		start_blk;		/* in 1024 byte blocks */
  long		len;			/* in 1024 byte blocks */
  unsigned char scsi_adr;		/* SCSI address */
  short		sector_per_blk;		/* sectors per BLOCK_SIZE */
  short		blk_per_track;
  short		interleave;
};

PRIVATE
struct partition_tbl part_tbl [] = {
  {0,			NR_BLK,	   SC_HD_ADR, SEC_PER_BLK, SEC_PER_CYL, 1},
  {0, 			BOOT_PART, SC_HD_ADR, SEC_PER_BLK, SEC_PER_CYL, 1},
  {BOOT_PART,		BIG_PART,  SC_HD_ADR, SEC_PER_BLK, SEC_PER_CYL, 1},
  {BOOT_PART+BIG_PART,	BIG_PART,  SC_HD_ADR, SEC_PER_BLK, SEC_PER_CYL, 2},
};
#define NR_DRIVES	(sizeof (part_tbl) / sizeof (struct partition_tbl))

/*===========================================================================*
 *				scsi_task				     * 
 *===========================================================================*/
PUBLIC scsi_task()
{
/* Main program of the scsi hard disk driver task. */
  int r, caller, proc_nr;

  /* Here is the main loop of the disk task.  It waits for a message, carries
   * it out, and sends a reply.
   */
  for (;;) {
	/* First wait for a request to read or write a disk block. */
	receive(ANY, &sc_msg);	/* get a request to do some work */
	if (sc_msg.m_source < 0)
		panic("scsi disk task got message from ", sc_msg.m_source);
	caller = sc_msg.m_source;
	proc_nr = sc_msg.PROC_NR;

	/* Now carry out the work. */
	switch(sc_msg.m_type) {
	    case DISK_READ:	r = sc_rdwt(&sc_msg);	break;
	    case DISK_WRITE:	r = sc_rdwt(&sc_msg);	break;
	    default:		r = EINVAL;		break;
	}

	/* Finally, prepare and send the reply message. */
	sc_msg.m_type = TASK_REPLY;	
	sc_msg.REP_PROC_NR = proc_nr;
	sc_msg.REP_STATUS = r;	/* # of bytes transferred or error code */
	send(caller, &sc_msg);	/* send reply to caller */
  }
}

/*===========================================================================*
 *				sc_rdwt					     * 
 *===========================================================================*/
PRIVATE int sc_rdwt(m_ptr)
message *m_ptr;
{
/* Carry out a read or write request from the disk. */
  int drive, track_block, retries;
  long virt_block, phys_block, sector;
  struct partition_tbl *part;
  U8 *p;

  /* Decode the message parameters. */
  drive = m_ptr->DEVICE;
  if (drive < 0 || drive >= NR_DRIVES) return(EIO);
  part = &part_tbl[drive];

  /* convert virtual block to physical block and sector */
  virt_block = m_ptr->POSITION / BLOCK_SIZE;
  if (virt_block >= part->len) return EOF;
  track_block = virt_block % part->blk_per_track;
  phys_block = virt_block - track_block +
	(track_block * part->interleave) % part->blk_per_track +
	part->start_blk;
  sector = phys_block * part->sector_per_blk;

  for (retries = 0; retries < MAX_SCSI_RETRIES; ++retries) {
    p = cmd_buf;		/* build SCSI command */
    *p++ = (m_ptr->m_type == DISK_READ)? CMD_READ: CMD_WRITE;
    *p++ = 0;
    *p++ = (sector >> 24) & 0xff;
    *p++ = (sector >> 16) & 0xff;
    *p++ = (sector >> 8) & 0xff;
    *p++ = (sector >> 0) & 0xff;
    *p++ = 0;
    *p++ = 0;
    *p++ = part->sector_per_blk;
    *p = 0;
    scsi_args.ptr[CMD_IX] = (long)cmd_buf;
    scsi_args.ptr[STAT_IX] = (long)stat_buf;
    scsi_args.ptr[IMSG_IX] = (long)msg_buf;
    if (m_ptr->m_type == DISK_READ) {
      scsi_args.ptr[IDATA_IX] = (long)(m_ptr->ADDRESS);
      scsi_args.ptr[ODATA_IX] = 0;
    } else {
      scsi_args.ptr[ODATA_IX] = (long)(m_ptr->ADDRESS);
      scsi_args.ptr[IDATA_IX] = 0;
    }
    if (OK != execute_scsi_cmd (&scsi_args, part->scsi_adr)) {
      /* Probably a major problem, SCSI device or bus dead -- no retry */
      /* now could be just timeout */
      /* printf ("SCSI %s command failed sector=%d\n",
        m_ptr->m_type == DISK_READ? "READ": "WRITE", sector); */
      continue;
    }
    if (*stat_buf == 0)
      /* Success -- this should be the usual case */
      return BLOCK_SIZE;
    if (*stat_buf != CHECK_CONDITION) {
      /* do not know how to handle this so return error */
      printf ("SCSI device returned unknown status: %d\n", *stat_buf);
      continue;
    }
    /* Something funny happened, need to execute request-sense command
     * to learn more.
     */
    if (OK == get_sense(part->scsi_adr))
      /* Something funny happened, but the device recovered from it and
       * the command succeeded.
       */
      return BLOCK_SIZE;
  }
  printf ("SCSI %s, sector %d failed even after retries\n",
    m_ptr->m_type == DISK_READ? "READ": "WRITE", sector);
  return EIO;
}

/*===========================================================================*
 *				get_sense				     * 
 *===========================================================================*/
/* Execute a "request sense" SCSI command and check results.  When a SCSI
 * command returns CHECK_CONDITION, a request-sense command must be executed.
 * A request-sense command provides information about the original command.
 * The original command might have succeeded, in which case it does not
 * need to be retried and OK is returned.  Examples: read error corrected
 * with error correction code, or error corrected by retries performed by
 * the SCSI device.  The original command also could have failed, in
 * which case NOT_OK is returned.
 */
#define LOGICAL_ADR	\
  (sense_buf[3]<<24 | sense_buf[4]<<16 | sense_buf[5]<<8 | sense_buf[6])

PRIVATE int
get_sense (scsi_adr)
int scsi_adr;
{
  U8 *p;

  p = cmd_buf;				/* build SCSI command */
  *p++ = CMD_SENSE;
  *p++ = 0;
  *p++ = 0;
  *p++ = 0;
  *p++ = SENSE_LEN;
  *p = 0;
  scsi_args.ptr[IDATA_IX] = (long)sense_buf;
  scsi_args.ptr[ODATA_IX] = 0;
  scsi_args.ptr[CMD_IX] = (long)cmd_buf;
  scsi_args.ptr[STAT_IX] = (long)stat_buf;
  scsi_args.ptr[IMSG_IX] = (long)msg_buf;
  if (OK != execute_scsi_cmd (&scsi_args, scsi_adr)) {
    printf ("SCSI SENSE command failed\n");
    return NOT_OK;
  }
  if (*stat_buf != 0) {
    printf ("SCSI SENSE returned wrong status %d\n", *stat_buf);
    return NOT_OK;
  }
  switch (sense_buf[SENSE_KEY] & 0xf) {
    case NO_SENSE:
    case UNIT_ATTN:			/* reset */
      return NOT_OK;			/* must retry command */
    case RECOVERY_ERR:
#if 0
      /* eventually, we probably do not want to hear about these. */
      printf (
	"SCSI ok with recovery, code 0x%x, logical address 0x%x\n",
	sense_buf[ADD_SENSE_CODE], LOGICAL_ADR);
#endif
      return OK;			/* orig command was ok with recovery */
    default:
      printf (
	"SCSI failure, key 0x%x, code 0x%x, log adr 0x%x, sense buf 0x%x\n",
	sense_buf[SENSE_KEY], sense_buf[ADD_SENSE_CODE],
	LOGICAL_ADR, sense_buf);
      return NOT_OK;			/* orig command failed */
  }
}
