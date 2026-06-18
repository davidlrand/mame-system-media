/*************************************************************
	Define port addresses for i/o operations.
	These operations use the functions
	read(port) and write(port,data).
**************************************************************/

#define PIOA 0x80
#define PIOB 0x81
#define PIOC 0x82

#define PIODDRA 0x84
#define PIODDRB 0x85
#define PIODDRC 0x86

#define PIOMDR 0x87

#define PIOAC 0x88
#define PIOBC 0x89
#define PIOCC 0x8A

#define PIOAS 0x8C
#define PIOBS 0x8D
#define PIOCS 0x8E


#define DMAADD 0x20
#define DMARCA 0x20

#define DMACNT 0x21
#define DMARWC 0x21

#define DMAMOD 0x2B

#define DMAMCL 0x2D

#define DMAMSK 0x2F


#define EASIODR 0x00
#define EASICSD 0x00

#define EASIICR 0x01

#define EASIMR2 0x02

#define EASITCR 0x03

#define EASISER 0x04
#define EASICSB 0x04

#define EASISDS 0x05
#define EASIBSR 0x05

#define EASISDT 0x06
#define EASIIDR 0x06

#define EASISDI 0x07
#define EASIRPI 0x07
#define EASIEMR 0x07
#define EASIIMR 0x07
#define EASIISR 0x07


#define INTMSK 0xBB
