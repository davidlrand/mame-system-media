/********************************************************
*							*
*	FILENAME: SCSI.C				*
*	VERSION:  1.1					*
*	DATE:	  MARCH 23 1988				*
*	APPLICATION: EASI DEMO BOARD			*
*	WRITTEN BY: ANDREW DAVIDSON			*
*		    CMOS LOGIC DESIGN			*
*		    NSUK GREENOCK			*
*							*
********************************************************/


/********************************************************************
*								    *
*	This program follows a diagnostic program,	    	    *
*	(PRINTER.SRC) written in assembly language, which tests and *
*	initialises the EASI, DMA and PIO, and then sets up the     *
*	interrupts.This program waits for a selection, checks it is *
*	valid, fetches the command, stores it in a buffer, executes *
*	a limited set of commands and returns status.If the 	    *
*	initiator supports disconnection (shown by sending an 	    *
*	identify message with the disconnection bit set) the device *
*	will disconnect, if selected during a print, then reconnect *
*	when it has sufficient space in its buffer.All constants    *
*	in upper case, functions and variables are in lower case.   *
*								    *
********************************************************************/ 

#include sym.h					/* These files contain the I\O addresses, constant values and	*/
#include const.h				/* SCSI command, message and status values.			*/
#include commands.h

extern char read(port);				/* All of these routines are written in assembly language and	*/
extern char write(port,data);			/* are in file EASIO.SRC.					*/
extern char dmawrit(port,data);
extern int dmaread(port);
extern char intA();
extern char IDtest();
extern char dsi();
extern char eni();

extern char error(num);				/* This is a public function in PRINTER.SRC	*/

extern char SCSIID;				/* SCSIID is read in from switches in PRINTER.SRC	*/
extern char DP8490;				/* DP8490 is a flag set in PRINTER.SRC to show whether a */
						/* DP5380 or DP8490 is inserted.			 */

extern int (*RESETA)();				/* This is a variables set up in PRINTER.SRC.This address */
						/* is required to set up the interrupt jump table.	  */

int gen_int();					/* These functions are in command.lib and print.lib.They are defined 	*/
int serva();					/* here as their addresses need to be known to set up the interrupt 	*/
int servn();					/* jump table.								*/


int i, data_len,data1,data2,recon_data;

char ano_interrupt,temp,recon,reserved,no_win,release,poss_sel,next,previous;
char mescon,fail,parity_error,phase_error,print_on,mess_err,in_print;
char stat,sense[SENSE_BYTES],mess_out[MESSAGE_BYTES],buffer[BUFFSZ];
char com[COMMAND_BYTES],com_r[COMMAND_BYTES],message;
char initid,initid_r,discon;
char *point,*front,*rear,*top,*bottom;

main()
{
	select();
	set_up();
	if (stat==CHECK_CONDITION)
		 fail=TRUE;
	else
	{
		fetch_cmd();
		if (parity_error)
		{
			fail=parity_error=FALSE;
			message=RESTORE_POINTERS;
			messin();
			if (!fail)				/* Check message phase succesful.		*/
			{
				fetch_cmd();
				if (!parity_error)
				{
					sense[0]=NO_SENSE;
					stat=GOOD;
				}
			}
		}
		if (!fail)
		{
			if (!reserved && print_on && !recon && discon)	/* No reservation but currently printing	*/
				disconect();
			else
			{
				if (!recon)
				{
					if (reserved)
					{
						if (reserved==initid)	
						{
							if (print_on)
							{
								if (next)
								{
									process_cmd();
									next=FALSE;
								}
								else
								{
									if (discon)
									{
										release=TRUE;
										disconect(); /* All other commands cause a 	  */
									}		     /* disconnection 			  */
									else
										stat=BUSY;  /* Initiator does not support    */
								}		     	/* disconnection and since currently */
										     	/* printing, cant process command.   */
							}
							else
								process_cmd();
						}
						else
							stat=RESERVATION_CONFLICT;
					}
					else
					{
						if(!print_on)
							process_cmd();		/* print_on must be checked in case a     */
										/* print is current, from an initiator    */						
										/* that has released the unit.		  */
						else
							if (!discon && next)
							{
								process_cmd();
								next=FALSE;
							}
							else			/* Either already waiting to reconnect or  */
								stat=BUSY;	/* no room on print buffer.		   */
					}
				}
				else
					stat=BUSY;			/* Already waiting to reconnect	*/
				if (!release)			        /* Flag set to show bus has been released for later */
				{				        /* reconnection. 				   */
					status();
					message=COMMAND_COMPLETE;
					messin();
					reset();
					if (!in_print  && print_on && !stat)  	/* status must be good */
						outbuf();
					while (recon && !print_on) reconnect();	/* Can only reconnect when print has finished	*/
				}
				release=FALSE;
			}
		}
	}
	if (fail)
	{
		status();
		message=COMMAND_COMPLETE;
		messin();
		reset();
	}
	eni();		/* Interrupts not enabled until previous decisions are finished	*/
}

#include command.lib
#include process.lib
#include dma.lib
#include print.lib
#include arbitrat.lib
