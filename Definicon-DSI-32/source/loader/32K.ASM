codeseg	segment word public 'code'
	public	MOVE_,EXEC_,INIT_,EXECT_,MOVE1_,GETSYS_
	extrn	io_proc:near
	assume	cs:codeseg, ds:nothing, es:nothing, ss:nothing
	include	32kh.inc
	include mcdos.equ
delay_m	equ	100

MOVE_	proc	near
	push	bp
	mov	bp,sp
	push	di
	push	si
	push	es
	mov	ax,physmem
	mov	es,ax
	mov	dx,pageport
	mov	si,4[bp]
	mov	di,6[bp]
	mov	ax,8[bp]
	out	dx,al
	mov	curport,al
	mov	cx,10[bp]
	mov	ax,di
	add	ax,cx
	jnc	move_x1
	jz	move_x1
	push	ax
	sub	cx,ax
	MOVEBYTES
	mov	al,curport
	inc	al
	out	dx,al
	pop	cx
move_x1:MOVEBYTES
	mov	dx,pageport
	xor	al,al
	out	dx,al
	pop	es
	pop	si
	pop	di
	pop	bp
	ret

curport	db	0

MOVE_	endp


MOVE1_	proc	near
	push	bp
	mov	bp,sp
	push	di
	push	si
	push	ds
	mov	dx,pageport
	mov	si,4[bp]
	mov	ax,6[bp]
	mov	di,8[bp]
	out	dx,al
	mov	curport,al
	mov	cx,10[bp]
	mov	ax,physmem
	mov	ds,ax
	mov	ax,si
	add	ax,cx
	jnc	move_x2
	jz	move_x2
	push	ax
	sub	cx,ax
	MOVEBYTES
	mov	al,curport
	inc	al
	out	dx,al
	pop	cx
move_x2:MOVEBYTES
	mov	dx,pageport
	xor	al,al
	out	dx,al
	pop	ds
	pop	si
	pop	di
	pop	bp
	ret
MOVE1_	endp



INIT_	proc	near
;set the segment at pageport to 0
; Initialize the Bus Flags port (at ioport)
; Bit 0 = Set DIAG vector
; Bit 1 = Set interrupt request to 32K
; Bit 2 = Pulse reset capacitor low
; Bit 3 = Inhibit refresh
	push	es
	mov	ax,physmem	;point to 32032 address space
	mov	es,ax
	MOV	AL,00000101B	;Set DIAG with RESET
	MOV	DX,ioport	;the port
	out	dx,al		;set the reset/diag
	mov	cx,delay_m	;25 is about 80 usecs on PC
lx1:	loop	lx1		;wait
	mov	al,00000001b	;set the DIAG
	out	dx,al
	mov	cx,10		;wait a few usecs
lx2:	loop	lx2

;	MOV	CX,0f0H		;index for loop below
;LP1:	OUT	DX,AL		;Pulse capacitor
;	LOOP	LP1
;	MOV	AL,00000001B	;Set the DIAG vector
;	OUT	DX,AL
;
; 	mov	cx,5000
; 	loop	$		;wait for the processor to reset.

	mov	al,duart1	;point to duart page
	mov	dx,pageport	
	out	dx,al		;select it

	mov	al,00010000b	;byte pointer reset
	mov	es:[durt_2],al	;

	mov	al,00010011b
	mov	es:[durt_0],al	;set port A to 8 no parity
	mov	al,00000111b
	mov	es:[durt_0],al	;set 1 stop, normal

	mov	al,00010000b	;byte pointer reset
	mov	es:[durt_a],al

	mov	al,00010011b
	mov	es:[durt_8],al	;set port B to 8 no parity
	mov	al,00000111b
	mov	es:[durt_8],al

	mov	al,10111011b
	mov	es:[durt_1],al	;set to 9600 baud, RX and TX A
	mov	es:[durt_9],al	;set to 9600 baud, RX and TX B
	mov	al,00000101b
	mov	es:[durt_2],al	;set to RX enable, TX enable A
	mov	es:[durt_a],al	;set to RX enable, TX enable B
	mov	al,0		;prep to set 8 bit out
	mov	es:[durt_d],al	;set 8 bit out port
	mov	al,00010000b	;set interrupt input off
	mov	es:[durt_4],al	;and count from RXa
	mov	al,00000000b	;set all interrupts off
	mov	es:[durt_5],al

	mov	al,00001000b	;prep to reset OP3
	mov	es:[durt_e],al	;reset OP3
	MOV	AL,0		;re-select page zero
	MOV	DX,pageport
	OUT	DX,AL
	mov	dx,ioport

	MOV	AL,00000101B	;Set DIAG with RESET
	MOV	DX,ioport	;the port
	out	dx,al		;set the reset/diag
	mov	cx,delay_m	;25 is about 80 usecs on PC
