#include <stdio.h>
#include <portab.h>
#include <errno.h>
#include "unixdev.h"

char *Vers="UNIXPR.C Version 1.4  Thu Jul 17 19:07:54 1986";
/*
    Dave Rand
    72 Longfellow St
    Thousand Oaks, CA  91360
    805-493-1987
    
    Version 1.4  Thu Jul 17 19:07:54 1986
    
    This file does the high-level protocol for the UNIXIO multi channel
    interface. New devices can be added into the DoDev loop.
*/

static struct PBQ *ihead,*ilast;

static int done;
static struct {
    int  fch;
    long fpos;
} chan[21];


extern EXEC(),INT(),dodsi();

DOIO() {
    
    done = 0;
    EXEC();

    
    while (!done) {
	while (INT())
	    readnxt();
	if (ihead)
	    DoDev(ihead);
	if (!done)
	    DoPoll();
    }
}

readnxt() {
    struct PBQ *temp;
    char *myalloc();
    
    if (!INT())
return;

    temp = (struct PBQ *)myalloc(sizeof(struct PBQ));
    recv(&temp->pb,sizeof(struct PB));
    
    temp->next = (struct PBQ *) 0;
    
    if (ilast) {
	ilast->next = temp;
	ilast = temp;
    }
    if (!ihead) 
	ihead = ilast = temp;
#ifdef DEBUG
printf("RPB: rt=%c id=%3d src=%6lx dst=%6lx cnt=%5ld\n",temp->pb.rt,temp->pb.id,temp->pb.src,temp->pb.dst,temp->pb.cnt); 
#endif
}
    
DoPoll() {
    struct PB cpb;
    int x,t;
    char *temp;
    
    if (x = scank()) {
	cpb.rt = CompTY;
	cpb.id = CONDEV;
	cpb.cnt = 1L;
	cpb.dt1 = x;
	send(&cpb,sizeof(cpb));
    }
    for (x=0; x<NUMSER; x++) {
	if (Serial[x].status.errbyte & 0x0f1e){ /* We have a receiver alert */
	    cpb.rt = CompTY;
	    cpb.id = SERPOR1 + x;
	    cpb.cnt = -1L;			/* Indicated alert message */
	    memcpy(&cpb.dt1,&Serial[x],6);	/* Copy 6 bytes over */
	    send(&cpb,sizeof(cpb));
	    Serial[x].status.errbyte &= ~0x0f1e; /*Reset receiver status */
	}
	if ((t = Serial[x].icnt) > 0) {
	    if (t>6) t = 6;
	    cpb.rt = CompTY;
	    cpb.id = SERPOR1 + x;
	    cpb.cnt = t;
	    read_ser(&Serial[x],(char *)(&cpb.dt1),t);
	    send(&cpb,sizeof(cpb));
	}
	if ((t = Serial[x].ocnt) > 0) 
	    DoSerOut(&Serial[x],x);
    }
}

read_ser(ser,dest,cnt)
struct SERIAL *ser;
char *dest;
int cnt;
{
    int x,y,tcnt=cnt;
    if (ser->icnt < cnt)
return;
    
    for (x=ser->headi,y=0; cnt>0; cnt--) {
	dest[y++] = ser->ibuf[x++];
	if (x >= SERBUF)
	    x=0;
    }
    ser->headi = x;
    ser->icnt -= tcnt;
}

write_ser(ser,src,cnt)
struct SERIAL *ser;
char *src;
int cnt;
{
    int x,y,tcnt=cnt;

    for (x=ser->heado,y=0; cnt>0; cnt--) {
	if (ser->ocnt > (SERBUF - 1 )) {
	    while (ser->ocnt > (SERBUF - 2)) {
		if (INT()) 
		    readnxt();
		DoPoll();
	    }
	}
	ser->obuf[x++] = src[y++];
	if (x >= SERBUF)
	    x=0;
	(ser->ocnt)++;
    }
    ser->heado = x;
}
    
    

