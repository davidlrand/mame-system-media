.pl 62













                     DEFINICON SYSTEMS, INC.



                    DSI-32 COPROCESSOR BOARD
                   & MS/PC DOS SYSTEM SOFTWARE



        










USERS GUIDE

and

INSTALLATION MANUAL

.pa
                            CONTENTS
     

1    Introduction and Product Overview

2    Quick Installation Guide
	2.1	Hardware Installation
	2.2	System Software Installation
	2.3	Hardware Test
	2.4	C Compiler, Assembler, Linker,
                and Loader Tests

3    System Software
	3.1	The Compilers
        3.2	Assembler
	3.3	Linker
        3.4     Librarian
	3.5	Loader
	3.6	MS-DOS Interface
	3.7	Debugger

4    General Theory of Operation
        4.1    Hardware
        4.2    Software
        4.3    Multi-Tasking

5    Benchmarks

6    Don't Panic...
.pa
1    Introduction and Product Overview

     The Definicon Systems' DSI-32 coprocessor  board is designed 
around  National Semiconductor's NS32032 full 32 bit CPU with  32 
bit data bus, and occupies a single expansion slot  in all models 
of  the IBM-PC (including the AT) or any compatible  clone.   The 
DSI-32  board is supported by three highly  optimising  compilers 
for the C,  PASCAL,  and FORTRAN languages which were ported from 
the UNIX environment.  The DSI-32 board utilises either the MSDOS 
or  Concurrent  C/PM  operating systems for all  file  level  and 
operator interface.

     The Definicon system provides all the necessary  software to 
interface the host IBM PC running MSDOS to the DSI-32 coprocessor 
board.  This  interface is totally transparent to  the  operator; 
therefore  in  normal operation,  the user need  not  distinguish 
which of the two CPUs is executing his program.

     The  DSI-32 coprocessor is an accelerator board for  IBM-PCs 
and compatibles.  It increases the processing speeds of the PC to 
that  of the Digital Equipment Corporation VAX 11/750 and  11/780 
minicomputers.   The  operating software resides under MS/PC  DOS 
and when the 32 bit coprocessor is executing code, the host PC is 
being used as an efficient file and input/output processor.

     The  NS32032 is supported by the NS32081  hardware  floating 
point  accelerator  and up to 2 Mbytes of no-wait-state RAM which 
is fully accessible to the host IBM PC.

     This  Users  Guide  describes  how  to  install  the  DSI-32 
coprocessor and software in an IBM-PC.  Sections are included on: 
installation;  system use;  theory of operation;  and benchmarks.  
.pa
2    Quick Installation Guide

2.1  Hardware Installation

     The installation of the DSI-32 coprocessor in your IBM-PC is 
very  simple  and  only requires the use of a  medium  flat-blade 
screwdriver to  remove the computer cover,  and a medium Philips-
head screwdriver to install the board in the Expansion Chassis.  

Please  note that if you have other than IBM-PC/XT or  AT,  these 
instructions may not be applicable to your computer.

      1.  TURN  OFF THE POWER TO YOUR COMPUTER.   It is essential           
          that  there is no power applied to the unit  while  the           
          board  is  being  installed or serious  damage  to  the 
          computer  and  board  may result.

      2.  Remove the power cord from the wall socket.

      3.  Release  the cover of the computer by  positioning  the 
          computer   for   access  to  the  rear  and   using   a 
          screwdriver,  remove the cover mounting screws as shown 
          in Figure 1A.

      4.  Slide  the  computer's cover towards the front  of  the 
          computer.   When the cover will go no further,  tilt it 
          up and remove it, as shown in Figure 1B.

      5.  You  can  now  install the DSI-32  coprocessor  in  any 
          available expansion slot as follows.  Face the front of 
          the computer or Expansion Chassis.  Remove an available 
          slot  cover  by loosening and removing the screw  which 
          holds it to the chassis (see Figure 1C).   Set the slot 
          cover and screw aside.   If the plastic card edge guide 
          (see  Figure  1D)  is  not on the  front  wall  of  the 
          computer,  install  the one supplied with  your  DSI-32 
          coprocessor.   This  card guide will hold the board  in 
          place.   Slide  the board down into the slot and ensure 
          that  the gold contacts are completely in the  computer 
          slot.   The serial ports should now be accessible  from 
          the  back of the computer.   Re-install the screw  into 
          the top to hold the board firmly in place. 

     6.   Remove any tools from inside of the computer

     7.   Replace the cover of the computer and install the cover 
          screws that were set aside.

