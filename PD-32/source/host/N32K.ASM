DEBUG	equ	0
dataseg	segment word public 'data'
	extrn	CPUtype_:word
cont1m	db	13,10,'Collision type 1!',13,10,'$'
cont2m	db	13,10,'Collision type 2!',13,10,'$'
	even
ptype	db	0		;packet type, R for Read, W for Write
plen	dw	0		;packet length
paddr1	dw	0		;low part of address
paddr2	db	0		;high part of address (on 32032)

paklen	equ	6

dataseg	ends

codeseg	segment word public 'code'
	public	MOVE_FROM_,MOVE_TO_,MOVE_ZTO_,INIT_,INT_,INPUT_,OUTPUT_,EXEC_
	public	send,recv,send_,recv_,UNINIT_
	extrn	readnxt_:near
;
; Dave Rand
; 72 Longfellow St.
; Thousand Oaks, CA  91360
; 805-493-1987
;
; Version 1.2  Tue Jul  8 13:12:39 1986
;
; This module does the low-level I/O for the initial ROM load, and the
; subsequent multi-channel UNIXIO support. It can be changed to used POLLED
; I/O, 8/16 bit I/O, or the 286 style I/O (REP INS) type mode.
;
;
	assume	cs:codeseg, ds:dataseg, es:dataseg, ss:nothing
	include mcdos.equ

	db	'Default port values follow -->'
IO32	dw	170h
RESET	dw	172h
INT32	dw	174h
INT86	dw	176h
	db	'<-- end of default values.'

POLL	equ	0

INPUT_	proc	near
	push	bp
	mov	bp,sp
	mov	dx,4[bp]	;get parameter for the xor
	in	al,dx		;read the data
	xor	ah,ah		;zap the high byte
	pop	bp		;and return
	ret
INPUT_	endp

OUTPUT_	proc	near
	push	bp
	mov	bp,sp		;prepare the bp
	mov	dx,4[bp]	;get the port
	mov	al,6[bp]	;get the data
	out	dx,al		;output it
	pop	bp		;and return
	ret
OUTPUT_	endp

MOVE_TO_	proc	near
	push	bp
	mov	bp,sp
	push	di
	push	si
	mov	si,4[bp]
	mov	di,6[bp]
	mov	ax,8[bp]
	mov	cx,10[bp]
	mov	paddr2,al	;stuff in high part of address
	mov	paddr1,di	;stuff low part of address
	mov	plen,cx		;stuff total length (less packet header)
	mov	ptype,'R'	;Tell the world we have a write

	push	cx
	push	si		;save old pointers
	mov	cx,paklen	;get packet length
	mov	si,offset ptype	;point to packet
	call	send		;send this packet
	pop	si
	pop	cx
	call	send		;and send the data
	pop	si
	pop	di
	pop	bp
	ret

MOVE_TO_	endp
MOVE_FROM_	proc	near
	push	bp
	mov	bp,sp
	push	di
	push	si
	mov	si,4[bp]
	mov	ax,6[bp]
	mov	di,8[bp]
	mov	cx,10[bp]
	mov	paddr2,al	;stuff in high part of address
	mov	paddr1,si	;stuff low part of address
	mov	plen,cx		;stuff total length (less packet header)
	mov	ptype,'W'	;Tell the world we have a read

	push	cx
	mov	cx,paklen	;get packet length
	mov	si,offset ptype	;point to packet
	call	send		;send this packet
	pop	cx
	call	recv		;and get the data
	pop	si
	pop	di
	pop	bp
	ret

MOVE_FROM_	endp

MOVE_ZTO_	proc	near
	push	bp
	mov	bp,sp
	push	di
	push	si
	mov	si,4[bp]
	mov	di,6[bp]
	mov	ax,8[bp]
	mov	cx,10[bp]
	mov	paddr2,al	;stuff in high part of address
	mov	paddr1,di	;stuff low part of address
	mov	plen,cx		;stuff total length (less packet header)
	mov	ptype,'R'	;Tell the world we have a write

	push	cx
	push	si		;save old pointers
	mov	cx,paklen	;get packet length
	mov	si,offset ptype	;point to packet
	call	send		;send this packet
	pop	si
	pop	cx
	call	zsend		;and send the zeros
	pop	si
	pop	di
	pop	bp
	ret

MOVE_ZTO_	endp


