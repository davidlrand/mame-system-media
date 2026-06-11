codeseg	segment word public 'code'

	public	io_proc
	extrn	_ARGP_:near,_OPEN_:near,_CREA_:near
	extrn	_CLOS_:near,_READ_:near,_WRIT_:near,_SEEK_:near
	extrn	_TELL_:near,_UNLI_:near,_RENAM_:near,malloc_:near
	extrn	free_:near
	assume	cs:codeseg, ds:dataseg, es:nothing, ss:nothing

;****************************************************************
;*    I/O Processor interface with Concurrent CPM and PCDOS   	*
;*								*
;* Interfaces Concurrent with the Definicon 32032 board. Handles*
;* all I/O related task, and limited file management.		*
;*								*
;*								*
;*  Copyright (C) 1985 by Definicon Systems Inc.		*
;*								*
;*   Definicon Systems Inc. 					*
;*   23930 Craftsman Road,					*
;*   Calabasas. California. U.S.A				*
;*								*
;*   All Rights Reserved					*
;*								*
;* This software is furnished under a license and may be used   *
;* and copied only in accordance with the terms of such license *
;* and with the inclusion of the above copyright notice. This   *
;* software or any other copies thereof may not be provided or  *
;* otherwise made available to any other person. No title to and*
;* ownership of the software is hereby transferred.             *
;*								*
;* The information in this software is subject to change without*
;* notice and should not be construed as a commitment by	*
;* Definicon Systems Inc.					*
;*								*
;*--------------------------------------------------------------*
;*								*
;* 32000 MEMORY MAP						*
;*								*
;*	0000		BOOTSTRAP CODE				*
;*	0020		I/O KERNEL MODULE TABLE			*
;*	0030		MONITOR/DEBUGGER MODULE TABLE		*
;*	0080		PROGRAM'S MODULE TABLE(S)		*
;*	2000		I/O SPACE FOR 8086/32000 COMMUNICATION	*
;*	3000		KERNEL					*
;****************************************************************

	include	32kh.inc
	include	mcdos.equ

io_proc proc	near
	mov	es_reg,es	;store ES register, so we can call C

	if	inter		;if interrupts on
	cli			;interrupts off
	call	resetint
	push	ds		;save the data segment
	push	si
	push	di		;save registers, so we can call C
	mov	ax,0		;point to the zero page
	mov	ds,ax		;with the DS
	push	cs
	pop	es		;point the ES at the local segment
	mov	si,intadr	;point to the physical interrupt address
	mov	di,offset savint	;point to the save area
	mov	cx,2		;move two words
	rep	movsw		;from ds->si to es->di
	sub	si,4		;point back to the physical interrupt
	mov	2[si],cs	;store the code segment
	mov	[si],offset INT_SERVICE  ;store the offset
	mov	al,20h		;juse for the hell of it
	out	20h,al		;do a specific EOI for this channel
	in	al,21h		;get the interrupt register
	and	al,intmask	;mask in the interrupt
	out	21h,al		;save the interrupt register
	mov	al,20h		;juse for the hell of it
	out	20h,al		;do a specific EOI for this channel
	pop	di		;restore registers
	pop	si		;so we can call C
	pop	ds		;restore local address
	sti			;allow interrupts
	endif
	

	mov	dx,pageport	;reselect page zero
	xor	al,al
	out	dx,al
	mov	ax,physmem
	mov	es,ax
	mov	es:word ptr [sw86],0	;make sure 8086 is null request
	mov	es:word ptr [sw32],1	;startup 32032

io1:	cmp	es:word ptr [sw32],0	;is it going?
	jnz	io1			;no, so try again

io_loop:
;	call	keyboard		;get a keyboard key
;	call	video			;display video info

	if	inter
	call	waitint			;wait for interrupt
	endif


	mov	bx,es:word ptr [sw86]	;get 8086 request
	if	not inter
	or	bx,bx			;if no request,
	jz	io_loop			;do a short poll
	endif

	if 	not inter
