; NSC 32000 ROM debugger.
; Bruce Culbertson  Bob Krause
;
; Boot starts here.  Also, code to switch between debugger and user code.


table_start:	.equ	h'20
modtab_adr:	.equ	table_start
inttab_adr:	.equ	modtab_adr + h'20
mem_len:	.equ	32*h'100000	;32 megabytes
db_stack:	.equ	h'800
psr_usp:	.equ	h'200
psr_ie:		.equ	h'800
bpt_vec:	.equ	8

cfg_i:		.equ	h'001
cfg_f:		.equ	h'002
cfg_m:		.equ	h'004
cfg_dc:		.equ	h'200
cfg_ic:		.equ	h'800
cfg_default:	.equ	h'0f0 or cfg_i or cfg_f or cfg_m or cfg_dc or cfg_ic

;These are offsets into _machState
ms_r:		.equ	0
ms_cpu:		.equ	32
ms_pc:		.equ	ms_cpu+4*0
ms_sb:		.equ	ms_cpu+4*1
ms_fp:		.equ	ms_cpu+4*2
ms_usp:		.equ	ms_cpu+4*3
ms_isp:		.equ	ms_cpu+4*4
ms_intbase:	.equ	ms_cpu+4*5
ms_mod:		.equ	ms_cpu+4*6
ms_psr:		.equ	ms_cpu+4*6+2
ms_mmu:		.equ	60
ms_ptb0:	.equ	ms_mmu+4*0
ms_ptb1:	.equ	ms_mmu+4*1
ms_tear:	.equ	ms_mmu+4*2
ms_mcr:		.equ	ms_mmu+4*3
ms_msr:		.equ	ms_mmu+4*4
ms_debug:	.equ	80
ms_dcr:		.equ	ms_debug+4*0
ms_dsr:		.equ	ms_debug+4*1
ms_car:		.equ	ms_debug+4*2
ms_bpc:		.equ	ms_debug+4*3
ms_fpu:		.equ	96
ms_fsr:		.equ	ms_fpu+0
ms_f:		.equ	ms_fpu+4
ms_cfg:		.equ	164

	.static
_cfg_init::	.word	cfg_default	;used in init_machState()
dbsp:		.blkd	1
dbfp:		.blkd	1
in_db:		.byte	1		;true if in debugger

	; Debugger starts here.  Initialize things.
	.program
;--------------------------------------------------------------------------
; This code is lifted from MONSTART by Dave Rand.
;--------------------------------------------------------------------------
codesp:	.equ	h'10000000		; swapped code space
datasp:	.equ	h'0			; real data space
SECOND:	.equ	4000000			; ACB count for 1 second (rough)
SCSI:	.equ	h'30000000		; SCSI address

; The code from start to reset1 starts up the refresh, and swaps the
; rom to high memory.  Start must be the first thing in the rom.

start::					; must be visible to linker
	lprw	cfg,cfg_default
	movd	(SECOND/1000000)*800,r1 ; get 800 microsecond delay
h1:	acbd	-1,r1,h1
	movd	h'fffffe00,r7		; point to ICU
	movb	h'15,22(r7)		; set up refresh
	movqb	0,16(r7)
	movb	13,24(r7)
	movqb	0,25(r7)
	movqb	-1,19(r7)		; set up a 1 in all outputs
	movqb	0,20(r7)		; set as all I/O
	movqb	-2,21(r7)		; g0 as output
	br	reset1+codesp

reset1:	movqb	-2,19(r7)		; kill rom at zero, swap RAM

;the following code lights all of the LEDs connected to the SCSI
	movd	SCSI,r0			; point to scsi
	movb	h'8,0(r0)		; register 8
	movb	h'4,1(r0)		; no reset, 20 Mhz
	movqb	7,0(r0)			; register 7
	movb	h'10,1(r0)		; port A output
	movb	h'd,0(r0)		; register D
	movqb	-1,1(r0)		; LED status

; This code wakes up the DRAM, according to the spec.
; It also initializes (at txl1) the first 2K of ram by overwriting.
	addr	@10,r2			; do it 10 times