MOVE1_	proc	near
	push	bp
	mov	bp,sp
	push	di
	push	si

	mov	si,4[bp]
	mov	ax,6[bp]
	mov	di,8[bp]
	mov	cx,10[bp]
	mov	paddr2,al	;stuff high address
	mov	paddr1,si	;stuff low address
	mov	plen,cx		;total transfer length
	mov	ptype,'W'	;read please
	push	cx		;save current transfer length
	mov	si,offset ptype
	mov	cx,paklen
	call	send
	pop	cx
	call	recv		;get the real data
	pop	si
	pop	di
	pop	bp
	ret
MOVE1_	endp

send_	proc	near
	push	bp		;save the bp
	mov	bp,sp		;address the stack
	mov	cx,6[bp]	;get the count
	mov	si,4[bp]	;get the address
	call	send		;send the data
	pop	bp		;and return
	ret
send_	endp


send	proc	near
	cld
	shr	cx,1		;divide by two
	jnc	send1a		;if it was even, leave it alone
	inc	cx		;else one extra word
send1a:	mov	dx,INT86	;point to the interrupt
	in	al,dx		;get the interrupt
	test	al,1		;are we interrupting?
	mov	dx,INT32	;point to int 32 port
	jnz	send1b		;no, continue with loop
	push	si 
	push cx 		;save a few registers
	call	readnxt_	;process the interrupt
	pop cx
	pop si			;restore the registers
	jmp	send1a		;check to see if another interrupt occured

send1b:	out	dx,al		;wake up the 32000
send1c:	mov	dx,INT32	;point to int32 port
	in	al,dx		;did we get an interrupt?
	test	al,1
	jnz	send1e		;no, continue

	if	DEBUG
	push	dx
	push	cx
	mov	dx,offset cont1m
	mov	ah,9
	int	21h
	pop	cx
	pop	dx
	endif

	mov	dx,RESET	;point to poll port
	in	al,dx		;do we have data waiting?
	test	al,1
	jz	send1a		;we have interrupt, and data. 32k won

send1e:	mov	dx,IO32		;point to the data port
	lodsw			;pick up the first word
	out	dx,al		;send it out as bytes
	mov	al,ah
	out	dx,al
	dec	cx		;one less to go
	jcxz	sendx		;if we are done, exit
				;else poll until first byte is gone
sel1:	mov	dx,INT86	;point to int86 port
	in	al,dx		;do we have an interrupt?
	test	al,1		;test interrupt
	jz	col1		;collision. handle it
	mov	dx,RESET	;point to data available port
	in	al,dx		;get data available flag
	test	al,1		;is data available
	jnz	sel1a		;data is gone from port. Fill it up
	jmp	sel1		;back for more

col1:				;at this point we have a collision.
	if	DEBUG
	push	dx
	push	cx
	mov	dx,offset cont2m
	mov	ah,9
	int	21h
	pop	cx
	pop	dx
	endif
				;back off and handle it.
	inc	cx		;up one word
	sub	si,2		;drop back two bytes
	out	dx,al		;clear down interrupt
colw:	in	al,dx		;wait until we have the interrupt again
	test	al,1		;until this goes low
	jnz	colw		;loop
	jmp	send1a		;and handle it

sel1a:	mov	dx,IO32		;back to regular port
	cmp	CPUtype_,2	;are we running 8088 style?
	jb	sendold		;yes, do it

	db	0f3h		;rep
	db	6fh		;outs(w)
sendx:	ret

sendold:cmp	CPUtype_,1	;are we on 8088?
	jb	sendo1		;yes, do it
sendo2:
	if	POLL		;if we are gonna poll
	add	dx,2		;point to poll port
sol1:	in	al,dx		;get data available flag
	test	al,1		;is data available
	jz	sol1		;no, wait a bit
	sub	dx,2		;back to regular port
	endif
	lodsw			;get a word
	out	dx,ax		;send it out
	loop	sendo2		;back for more
	jmp	sendx

sendo1:
	if	POLL		;if we are gonna poll
	add	dx,2		;point to poll port
sol2:	in	al,dx		;get data available flag
	test	al,1		;is data available
	jz	sol2		;no, wait a bit
	sub	dx,2		;back to regular port
	endif
	lodsw
	out	dx,al
	mov	al,ah
	out	dx,al
	loop	sendo1		;back for more
	jmp	sendx
send	endp

zsend	proc	near
	cld
	shr	cx,1		;divide by two
	jnc	zsend1a		;if it was even, leave it alone
	inc	cx		;else one extra word
zsend1a:mov	dx,INT32	;point to int 32 port
	out	dx,al		;wake up the 32000
	mov	dx,IO32		;point to the data port
	xor	ax,ax		;zero data
	out	dx,al
	out	dx,al		;out it goes...
	dec	cx		;one less to go
	jcxz	zsendx		;if we are done, exit
				;else poll until first byte is gone
	add	dx,2		;point to poll port