;	call	resetint		;reset interrupts
;	call	resetint		;reset interrupts
;	call	resetint		;reset interrupts
;	call	resetint		;reset interrupts
;	call	resetint		;reset interrupts
;	call	resetint		;reset interrupts
	endif

	cmp	bx,MAX_PROC		;are we over max
	jnc	bad			;yup - abort
	shl	bx,1			;shift left one, for word offset
	call	cs:word ptr vector[bx]	;call and process function
	
	if	inter
	in	al,21h			;get the interrupt register
	and	al,intmask		;mask in the interrupt
	out	21h,al			;save the interrupt register
	endif

	mov	es:word ptr [sw86],0	;null the service request

	jmp	io_loop			;and lop for another process

bad:	
io_exit:
	if	inter			;if doing interrupts
	cli				;stop it
	call	resetint		;reset external interrupts
	push	ds			;save the DS
	push	si			;save the SI and DI
	push	di			;registers so we can return to C
	mov	ax,0			;point to the interrupt page
	mov	es,ax			;with ES
	push	cs			;point at codeseg
	pop	ds			;with DS
	mov	si,offset savint	;restore the saved interrupt
	mov	di,intadr		;to the interrupt address
	mov	cx,2			;two words only
	rep	movsw
	pop	di
	pop	si			;restore registers for C
	pop	ds
	in	al,21h			;get the interrupt register
	or	al,not intmask		;mask out the interrupt
	out	21h,al			;save the interrupt register
	sti				;allow interrupts
	endif				;and that be it


	pop	ax			;pop extra return off
	ret				;and return

	if	inter

waitint:

	if	pcdos
	test	iflag,1			;have we an interrupt?
	jz	waitint			;no
	endif

	if	cc			;on CCPM, we can dispatch
	test	iflag,1			;is it set?
	jnz	wait1			;yup... go to it
	push	es			;prevent CPU resource hogging
	mov	cl,P_DISPATCH		;dispatch yourself today!
	int	ccpm			;do it
	pop	es
	jmp	waitint			;and loop for more
	endif
wait1:
	mov	iflag,0			;reset the interrupt
	ret				;yes!

	endif

resetint:
	push	es
	mov	ax,physmem	;point es: to the right area
	mov	es,ax
	mov	dx,pageport		;point to the page register
	mov	al,duart1		;select the duart page
	out	dx,al			;done the page select
	mov	al,00100000b		;set the duart bit
	mov	es:byte ptr [durt_f],al
	xor	al,al			;and point back to zero page
	out	dx,al
	pop	es
	ret


	if	inter

INT_SERVICE:
	cli
	push	ax
	push	dx
	mov	cs:iflag,1		;tell the world we got it
	call	resetint

	in	al,21h			;get the interrupt register
	or	al,not intmask		;mask out the interrupt
	out	21h,al			;save the interrupt register


;	if	CC
;	push	bx
;	push	cx
;	push	ds
;	push	es
;	push	si
;	push	di
;	mov	cl,dev_setflag		;set the flag
;	mov	dx,my_flag		;that we want
;	int	ccpm			;do it
;	cli
;	pop	di
;	pop	si
;	pop	es
;	pop	ds
;	pop	cx
;	pop	bx
;	endif

	mov	al,20h
	out	20h,al			;issue non-specific EOI
	pop	dx
	pop	ax
	sti
	iret


	endif

vector	dw	offset null		;null routine
	dw	offset bdos		;bdos generic routine
	dw	offset null		;null video
;	dw	offset video		;video routine
	dw	offset printer		;printer routine
	dw	offset disk_ser		;disk service
	dw	offset argrq		;Argument request
	dw	offset io_exit		;exit request
	dw	offset move_to		;move to 8086
	dw	offset move_from	;move to 32032
	dw	offset port_read	;Read from io port
	dw	offset port_write	;Write to io port
	dw	offset mem_alloc	;allocate local memory
	dw	offset mem_free		;free local memory

	dw	offset null		;one more null

