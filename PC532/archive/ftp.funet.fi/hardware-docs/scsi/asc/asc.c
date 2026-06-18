/********************************************************
*							*
*	FILENAME: ASC.C					*
*	DATE: March 8 1988				*
*	VERSION: 1.0					*
*	APPLICATION: ASC-88 driver software for SPC	*
*	WRITTEN BY: ANDREW M. DAVIDSON			*
*		    LOGIC DESIGN			*
*		    NATIONAL SEMICONDUCTOR UK. LTD.	*
*							*
********************************************************/

/************************************************************************
*	This file is used to implement a PRINT using an ASC-88 host 	*
*	adaptor, installed in a PC, to select an SPC, SCSI Printer	*
*	Controller. The host adaptor is accessed by an MS-DOS interrupt	*
*	, and its functions determined by a Job Control Block. To	*
*	implement a print this software sends the following sequence of	*
*	commands:							*
*									*
*	TEST UNIT READY: to ensure the printer is operational.		*
*									*
*	RESERVE UNIT: the printer must be reserved for use by this	*
*		      initiator only before a print.			*
*									*
*	PRINT: this transfers the data to be printed to the SPC. The	*
*	       maximum length of data per transfer is 2KB, for reasons  *
*	       explained in the SPC applications note.			*
*									*
*	REQUEST SENSE: to ensure the printer is still operational. If 	*
*		       an error occurs when the SPC transfer data to 	*
*		       the printer sense will be set.			*
*									*
*	RELEASE UNIT: when finished transferring data to the SPC the 	*
*		      unit should be released to allow other initiators *
*		      to use it.					*
*									*
************************************************************************/

#include <stdio.h>		/* Contains the definitions of stream I/O functions	*/
#include <conio.h>		/* Contains the definitions of console I/O functions	*/
#include <dos.h>		/* Contains the definitions for MS-DOS interface functions	*/

#include <constant.h>		/* Contains the constant values used in this software	*/
#include <ascstruc.lib>		/* Contains Job Control Block structure	*/
#include <asccom.lib>		/* Contains Job Control Blocks for particular SCSI commands	*/

FILE *fopen(),*stream;

char ch,try_again,finished,fail;
int i;

main(argc,argv)		/* This fetches the filename from the keyboard	*/
int argc;
char *argv[];
{
	fail=finished=FALSE;
	try_again=TRUE;

	if ((stream=fopen(*++argv,"r"))==NULL)		/* Opens specified file and returns a pointer, stream, 	*/
	{						/* which is subsequently used to access the file.	*/
		printf("cannot open file");
		goto true_end;
	}
	while (try_again)	/* Loop is executed until status() resets the variable	*/
	{
		test();		/* Sends TEST UNIT READY command to SPC	*/
		status();	/* This checks the returned status	*/
	}
	if (fail)	goto true_end;
	try_again=TRUE;
	while (try_again)
	{
		reserve();	/* Sends RESERVE UNIT command to SPC	*/
		status();
	}
	if (fail)	goto true_end;
	while (!finished)
	{
		for (i=0;(i<=BUFFLIM) && ((ch = getc(stream)) != EOF);i++)	/* Fetches data from file until it reaches the	*/
			buffer[i]=ch;						/* end of file charater EOF or until the buffer */
		if (ch==EOF)							/* is full.					*/
		{
			finished=TRUE;
			if (feof(stream))	/* EOF is also used to indicate an error, so this function checks */	
			{			/* that the file is finished.					  */
				buffer[i++]=FORM_FEED;		/* Set the last character to give a form feed	*/
				try_again=TRUE;
				while(try_again)
				{
					print_data(i);		/* Sends PRINT command to SPC	*/
					status();
					if (fail)	goto end;	/* An effort must be made to release the unit	*/
				}
			}
			else
				printf("error reading file");		/* EOF was returned to indicate an error	*/
		}
		else
		{
			try_again=TRUE;
			while(try_again)
			{
				print_data(i);		/* Sends PRINT command to SPC	*/
				status();
				if (fail)	goto end;
			}
		}
	}
	try_again=TRUE;
	while(try_again)
	{
		request();		/* Sends REQUEST SENSE command to SPC	*/
		status();
	}
	message();
	if (fail)	goto true_end;
end:	try_again=TRUE;
	while(try_again)
	{
		release();		/* Sends RELEASE UNIT command to SPC	*/
		status();
	}
true_end:	fclose(stream);		/* Close file	*/
}	

#include <ascrot.lib>			/* Contains routines for responding to returned data	*/
#include <util.lib>			/* Contains command for writing error data on screen	*/
