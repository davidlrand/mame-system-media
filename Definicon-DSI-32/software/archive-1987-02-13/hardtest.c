#include "stdio.h"
#include "ctype.h"
#ifdef NOTDEF
#include "duart.h"
#endif
#define STARTAT 8  /* please start at 2400 baud */
#define VISUAL 0   /* Turn this on to see each char as it is xmitted */
int Duart_Base;
main() {
    
    Duart_Base = *(int *)0x2058;
    if (Duart_Base == 0)
	Duart_Base = 0x200100;
	
    printf("DSI-32 Hardware Test Program, Version 1.2\n");
    
#ifdef NOTDEF
    init();
    
    loopback(TRUE); /* Set mode to internal loopback */
    
    printf("Please wait - hardware being tested.\n");
    
    printf("Testing interface.\n");
    testall(); /* Test all baud rates */
#endif
    memtest();
    
#ifdef NOTDEF    
    loopback(FALSE); /* Set mode to external loopback */
#endif
    printf("\nHardware Validated\n");
}
#ifdef NOTDEF
testall() {
    int x,y;
    for (y=0; y<2; y++) {
	for (x=STARTAT; x<NUMBAUD; x++) {
	    test(baudrate[y][x],y,0);
	    test(baudrate[y][x],y,1);
	}
    }
}
    
int test(baud,mode,port)
int baud,mode,port;
{
    int x,t;
	    
    chbaud(baud,mode,port);
    for (x=0; x<256; x++) {
	out(x,port);
#if VISUAL
	printf("Sending %d\r",x);
#endif
	if ((t = inp(port)) != x) {
	    printf("Port %d Bad. Was %d, should be %d\n",port,t,x);
	    while (inp(port) != -1) 
		;
return(TRUE); 
	}
    }
#if VISUAL
    printf("\n");
#endif
		
    return(FALSE);
}
int chbaud(baud,mode,port)
int baud,mode,port;
{
    int x,csr;
    
    csr = -1;
    for (x=0; x<NUMBAUD; x++) {
	if (baud==baudrate[mode][x]) {
	    csr = x;
	    break;
	}
    }
    
    if (csr == -1)
return(TRUE);
	
    csr |= csr << 4; /* setup both rx and tx */
    
    if (port == 0) 
	DURT_1 = csr;
    else
	DURT_9 = csr;
	
    DURT_4 = (mode << 7) | 0x10; /* counter/timer mode */
    
    return(FALSE);
}
out(byte,port)
UBYTE byte;
int port;
{
    int x;
    
    if (port==0) {
	for (x=0; x<OTIMEOUT; x++)
	    if ((DURT_1) & 0x4)
		break;
	DURT_3 = byte;
    } else {
	for (x=0; x<OTIMEOUT; x++)
	    if ((DURT_9) & 0x4)
		break;
	DURT_B = byte;
    }
}
int inp(port)
int port;
{
    int x,byte;
    
    if (port == 0) {
	for (x=0; x<ITIMEOUT; x++)
	    if ((DURT_1) & 1)
		break;
	if (x == ITIMEOUT)
	    byte = -1;
	else
	    byte = DURT_3;
    } else {
	for (x=0; x<ITIMEOUT; x++)
	    if ((DURT_9) & 1)
		break;
	if (x == ITIMEOUT)
	    byte = -1;
	else
	    byte = DURT_B;
    }
    
    return(byte);
}
    
loopback(well)
int well;
{
    int mr2;
    
    mr2 = 0x07; /* Normal mode, one stop */
    
    if (well)   /* We do want to loopback */
	mr2 |= 0x80; /* set to local loopback */
    
    DURT_0 = mr2;
    DURT_8 = mr2; /* set up both channels */
}
	
init() {
    
    DURT_2 = 0x10; /* Byte pointer reset */
    
    DURT_0 = 0x13; /* set to 8 bits, no parity */
    DURT_0 = 0x07; /* Set to no loopback, 1 stop */
    
    DURT_A = 0x10; /* Byte pointer reset */
	
    DURT_8 = 0x13; /* set to 8 bits, no parity */
    DURT_8 = 0x07; /* Set to no loopback, 1 stop */
    
    DURT_1 = 0xbb; /* set to 9600 baud, rx and tx on A */
    DURT_9 = 0xbb; /* set to 9600 baud, rx and tx on B */
    
    DURT_D = 0;	   /* Set 8 bit output port */
    
    DURT_4 = 0x10;  /* set interrupt input off, count from RXa */
    DURT_5 = 0x0;   /* set all interrupts off */
    DURT_2 = 0x05;  /* enable cha rx and tx */
    DURT_A = 0x05;  /* enable chb rx and tx */
}
#endif     
memtest() {
    register ULONG *low, *high;
    ULONG *lowmem = (ULONG *)0x2020, *highmem = (ULONG *)0x2024;
    
    low  = (ULONG *) *lowmem;
    high = (ULONG *) *highmem;
    high -= 256; /* Save Our Stacks! */
    
    printf("Testing memory from %X to %X\n",low,high);
    
    
    testm(low,high,0);
    testm(low,high,0xFFFFFFFF);
    testm(low,high,0x55AA55AA);
    testm(low,high,0xAA55AA55);
    testm(low,high,0x18811881);
    printf("Memory test complete\n");
}
testm(low,high,quad)
register ULONG *low, *high, quad;
{
    ULONG *test;
    printf("Testing pattern %X\n",quad);
    
    for (test = low; test < high; test++)
	*test = quad;
    
    for (test = low; test < high; test++)
	if (*test != quad)
	    error(test,quad);
}
error(test,quad)
ULONG *test,quad;
{
    printf("Memory failed. Address = %x, was %X, should be %X",test, *test,quad);
    exit(1);
}
    
    
    