zsl1:	in	al,dx		;get data available flag
	test	al,1		;is data available
	jz	zsl1		;no, wait a bit
	sub	dx,2		;back to regular port

zsend1:
	if	POLL		;if we are gonna poll
	add	dx,2		;point to poll port
zsel1:	in	al,dx		;get data available flag
	test	al,1		;is data available
	jz	zsel1		;no, wait a bit
	sub	dx,2		;back to regular port
	endif
	xor	ax,ax		;zero data
	out	dx,al		;send it out
	out	dx,al
	loop	zsend1		;back for more
zsendx:	ret
zsend	endp

INT_	proc	near
	mov	dx,INT86	;point to the interrupt
	in	al,dx		;get the interrupt
	and	al,1		;test the status
	xor	al,1		;reverse it (0=no int, 1=int)
	cbw			;extend it
	ret
INT_	endp

recv_	proc	near
	push	bp		;save the bp
	mov	bp,sp		;address the stack
	mov	cx,6[bp]	;get the count
	mov	di,4[bp]	;get the destination
	call	recv		;read the data
	pop	bp		;and return
	ret
recv_	endp

recv	proc	near
	cld
	mov	dx,INT86	;point to the interrupt area
recv1:	in	al,dx		;get the interrupt
	test	al,1		;look at them bits
	jnz	recv1		;wait until interrupt comes
	out	dx,al		;reset the flip flop when it comes
	mov	dx,IO32		;point to IO port
	shr	cx,1		;divide by two
	pushf			;save the flags
;This code waits for the first byte to come up, before
;doing the rep in instruction
	add	dx,2		;point to poll port
rcl1:	in	al,dx		;get data available flag
	test	al,1		;is data available
	jnz	rcl1		;no, wait a bit
	sub	dx,2		;back to regular port
	cmp	CPUtype_,2	;are we running a 8088?
	jb	rcold		;yes, do it the hard way
	db	0f3h		;rep
	db	6dh		;ins(w)
rcl2:	popf			;get back our flags
	jnc	rclx		;if no last byte, just exit
	in	al,dx		;else get the last word
	stosb			;and stuff it away.
	in	al,dx		;get true last byte
rclx:	ret

rcold:	cmp	CPUtype_,1	;are we on a 8088?
	jb	rcold1		;yes, do 8 bit i/o
rcold2:	jcxz	rcl2		;if we are done, exit now
	if	POLL		;if we are gonna poll
	add	dx,2		;point to poll port
rcl3:	in	al,dx		;get data available flag
	test	al,1		;is data available
	jnz	rcl3		;no, wait a bit
	sub	dx,2		;back to regular port
	endif
	in	ax,dx		;get the data
	stosw			;stuff it away
	dec	cx
	jmp	rcold2
	
	
rcold1:	jcxz	rcl2		;if we are done, exit now
	if	POLL		;if we are gonna poll
	add	dx,2		;point to poll port
rcl4:	in	al,dx		;get data available flag
	test	al,1		;is data available
	jnz	rcl4		;no, wait a bit
	sub	dx,2		;back to regular port
	endif
	in	al,dx		;get the data
	stosb			;store the byte
	in	al,dx		;get the rest of the data
	stosb
	dec	cx
	jmp	rcold1

recv	endp

dsval	dw	0



INIT_	proc	near

	mov	dx,RESET	;send out a reset
	out	dx,al		;command
	mov	cx,1000
initl:	loop	initl		;loop for a bit
	mov	dx,INT32	;remove the reset command
	out	dx,al		;and we are done
	mov	dx,INT86	;point to the interrupt area
init1:	in	al,dx		;get the interrupt
	test	al,1		;look at them bits
	jnz	init1		;wait until interrupt comes
	out	dx,al		;when it does, reset the flip flop.
	mov	dx,IO32		;point to IO port
	in	al,dx		;get the data
	xor	ah,ah		;zap high byte
	push	ax		;and save it
	add	dx,2		;point to poll port
	in	al,dx		;get data available flag
	test	al,1		;is data available
	jnz	initx		;no, exit now
	sub	dx,2		;back to regular port
	in	al,dx		;get data
initx:	pop	ax		;restore return value
	ret			;and return
INIT_	endp

UNINIT_	proc	near
	extrn	DoSerUn:near
    	call	DoSerUn
	ret
UNINIT_	endp

EXEC_	proc	near
	mov	ptype,'E'	;do an execute
	mov	cx,6
	mov	si,offset ptype
	call	send
	ret

EXEC_	endp


codeseg	ends

