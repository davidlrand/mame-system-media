






















                     DEFINICON SYSTEMS, INC.


                    DSI-32 COPROCESSOR BOARD
                  & MS/PC DOS SYSTEM SOFTWARE












     DESCRIPTION OF LOAD SOFTWARE



.pa
1.0 Loader Overview

The  loader is the only portion of software that runs on the host 
computer  (the  IBM  PC or AT compatible).  It  performs  several 
distinct functions, which are listed below.

     o    Resets and initializes the DSI-32

     o    Determines the presence and type of DSI-32

     o    Under Concurrent DOS, determines number of DSI-32
          coprocessor boards that are plugged into the host.

     o    Under Concurrent DOS, determines board activity, and
          selects a currently unused board.

     o    Loads the operating system into the selected DSI-32

     o    Loads the specified program into the DSI-32

     o    Resets and releases control to the DSI-32

     o    Services requests from the DSI-32 until termination


1.1 Command Line Options

The  loader  has  a number of command line options  that  can  be 
specified. These are shown below.

     o    T (for Timing program execution)

     o    M (for Memory utilization)

     o    A (for Argument list indication)

The options must be preceeded by a '-',  and must be used in  the 
correct sequence. 

 -T  To  display the time that a program takes to execute  (after       
     it  is loaded),  the following the '-T' option is  used.  An        
     example of its use is shown below:

               A>LOAD -T SIEVE

     The  execution  time  is  displayed on  the  screen  with  a        
     resolution  of  10 milliseconds under  Concurrent  CPM,  and        
     60 milliseconds under MSDOS.
.pa
-M   If  memory utilization information on the execution  of  the        
     Sieve  program  was  required  then  one  of  the  following        
     commands  would display the figures just prior to  returning        
     control back to the host. 

               A>LOAD -M -T SIEVE
       or

               A>LOAD -T -M SIEVE
       or

               A>LOAD SIEVE -T -M

-A   The   argument  list  option  enables  the  user  to  supply         
     command  line arguments to the program at  invocation  time.        
     Any  characters  that  follow the '-A'  option  are  totally         
     ignored  by  the LOAD program but are passed through to  the        
     program  that  is being loaded into the  DSI-32  board.  The         
     chararacters   following  the  '-A'  can  be   accessed   by         
     programs  on  the DSI-32 via the argument list and  argument        
     count parameters, eg in C the 'argv' & 'argc' variables. 

     As  an example,  if we wanted to time the execution  of  the         
     compile   of   the   sieve  program,   and   report   memory         
     utilization, the command would read:

               A>LOAD -M -T CC -A -GE -O1 -O2 SIEVE.C

     All  characters  that follow the '-A' are passed through  to         
     the CC program,  ie the C compiler.  To further clarify  how        
     the  characters  after  '-A' are passed to the  program  the         
     'argv'  and  'argv'  variables for  the  above  example  are         
     shown below:

               argc = 5

               argv[0]="CC"
               argv[1]="-GE"
               argv[2]="-O1"
               argv[3]="-O2"
               argv[4]="SIEVE.C"
               argv[5]=NULL

Of course if no command line options are required by the  program 
then  they  do not have to entered.  The example below shows  the 
sequence  that is required to load and execute the  simple  HELLO 
program on the distribution diskette.

               A>LOAD HELLO
.pa
1.2 Loader types

On  the  DSI-32  distribution disk,  two  different  loaders  are 
supplied: LOADINT and LOADMEM. 

LOADINT  uses the ability of the DSI-32 to tell the 8086/88  when 
it needs service via interrupt #2 (IRQ2 on the IBM Bus).

The  8086/88  under Concurrent DOS uses that  operating  system's 
FLAG  system  call to suspend program execution until the  DSI-32 
requires service.  This means that under Concurrent DOS the other 
tasks that may be running will only be slowed down when the  DSI-
32  actually requires service.  For the rest of the time 100%  of 
the processor is available for the other tasks.

Under PC-DOS, however, LOADINT just waits in a tight loop for the 
interrupt to occur. When the interrupt occurs the LOADINT program 
services the request, as described below (Section 1.3).


LOADMEM  assumes  that the IRQ2 is already in use by  some  other 
card.  By providing this alternate loader the DSI-32 may still be 
used  along  with  the other card that requires use of  the  IRQ2 
line.

LOADMEM simply polls the 8086SVC location (a word in memory which 
indicates that service is required),  on the DSI-32  board.  When 
the  DSI-32 requests an operation,  it places a non-zero value in 
the  8086SVC  location.  LOADMEM then services  the  request,  as 
described below (Section 1.3). Note that use of LOADMEM will slow 
down the DSI-32 by about 1% (total execution time).

1.3 Servicing requests

When  the DSI-32 requires service,  the LOAD program obtains  the 
service  request  information by looking in  the  Inter-Processor 
Communications  Area (0x2000 to 0x2fff in the DSI-32).  From this 
information,  the  LOAD  program performs the  requested  service 
(writing a character to the screen,  reading information from the 
disk,  etc.).  When  the operation is complete,  LOAD resets  the 
8086SVC  location  to  zero to indicate  that  the  operation  is 
complete.

The  DSI-32  does  not have to wait for LOAD to complete  the  IO 
request,  however.  In  the  case of the background  memory  move 
operation,  for example,  the LOAD program does the actual memory 
move   operation.   The  DSI-32  is  free  to  continue   to   do 
computational  work.   If  the  memory  move  operation  has  not 
completed by the time the DSI-32 wishes to do another IO request, 
the DSI-32 will wait until the LOAD program has indicated that it 
is done (by setting 8086SVC to zero).
1.4 Loading multiple programs

The  LOAD program has the ability to load multiple programs  into 
memory.  This ability is used everytime any program is loaded, as 
the  DSI-32  operating system is contained in the file  32IO.E32, 
and this is loaded automatically.

A  common  use  for  loading multiple  programs  is  loading  the 
monitor, in conjunction with a user program, as in:

               A>LOAD MON SIEVE

This  loads the operating system (32IO),  the  monitor,  and  the 
sieve program, in that order. Note that the monitor loads in high 
memory,  and  thus will not normally affect user  programs.  User 
programs may be overlayed in this fashion as well. The command:

               A>LOAD PART1 PART2 PART3 PART4

will  load  the operating system  (32IO),  PART1.E32,  PART2.E32, 
PART3.E32 and PART4.E32 into memory.  The user is responsible for 
ensuring  that  memory conflicts do not arise,  as LOAD  does  no 
checking  for  overlapping programs.  The MOD= directive  in  the 
linker may be used to assign correct module table values, and the 
CODE=  directive  may be used to set  appropriate  code  starting 
point values.


