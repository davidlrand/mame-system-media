 /********************************************************************
*								    *
*      This File contains the value of constants used in SCSI.C     *
*								    *
********************************************************************/

#define	TRUE	1
#define	FALSE	0
#define DONT_CARE 0
#define LED_ON 8

/****************************
*        SCSI Phases        *
****************************/

#define	DATA_OUT	0
#define DATA_IN		1
#define COMMAND_OUT	2
#define	STATUS_IN	3
#define	MESSAGE_OUT	6
#define	MESSAGE_IN	7

/****************************
*	Register bits	    *
****************************/

/*	TCR	*/

#define AS_IO	1
#define AS_CD	2
#define AS_MSG	4
#define REQ	8
#define CK_EEDMA	0x80

/*	ICR	*/

#define AS_DBUS	1		/* The prefix as_ should be read as assert */
#define AS_ATN	2
#define	AS_SEL	4
#define	AS_BSY	8
#define AS_ACK	0x10
#define	LA	0x20
#define MODE_N	00
#define MODE_E	0x40
#define	AIP	0x40
#define AS_RST	0x80

/*	MR2	*/

#define	EN_ARB		1	/* The prefix EN_ should be read as enable */
#define	EN_DMA		2
#define EN_BSYINT	4
#define EN_EOP		8
#define EN_PINT		0x10
#define EN_PCHK		0x20
#define TARGET		0x40
#define	EN_BLK		0x80

#define SINGLE_MODE_DMA EN_DMA | EN_EOP | TARGET | EN_PINT | EN_PCHK
#define BLK_MODE_DMA EN_DMA | EN_EOP | EN_BLK | TARGET | EN_PINT | EN_PCHK

/*	CSB	*/

#define CK_DBP	1		/* The prefix CK_ should be read as check  */
#define CK_SEL	2
#define CK_IO	4
#define CK_CD	8
#define CK_MSG	0x10
#define CK_REQ	0x20
#define CK_BSY  0x40
#define CK_RST	0x80

/*	BSR	*/

#define CK_ACK	1
#define CK_ATN	2
#define BSY_INT 4
#define PHASE_INT	8
#define INTERRUPT	0x10
#define SPER	0x20
#define	CK_DRQ	0x40
#define	EDMA	0x80

#define BUS_MASK	0xfc	/* This value can be used to mask off the two SCSI bus bits of the BSR	*/

/****************************
*	   EMR		    *
****************************/

#define EN_EEARB	1
#define NO_OP	0
#define RPI	2
#define SDIR	4
#define ISR	6
#define IMR	6
#define LOOP	8
#define SPOL	0x10
#define MPOL	0x20
#define MPEN	0x40
#define EN_APHS	0x80

/****************************
*	  ISR & IMR	    *
****************************/

#define EARB	1
#define ESEL	2
#define EBSY	4
#define APHS	8
#define DPHS	0x10
#define EEDMA	0x20
#define MPE	0x40
#define ESPER	0x80

/****************************
*      Printer Signals      *
****************************/

#define PRINTER_ACK	1
#define PRINTER_BUSY	2
#define PRINTER_PE	4
#define PRINTER_ERROR	8

#define PRINTER_STROBE	1
#define PRINTER_SELECT	2
#define PRINTER_INIT	4

/****************************
*	DMA Signals	    *
****************************/

#define SINGLE_MODE 0x40
#define BLOCK_MODE 0x80
#define WRITE_TRANSFER 4
#define READ_TRANSFER 8
#define CHANNEL0 0xe

/****************************	
*     Data Constants	    *
****************************/

#define BUFFSZ 0x7800
#define SEL_TIMEOUT 4673	/* Selection timeout=250ms. So for 2.5Mhz sel_timeout=2921		*/
				/*			       for 4 Mhz sel_timeout=4673		*/
#define BUFFLIM 0X800
#define MAX_SENSE 4

/****************************
*	Block Sizes	    *
****************************/

#define COMMAND_BYTES 6
#define MESSAGE_BYTES 256
#define SENSE_BYTES 4