This completes the installation of the DSI-32 coprocessor.  It is 
now safe to apply power to the computer.
.pa
2.2  System Software Installation
     
     1.   Boot the computer using MS/PC DOS version 2 or 3

     2.   Remove system disk

     3.   Insert the Definicon distribution disk into drive A: of           
          your computer.

     4.   For further information refer to the READ.ME file on
          the Assembler/Linker/Loader distribution disk.

2.3  Coprocessor Test.

      1.  At  the DOS prompt type "BOARDTES".  A batch file  will 
          load  a  short test program into the DSI-32.  When  the 
          message  "HARDWARE VALIDATED" appears,  the hardware is 
          fully functional.  If this message does not appear,  or 
          error messages appear, please see section 6.

2.4  C Compiler, Assembler, Linker and Loader Tests

      1.  Change  the  logged disk drive to the one you  want  to 
          hold your system files.

      2.  At the DOS prompt, type CCTEST and wait. A lot of batch 
          file  and  program activity will  occur while the  self 
          test  is  completing.  This  will  have  tested  the  C 
          compiler and the assembler, linker, and loader.

      3.  The  commands  "FORTEST" and "PASTEST"  will  similarly 
          test your FORTRAN and PASCAL compilers.

3    System Software

     Three optimizing compilers for C,  FORTRAN and PASCAL  are 
offered  with  the  DSI-32 Coprocessor. 

3.1  The Compilers

     The  Compilers  supplied  with the  DSI-32  Coprocessor  are 
advanced,  highly  optimizing compilers developed by Green  Hills 
Software,  Pasadena,  CA,  for  the UNIX environment.   The major 
advantage  of  these compilers is  their  mainframe  optimization 
techniques which produce code so efficient that it usually cannot 
be  further  hand optimized.   Definicon has adapted  these  UNIX 
compilers  so that they operate in the MS/PC DOS environment. 
.pa
     The  compilers are single pass compilers.  This  means  that 
source  code need be read only once during compilation. 

     The  three  compilers each use the same  subroutine  calling 
conventions.  For example, subroutines written in C can be called 
from  the  FORTRAN or PASCAL compilers.  This convention is  also 
compatible with that of Berkeley UNIX v 4.2

3.2  The Assembler

     Definicon Systems has adapted the PC written 32000 assembler 
of Computer Systems Design (Perth, Western Australia) to the DSI-
32  Coprocessor.    Taking  the  intermediate  code  from  the  a 
Compiler, the assembler generates an object file which the linker 
then forms into  32000 executable machine code.  The assembler is 
compatible  with National Semiconductor's cross development tools 
for the 32000 series microprocessors,  with necessary  extensions 
to  support  the  Green Hill's  compilers.  A  detailed  language 
description  of this assembler is presented in Definicon Systems' 
ASSEMBLER REFERENCE MANUAL.

3.3  Linker

     A  linker  is  also provided,  which is  based  on  the  one 
developed by Computer Systems Design.  The Definicon linker takes 
32000   object  code  and  produces  executable  code  which   is 
the  LOAD  program  then loads into  the  DSI-32  card.  Detailed 
operational  commands  on the linker are given in  the  ASSEMBLER 
REFERENCE MANUAL.

3.4  Librarian

     A librarian is available for the DSI-32, which was developed 
