/*
 * dp8490.c -- pc532 DP8490 (NCR5380-class) SCSI for the ROM's NetBSD path.
 *
 * Same 0x30000000 window as the AIC6250 but selected with ICU G7=0.  Register
 * model = NCR5380 (Culbertson monitor scsi.c, proven on this MAME pc532):
 *   +1 ICMD  (0x80 RST,0x10 ACK,0x08 BSY,0x04 SEL,0x01 DBUS)
 *   +0 CSDATA(r)/ODATA(w)   +4 CSSTAT(r: 0x40 BSY,0x20 REQ,0x10 MSG,0x08 CD,0x04 IO)
 * PIO only (the pc532 wires DRQ for the DP8490 but we transfer byte-at-a-time
 * with the REQ/ACK handshake, like boot/dc.c does for the AIC).
 *
 * Provides: dp_live()    -- bounded TEST-UNIT-READY probe (is a disk there?)
 *           netbsd_boot()-- read the NetBSD primary bootstrap and jump to it.
 *
 * [V-NBSD] The NetBSD primary-bootstrap disk layout (start sector / size / load
 * address) is the one thing here NOT yet pinned to proven code.  The values
 * below are placeholders to confirm against the NetBSD/pc532 bootxx convention
 * (== what the Phil Nelson autoboot monitor already does to boot NetBSD off the
 * DP8490).  Everything else (the NCR5380 phase loop, G7=0) is modelled on proven
 * drivers.
 */
#include "stand.h"

#define DPADDR	0x30000000		/* shared SCSI window (G7=0 -> DP8490) */
#define ICUADDR	0xfffffe00

/* NCR5380 register offsets */
#define R_ODATA	0			/* w: output data   r: current scsi data */
#define R_ICMD	1
#define R_MODE	2
#define R_TCMD	3
#define R_CSSTAT 4			/* r: current status */
/* ICMD bits */
#define IC_RST	0x80
#define IC_ACK	0x10
#define IC_BSY	0x08
#define IC_SEL	0x04
#define IC_DBUS	0x01
/* CSSTAT bits */
#define ST_BSY	0x40
#define ST_REQ	0x20
#define ST_MSG	0x10
#define ST_CD	0x08
#define ST_IO	0x04

/* [V-NBSD] confirm these against NetBSD/pc532 */
#define NBSD_TARGET	0		/* DP8490 disk SCSI id */
#define NBSD_BOOTSEC	0		/* first sector of the primary bootstrap */
#define NBSD_BOOTCNT	16		/* sectors (16*512 = 8KB bootxx area) */
#define NBSD_LOADADDR	0x00001000	/* where bootxx is loaded + entered */
#define SECTSIZE	512

#define READ_OP		0x08		/* SCSI READ(6) */
#define TEST_UNIT_READY	0x00

extern int pputc();
extern void jumpto();

static unsigned char *dp  = (unsigned char *)DPADDR;
static unsigned char *icu = (unsigned char *)ICUADDR;

/* select the DP8490: ICU G7=0, preserving G0 (RAM/ROM overlay) -- mirror of
 * boot/dc.c aic_g7() but driving G7 LOW. */
static
dp_g7()
{
	icu[0x11] &= ~0x80;		/* OCASN bit7 = 0 */
	icu[0x14] &= ~0x80;		/* IPS   bit7 = 0 */
	icu[0x15] &= ~0x80;		/* PDIR  bit7 = 0 (output) */
	icu[0x13] &= ~0x80;		/* PDAT  bit7 = 0 -> DP8490 */
}

/* one byte in, REQ already asserted */
static
nin()
{
	int b;
	b = dp[R_ODATA];		/* CSDATA */
	dp[R_ICMD] |= IC_ACK;
	while (dp[R_CSSTAT] & ST_REQ)
		;
	dp[R_ICMD] &= ~IC_ACK;
	return (b & 0xff);
}

static
nout(byte)
{
	dp[R_ODATA] = byte;
	dp[R_ICMD] |= IC_DBUS;
	dp[R_ICMD] |= IC_ACK;
	while (dp[R_CSSTAT] & ST_REQ)
		;
	dp[R_ICMD] &= ~IC_ACK;
}

/*
 * dp_scsi(target, cdb, buf, count) -- NCR5380 PIO select + phase loop.
 * Returns the SCSI status byte (0 = GOOD), or -48 if selection never completes.
 */
static
dp_scsi(target, cdb, buf, count)
char *cdb, *buf;
{
	int i, st, ph, status, ndata;
	char *cp;

	dp_g7();
	dp[R_ICMD] = IC_RST;			/* bus reset, settle, release */
	for (i = 0; i < 1000; i++)
		;
	dp[R_ICMD] = 0;
	dp[R_MODE] = 0;

	/* select: drive (1<<own=0)|(1<<target) on the data bus, assert SEL */
	dp[R_ODATA] = 1 | (1 << target);
	dp[R_ICMD] = IC_SEL | IC_DBUS;
	for (i = 0; i < 200000; i++)
		if (dp[R_CSSTAT] & ST_BSY)	/* target took the bus */
			break;
	if (!(dp[R_CSSTAT] & ST_BSY)) {
		dp[R_ICMD] = 0;
		return (-48);
	}
	dp[R_ICMD] = 0;				/* drop SEL + data */

	status = 0xff;
	ndata = 0;
	cp = cdb;
	for (;;) {
		for (i = 0; i < 1000000; i++) {
			st = dp[R_CSSTAT];
			if (st & ST_REQ)
				break;
			if (!(st & ST_BSY))	/* bus free -> command done */
				return (status);
		}
		ph = st & (ST_MSG | ST_CD | ST_IO);
		dp[R_TCMD] = (ph >> 2);		/* phase match for the chip */
		if (ph == ST_CD)		/* COMMAND (CD=1,IO=0,MSG=0) */
			nout(*cp++ & 0xff);
		else if (ph == ST_IO) {		/* DATA IN (IO=1) */
			i = nin();
			if (ndata < count)
				buf[ndata] = i;
			ndata++;
		} else if (ph == 0) {		/* DATA OUT */
			nout(ndata < count ? (buf[ndata] & 0xff) : 0);
			ndata++;
		} else if (ph == (ST_CD | ST_IO))	/* STATUS */
			status = nin();
		else if (ph == (ST_MSG | ST_CD | ST_IO))	/* MSG IN */
			(void) nin();
	}
}

int
dp_live()
{
	char cdb[6];
	int i, s;

	for (i = 0; i < 6; i++)
		cdb[i] = 0;
	cdb[0] = TEST_UNIT_READY;
	s = dp_scsi(NBSD_TARGET, cdb, (char *)0, 0);
	return (s != -48);
}

void
netbsd_boot()
{
	char cdb[6];
	char *load;
	int blk, i, st;

	load = (char *)NBSD_LOADADDR;
	for (blk = 0; blk < NBSD_BOOTCNT; blk++) {
		int sec = NBSD_BOOTSEC + blk;
		for (i = 0; i < 6; i++)
			cdb[i] = 0;
		cdb[0] = READ_OP;
		cdb[1] = (sec >> 16) & 0x1f;
		cdb[2] = sec >> 8;
		cdb[3] = sec;
		cdb[4] = 1;			/* one block */
		st = dp_scsi(NBSD_TARGET, cdb, load + blk * SECTSIZE, SECTSIZE);
		if (st != 0)
			return;			/* read error -> let bootsel report */
	}
	/* [V-NBSD] entry == load base by convention; confirm bootxx's real entry */
	jumpto(NBSD_LOADADDR);
}