DoDev(rpb)
struct PBQ *rpb;
{
    int x;
    
#ifdef DEBUG
printf("RPB=rt=%c, RPB.id=%d, RPB.cnt=%d\n",rpb->pb.rt,rpb->pb.id,(int)rpb->pb.cnt);
#endif
    switch(rpb->pb.id) {
	case MEMDEV:
	    domem(&rpb->pb);
	    break;

	case DISK1:
	case DISK2:
	case DISK3:
	case DISK4:
	case DISK5:
	case DISK6:
	case DISK7:
	case DISK8:
	case DISK9:
	    dodisk(&rpb->pb);
	    break;

	case PDISK1:
	case PDISK2:
	case PDISK3:
	case PDISK4:
	case PDISK5:
	case PDISK6:
	case PDISK7:
	case PDISK8:
	case PDISK9:
	    dophys(&rpb->pb);
	    break;
	    
	case CONDEV:
	    docon(&rpb->pb);
	    break;

	case SERPOR1:
	case SERPOR2:
	    doser(&rpb->pb);
	    break;
	
	case LPT1:
	case LPT2:
	case LPT3:
	    dolpt(&rpb->pb);
	    break;
		
	case DSIDEV:
	    if (!(done = dodsi(&rpb->pb))) {
		rpb->pb.rt = CompTY;
		send(&rpb->pb,sizeof(struct PB));
	    }
	    break;

	case EXITDEV:
	    done = 1;
	    break;
	    
	default:
	    abort("Unknown device ID passed: %d",rpb->pb.id);
    }
    if (!(ihead = rpb->next)) 
	ilast = 0;
    
    free(rpb);
}

domem(rpb)
struct PB *rpb;
{
    switch(rpb->rt) {
	case ReadTY:
	    break;
	default:
	    abort("Illegal memory request type: %d",rpb->rt);
    }
}

dophys(rpb)
struct PB *rpb;
{
    int lch,lcnt,ret,lspt,drive,cyl,head,sec;
    char *tbuf,*myalloc();
    long taddr;
    
    lch = rpb->id;
    lcnt = (int)rpb->cnt;
    tbuf = myalloc(lcnt);
    lcnt /= 512;
    lspt = chan[lch].fch;	/* Get sectors per track */
    drive = lch - PDISK1;	/* Figure drive number */
    if (drive >= 4)
	drive = 0x80 + drive-4;

    switch(rpb->rt) {
	case ReadTY:
	    taddr = rpb->src / 512L;	/* Now have relative sector number */
	    cyl = taddr / lspt ; 	/* Cyl number */
	    sec = (int)taddr - (cyl * lspt)+1; /* Sector number */
	    head = cyl & 1;		/* Head is LSB of cyl */
	    cyl /= 2;			/* last, get true cylinder */
#ifdef DEBUG
printf("\nPHYSIO: About to read drive=%d, cyl=%d, head=%d, sec=%d, cnt=%d",
	drive,cyl,head,sec,lcnt);
#endif
	    ret = phread(drive,cyl,head,sec,lcnt,tbuf);
	    putdat(tbuf,rpb->dst,lcnt * 512);
	    break;
	    
	case WritTY:
	    getdat(rpb->src,tbuf,lcnt * 512);
	    taddr = rpb->dst / 512L;	/* Now have relative sector number */
	    cyl = taddr / lspt ; 	/* Cyl number */
	    sec = (int)taddr - (cyl * lspt)+1; /* Sector number */
	    head = cyl & 1;		/* Head is LSB of cyl */
	    cyl /= 2;			/* last, get true cylinder */
	    ret = phwrite(drive,cyl,head,sec,lcnt,tbuf);
	    break;
	    
	case InitTY:
	    ret = phgetp(drive,&lspt);	/* Return sectors per track */
	    chan[lch].fch = lspt;
	    switch(lspt) {
		case 9:
		    rpb->dt1 = 368640L;
		    break;
		case 15:
		    rpb->dt1 = 1228800L;
		    break;
		default:
		    rpb->dt1 = 0;
		    break;
	    }
	    break;
	
	default:
	    abort("Illegal PDISK I/O request, type %d\n",rpb->rt);
	    break;
    }
    free(tbuf);
    
    rpb->rt = CompTY;
    rpb->dt2 = ret;
#ifdef DEBUG
printf("ret = %d\n",ret);
#endif
    send(rpb,sizeof(struct PB));
}
    
    

dodisk(rpb)
struct PB *rpb;
{
    int open(),lch,lcnt,ret,lfch;
    long lseek();
    char *tbuf,*myalloc();
    
