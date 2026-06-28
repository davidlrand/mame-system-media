#!/bin/sh
# mkrom.sh -- build the pc532 dual-boot ROM (32KB EPROM image: pcrom.bin).
#
# Burns the PROVEN RAM bootloader (../boot: main.c COFF load + dc.c AIC6250 +
# conf.c device tables + lib2.a s5 fs) straight into the reset EPROM, fronted by
# a ROM reset prologue (pc532romstart.s) and a dual-boot dispatcher (bootsel.c):
#
#   reset -> pc532romstart (CFG/MOD/INTBASE/SB, overlay flip, DUART, .data copy)
#         -> main()=bootsel.c
#              AIC6250 live? -> sysv_boot()   (== boot/main.c, -Dmain=sysv_boot)
#              DP8490  live? -> netbsd_boot() (dp8490.c)
#
# Run on the cross host that has the National ns32k `as`/`cc`/`ld` + lib2.a
# (the SAME toolchain that builds ../boot/mkboot.sh).  STATIC build only -- do
# NOT expect to run the result until we can test on MAME (see README "verify").
#
# NOTE: this script is intentionally NOT auto-run here.  Adjust SRC/LIB to the
# toolchain layout, then invoke by hand on the cross host.

set -e

ROM=`dirname $0`
BOOT=$ROM/../boot			# proven RAM-loader sources we reuse verbatim
LIB=-l2					# lib2.a: standalone s5 fs (open/read/mount/bmap)

cd "$ROM"
rm -f *.o pcrom pcrom.bin

# ---- reset prologue + console + jumpto (ROM-resident, replaces pc532bstart) --
# CRITICAL: the build host's SysV filesystem truncates filenames to 14 chars, so
# `pc532romstart.s' and `pc532romstart.o' BOTH map to `pc532romstart.' -- i.e.
# the SAME file.  `as -o pc532romstart.o pc532romstart.s' then opens the output
# (truncating the source to 0) and reads back nothing -> a valid-looking but
# EMPTY object (rc=0), and the link fails "undefined start/_pputc/_jumpto".
# Assemble to a short, non-colliding object name.
as -o romstart.o pc532romstart.s

# ---- dual-boot dispatcher + per-controller probes/boots -----------------------
cc -DSTANDALONE -c bootsel.c			# main(): probe + dispatch
cc -DSTANDALONE -c aic_live.c			# aic_live(): bounded AIC TUR probe
cc -DSTANDALONE -c dp8490.c			# dp_live()/netbsd_boot(): DP8490 path

# ---- reused proven loader (AIC6250 / System V path) ---------------------------
# boot/main.c is the COFF loader; rename its main() so bootsel's main() is the
# entry and main.c becomes sysv_boot().  dc.c + conf.c reused verbatim.
cc -DSTANDALONE -Dmain=sysv_boot -c "$BOOT/main.c"
cc -DSTANDALONE -c "$BOOT/dc.c"
cc -DSTANDALONE -c "$BOOT/conf.c"

# ---- link: pc532romstart FIRST (reset vector at the section base) -------------
ld rom.map \
	romstart.o \
	bootsel.o aic_live.o dp8490.o \
	main.o dc.o conf.o \
	$LIB
mv a.out pcrom

size pcrom

# ---- extract the raw 32KB EPROM image -----------------------------------------
# The SVR2 build host has NO objcopy (and no /dev/zero for a dd pad), so carve the
# COFF with our own tool: coffbin lays .text at ROM offset 0 and the initialised
# .data IMAGE right after it at _etext (the pc532romstart copy source), 0xff-pads
# to the 32KB device.  NB host ns32k COFF section headers are 48 bytes on disk
# (coffbin strides 48, reads the 40 standard fields) -- verified via `dump -h'.
cc -o coffbin coffbin.c
./coffbin pcrom pcrom.bin			# -> 32768-byte EPROM image
ls -l pcrom.bin
sum -r pcrom.bin				# position-sensitive checksum (verify xfer)
echo "BUILTOK pcrom.bin (32768 bytes) -- this is the pc532 -bios EPROM image"