by Computer Systems Design.  The librarian enables library  files 
to  be  created form separate object files.  Object files may  be 
extracted from or inserted into library files.

3.5  Loader

     The DSI-32 Coprocessor communicates with the IBM-PC hardware 
through  the Loader.   This program instructs the PC to  load  an 
executable  file  onto the Coprocessor board,  and instructs  the 
32032  to  begin  execution.   Upon  completion  of  loading  the 
executable program into the DSI-32 it is responsible  interfacing 
requests from the running program to the PC MS/PC DOS.  After the 
program  has completed,  control returns to the MS/PC DOS command 
processor.
.pa
     Due  to the lack of re-entrancy in MS/PC DOS the PC must  be 
dedicated  to  performing  only  I/O tasks while  the  DSI-32  is 
executing  under MS/PC DOS.  This limitation does not exist  with 
Concurrent DOS from Digital Research.

     If  multiple  processing on the  PC  is  desired,  Definicon 
offers  a  software  interface  compatible  with  CONCURRENT  DOS 
(Digital  Research,  Pacific Grove,  CA).  The advantage of using 
CONCURRENT  DOS with the DSI-32 coprocessor,  is that the PC  may 
still be used while the DSI-32 board is executing in a background 
mode.

3.5  MS-DOS Interface

     A unique feature of Definicon's Software is that it can take 
full  advantage of the existing interface between the  MS/PC  DOS 
operating  system  and the hardware on the PC-Bus  (Disk  drives, 
Keyboard,  Display,  etc.).   Using  the PC as an intelligent I/O 
processor,  the DSI-32 is free to offload I/O requests to the  PC 
while conserving its own resources for handling data processing.

3.7  The Debugger

     The Definicon debugger is used to debug code written for the 
DSI-32 Coprocessor.   The debugger can display disassembled code, 
data,  general purpose registers,  floating point registers,  and 
MMU  registers  (if the optional MMU is installed on  the  DSI-32 
board).  It also permits single step execution of  code.  Command 
syntax  is  essentially  compatible  with  the  MS/PC DOS  'DEBUG' 
program.

4    General Theory of Operation

     The Definicon DSI-32 coprocessor card occupies a single slot 
in  an IBM-PC,  XT,  AT,  COMPAQ,  or any of the other compatible 
clones.

     The  operating  software resides under the  MSDOS  or  PCDOS 
operating  system  of  the host PC.  All files that are  read  or 
generated  by  programs  running on the DSI-32  board  reside  on 
MS/PC DOS media, no disk partitioning is necessary. The Definicon 
Compilers,  and assembler/linker/loader provide all the necessary 
tools to develop and debug code for the DSI-32 coprocessor board. 
The DSI-32 may also me used as a development system for the 32000 
series of microprocessors.
.pa
     There  are  two possible Coprocessor CPU  configurations.  A 
full 32 bit CPU (32032) may be used with a Memory Management Unit 
(32082) and a Floating Point Unit (32081) or alternatively the 16 
bit  external  bus CPU (32016) may be used in lieu  of  the  MMU.  
This  description  will  only address the operation of  the  full 
10MHz, no wait state, 32032, 32082 and 32081 configuration.

     In addition to the coprocessor CPU and support chips, 2 high 
speed  RS232  serial  ports and a 16 bit programmable  timer  are 
available.  These  may be used in addition to the I/O  facilities 
resident in the host PC.

     Up  to 2 Mbytes of no-wait-state RAM are available  to  both 
the 32032 and the 8088 (which can use it as a high speed RAM disk 
drive).
.pa
4.1  Hardware

     The DSI-32 is a 6 layer PCB assembly occupying a single slot 