MAX_PROC equ	((offset $) - (offset vector)) /2

null:	ret				;null routine, return

bdos:
	if	cc			;if on ccpm
	mov	cl,es:byte ptr [var1]	;put function code in cl
	endif
	if	pcdos			;if on pcdos
	mov	ah,es:byte ptr [var1]	;put function code in ah
	endif
	mov	dx,es:word ptr [var2]	;put dx arg into dx
	push	ds			;save the segments
	push	es
	if	pcdos			;if on pcdos
	int	21h			;do int 21
	endif
	if	cc			;if on ccpm
	int	224			;do int e0
	endif
	pop	es
	pop	ds
	mov	es:byte ptr [var1],al	;return al in var1
	ret				;and that be that!

printer:
	ret				;ignore the printer

disk_ser:
	mov	bx,es:word ptr [dskrw]	;get the disk code
	shl	bx,1			;shift left (*2)
	call	cs:word ptr dvect[bx]	;service the request
	mov	es:word ptr [dskrw],0	;zero the request
	ret				;and return

dvect	dw	offset null		;null code
	dw	offset open		;open a file
	dw	offset creat		;create a file
	dw	offset close		;close a handle
	dw	offset renam		;rename a handle
	dw	offset read		;read from a handle
	dw	offset write		;write to a handle
	dw	offset seek		;seek to location
	dw	offset tell		;tell current location
	dw	offset unlink		;unlink this file

open:	
	push	es
	push	es:word ptr [dskid]	;get mode
	push	es:word ptr [dskdta+2]	;push segment of filename
	push	es:word ptr [dskdta]	;push offset of filename
	mov	es,es_reg		;get back the C es reg
	call	_OPEN_			;process the open request.
	add	sp,6
	mov	bx,errno_		;get the errorno
	pop	es			;restore ES
	mov	es:word ptr [dskerr],bx	;put in error
	mov	es:word ptr [dskid],ax	;put in handle
	ret

creat:
	push	es
	push	es:word ptr [dskid]	;get mode
	push	es:word ptr [dskdta+2]	;push segment of filename
	push	es:word ptr [dskdta]	;push offset of filename
	mov	es,es_reg		;get back the C es reg
	call	_CREA_			;process the open request.
	add	sp,6
	mov	bx,errno_		;get the errorno
	pop	es			;restore ES
	mov	es:word ptr [dskerr],bx	;put in error
	mov	es:word ptr [dskid],ax	;put in handle
	ret				;and return

close:
	push	es
	push	es:word ptr [dskid]	;get the current disk ID
	mov	es,es_reg		;get back the C es reg
	call	_CLOS_			;process the arg request.
	add	sp,2
	mov	bx,errno_		;get the errorno
	pop	es			;restore ES
	mov	es:word ptr [dskerr],bx	;put in error
	ret				;and return

read:
	push	es
	push	es:word ptr [dskl]	;push the max number of bytes
	push	es:word ptr [dskdta+2]	;push the "segment" of the dta
	push	es:word ptr [dskdta]	;push the "offset" of the dta
	push	es:word ptr [dskid]	;push the disk ID
	mov	es,es_reg		;get back the C es reg
	call	_READ_			;process the arg request.
	add	sp,8
	mov	bx,errno_		;get the errorno
	pop	es			;restore ES
	mov	es:word ptr [dskerr],bx	;put in error
	mov	es:word ptr [dskrc],ax	;save return code
	ret				;and return
	
write:
	push	es
	push	es:word ptr [dskl]	;push the max number of bytes
	push	es:word ptr [dskdta+2]	;push the "segment" of the dta
	push	es:word ptr [dskdta]	;push the "offset" of the dta
	push	es:word ptr [dskid]	;push the disk ID
	mov	es,es_reg		;get back the C es reg
	call	_WRIT_			;process the arg request.
	add	sp,8
	mov	bx,errno_		;get the errorno
	pop	es			;restore ES
	mov	es:word ptr [dskerr],bx	;put in error
	mov	es:word ptr [dskrc],ax	;save return code
	ret				;and return
	