lx3:	loop	lx3		;wait
	mov	al,00000001b	;set the DIAG
	out	dx,al
	mov	cx,10		;wait a few usecs
lx4:	loop	lx4

	mov	cx,5000		;wait for a bit
lx4a:	loop	lx4a		;so processor can reset

	mov	di,03ch		;reset debug vector information
	mov	es:word ptr 0[di],0
	mov	es:word ptr 2[di],0
	mov	di,08ch		;reset program vector information
	mov	es:word ptr 0[di],0
	mov	es:word ptr 2[di],0
	pop	es
	ret
INIT_	endp

EXEC_	proc	near
	call	init_proc	;initialize processor
	push	es		;save state, so we
	push	di		;can call C routines
	push	si

	call	io_proc		;do I/O processor

exit:	pop	si		;restore everyone, and go home
	pop	di
	pop	es
	ret
EXEC_	endp


EXECT_	proc	near
	push	ds
	push	es
	push	si
	push	di

	call	init_proc	;initialize processor, and start it going


	push	es

	if	cc
	push	cs
	SVC	S_BDOSVER	;get version number
	mov	op_ver,al	;put it away for later

	mov	ax,0		;point to zero page
	mov	es,ax
	mov	bx,i_nmi
	les	ax,es:[bx]
	cmp	op_ver,41h	;is this version 4.1?
	jz	exetn		;no, so this number is valid
	sub	ax,039h		;index to tick
	jmp	short exetn1
exetn:	mov	bx,ax
	cmp	es:byte ptr [bx],1eh ;are we in NDP mode?
	jnz	exetn2		;no, so just offset
	sub	ax,37h		;else offset the NMI vector
exetn2:
	sub	ax,108		;else number is this
exetn1:
	pop	ds
	mov	sixty_off,ax
	mov	sixty_seg,es
	endif

	if	pcdos
	push	cs
	pop	ds
	endif

	mov	bx,offset old_tod
	call	gettime
	pop	es

	pop	si
	pop	di
	pop	es
	pop	ds

	push	es
	push	di
	push	si
	push	ds
	call	io_proc			;go do I/O process, and wait until
					;done
	push	cs
	pop	ds

	mov	bx,offset new_tod
	call	gettime


	mov	dx,offset mess1
	SVC	C_WRITESTR

	push	ds
	pop	es
	mov	si,offset new_tod+4	;point to the seconds on the new
	mov	bx,offset old_tod+4	;point to the seconds on the old
	mov	di,offset elapsed_tod+4	;point to the seconds on the elapsed

	mov	al,byte ptr 1[si]	;get the sixtyth seconds from the new
	sub	al,byte ptr 1[bx]	;subtract from the old
	pushf				;save flags for loop
	cmp	al,60			;adjust it if it new is < old
	jb	is_ok2
	sub	al,196
is_ok2:	popf				;restore flags for loop
	mov	byte ptr 1[di],al	;save sixtyth seconds in elapsed

	mov	cx,3			;three bytes to subtract
t_loop:	mov	al,byte ptr [si]	;get new
	sbb	al,byte ptr [bx]	;subtract from old
	das				;decimal adjust it
	pushf				;save flags for next loop
	jo	not_ok			;if overflow, then subtract it
	cmp	al,60h			;if it is over 60
	jb	is_ok1
not_ok:	sub	al,40h			;subtract 40h
is_ok1:	popf				;get flags back for loop
	mov	byte ptr [di],al	;and store the elapsed time
	dec	si			;new++
	dec	bx			;old++
	dec	di			;elapsed++
	loop	t_loop			;and loop for rest

	mov	bx,offset elapsed_tod	;point to elapsed time
	call	prtime			;print it

exitt:	pop	ds
	pop	si
	pop	di
	pop	es
	ret

gettime:
	push	bx
	
	if	cc
	mov	dx,bx
	SVC	T_SECONDS
	mov	bx,offset sixty_off
	les	si,[bx]
	pop	bx
	mov	al,60
	sub	al,es:[si]
	mov	tod_ssec,al
	endif

	if	pcdos
	SVC	2ch		;get time call
	pop	bx		;get back address to stuff it
	mov	al,ch		;convert hours to BCD
	call	c_bcd
	mov	tod_hour,al	;and store it
	mov	al,cl		;convert minutes to BCD
	call	c_bcd
	mov	tod_min,al
	mov	al,dh		;convert seconds to BCD
	call	c_bcd
	mov	tod_sec,al	;store it away

	mov	al,dl		;get hundredths
	xor	ah,ah		;zap high byte
	mov	cl,60		;multiply by sixty
	mul	cl		;now we have 60*hundredths
	mov	cl,100		;prep to divide by 100
	div	cl		;do it.
	mov	tod_ssec,al	;and stuff it all away
	endif
	ret

	if	pcdos		;if (and only if) PCDOS