    lch = rpb->id;
    lcnt = (int)rpb->cnt;
    lfch = chan[lch].fch;
    tbuf = myalloc(lcnt+1);
#ifdef DEBUG
printf("DISKIO:r/w=%c, chan=%d, pos=%ld, cnt=%d, seek=%d\n",rpb->rt,lch,
	(rpb->rt==ReadTY) ? rpb->src : rpb->dst,lcnt,chan[lch].fpos == rpb->src);
#endif
    switch(rpb->rt) {
	case ReadTY:
	    if (chan[lch].fpos != rpb->src) 
		chan[lch].fpos = lseek(lfch,rpb->src,0);
	    ret = (read(lfch,tbuf,lcnt) != lcnt);
	    putdat(tbuf,rpb->dst,lcnt);
	    if (ret == 0)
		chan[lch].fpos += lcnt;
	    else
		chan[lch].fpos = lseek(lfch,0L,1);
	    break;
	    
	case WritTY:
	    if (chan[lch].fpos != rpb->dst) 
		chan[lch].fpos = lseek(lfch,rpb->dst,0);
	    getdat(rpb->src,tbuf,lcnt);
	    ret = (write(lfch,tbuf,lcnt) != lcnt);
	    if (ret == 0)
		chan[lch].fpos += lcnt;
	    else
		chan[lch].fpos = lseek(lfch,0L,1);
	    break;
	    
	case InitTY:
	    if (lfch)
		close(lfch);
	    else {
		getdat(rpb->src,tbuf,lcnt+1);
		ret = ((chan[lch].fch = open(tbuf,2)) == -1);
		chan[lch].fpos = 0L;
	    }
	    break;
	
	default:
	    abort("Illegal DISK I/O request, type %d\n",rpb->rt);
	    break;
    }
    free(tbuf);
    
    rpb->rt = CompTY;
    rpb->dt2 = ret;
    if (ret)
	rpb->dt1 = errno;
    else
	rpb->dt1 = 0;
    send(rpb,sizeof(struct PB));
}
		

docon(rpb)
struct PB *rpb;
{
    char *t,*temp,*myalloc();
    int dofree=1,lcnt;
    
    lcnt = (int)rpb->cnt;

#ifdef DEBUG
    printf("Console request: src=%lx, cnt=%d\n",rpb->src,lcnt);
#endif

    if (lcnt <=6 && rpb->src == -1L) {
	t = (char *)&rpb->dt1;
	dofree=0;
    } else {
	t = temp = myalloc(lcnt);
	getdat(rpb->src,temp,lcnt);
    }
    while (lcnt--)
	bdos(6,*t++);
    
    if (dofree) {
	free(temp);
	rpb->rt = CompTY;
	rpb->cnt = 0;
	send(rpb,sizeof(struct PB));
    }
}

doser(rpb)
struct PB *rpb;
{
    char *tbuf,*myalloc();
    int lcnt;
    
    switch(rpb->rt) {
	case WritTY:
	    lcnt = (int)rpb->cnt;
	    if (lcnt > 6 || rpb->src != -1L) {
		tbuf = myalloc(lcnt);
		getdat(rpb->src,tbuf,lcnt);
		write_ser(&Serial[rpb->id-SERPOR1],tbuf,lcnt);
		free(tbuf);
		rpb->rt = CompTY;
		rpb->cnt = 0L;
		send(rpb,sizeof(struct PB));
	    } else
		write_ser(&Serial[rpb->id-SERPOR1],&rpb->dt1,lcnt);
	    break;
	    
	case InitTY:
	    lcnt = rpb->id-SERPOR1;
	    memcpy(&Serial[lcnt],&(rpb->dt1),6);
	    DoSerInit(&Serial[lcnt],lcnt);
	    break;
	
	default:
	    abort("Illegal SERIAL I/O request, type %d\n",rpb->rt);
	    break;
    }
}
dolpt(){}

    
	
getdat(src,dst,cnt)
UWORD dst,cnt;
ULONG src;
{
    struct PB rpb;
    
    rpb.rt = WritTY;
    rpb.id = MEMDEV;
    rpb.src = src;
    rpb.dst = dst;
    rpb.cnt = cnt;
    
    send(&rpb,sizeof(struct PB));
    recv(dst,cnt);
}

