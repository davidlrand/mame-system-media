/*
 * aic_live.c -- bounded "is there a System V disk on the AIC6250?" probe.
 *
 * sysv_boot() -> mount -> _dcopen issues a REZERO and retries do_scsi forever on
 * a no-select (-48), so it HANGS if the AIC bus is empty.  Call aic_live() first:
 * one bounded do_scsi() TEST-UNIT-READY to the SysV target (id 1, per conf.c
 * /dev/dsk/0s0 = channel 1).  do_scsi returns -48 only if selection never
 * completes within its bound, so != -48 means a target answered = live.
 *
 * (Liveness only.  _dcopen then validates it's really a System V disk via the
 * block-0 mode-param checksum, so a non-SysV disk here fails the mount cleanly
 * and bootsel falls through to the DP8490/NetBSD probe.)
 */
#include "stand.h"
#include "sys/scsi.h"
#include "sys/dc.h"

extern int do_scsi();		/* boot/dc.c (AIC6250) */
extern bzero();

#define AIC_SYSV_TARGET	1	/* conf.c /dev/dsk/0s0 -> channel 1 */
#define TEST_UNIT_READY	0x00

int
aic_live()
{
	struct cdb6 cdb6;
	int s;

	bzero(&cdb6, sizeof(cdb6));
	cdb6.opcode = TEST_UNIT_READY;
	s = do_scsi(AIC_SYSV_TARGET, 0, &cdb6, (char *)0, 0);
	return (s != -48);
}