seek:
	push	es
	push	es:word ptr [dskl]	;push the mode of the seek
	push	es:word ptr [dskdta+2]	;push the LSW offset of the file
	push	es:word ptr [dskdta]	;push the MSW offset of the file
	push	es:word ptr [dskid]	;push the disk ID
	mov	es,es_reg		;get back the C es reg
	call	_SEEK_			;process the arg request.
	add	sp,8
	mov	cx,errno_		;get the errorno
	pop	es			;restore ES
	mov	es:word ptr [dskerr],cx	;put in error
	mov	es:word ptr [dskdta],ax	;save return code
	mov	es:word ptr [dskdta+2],dx;save rest of return code
	ret				;and return
	

tell:
	push	es
	push	es:word ptr [dskid]	;push the disk ID
	mov	es,es_reg		;get back the C es reg
	call	_TELL_			;process the arg request.
	add	sp,2
	mov	cx,errno_		;get the errorno
	pop	es			;restore ES
	mov	es:word ptr [dskerr],cx	;put in error
	mov	es:word ptr [dskdta],ax	;save return code
	mov	es:word ptr [dskdta+2],dx;save rest of return code
	ret				;and return
	
unlink:
	push	es
	push	es:word ptr [dskl]	;push the length
	push	es:word ptr [dskdta+2]	;push the "segment" of the dta
	push	es:word ptr [dskdta]	;push the "offset" of the dta
	mov	es,es_reg		;get back the C es reg
	call	_UNLI_			;process the arg request.
	add	sp,6
	mov	bx,errno_		;get the errorno
	pop	es			;restore ES
	mov	es:word ptr [dskerr],bx	;put in error
	mov	es:word ptr [dskrc],ax	;save return code
	ret				;and return

renam:
	push	es
	push	es:word ptr [var1+2]	;push the first argument
	push	es:word ptr [var1]
	push	es:word ptr [var2+2]	;push the second filename
	push	es:word ptr [var2]
	mov	es,es_reg		;get back the C es reg
	call	_RENAM_			;go rename it
	add	sp,8
	mov	bx,errno_		;get the errorno
	pop	es			;restore ES
	mov	es:word ptr [dskerr],bx	;put in error
	mov	es:word ptr [dskdta],ax	;store the return code
	mov	es:word ptr [dskdta+2],0	;and zap the high word
	ret				;that be it

argrq:
	push	es
	push	es:word ptr [dskdta+2]	;push the "segment" of the dta
	push	es:word ptr [dskdta]	;push the "offset" of the dta
	push	es:word ptr [dskl]	;push the max number of bytes
	mov	es,es_reg		;get back the C es reg
	call	_ARGP_			;process the arg request.
	add	sp,6
	pop	es			;restore ES
	ret				;and return


move_to:
	push	es			;save our segments
	push	ds			;we are going to change this
	mov	si,es:word ptr [var2]	;get low part of destination address
	mov	ax,es:word ptr [var2+2]	;get high part of destination
	mov	di,si			;copy into DI
	and	di,15			;mask all but last four bits off
	mov	cx,4			;prep to shift right 4
mov21:	shr	ax,1			;shift MSW 1
	rcr	si,1			;shift LSW 1
	loop	mov21			;and loop for rest
	mov	ax,si			;save segment address of destination
	mov	si,es:word ptr [var1]	;get LSW pointer
	push	es:word ptr [var1+2]	;MSW pointer to 32032 memory space
	mov	cx,es:word ptr [var3]	;get count
	mov	es,ax			;ES->DI points to destination
	
	mov	ax,physmem		;point to physical memory
	mov	ds,ax			;DS->SI almost points to right place
	mov	dx,pageport		;select correct 32032 page
	pop	ax			;get MSW pointer
	out	dx,al			;select right page now
	mov	curport,al		;save a copy of current page for later
	mov	ax,si			;see if we are going to wrap
	add	ax,cx			;by adding count to current source
	jnc	move_x1			;no wrap, is ok
	jz	move_x1			;exact transfer, is ok
	push	ax			;else save new transfer count
	sub	cx,ax			;transfer as much as possible
	MOVEBYTES			;with this rep
	mov	al,curport		;point to the next segment
	inc	al			;of 32032 memory
	out	dx,al			;and continue the transfer
	pop	cx			;with the new count
