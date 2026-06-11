;pcdos	equ	00		;true if puck-dos
;cc	equ	not pcdos
;
;stand	equ	-1		;true if standard I/O location
;
;inter	equ	-1		;false for no interrupts.
;

intno	equ	2		;Hard interrupt number 
intmask	equ	11111011b	;Mask for IBM int. reg.
intadr	equ	28h		;physical address for interrupt

my_flag	equ	12h		;dunno if this is good but...





ios	equ	2000h		;I/O space definition

sw86	equ	ios		;8086 SERVICE REQUEST WORD 
rc86	equ	ios+2		;8086 COMPLETION CODE
sw32	equ	ios+4		;32032 SERVICE REQUEST WORD
rc32	equ	ios+6		;32032 COMPLETION CODE
	;
qin86	equ	ios+8		;8086 QUEUE count
qin32	equ	ios+0ah		;32032 QUEUE count
	;
qout86	equ	ios+0ch		;8086 QUEUE count
qout32	equ	ios+0eh		;32032 QUEUE count
	;
qpr86	equ	ios+10h		;8086 QUEUE count
qpr32	equ	ios+12h		;32032 QUEUE count
	;
dskrw	equ	ios+14h		;DISK SERVICE REQUEST
dskid	equ	ios+16h		;CURRENT FILE ID
dskl	equ	ios+18h		;# OF BYTES TO TRANSFER
dskdta	equ	ios+1ah		;POINTER TO DISK DTA
dskrc	equ	ios+1eh		;DISK COMPLETION CODE

heapl	equ	ios+20h
heaph	equ	ios+24h
stackl	equ	ios+28h
stackh	equ	ios+2ch

dskerr	equ	ios+30h		;disk lerrno

var1	equ	ios+32h		;interprocess variable area
var2	equ	ios+36h
var3	equ	ios+3ah
var4	equ	ios+3eh
var5	equ	ios+42h
var6	equ	ios+46h
var7	equ	ios+4ah
var8	equ	ios+4eh
	;
;	rs	30		;RESERVED
	;
kqsize	equ	63		;Keyboard queue size
vqsize	equ	2047		;Video queue size
pqsize	equ	1023		;Printer queue size
keyin	equ	ios+50h		;KEYBOARD INPUT BUFFER
vduout	equ	ios+90h		;VIDEO OUTPUT BUFFER
prout	equ	ios+890h	;PRINTER OUTPUT BUFFER
	;
;	rs	880		;RESERVED

		if stand eq 1
physmem		equ	0e000h	;physical I/O memory
ioport		equ	150h
pageport	equ	0160h	;page port number
		endif

		if stand eq 0	;alternate location
physmem		equ	0d000h	;physical I/O memory
ioport		equ	2a0h
pageport	equ	02b0h	;page port number
		endif

duart1		equ	20h ;page address
durt_base	equ	100h
durt_0		equ	durt_base
durt_1		equ	durt_base+4
durt_2		equ	durt_base+8
durt_3		equ	durt_base+12
durt_4		equ	durt_base+16
durt_5		equ	durt_base+20
durt_6		equ	durt_base+24
durt_7		equ	durt_base+28
durt_8		equ	durt_base+32
durt_9		equ	durt_base+36
durt_a		equ	durt_base+40
durt_b		equ	durt_base+44
durt_c		equ	durt_base+48
durt_d		equ	durt_base+52
durt_e		equ	durt_base+56
durt_f		equ	durt_base+60

ccpm	equ	224
i_nmi	equ	2*4




tod_day		equ	word ptr 0[bx]
tod_hour	equ	byte ptr 2[bx]
tod_min		equ	byte ptr 3[bx]
tod_sec		equ	byte ptr 4[bx]
tod_ssec	equ	byte ptr 5[bx]