in  the host IBM compatible PC.  The two serial port  connectors, 
one  DB25 and one DB9,  are located at the external (right  hand) 
end of the board.  The RAM array is located at the left end.  The 
32032  is  housed in a 68 pin Leadless Chip Carrier package  near 
the  center.  Between  the RAM array and the CPU are  the  DP8409 
Refresh  Controller and the 32082 MMU.  The 32201 Timing  Control 
Unit  (TCU)  is  mounted  above the CPU  and  the  32081  FPU  is 
immediately  to the right of the CPU.  The serial port controller  
DUART and timer (2681) is to the right of the FPU.

     The  remaining logic subdivides into several  basic  blocks.  
Below the RAM array are 4 bidirectional latches which convert the 
data from the 32 bit internal (RAM and CPU) bus to the external 8 
bit IBM-PC bus.

     The  two  TTL  chips to the right of the TCU  handle  device 
address  decoding for the 32032 peripherals.  The 5 chips to  the 
lower right of the DUART are the RS232 drivers.  The remaining 12 
chips  control  the  interface between the host PC  CPU  and  the 
32032.

     When  the  host  PC  wishes  access  to  the  32032  RAM  or 
peripherals a HOLD (DMA) request is issued to the 32032. When the 
Hold Acknowledge is received the 8088 drivers take control of the 
internal board bus and read or write the data appropriately.  One 
byte  of data is transferred at a time.  The total time taken for 
such  a  transaction is less than 800  nanoseconds  (nsecs).  The 
execution time lost on the 32032,  however, is generally only 1 T 
state,  or 100 nsecs,  as its internal state machine continues to 
process the data in its 8 byte queue.

     Interrupts  can  be issued by the 32032 to the host  PC  (on 
IRQ2)  or  by  the host to the 32032 (via  the  DUART's  maskable 
interrupt  features).  The DUART can also generate  an  interrupt 
when  any of its serial facilities require servicing or when  the 
timer  is  programmed in that mode.  If  multiple  interrupts  of 
various  priorities  are  to  be  processed  then  the  Interrupt 
Controller Unit (ICU) may be used.

     At  power-on  the  host mother board issues a reset  to  the 
coprocessor. The coprocessor is then made to execute a tight loop 
until  its  control program has been loaded by the  host  and  an 
EXECUTE command issued.
.pa
     The  host 8088 has access to a number of special  diagnostic 
features of the coprocessor system. For instance, both the serial 
ports and timer may be checked during the power-on initialization 
sequence.   In  addition  the  host may check the  RAM  with  the 
refresh  controller  disabled,  in order not only to detect  hard 
errors,  but  also measure the refresh margins so as  to  predict 
when soft errors become likely.

4.2  Software

     The  software currently provided with the DSI-32 provides  a 
high  speed development and execution environment using the  file 
and I/O utilities of the host.

     Compilers are currently available for C, PASCAL and FORTRAN. 
The  compilers  are compatible with those implemented  under  the 
UNIX operating system.

     These  compilers,  (available  from  Green  Hills  Software, 
Pasadena,  CA)  have been ported by Definicon to the DSI-32  from 
the  UNIX  environment.

     Any  special hardware resident in the PC bus,  such as mouse 
drivers,  A/D converters, etc, are accessed by the coprocessor in 
the same way as a file.  This is possible as special devices  can 
be  accessed  using the MSDOS 'special drivers' facilities  (.SYS 
driver files).

An  optional set of interface routines (available from Definicon) 
allow  multiple  virtual screens to exist  in  the  DSI-32,  with 
movement  to the physical screen occuring at about 250K bytes per 
second.

4.3  Multi-Tasking

     One  feature of the UNIX operating system is the ability  to 
run several tasks concurrently.  Definicon has implemented an I/O 
kernel for CONCURRENT-DOS which allows background tasks using MS-
DOS software (such as editing) to take place transparently  while 
the  DSI-32  is  executing.  Windows can be set up  so  that  the 
progress of each process can be monitored.

     CONCURRENT-DOS  is available for most IBM-PC  clones.   Most 
MSDOS software (including Lotus-123 and Wordstar) will run under 
CONCURRENT-DOS.
.pa
5.   Benchmarks

     The  first set of benchmarks presented are representative of 