txl2:	movd	(SECOND/1000000)*8000,r1 ; get 8 millisecond delay
h2:	acbd	-1,r1,h2
	movd	datasp,r0		; point to data space
	movqd	0,0(r0)			; write to page zero
	movqd	0,h'2000000-4(r0)	; write to last page
	movqd	0,0(r0)			; write to page zero
	movqd	0,h'2000000-4(r0)	; write to last page
	movqd	0,0(r0)			; write to page zero
	movqd	0,h'2000000-4(r0)	; write to last page
	movqd	0,0(r0)			; write to page zero
	movqd	0,h'2000000-4(r0)	; write to last page
	addr	@512,r1			; write first 512 doubles (2048 bytes)
txl1:	movqd	0,0(r0)	; write data
	addqd	4,r0			; next block
	acbd	-1,r1,txl1		; and loop
	acbd	-1,r2,txl2		; loop 10 times

;
; The following actually enables the NMI/parity error.
	movd	h'28000040,r0
	cmpqb	0,h'10(r0)		; clear NMI error

;Initialize the first 4 megabyte of memory
;
parin2:
	movd	datasp+h'800,r0		; point to start of data space
	movd	(mem_len-h'800)/32,r1; number of chunks to write
parin3:	movqd	0,0(r0)
	movqd	0,4(r0)
	movqd	0,8(r0)
	movqd	0,12(r0)
	movqd	0,16(r0)
	movqd	0,20(r0)
	movqd	0,24(r0)
	movqd	0,28(r0)
	addd	32,r0
	acbd	-1,r1,parin3
	movd	h'28000040,r0
	cmpqb	0,h'10(r0)		; clear NMI error

rst14:
	bicpsrw	psr_usp	or psr_ie	;int stack
	lprd	sp,db_stack
	lprd	intbase,inttab_adr
	lprw	mod,modtab_adr
	lprd	sb,0
;--------------------------------------------------------------------------
; End of MONSTART
;--------------------------------------------------------------------------
	br	_main

_isr_nvi::
	bicpsrw	psr_ie
	movqd	0,tos
	br	exit_user
_isr_nmi::
	bicpsrw	psr_ie
	cmpqb	0,in_db(pc)
	beq	_isr_nmi_user
	reti
_isr_nmi_user:
	movqd	1,tos		;in user
	br	exit_user
_isr_abt::
	bicpsrw	psr_ie
	movqd	2,tos
	br	exit_user
_isr_slv::
	bicpsrw	psr_ie
	movqd	3,tos
	br	exit_user
_isr_ill::
	bicpsrw	psr_ie
	movqd	4,tos
	br	exit_user
_isr_svc::
	bicpsrw	psr_ie
	movqd	5,tos
	br	exit_user
_isr_dvz::
	bicpsrw	psr_ie
	movqd	6,tos
	br	exit_user
_isr_flg::
	bicpsrw	psr_ie
	movqd	7,tos
	br	exit_user
_isr_bpt::
	bicpsrw	psr_ie
	addr	@8,tos
	cmpd	_ret_save,4(sp)
	bne	_isr_bpt1
	addr	@11,0(sp)	;really user just did ret
_isr_bpt1:
	br	exit_user
_isr_trc::
	bicpsrw	psr_ie
	addr	@9,tos
	br	exit_user
_isr_und::
	bicpsrw	psr_ie
	addr	@10,tos
	br	exit_user
_isr_rbe::
	bicpsrw	psr_ie
	addr	@11,tos
	br	exit_user
_isr_nbe::
	bicpsrw	psr_ie
	addr	@12,tos
	br	exit_user
_isr_ovf::
	bicpsrw	psr_ie
	addr	@13,tos
	br	exit_user
_isr_dbg::
	bicpsrw	psr_ie
	addr	@14,tos
	br	exit_user
_ret_save::
	bpt			;use bpt to return to debugger
_ret_save_bpt:			;to distinguish from reg bpt
	br	_ret_save	;in case user resumes

; There is an image of the 532 registers in RAM.  The image is called machState
; and is a MachState struct, defined in debugger.h.  The monitor's SET and
; various SHOW commands change and display this image.  When the RUN and
; STEP commands are executed, the image is copied to the actual registers
; using instructions like lprd and lmr.  Conversely, when the user code
; re-enters the monitor via a trap, the 532 registers are copied to the
; RAM image, using instructions like sprd and smr.  All this copying
; happens in the code which follows.

exit_user::
	sprd	sp,_machState+ms_isp(pc)
	lprd	sp,_machState+32
	save	[r0,r1,r2,r3,r4,r5,r6,r7]
	sprd	sp,r1		;r1 = &_machState
	lprd	sp,ms_isp(r1)
	movd	tos,r0		;return val = trap type	
	movd	tos,ms_pc(r1)
	movw	tos,ms_mod(r1)
	movw	tos,ms_psr(r1)
	sprd	sp,ms_isp(r1)
	sprd	sb,ms_sb(r1)
	sprd	fp,ms_fp(r1)
	sprd	intbase,ms_intbase(r1)
	bispsrw	psr_usp
	sprd	sp,ms_usp(r1)
	bicpsrw	psr_usp
	sprw	cfg,r2
	movw	r2,ms_cfg(r1)

	sprd	dcr,ms_dcr(r1)		;debug
	sprd	dsr,ms_dsr(r1)
	sprd	car,ms_car(r1)
	sprd	bpc,ms_bpc(r1)

	tbitb	cfg_m,r2
	bfc	x_no_mmu
	smr	ptb0,ms_ptb0(r1)	;mmu
	smr	ptb1,ms_ptb1(r1)
	smr	tear,ms_tear(r1)
	smr	mcr,ms_mcr(r1)
	smr	msr,ms_msr(r1)
x_no_mmu:

	tbitb	cfg_f,r2
	bfc	x_no_fpu
	sfsr	ms_fsr(r1)		;fpu
	movl	f0,ms_f+0(r1)
	movl	f2,ms_f+8(r1)
	movl	f4,ms_f+16(r1)
	movl	f6,ms_f+24(r1)
	movl	f1,ms_f+32(r1)
	movl	f3,ms_f+40(r1)
	movl	f5,ms_f+48(r1)
	movl	f7,ms_f+56(r1)
x_no_fpu:

	;user saved, now restore debugger
	lprd	mod,modtab_adr
	lprd	intbase,inttab_adr
	lprd	sb,0
	lprd	sp,dbsp(pc)
	lprd	fp,dbfp(pc)
	restore	[r1,r2,r3,r4,r5,r6,r7]
	movqb	1,in_db(pc)
	ret	0

_resume::
	save	[r1,r2,r3,r4,r5,r6,r7]	;monitor state
	sprd	sp,dbsp(pc)
	sprd	fp,dbfp(pc)
	movqb	0,in_db(pc)

	lprd	sp,_machState
	lprd	dcr,ms_dcr(sp)		;debug
	lprd	dsr,ms_dsr(sp)
	lprd	car,ms_car(sp)
	lprd	bpc,ms_bpc(sp)

	movw	ms_cfg(sp),r2
	lprw	cfg,r2

	tbitb	cfg_f,r2
	bfc	r_no_fpu
	lfsr	ms_fsr(sp)		;fpu
	movl	ms_f+0(sp),f0
	movl	ms_f+8(sp),f2
	movl	ms_f+16(sp),f4
	movl	ms_f+24(sp),f6
	movl	ms_f+32(sp),f1
	movl	ms_f+40(sp),f3
	movl	ms_f+48(sp),f5
	movl	ms_f+56(sp),f7
r_no_fpu:

	lprd	fp,ms_fp(sp)
	lprd	intbase,ms_intbase(sp)
	restore [r0,r1,r2,r3,r4,r5,r6,r7]	;clobbers sp
	bispsrw	psr_usp
	lprd	sp,_machState+ms_usp(pc)
	bicpsrw	psr_usp
	lprd	sp,_machState+ms_isp(pc)
	movw	_machState+ms_psr(pc),tos
	movw	_machState+ms_mod(pc),tos
	movd	_machState+ms_pc(pc),tos

	tbitb	cfg_m,_machState+ms_cfg(pc)
	bfc	r_no_mmu
	lmr	ptb0,_machState+ms_ptb0(pc)	;mmu
	lmr	ptb1,_machState+ms_ptb1(pc)
	lmr	tear,_machState+ms_tear(pc)
	lmr	msr,_machState+ms_msr(pc)
	lmr	mcr,_machState+ms_mcr(pc)
r_no_mmu:
	rett	0
