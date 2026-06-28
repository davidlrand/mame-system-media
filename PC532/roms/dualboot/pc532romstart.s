# pc532romstart.s -- ROM-resident pc532 bootloader reset/start (AT&T ns32k as).
#
# AUGMENTS the proven boot/pc532bstart.s (same stack / bss-zero / jsr _main /
# _pputc / _jumpto body) with a ROM reset prologue so the EPROM IS the
# bootloader at reset -- no NS32000 monitor, no DP8490.  Written in the proven
# SVR2 `as' dialect of stand/pivot/rommon/start.s:
#   - ALPHABETIC labels only.  SVR2 as does NOT support GNU numeric `1:'/`1f'/
#     `1b' local labels -- it silently emits an empty-symbol object (rc=0).
#   - `.set' equates, `setcfg', `addr', `movsb' string copy: all proven there.
#
# Reset overlay (pc532): the EPROM is mapped at 0x10000000 AND overlaid at phys
# 0 at reset.  We `jump' to the high mirror first (like rommon's `jump start'),
# then drop NS32202 G0 to swap RAM in over low memory -- so the code under us
# never disappears.  .data is copied ROM(_etext)->RAM(0x200000); .bss zeroed.

	.set	ICU,      0xfffffe00	# NS32202 ICU base (native byte offsets)
	.set	DATABASE, 0x00200000	# .data/.bss RAM VMA (= rom.map ram org)
	.set	STACKTOP, 0x002c0000	# loader stack (clear of data/bss + kernel)
	.set	DUART0,   0x28000000	# mc68681 DUART0; +1 SRA(b2=TxRDY) +3 THRA

	.globl	start
	.globl	_main
	.globl	_pputc
	.globl	_jumpto
	.globl	_etext			# end of .text == .data image base in ROM
	.globl	_edata
	.globl	_end

	.text
start:
	# --- running from the ROM overlay at phys 0 ---
	setcfg	[i,f,m]			# declare ICU/FPU/MMU present (vectoring)
	jump	romhi			# absolute jump to the 0x10000000 mirror
romhi:
	# --- now executing from the high mirror: safe to drop the low overlay ---
	movd	$ICU,r7
	movb	$0x15,0x16(r7)		# CCTL : DRAM refresh enable
	movqb	$0,0x10(r7)		# MCTL
	movqb	$0,0x14(r7)		# IPS  : G pins are board I/O, not int in
	movqb	$-2,0x15(r7)		# PDIR : G0 = OUTPUT (0xfe)
	movqb	$-2,0x13(r7)		# PDAT : G0 = 0 -> RAM swapped in at low mem
	# NB no full-RAM clear here.  The defensive 0..STACKTOP clear was dropped:
	# the .bss-zero loop below covers the loader's own bss, the loaded kernel
	# zeroes its own bss in start.s, and the loader never reads low RAM as a
	# module table (MOD=0 but SB is set explicitly below, no CXP).  (The old
	# clear also hung: SVR2 as can't divide a $(STACKTOP/4) immediate and
	# mis-encoded the acbd counter -> non-terminating loop, never reached
	# duart_init.)
	# --- processor state: ints off, supervisor, interrupt stack, SB/MOD ---
	bicpsrw	$0xf00			# clear U,S,P,I (proven rommon value)
	movd	$STACKTOP,r5
	lprd	sp,r5
	lprd	fp,r5
	lprw	mod,$0			# MOD = 0 (no CXP/external module calls)
	lprd	sb,$DATABASE		# SB = static base of loader .data (0x200000)
	# --- copy .data image: ROM @ _etext -> RAM @ DATABASE  [rommon idiom] ---
	addr	_etext,r1		# src = .data image, laid in ROM after .text
	movd	$DATABASE,r2		# dst = .data VMA
	addr	_edata,r0
	subd	r2,r0			# count = _edata - DATABASE = sizeof(.data)
	movsb				# block move r0 bytes r1 -> r2
	# --- zero .bss (_edata.._end)  [proven pc532bstart.s body] ---
	movd	$_edata,r0
	movd	$_end,r1
bz:	cmpd	r0,r1
	bhs	bzd
	movqb	$0,0(r0)
	addqd	$1,r0
	br	bz
bzd:
	# --- init console DUART0 chA: 9600 8N1, enable Rx+Tx  [proven sig2681.c] ---
	# This standalone reset ROM replaces the monitor, so it MUST init the SCN2681
	# itself -- otherwise _pputc spins forever on TxRDY (Tx never enabled) and the
	# banner never prints (the bug the debugger trace found: stuck at the TxRDY
	# poll, R1=0x28000000, R0=0x0d, no THRA write).
	movd	$DUART0,r1
	movb	$0x20,2(r1)		# CRA: reset receiver
	movb	$0x30,2(r1)		# CRA: reset transmitter
	movb	$0x40,2(r1)		# CRA: reset error status
	movb	$0x10,2(r1)		# CRA: reset MR pointer
	movb	$0x13,0(r1)		# MR1A: 8 bits, no parity
	movb	$0x07,0(r1)		# MR2A: 1 stop bit
	movb	$0xbb,1(r1)		# CSRA: 9600 baud
	movb	$0x80,4(r1)		# ACR : baud rate set 2
	movb	$0x05,2(r1)		# CRA : enable Rx + Tx
	jsr	_main			# loader: AIC6250 mount /unix -> COFF -> jumpto
self:	br	self			# _main must not return

# _pputc(c) -- polled console putchar (verbatim from boot/pc532bstart.s).
_pputc:
	movd	4(sp),r0
	movd	$DUART0,r1
ptx:	movb	1(r1),r2		# SRA
	andb	$0x04,r2		# TxRDY
	cmpqb	$0,r2
	beq	ptx
	movb	r0,3(r1)		# THRA
	ret	$0

# _jumpto(addr) -- transfer to the loaded kernel (verbatim; push + ret).
_jumpto:
	movd	4(sp),r0
	movd	r0,tos
	ret	$0
