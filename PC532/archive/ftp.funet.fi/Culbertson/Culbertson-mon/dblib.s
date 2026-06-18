;****************************************************
; SCN 2681 routines for debugger
;
; Note: These i/o routines are raw.  Do mappings
;       (e.g. cr -> nl) elsewhere.
;
; Uart numbers:
;   There are eight uart addresses currently decoded
;   in the address space:
;	 h'28000000 duart 0 (uart 0, 1)
;	 h'28000010 duart 1 (uart 2, 3)
;	 h'28000020 duart 2 (uart 4, 5)
;	 h'28000030 duart 3 (uart 6, 7)
;****************************************************

duart:		.equ	h'28000000
stat_reg:	.equ	1
data_reg:	.equ	3
in_rdy:		.equ	0
out_rdy:	.equ	2
	.program

bconv:	.word	75
	.word	110
	.word	134
	.word	150
	.word	300
	.word	600
	.word	1200
	.word	2000
	.word	2400
	.word	4800
	.word	1800
	.word	9600
	.word	19200
	.word	0		; end of table marker




;****************************************************
; _db_fgetc --  read a character from a uart into r0.
;
;     Enter: db_fgetc (<uart number>)
;      Exit: character in r0
;  Destroys: r0, flags
;****************************************************
_db_fgetc::
	enter	[r1],0
	movd	8(fp),r1	;r1 = uart number
	bsr	get_uart_adr
getchar_loop:
	tbitb	in_rdy,stat_reg(r1)
	bfc	getchar_loop
	movzbd	data_reg(r1),r0
	exit	[r1]
	ret	0

;****************************************************
; _db_fputc --  write character to a uart
;
;     Enter: db_putchar (<character>, <uart number>)
;      Exit:
;  Destroys: flags
;****************************************************
_db_fputc::
	enter	[r1],0
	movd	12(fp),r1	;r1 = uart number
	bsr	get_uart_adr
putchar_loop:
	tbitb	out_rdy,stat_reg(r1)
	bfc	putchar_loop
	movb	8(fp),data_reg(r1)
	exit	[r1]
	ret	0

;****************************************************
; _db_setbaud -- Set baud rate of uart.
;
;    Enter: db_setbaud (<rate>, <uart>)
;     Exit: r0 = 1 if Ok, else = 0
; Destroys: flags
;****************************************************
_db_setbaud::
	enter	[r1,r2,r3],0
	movd	12(fp),r1	;r1 = uart number
	bsr	get_uart_adr
	movd	8(fp),r0	;r0 = baud rate
	addr	@bconv,r2	; point to baud conversion table
	movqd	0,r3		; start with a zero offset
dbsbl1:	cmpqw	0,r2[r3:w]	; are we at the end of the table?
	beq	setbaud_err	; yes, exit with error now
	cmpw	r0,r2[r3:w]	; have a look at the table
	beq	dbsbl2		; matched, exit now
	addqd	1,r3		; move to next table entry
	br	dbsbl1		; loop to do next one

dbsbl2:	mulb	17,r3		; this moves the index into both
				; nibbles of r3, for the 2681 baud
				; rate register. 0xb becomes 0xbb
	movd	r1,r2		; get address of uart
	bicb	h'8,r2		; point to base of duart
	movb	h'20,2(r1)	; clear rx
	movb	h'30,2(r1)	; clear tx
	movb	h'40,2(r1)	; clear any errors
	movb	h'10,2(r1)	; clear byte pointer
	movb	h'13,0(r1)	; no rts, no parity, 8 bits
	movb	h'7,0(r1)	; no cts, 1 stop bit
	movb	r3,1(r1)	; set baud rate
	movb	h'80,4(r2)	; set ACR to set 2 (19.2 Kbps)
	movb	h'85,2(r1)	; enable rx & tx, set RTS for 2692
	movb	h'5,2(r1)	; enable rx & tx for 2681
	movb	h'0,13(r2)	; ensure RTS mode set
	movb	h'3,14(r2)	; set RTS on

	movqd	1,r0		;return value
	br	setbaudx
setbaud_err:
	movqd	0,r0
setbaudx:
	exit	[r1,r2,r3]
	ret	0

;****************************************************
; Get uart base address.
;
; Enter: uart number in r1
;  Exit: uart base address in r1
;****************************************************
get_uart_adr:
	andd	7,r1		;be sure 0 - 7
	lshd	3,r1		;mult by h'8
	addd	duart,r1	;r1 = uart base adr
	ret	0
