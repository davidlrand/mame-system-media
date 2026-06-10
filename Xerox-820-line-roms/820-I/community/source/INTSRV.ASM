 	PAGE
;********************************************************
;*							*
;*	INTERRUPT SERVICE ROUTINES FOR KEYBOARD 	*
;*	INPUT AND REAL-TIME CLOCK FUNCTIONS		*
;*							*
;*	XEROX 820 VERSION 1.0	 10-OCT-80		*
;*		  VERSION 2.0	 21-JULY-81		*
;*							*
;********************************************************

KBDST:	LD	A,(FIFCNT)	;GET INPUT FIFO BYTECOUNT
	OR	A		;TEST IF EQUAL ZERO
	RET	Z		;EXIT WITH A=0 IF QUEUE IS EMPTY
	LD	A,255
	RET			;ELSE SET A=255 TO INDICATE DATA READY

KBDIN:	CALL	KBDST
	JR	Z,KBDIN 	;LOOP UNTIL KEYBOARD INPUT READY
	PUSH	HL
	CALL	REMOVE		;GET CHARACTER FROM INPUT QUEUE
	POP	HL
	RET

STASH3: LD	C,A
	LD	HL,FIFCNT	;BUMP INPUT FIFO CHARACTER COUNT
	LD	A,(HL)
	INC	A
	CP	16
	RET	NC		;EXIT NOW IF FIFO IS FULL
	LD	(HL),A		; ELSE INCREMENT FIFO COUNT
	LD	HL,FIFIN	;POINT HL TO FIFO INPUT OFFSET
	CALL	INDEX
	LD	(HL),C		;STORE CHARACTER IN FIFO @ HL
	RET

REMOVE: LD	HL,FIFCNT
	DEC	(HL)
	LD	HL,FIFOUT	;POINT HL TO FIFO OUTPUT OFFSET
INDEX:	LD	A,(HL)
	INC	A
	AND	00001111B	;INCREMENT FIFO POINTER
	LD	(HL),A		; MODULO 16 AND REPLACE
	LD	HL,FIFO
	ADD	A,L		;INDEX INTO FIFO BY OFFSET IN A
	LD	L,A
	LD	A,(HL)
	RET

;	SOFTWARE DISK MOTOR TURN-OFF TIMER ROUTINE

DSKTMR: LD	HL,MOTOR	;DECREMENT DISK TURN-OFF TIMER
	DEC	(HL)
	RET	NZ		;EXIT IF NOT TIMED OUT YET
	IN	A,(BITDAT)
	AND	11111000B	;DISABLE ALL DRIVE SELECTS AND
	OUT	(BITDAT),A	; TURN OFF THE SPINDLE MOTORS
	RET

;	-- INTERRUPT SERVICE ROUTINE FOR PARALLEL KEYBOARD --

KEYSRV: LD	(SPSAVE),SP	;SAVE USER STACK POINTER AND
	LD	SP,TMPSTK+32	; SWITCH TO LOCAL STACK
	PUSH	HL
	PUSH	DE
	PUSH	BC
	PUSH	AF		;SAVE MACHINE STATE
	IN	A,(KBDDAT)	;READ KEYBOARD INPUT PORT
	CPL
	AND	01111111B
	CALL	STASH3
	LD	A,0FFH 		;RESTORE VIDEO COUNTER
	LD	(VIDCNT),A
	IN	A,(BITDAT)	;RESTORE VIDEO
	OR	00100000B
	OUT	(BITDAT),A
	POP	AF
	POP	BC
	POP	DE
	POP	HL
	LD	SP,(SPSAVE)
	EI			;RE-ENABLE INTERRUPTS AND RETURN
	RETI

;	-- INTERRUPT SERVICE ROUTINE FOR ONE SECOND TIMER --

TIMER:	LD	(SPSAVE),SP	;SAVE USER STACK POINTER AND
	LD	SP,TMPSTK+32	; SWITCH TO LOCAL STACK
	PUSH	HL
	PUSH	DE
	PUSH	BC
	PUSH	AF
	CALL	CLOCK		;GO SERVICE THE CLOCK
	CALL	DSKTMR		;GO SERVICE THE DISK TURN OFF TIMER
	CALL	VIDTMR		;GO SERVICE THE VIDEO TIMER
	POP	AF
	POP	BC
	POP	DE
	POP	HL
	LD	SP,(SPSAVE)
	EI			;RE-ENABLE INTERRUPTS AND RETURN
	RETI