move_x1:MOVEBYTES			;here if it was ok
	mov	dx,pageport		;always re-select page zero
	xor	al,al			;so communications is OK
	out	dx,al
	pop	ds
	pop	es
	ret


move_from:
	push	es			;save our segments
	push	ds			;we are going to change this
	mov	di,es:word ptr [var1]	;get low part of source address
	mov	ax,es:word ptr [var1+2]	;get high part of source
	mov	si,di			;copy into DI
	and	si,15			;mask all but last four bits off
	mov	cx,4			;prep to shift right 4
movf1:	shr	ax,1			;shift MSW 1
	rcr	di,1			;shift LSW 1
	loop	movf1			;and loop for rest
	mov	ds,di			;save segment address of destination
	mov	di,es:word ptr [var2]	;get LSW pointer
	push	es:word ptr [var2+2]	;MSW pointer to 32032 memory space
	mov	cx,es:word ptr [var3]	;get count
	
	mov	ax,physmem		;point to physical memory
	mov	es,ax			;ES->DI almost points to right place
	mov	dx,pageport		;select correct 32032 page
	pop	ax			;get MSW pointer
	out	dx,al			;select right page now
	mov	curport,al		;save a copy of current page for later
	mov	ax,di			;see if we are going to wrap
	add	ax,cx			;by adding count to current source
	jnc	move_x2			;no wrap, is ok
	jz	move_x2			;exact transfer, is ok
	push	ax			;else save new transfer count
	sub	cx,ax			;transfer as much as possible
	MOVEBYTES			;with this rep
	mov	al,curport		;point to the next segment
	inc	al			;of 32032 memory
	out	dx,al			;and continue the transfer
	pop	cx			;with the new count
move_x2:MOVEBYTES			;here if it was ok
	mov	dx,pageport		;always re-select page zero
	xor	al,al			;so communications is OK
	out	dx,al
	pop	ds
	pop	es
	ret

port_read:
	mov	dx,es:word ptr [var1]	;get the requested port location
	xor	ah,ah			;zap the high byte
	in	al,dx			;get the data
	mov	es:word ptr [var2],ax	;stuff it back
	ret

port_write:
	mov	dx,es:word ptr [var1]	;get the port location
	mov	ax,es:word ptr [var2]	;get the data value
	out	dx,al			;send it out
	ret				;and return
	
mem_alloc:
	push	es			;save es for a bit
	push	es:word ptr [var1]	;put size of request on stack
	mov	es,es_reg		;get back the C es reg
	call	malloc_			;call the C memory allocator
	add	sp,2			;unstack
	pop	es			;get back es
	mov	es:word ptr [var1],ax	;return memory pointer
	mov	es:word ptr [var1+2],ds	;and dseg pointer
	ret				;that be it


mem_free:
	push	es			;save es for a bit
	push	es:word ptr [var1]	;put pointer on stack
	mov	es,es_reg		;get back the C es reg
	call	free_			;call the C memory allocator
	add	sp,2			;unstack
	pop	es			;get back es
	mov	es:word ptr [var1],ax	;return free status
	mov	es:word ptr [var1+2],0	;extend to integer
	ret				;that be it






es_reg	dw	0
iflag	db	0
curport	db	0

savint	dw	0
	dw	0


io_proc endp
codeseg	ends


dataseg	segment word public 'data'
	extrn	errno_:word
dataseg	ends
