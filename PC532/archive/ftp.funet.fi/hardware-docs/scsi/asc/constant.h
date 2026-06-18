/********************************************************
*	This file lists all status, message, printer	*
*	commands and sense codes, but only describes 	*
*	those that are used within this software.ASC.C	*
*	specific constants are also defined.		*
********************************************************/
	  
/****************************
*        Status Codes	    *
****************************/

#define GOOD		0				/* Operation completeted successfully	*/
#define CHECK_CONDITION	2				/* Valid sense data exists to explain error condition	*/
#define CONDITION_MET	4
#define BUSY		8				/* Target can not accept commands, as it is currently processing	*/
#define INTERMEDIATE_MET		0x10
#define	INTERMEDIATE_CONDITION_MET	0x14
#define RESERVATION_CONFLICT		0x18		/* Target is reserved by another initiator	*/

/****************************
*       Message Codes	    *
****************************/

#define COMMAND_COMPLETE	0			/* Command has been terminated after valid status returned	*/
#define EXTENDED_MESSAGE	1
#define	SAVE_DATA_POINTERS	2
#define RESTORE_POINTERS	3			/* Prepare to resend previous command, data or message	*/
#define DISCONNECT		4			/* Target is about to disconnect from initiator	*/
#define INITIATOR_DETECTED_ERROR	5
#define ABORT	6					/* Reserving initiator may use this to abort command	*/
#define MESSAGE_REJECT		7			/* Previously sent message is not compatible	*/
#define	NO_OPERATION
#define MESSAGE_PARITY_ERROR	9			/* Parity error occurred during sending of previous message	*/
#define LINKED_COMMAND_COMPLETE		0xa
#define LINKED_COMMAND_COMPLETE_FLAG	0xb
#define	BUS_DEVICE_RESET	0xc			/* Any initiator may use this to reset board	*/
#define IDENTIFY		0x80			/* This establishes physical path and whether device supports	*/
							/* more messages than COMMAND COMPLETE				*/

#define EN_DISCON	0x40			/*This is not a message, but is used in identify, to recognise disconnection */

/****************************
*	  Commands          *
****************************/

#define TEST_UNIT_READY 0			/* Checks the printer is on, on-line, has paper and not in an error condition	*/	
#define REQUEST_SENSE	3			/* Retrieves sense data for initiator, after CHECK CONDITION status	*/
#define FORMAT	4
#define PRINT	0xa				/* Transfers data to be printed	*/
#define	SLEW_PRINT	0xb
#define FLUSH_BUFFER	0x10			/* Resets buffer pointers to clear buffer	*/
#define INQUIRY	0x12
#define RECOVER_BUFFERED_DATA	0x14
#define MODE_SELECT	0x15
#define RESERVE_UNIT	0x16			/* Reserves unit for use by only one initiator	*/
#define RELEASE_UNIT	0x17			/* Releases unit so it can be used by any initiator	*/
#define COPY	0x18
#define MODE_SENSE	0x1a
#define STOP_PRINT	0x1b
#define RECEIVE_DIAGNOSTIC_RESULTS	0x1c
#define SEND_DIAGNOSTICS	0x1d

/****************************
*	 Sense Codes        *
****************************/

#define NO_SENSE 	0			/* No error has occurred	*/
#define RECOVERED_ERROR	1
#define NOT_READY	2
#define MEDIUM_ERROR	3			/* Printer out of paper	*/
#define HARDWARE_ERROR	4			/* Series harware  error ocurred	*/
#define ILLEGAL_REQUEST	5			/* Initiator attempted incompatible command	*/
#define UNIT_ATTENTION	6			/* Printer off, off-line or in error condition	*/ 
#define DATA_PROTECT	7
#define BLANK_CHECK	8
#define COPY_ABORTED	0xa
#define ABORTED_COMMAND	0xb			/* Target has terminated command due to error (check sense data)	*/
#define EQUAL	0xc
#define VOLUME_OVERFLOW	0xd
#define MISCOMPARE	0xe


/****************************
*	ASC.C Constants	    *
****************************/

#define TRUE 1
#define FALSE 0
#define FORM_FEED 0xc			/* This is the ASCII character to give a form feed	*/
#define ESCAPE 0x1b			/* This is the ASCII character for an escape	*/
#define BUFFLIM 0x7ff
#define SENSE_BYTES 4
#define ARBITRATION_TIMEOUT 0x85	/* This is a vendor specific status code for the ASC-88	*/

#define FAULT 0xff			/* These constants are used in asccom.lib	*/
#define TARGET_ID 0			/* This constant contains the ID of the SPC	*/
#define YES 0xff
#define NO 0
#define IN 0
#define OUT 0xff
#define SCSI_PRO 0x94
#define SCSI_BIOS 0x40