c_bcd:	xor	ah,ah		;clear high byte
	push	bx		;save bx for later
	mov	bx,ax		;copy into BX for index into REVOLTING
	mov	al,cs:BCD_TBL[bx] ;HORRIBLE BCD CONVERSION TABLE!!!
	pop	bx		;restore bx
	ret			;and return
	
			;This is revolting. but fast.
BCD_TBL	db	0h, 1h, 2h, 3h, 4h, 5h, 6h, 7h, 8h, 9h
	db	10h, 11h, 12h, 13h, 14h, 15h, 16h, 17h, 18h, 19h
	db	20h, 21h, 22h, 23h, 24h, 25h, 26h, 27h, 28h, 29h 
	db	30h, 31h, 32h, 33h, 34h, 35h, 36h, 37h, 38h, 39h 
	db	40h, 41h, 42h, 43h, 44h, 45h, 46h, 47h, 48h, 49h
	db	50h, 51h, 52h, 53h, 54h, 55h, 56h, 57h, 58h, 59h, 60h

	endif



prtime:
	mov	al,tod_hour
	call	prbcd
	mov	al,':'
	call	print
	mov	al,tod_min
	call	prbcd
	mov	al,':'
	call	print
	mov	al,tod_sec
	call	prbcd
	mov	al,'.'
	call	print
	mov	al,tod_ssec
	mov	ah,0
	mov	cl,10
	mul	cl
	mov	cl,6
	mov	dx,0
	div	cl
	call	pd
	ret


prbcd:	push	ax
	mov	cl,4
	shr	al,cl
	call	phex2
	pop	ax
phex2:	and	al,15		;mask off high order
	add	al,'0'
	call	print
	ret

pd:	cmp	al,10
	jae	pdec
	push	ax
	mov	al,'0'
	call	print
	pop	ax
pdec:	push	ax	;standard, recursive, dec. print routine
	mov	ah,0
	mov	cl,10
	div	cl	;divide by 10
	or	al,al
	jz	pdec1
	call	pdec
pdec1:	mov	al,ah
	add	al,'0'
	call	print
	pop	ax
	ret

print:	push	bx
	push	dx
	mov	dl,al
	SVC	C_WRITE
	pop	dx
	pop	bx
	ret


init_proc:
	push	es
	mov	ax,physmem	;point to 32032 address space
	mov	es,ax
	mov	es:[2004h],0ffffh	;make sure not going anywhere

START:
; Initialize the Bus Flags port (at ioport)
; Bit 0 = Set DIAG vector
; Bit 1 = Set interrupt request to 32K
; Bit 2 = Pulse reset capacitor low
; Bit 3 = Inhibit refresh
;set the segment at pageport to 0
	MOV	AL,0
	MOV	DX,pageport
	OUT	DX,AL

	MOV	AL,00000101B	;Set DIAG with RESET
	MOV	DX,ioport	;the port
	out	dx,al		;set the reset/diag
	mov	cx,delay_m	;25 is about 80 usecs on PC
lx5:	loop	lx5		;wait
	mov	al,00000000b	;reset the DIAG and RESET
	out	dx,al
	mov	cx,10		;wait a few usecs
lx6:	loop	lx6

;	MOV	AL,00000101B	;Don't remove DIAG vector yet
;	MOV	DX,ioport	;the bus control port
;
;	MOV	CX,0f0H		;index for loop below
;LP2:	OUT	DX,AL		;Pulse capacitor
;	LOOP	LP2		;lots of times (about 1.5 msec)
;
;	MOV	AL,00000000B	;Now remove the DIAG and do one more reset
;	OUT	DX,AL
;
;	mov	cx,5000		;wait for a bit
;	loop	$		;so processor can reset

	pop	es
	ret



sixty_off	dw	0
sixty_seg	dw	0

old_tod		db	6 dup (?)
new_tod		db	6 dup (?)
elapsed_tod	db	6 dup (?)

mess1	db	13,10,13,10,'Elapsed time: $'


op_ver	db	0		;operating system version
EXECT_	endp


GETSYS_	proc	near
	push	bp
	push	es
	push	si
	push	di
	push	bx
	push	cx
	push	dx
	push	ds
	if	cc
	cli
	mov	cl,154		;get system data area
	int	ccpm
	mov	al,es:byte ptr 4bh[bx]	;pick up system disk
	xor	ah,ah
	sti
	endif
	if	pcdos		;if PC DOS
	xor	ax,ax		;always search drive A
	endif
	pop	ds
	pop	dx
	pop	cx
	pop	bx
	pop	di
	pop	si
	pop	es
	pop	bp
	ret

GETSYS_	endp
codeseg	ends