putdat(src,dst,cnt)
UWORD src,cnt;
ULONG dst;
{
    struct PB rpb;
    
    rpb.rt = ReadTY;
    rpb.id = MEMDEV;
    rpb.src = src;
    rpb.dst = dst;
    rpb.cnt = cnt;
    
    send(&rpb,sizeof(struct PB));
    send(src,cnt);
}

    

phread(drive,cyl,head,sector,nsec,buf)
int drive,cyl,head,sector,nsec;
char *buf;
{
    int ret=1,retry;
    sector |= (cyl >> 2) & 0xc0;	/* Convert big cylinders */
top:
    for (retry=0; retry < 10 && ret; retry++) {
#asm
	mov	dl,4[bp]	;drive number
	mov	ch,6[bp]	;cylinder number
	mov	dh,8[bp]	;head number
	mov	cl,10[bp]	;sector number
    	mov	al,12[bp]	;read xx sectors
	mov	ah,02h		;read data
	mov	bx,14[bp]	;get buffer address
	int	13h		;do the i/o
	jc	error		;did we error?
	xor	ax,ax		;no, zero return
error:	mov	-2[bp],ax	;store error return
	jnc	err1		;if no error, just return
	mov	ah,0		;do a drive reset
	int	13h
	mov	bl,12[bp]	;get EOT value
	cmp	bl,8		;is it bigger than 8?
	jc	err1		;no, skip it
	push	ds		;save our ds for a second
	push	si		;and si
	xor	ax,ax		;point to zero segment
	mov	ds,ax
	lds	si,dword ptr [78h]	;get the stuff at 78
	mov	4[si],bl	;stuff new EOT in
	pop	si
	pop	ds

err1:
#endasm
    }
    return(ret);
}

phwrite(drive,cyl,head,sector,nsec,buf)
int drive,cyl,head,sector,nsec;
char *buf;
{
    int ret=1,retry;
    sector |= (cyl >> 2) & 0xc0;	/* Convert big cylinders */
    for (retry=0; retry < 10 && ret; retry++) {
#asm
	mov	dl,4[bp]	;drive number
	mov	ch,6[bp]	;cylinder number
	mov	dh,8[bp]	;head number
	mov	cl,10[bp]	;sector number
    	mov	al,12[bp]	;write xx sectors
	mov	ah,03h		;write one sector
	mov	bx,14[bp]	;get buffer address
	int	13h		;do the i/o
	jc	errorw		;did we error?
	xor	ax,ax		;no, zero return
errorw:	mov	-2[bp],ax	;store error return
	jnc	errw1		;no error
	mov	ah,0		;do a drive reset
	int	13h
	mov	bl,12[bp]	;get EOT value
	cmp	bl,8		;is it bigger than 8?
	jc	errw1		;no, skip it
	push	ds		;save our ds for a second
	push	si		;and si
	xor	ax,ax		;point to zero segment
	mov	ds,ax
	lds	si,dword ptr [78h]	;get the stuff at 78
	mov	4[si],bl	;stuff new EOT in
	pop	si
	pop	ds

errw1:
#endasm
    }
    return(ret);
}

phgetp(drive,spt)
int drive,*spt;
{
    char temp[512];
    int retry=0;
    
    do {
	if (phread(drive,0,0,1,1,temp) == 0) {
	    if (phread(drive,0,1,15,1,temp) == 0) 
		*spt = 15;
	    else
		*spt = 9;
	    return(0);		/* We found it... */
	}
#asm
    mov	dl,4[bp]		;get drive number
    xor	ax,ax			;do a reset command
    int	13h
#endasm
    } while (retry++ < 3);
    *spt = 1;			/* Unknown format/unreadable */
return(-1);
}

scank(){
#asm
    mov	ah,6		;do keyboard scan
    mov	dl,255
    int	21h
    jz	kret		;if no key, just return
    or	al,al		;if non-zero scan code, return now
    jnz	kret
    mov	ah,6		;else get the extended code
    mov	dl,255
    int 21h
    or	al,80h		;and set the high bit
kret:
    xor	ah,ah		;zap the high byte
#endasm
}

   