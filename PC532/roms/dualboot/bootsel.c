/*
 * bootsel.c -- pc532 ROM dual-boot dispatcher.
 *
 * Dave's rule:
 *   - DP8490 live + holds NetBSD  -> boot NetBSD off the DP8490.
 *   - AIC6250 live + holds System V -> boot System V off the AIC6250.
 *
 * The controller picks the OS (DP8490=NetBSD, AIC6250=SysV); we just find which
 * controller has a live, bootable disk.  Both controllers share the 0x30000000
 * window, selected by NS32202 ICU port G7 (G7=0 DP8490, G7=1 AIC6250), so we
 * probe one at a time.
 *
 * pc532romstart.s calls main() here after the reset prologue.  Each *_boot()
 * does not return on success (it jumps into the loaded kernel / bootstrap).
 *
 * Probe order = AIC6250/SysV first (the active port goal), then DP8490/NetBSD.
 * Reverse the two if-blocks to prefer NetBSD.
 */
#include "stand.h"

extern int  aic_live();		/* aic_live.c: G7=1, bounded TUR target1; 1 = disk answered */
extern void sysv_boot();	/* main.c   : mount s5 /dev/dsk/0s0, load+jump /unix (AIC)  */
extern int  dp_live();		/* dp8490.c : G7=0, bounded TUR; 1 = disk answered         */
extern void netbsd_boot();	/* dp8490.c : read the NetBSD primary bootstrap, jump      */
extern      pputs();		/* main.c   : polled console puts (shared; do NOT redefine) */

main()
{
	pputs("\r\npc532 boot ROM (dual: AIC6250/SysV, DP8490/NetBSD)\r\n");

	/* System V on the AIC6250 */
	pputs("probe AIC6250... ");
	if (aic_live()) {
		pputs("live -> System V\r\n");
		sysv_boot();			/* no return on success */
		pputs("SysV boot failed\r\n");	/* fall through to try NetBSD */
	} else
		pputs("none\r\n");

	/* NetBSD on the DP8490 */
	pputs("probe DP8490... ");
	if (dp_live()) {
		pputs("live -> NetBSD\r\n");
		netbsd_boot();			/* no return on success */
		pputs("NetBSD boot failed\r\n");
	} else
		pputs("none\r\n");

	pputs("no bootable disk on either controller -- halt\r\n");
	for (;;)
		;
}