numerically  intensive algorithms which require both integer  and 
floating  point arithmetic.   The Sieve of Eratosthenes tests the 
performance of a high level language implementing boolean algebra 
and  integer  arithmetic.   The  FLOAT  benchmark  examines   the 
processor's  ability to execute floating point array  arithmetic. 
FLT  tests the speed of the floating point  co-processor.   Array 
handling is primarily exercised by FLOAT and Sieve; FLT uses only 
scalar calculations.  It should be noted, however, that the Sieve 
uses  only  a  boolean  array,  and  this  negates  much  of  the 
throughput  advantage  of the NS32032 32 bit bus (and indeed  the 
VAX 32 bit bus), tending to favor the 8 and 16 bit processors.

            Execution_Time_(in_seconds)_for_10_Sieves
          All_Machines_have_Floating_Point_Accelerators

                        SIEVE_BENCHMARK *

n **   IBM-PC/XT   IBM-PC/AT   VAX-11/750  VAX-11/780   DSI-32
--------------------------------------------------------------
 8191   11.60        3.71         2.41       1.09        1.85
20000   35.30        8.13         6.11       3.04        4.52
30000   44.90       12.40         N/D***    N/D***       6.78
40000  351.50       99.71        13.13       6.38        9.07
80000  N/A****      N/A****      29.65      13.34       18.12

                         FLOAT_BENCHMARK

n      IBM-PC/XT   IBM-PC/AT   VAX-11/750  VAX-11/780   DSI-32
--------------------------------------------------------------
40000   11.46        17.71        0.83        0.50       0.80

                          FLT_BENCHMARK

n **   IBM-PC/XT   IBM-PC/AT   VAX-11/750  VAX-11/780   DSI-32
--------------------------------------------------------------
256000  119.3        134.0        9.48        6.18      14.00

* -    The  sieve  benchmarks run on the IBM-PC/XT  and  AT  were 
       written  in  Digital Research C.  The FLT  benchmark  used 
       Microsoft Fortran for the XT and DR F77 for the AT.
** -   n  represents the maximum control number on the major loop 
       of  the benchmark test.  In the sieve benchmark, the major 
       loop was run 10 times.
***  - N/D indicates a test not run. 
**** - N/A 8088 compiler not found for arrays greater than 64K
.pa
     The  five target machines being compared are  the  IBM-PC/XT 
(8088 CPU),  the IBM-PC/AT (80286 CPU),  the VAX 11/750, the VAX-
11/780  and  Definicon System's DSI-32 coprocessor  (10MHz  32032 
CPU).   All  five  machines  have additional  numeric  processing 
hardware:

    o    IBM-PC/XT has Intel's 8087 floating point chip (4.77Mhz)

    o    IBM-PC/AT has Intel's 80287 floating point chip (4.0MHz)

    o    VAX-11/780  has  Digital  Equipment's  Floating   Point 
         Accelerator

    o    VAX-11/750  has  Digital  Equipment's  Floating   Point 
         Accelerator

    o    DSI-32  coprocessor has National Semiconductor's  32081 
         floating point unit

     The  compilers  used for the PC/XT and PC/AT were chosen  on 
their  published  reputation  for  generating  high  speed  code. 
Microsoft Fortran v3.1 was used for the FLT and FLOAT  benchmarks 
on  the  PC/XT.  As  it did not execute  on  the  PC/AT,  Digital 
Research F77 was used for FLT and FLOAT on that machine.  Digital 
Research C was used for the sieve benchmark on both the PC/XT and 
the PC/AT.

     The  FORTRAN  compiler  for the VAX was written  by  Digital 