MILLI:	LD	(SPSAVE),SP	;SAVE USER STACK POINTER AND
	LD	SP,TMPSTK+32	; SWITCH TO LOCAL STACK
	PUSH	HL
	PUSH	AF
	LD	HL,(INDTMR)
	DEC	HL		;DECREMENT INDEX PERIOD TIMER
	LD	(INDTMR),HL
	POP	AF
	POP	HL
	LD	SP,(SPSAVE)
	EI
	RETI

	PAGE
;********************************************************
;*                                                      *
;*   CLOCK FOR BIGBOARD WITH PFM-80 VERSION 3.3         *
;*                                                      *
;*   2/DEC/80            RUSSELL SMITH                  *
;*                                                      *
;*   MODIFIED BY DON RETZLAFF - JANUARY 23, 1980        *
;*                                                      *
;*   TO INCLUDE SUPPORT OF DAY, MONTH, AND YEAR         *
;*                                                      *
;*   MODIFIED BY JIM MAYHUGH -11 JAN 84                 *
;*                                                      *
;*   TO RUN ON XEROX 820-1                              *
;*                                                      *
;********************************************************
;
CLOCK:	  LD   HL,SECS
       CALL INCTIM       ;INCREMENT TIME IN BINARY
       LD   A,(CLKEN)	 ;GET THE CLOCK DISPLAY ENABLE BYTE
       OR   A		 ;TEST IT
       RET  NZ		 ;RETURN IF CLOCK DISABLE
       LD   A,(BASE)     ;GET LINE # OF BOTTOM OF SCREEN
       INC  A            ;ADD 1 TO WRAP ARND TO TOP LINE
       CP   24
       JR   C,CLOCK2   ;WATCH FOR MODULO 24
       XOR  A
CLOCK2:	 SRL  A            ;TRNSFRM LINE # INTO 16 BIT ADRS
       LD   L,70*2       ; WITH COL # COMPONENT = 70
       RR   L
       LD   DE,CRTMEM    ;FIND PLACE IN SCREEN MEMORY
       OR   D
       LD   H,A
       IN   A,(BITDAT)
       SET  7,A
       OUT  (BITDAT),A
       LD   DE,HRS       ;POINT DE TO CLOCK HOURS
       LD   (HL),' '
       CALL PUTDEC       ;DISPLAY HOURS
       LD   (HL),':'
       INC  HL
       CALL PUTDEC       ;DISPLAY MINUTES
       LD   (HL),':'
       INC  HL
       CALL PUTDEC       ;DISPLAY SECONDS
       LD   (HL),' '
       IN   A,(BITDAT)
       RES  7,A
       OUT  (BITDAT),A
       RET		;RETURN
;
;
;;  PUT HRS, MINS, OR SEC ON SCREEN
;
PUTHRS:	 LD   A,(DE)
       CP   0            ;CHECK IF 0
       JP   NZ,PUTD0     ;NO - OUTPUT IT
       LD   A,24         ;YES - MAKE IT 24
       JR   PUTD0
PUTDEC:	 LD   A,(DE)
PUTD0:	  INC  DE
       LD   C,0
PUTD1:	  SUB  10
       JR   C,PUTD2
       INC  C
       JR   PUTD1
PUTD2:	  ADD  A,10
       PUSH AF
       LD   A,C
       CALL PUTDIG       ;DISPLAY 10'S DIGIT OF TIME
       POP  AF
PUTDIG:	 OR   '0'          ;MAKE MSB OF ACC INTO ASCII
       LD   (HL),A
       INC  HL           ;STORE CHAR AND BUMP POINTER
       RET
