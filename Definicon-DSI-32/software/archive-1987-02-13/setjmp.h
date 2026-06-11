/****************************************************************************/
/*									    */
/*		    L o n g j u m p   H e a d e r   F i l e		    */
/*		    ---------------------------------------		    */
/*									    */
/*	Calling sequence:						    */
/*									    */
/*		#include	<setjmp.h>	(definitions)		    */
/*		jmp_buf	 env;	(define a buffer for saved stuff)	    */
/*									    */
/*		setjmp(env);						    */
/*	a:								    */
/*									    */
/*		longjmp(env,val);					    */
/*									    */
/*	Setjmp returns a LONG of 0 on first call, and "val" on the 	    */
/*	subsequent "longjmp" call.  The longjmp call causes execution to    */
/*	resume at "a:" above.						    */
/*									    */
/****************************************************************************/
typedef	long	jmp_buf[20];