Equipment running under the VMS operating system.   The compilers 
for  the DSI-32 coprocessor were written by Green Hills  Software 
and ported to the Definicon PC/MSDOS environment.  
.pa
               The_`BYTE_MAGAZINE'_UNIX_BENCHMARKS

The  following benchmarks,  devised by David F Hinnant,  test the 
speed of the UNIX operating environment and were published in the 
August, 1984 BYTE magazine. 

Benchmark 1: 
tests Pipes, which are not implemented on the DSI-32

Benchmark 2:
tests the overhead involved in a simple operating system call

Benchmark 3:
tests the C compiler's overhead in making an empty function call

Benchmark 4:
is the Sieve of Eratosthenes, in a slightly different form

Benchmark 5a:
checks the time taken to write a 128K disk file sequentially

Benchmark 5b:
checks the time taken to randomly read it back 

Benchmark 6:
test shells, which are not implemented on the DSI-32

Benchmark 7:
tests the time to perform an empty loop






                      Execution_Times_(in_seconds)

                  DSI-32     VAX 11/780   VAX 11/750

Benchmark 2:       3.88         4.8           7.0 
Benchmark 3:       0.55         1.0           1.7
Benchmark 4:       1.93         1.7           2.4
Benchmark 5a:      5.60         2.0           3.0
Benchmark 5b:     12.00         8.0           8.0
Benchmark 7:       2.36         2.6           5.1

Note:  that  the  disk  I/O timings were performed  on  a  Compaq 
Deskpro running MSDOS 2.1 with 20 MByte CMI fixed disk.
.pa
                        SIEVE_BENCHMARKS

      PROGRAM SIEVE
C     SIEVE OF  ERATOSTHENES BENCHMARK
      LOGICAL FLAGS(8191)
      INTEGER I,J,K,COUNT,ITER,PRIME

      WRITE(*,*) ' 10 ITERATIONS'
      DO 92 ITER = 1,10
      COUNT = 0
      I = 0

      DO 10 I = 1,8191
10    FLAGS(I) = .TRUE.

      DO 91 I=1,8191
      IF(.NOT. FLAGS(I)) GO TO 91
      PRIME = I + I + 1
      COUNT = COUNT + 1
      K = I + PRIME
      IF (K.GT.8191) GO TO 91

      DO 60 J = K, 8191, PRIME
60    FLAGS(J) = .FALSE.

91    CONTINUE
92    CONTINUE

      WRITE(*,300) PRIME,COUNT
300   FORMAT(1X,I6,' IS THE LARGEST OF ',I6,' PRIMES')
      STOP
      END

---------------------------------------------------------------

     FLOAT_BENCHMARK

      PROGRAM FLOAT
      DIMENSION RARRAY(40000)
      COMMON /FAST/ RARRAY
      INTEGER*4 I

      DO 10 I = 1,40000
10    RARRAY(I) = 1.0

      DO 91 I = 1,40000
      RARRAY(I) = RARRAY(I) * RARRAY(40001 - I)
91    CONTINUE

      STOP
      END
#define LIMIT 8191
#define ITS 10
#define FALSE 0
#define TRUE 1
main()
{
        char flags[LIMIT + 1];
        register long i,prime,k;
        int count,iter;

        for (iter = 1; iter <= ITS; iter++) {
            count=0;
            for (i = 0; i <= LIMIT; i++)
                 flags[i] = TRUE;
            for (i= 0; i <= LIMIT; i++) {
	         if (flags[i]) {
                     prime = i + i + 3;
                 for (k = i + prime; k <= LIMIT; k += prime)
                     flags[k] = FALSE;
                 count++;
	    }
	}
    }
    printf("Found %d primes",count);
}

-----------------------------------------------------------

     FLT_BENCHMARK

      PROGRAM FLT
      INTEGER*4 I,J
      REAL*8 X,Y,Z

      DO 10 I = 1,256000
      J = 256000 - I
      X = FLOAT(I)
      Y = FLOAT(J)
      Z = Y / X
      X = Y - Z
      Y = Z * X
      Z = Y + X
10    CONTINUE

C  Force  both the Green Hills and Vax compilers  to  retain  the 
C  floating point operations by:

      X = Z + Y    
      STOP
      END
.pa
6    Don't Panic...

     If   after  running  the  "BOARDTES"  program  the   message 
"HARDWARE  VALIDATED" is not displayed then the following  checks 
should be made.

   o     Check that the LOADER diskette is labelled appropriately 
for  your operating system and hardware configuration.  Definicon 
Systems supplies one of four loader types, they are:

     a)  LOADER for MSDOS on the XT
     b)  LOADER for MSDOS on the AT
     c)  LOADER for Concurrent CPM on the XT
     d)  LOADER for Concurrent CPM on the AT

   o      Check  the  address jumper on  the  DSI-32  coprocessor 
board.  The  address jumper block is just left of and  below  the 
three  coloured lights.  The address jumper block is a 2 pin by 3 
pin array of pins. Below are the two possible configurations, one 
for  the IBM PC/XT and compatibles,  and the other for the IBM AT 
and compatibles.

     o--o                          o--o
     o--o                          o  o
     o  o                          o--o

   IBM PC/XT                      IBM AT

   o       If the correct loader diskette is being used  and  the 
address jumpers are also correct then the last possibility is the 
the  loader type in use.  Definicon Systems supplies two types of 
loaders on each distribution diskette.  One is called LOADINT and 
the other is LOADMEM. LOADINT uses hardware interrupt IRQ2 on the 
IBM bus to interface the DSI-32 to the IBM. LOADMEM uses a memory 
polled method of performing the interface.

The default loader on the loader distribution diskette (LOAD)  is 
LOADINT.  In  most computers the interrupt version of the  loader 
will  work  correctly  but  in some computers the  IRQ2  line  is 
already  used by another board and thus the DSI-32 interface will 
not work. In this case the LOADMEM loader may be used instead  by 
simply  deleting  the existing LOAD and copying LOADMEM to  LOAD. 
Ensure  you  make  these changes to a copy  of  the  distribution 
diskette and NOT the original.

If  the  use  of  LOADMEM enables the DSI-32  board  to  function 
properly  then  it  will  also  be  necessary  to  make  a  minor 
modification to the board as described below.
.pa
Firstly, identify the Integrated Circuit (IC) at the bottom right 
of the DSI-32 (near the 25 pin Serial connector), its part number 
should be PAL20X4. Secondly, extract the IC out of the socket and 
bend up pin 14, then insert the IC back into the socket. Below is 
a diagram of the IC and its pin numbers:

   Pin 12 o--o--o--o--o--o--o--o--o--o--o--o Pin 1
          |                                |
          |         PAL20X4 (BFLAGS)       (
          |                                |
   Pin 13 o--o--o--o--o--o--o--o--o--o--o--o Pin 24
             ^|
             |
           Pin 14


   o      If the DSI-32 is still not working after all the  above
steps  then  prior  to  calling  Definicon  Systems  perform  the 
following two steps:

     a)   run the BOARDTES program and copy down the  status  of
the three coloured lights.

     b)   run the LOAD program without any options and write down
all the messages that are displayed on the monitor.

     c)   switch  your computer off and then back on  again  and
copy down the status of the three coloured lights.  If this phase 
works  correctly then the red light should be off,  and the green 
and the orange lights should be on.

   o       The  DSI-32 coprocessor board occupies  64K  bytes  of 
memory  space  in both the IBM PC/XT and IBM AT  compatibles.  It 
also occupies two input/output port addresses.

     In the IBM PC/XT address mode the DSI-32 occupies 64K  bytes 
of  memory from 0E0000 Hex to 0EFFFF Hex and input/outputs  ports 
150 Hex to 15F Hex and 160 Hex to 16F Hex.

     In  the IBM AT address mode the DSI-32 occupies 64K bytes of 
memory  from 0D0000 Hex to 0DFFFF Hex and input/output ports  2A0 
Hex to 2AF Hex and 2B0 Hex to 2BF Hex.