;
;;  INCREMENT TIME ROUTINE
;
INCTIM:	 INC  (HL)
       LD   A,(HL)       ;BUMP CLOCK SECS AND CHECK FOR
       CP   60           ; ROLL-OVER AT END OF MUNUTE
       RET  C            ;EXIT IF NO CARRY TO MINUTES
       LD   (HL),0       ;ELSE RESET SECONDS TO ZERO
       DEC  HL           ; AND POINT NEXT TO MINUTES
       INC  (HL)
       LD   A,(HL)       ;BUMP CLOCK MINS AND CHECK FOR
       CP   60           ; ROLL OVER AT END OF HOUR
       RET  C            ;EXIT IF NO CARRY INTO HOURS
       LD   (HL),0       ;ELSE RESET MINUTES TO ZERO
       DEC  HL           ; AND POINT NEXT TO HOURS
       INC  (HL)
       LD   A,(HL)       ;BUMP HOURS AND CHECK FOR
       CP   24           ; ROLL OVER AFTER 24 HOURS
       RET  C            ;EXIT IF NO ROLL OVER
       LD   (HL),0       ;ELSE RESET HOURS TO 0 AND
       DEC  HL           ; AND POINT NEXT TO DAYS
       INC  (HL)         ;BUMP DAYS
       LD   IX,DAYTAB    ;GET ADRS OF DAY TABLE
       LD   A,(MONTH)    ;GET MONTH
       DEC  A            ;DECREMENT DISPLACEMENT
       LD   (MNUM),A     ;PUT MONTH # IN INSTRUCTION
       LD   A,(DAY)      ;GET DAY #
       CP   (IX+0)       ;COMPARE DAY TO TABLE
MNUM   EQU  $-1
       RET  M            ;RETURN IF NO ROLL OVER REQUIRED
       LD   (HL),1       ;SET DAY TO 1
       DEC  HL           ; AND POINT NEXT TO MONTH
       INC  (HL)         ;BUMP MONTH
       LD   A,(HL)       ;GET MONTH
       CP   13           ;CHECK IF >12
       RET  C            ;RETURN IF NO CARRY
       LD   (HL),1       ;RESET MONTH TO 1
       DEC  HL           ;POINT TO YEAR
       INC  (HL)         ;INCREMENT YEAR
       RET               ;RETURN
;
;;  TABLE OF #DAYS+1 PER MONTH
;
DAYTAB:	 DEFB 32           ;JAN
       DEFB 29           ;FEB
       DEFB 32           ;MAR
       DEFB 31           ;APR
       DEFB 32           ;MAY
       DEFB 31           ;JUN
       DEFB 32           ;JUL
       DEFB 32           ;AUG
       DEFB 31           ;SEP
       DEFB 32           ;OCT
       DEFB 31           ;NOV
       DEFB 32           ;DEC

;	VIDEO ON/OFF ROUTINE

VIDTMR:	LD	A,(VIDCNT)	;GET THE VIDEO COUNT
	DEC	A		;DECREMENT IT
	LD	(VIDCNT),A	;REPLACE IT
	RET	NZ		;RETURN IF NOT TIMED OUT
	IN	A,(BITDAT)	;GET THE CONTROL DATA
	AND	11011111B	;TURN OFF THE VIDEO
	OUT	(BITDAT),A
	RET			;ALL DONE

;	POLLED MODE I/O ROUTINES FOR SIO CHANEL B

SIOST:	IN	A,(SIOCPB)	;GET SIO STATUS REGISTER
	AND	00000001B
	RET	Z		;ACC=0 IF NO DATA AVAILABLE
	LD	A,255
	RET

SIOIN:	CALL	SIOST		;TEST CONSOLE STATUS
	JR	Z,SIOIN 	;LOOP UNTIL DATA IS
	IN	A,(SIODPB)	; READY AT SIO DATA PORT
	AND	01111111B
	RET

SIOOUT: CALL	SIOST
	JR	Z,SIOX1
	CALL	SIOIN
	CP	XOFF
	JR	NZ,SIOX1
SIOHLD:	CALL	SIOIN
	CP	XON
	JR	NZ,SIOHLD
SIOX1:	IN	A,(SIOCPB)
	AND	00000100B	;TEST TBE STATUS BIT
	JR	Z,SIOX1 
	LD	A,C
	OUT	(SIODPB),A	;OUTPUT DATA TO SIO
	RET

