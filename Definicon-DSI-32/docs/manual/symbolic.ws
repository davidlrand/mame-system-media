.pl 62

















                     DEFINICON SYSTEMS, INC.



                    DSI-32 COPROCESSOR BOARD
                   & MS/PC DOS SYSTEM SOFTWARE









        



     SYMBOLIC DEBUGGER USER'S GUIDE

     and

     REFERENCE MANUAL








.pa
1.0 Symbolic debugger overview

The  Definicon symbolic debugger,  replaces the debugger that  is 
supplied  with  the  DSI-32  coprocessor  board.  It  allows  the 
programmer  to  debug a 32000 executable  program  with  symbolic 
labels.  Since the symbolic debugger is totally upward compatible 
to the standard debugger, it is also named MON. 

The  debugger syntax is as similar to DEBUG (under MSDOS) and SID 
(under  C/CPM) as possible.  Most features of standard  debuggers 
are available in MON, such as program trace, memory dump and code 
disassembly.  Extra  features exist in MON such as being able  to 
display  and change the contents of both the Floating Point Unit, 
FPU, and the optional Memory Management Unit, MMU.

When the MMU is installed two extra breakpoints are enabled which 
allow  the  programmer to set a breakpoint on either  a  read  or 
write to a data location. This powerful debugging feature enables 
a  programmer to locate the portion of code that may be illegally 
overwriting some data location.

The following sections treat the <start addr> parameter as either 
being  a hexadecimal string or a symbol name.  All  symbol  names 
must  start with '.' to inform the debugger that the string is  a 
symbol  and  not  a hexadecimal string.  MON  will  automatically 
search for the symbol file when it is loading a program,  thus to 
debug the program TEST.E32 the user would type the following:

     LOAD MON TEST

In  the above example,  if MON can not find the file  "TEST.S32", 
then  the symbolic information is not available and  symbols  can 
not be specified.  In this case MON will act the same as the non-
symbolic debugger.

Note
To  utilise the features of the symbolic debugger,  the file that 
is being debugged must have an associated symbol file. The symbol 
file is output by the newer versions of the DSI linker,  LN.  The 
newer  version  of  the  linker is  supplied  with  the  symbolic 
debugger.  The  directive  that causes the linker to  output  the 
symbol file is SYMBOL,  refer to the linker manual for all  other 
directives.

The  following sections show the syntax of the commands that  are 
supported  by MON as well as giving an example of a sample  debug 
session.
.pa
1.1 The Display memory command

Syntax:   D [<start addr> [<length>]]

The 32032 memory is dumped in hexadecimal and in ASCII  beginning 
with the <start address> for <length> bytes.  Any characters that 
do  not have a legal ASCII character representation are displayed 
as '.' in the ASCII portion of the display.  If the <start  addr> 
parameter is not supplied,  the debugger will started  displaying 
from  the next consecutive location since the last dump  request, 
or from the start of the user program.  If the <length> parameter 
is not supplied, the default length of 128 is used.



Example:

Definicon 32000 Symbolic Debugger 1Meg Version x.xx
*D3000

.cw 10
 003000 27 D8 83 36 6F 07 0E 0B 01 27 D0 8F F8 97 06 8F    ''..6o....'......'
 003010 F8 EF D4 2C 85 A6 00 01 04 1A 7B 5D D0 04 1F A8    '...,......{]....'
 003020 3C 0A 11 17 A8 3C 97 06 24 6F AD 30 ED A7 00 30    '<....<..$o.0...0'
 003030 7F 02 17 A8 80 8C 6F AD 80 80 ED A7 00 80 7F 02    '......o.........'
 003040 28 43 29 20 31 39 38 35 20 44 65 66 69 6E 69 63    '(C) 1985 Definic'
 003050 6F 6E 20 53 79 73 74 65 6D 73 20 49 6E 63 2E 20    'on Systems Inc. '
 003060 56 65 72 73 69 6F 6E 20 31 2E 30 61 2A 2A 2A 2A    'Version 1.0a****'
 003070 11 00 AF D4 8F FC 8F C8 00 EF D4 8F F8 62 0C 2B    '.............b.+'
.cw12
*
*D._mystrt 30

.cw10
 003040 28 43 29 20 31 39 38 35 20 44 65 66 69 6E 69 63    '(C) 1985 Definic'
 003050 6F 6E 20 53 79 73 74 65 6D 73 20 49 6E 63 2E 20    'on Systems Inc. '
 003060 56 65 72 73 69 6F 6E 20 31 2E 30 61 2A 2A 2A 2A    'Version 1.0a****'
.cw 12
*




Note
In   the   second  command  sequence  instead  of  specifying   a 
hexadecimal start address,  a symbol address was specified.  Note 
that  all  symbols that are output from the Green Hill  compilers 
start with an underscore character.
.pa
1.2 Substitute memory

Syntax:   S <addr>

Display and substitute the hex data at <addr>

Valid inputs are:

 o  Carriage returns alone (proceed to next byte), 
 o  A hex number and <CR> (causes the previous memory value to be
    replaced by the hex number)
 o  -  (go back to previous location)
 o  . (finish this substitution session).

Example:

*s3000
003000 27 56
003001 D8 09
003002 83 -
003001 09 02
003002 83 .
*
________  ___
   |       |Typed by operator
   |Output from monitor

1.3 List memory in assembler mnemonics

Syntax: L [<addr>]

Lists the contents of the memory as 32032 instructions,  starting 
at <addr>.
 
Example:
Definicon 32000 Symbolic Debugger 1Meg Version x.xx
*L
Module name = START   , Symbol name = __MAIN_  
008490 ADDR    ARGB,R2
008494 MOVD    H'3FF,R1
00849A MOVB    H'E,R0
00849D SVC     
00849E MOVD    H'2020(SB),CHEAPL
0084A6 MOVD    H'2024(SB),CHEAPH
0084AE BSR     H'8541
0084B1 CXP     H'1
0084B3 CXP     __strt   
0084B5 CXP     ___vers_ 
0084B7 CMPD    R0,H'6A
0084BD BHS     H'84CD
*
1.4 Display Registers

Syntax:   R         to display the CPU data registers
          RC        to display the CPU control registers
          RF        to display the Floating Point registers
          RM        to display the optional MMU registers

Example:
.cw 10

Definicon 32000 Monitor x.xx
*r
      R0       R1       R2       R3       R4       R5       R6       R7
       0        0        0        0        0        0        0        0
*rc
 PSR     PC     SP     FP     SB     US     IS   INTB  MOD
   0      0  17514      0 20AD6F      0  17514  FE248    0
LPRD    SB,@H'20
*rf
  FS       F0       F1       F2       F3       F4       F5       F6       F7
   0        0 40240000    40000    40000    40000    40000    40000 77FFFFFF
*
.cw 12


1.5 Change Registers

Syntax:   C<register>=<value>

Changes the contents specified register to the given hex value.

Example:

.cw 10
*r
      R0       R1       R2       R3       R4       R5       R6       R7
       0        0        0        0        0        0        0        0
*rc
 PSR     PC     SP     FP     SB     US     IS   INTB  MOD
   0      0  17514      0 20AD6F      0  17514  FE248    0
LPRD    SB,@H'20
*rf
  FS       F0       F1       F2       F3       F4       F5       F6       F7
   0        0 40240000    40000    40000    40000    40000    40000 77FFFFFF
*cr0=ff
*r
      R0       R1       R2       R3       R4       R5       R6       R7
      FF        0        0        0        0        0        0        0
*cf0=3e2d
.pa
*rf
  FS       F0       F1       F2       F3       F4       F5       F6       F7
   0     3E2D 40240000    40000    40000    40000    40000    40000 77FFFFFF
*
.cw 12

Note:  RF  will  cause a system that is configured for an FPU  to 
hang if the FPU is not present.

1.6 Fill memory

Syntax:   F <start_addr> <end_addr> <data>

This  command  allows  memory  to  be  filled  with  a  specified      
data  pattern.  The  fill  command  fills  memory  starting  from      
the  specified  start  address through to the  end  address  with      
byte sized data. The addresses are assumed to be inclusive.

Example:

.cw 10
*d6000
 006000 C5 58 BE C4 C5 14 D7 1D 22 38 7C A5 F0 1C C0 6C    '.X......"8|....l'
*f6001 6005 aa
*d6000
 006000 C5 AA AA AA AA AA D7 1D 22 38 7C A5 F0 1C C0 6C    '........"8|....l'
*
.cw 12

1.7 Move memory

Syntax:     M <source_addr> <destination_addr> <length>

This  command  moves  memory  from  the  source  address  to  the      
destination address for the specified length.

Example:

.cw 10
*d6000 10
 006000 C5 58 BE 00 00 00 00 00 00 00 00 00 00 00 00 00    '.X..............'
*m6000 6005 3
*d6000 10
 006000 C5 58 BE 00 00 C5 58 BE 00 00 00 00 00 00 00 00    '.X....X.........'
*
.cw 12
.pa
1.8 Set Breakpoints

Syntax: CBP<n>=<addr>

Will set the nth breakpoint to be at address <addr>.
Currently n=0,1 are reserved for the MMU
          n=2,3...F are available for user specified PCs

Note  that  when breakpoint 0 or 1 is specified that an MMU  must 
be  present  in  the  system and that  an  optional  'R'  or  'W' 
parameter  must be entered after the command.  ie the  user  must 
enter: CBP<n>=<addr> <R | W> where <n> ie either 0 or 1.

Note that breakpoint F (decimal 15) is reserved by the monitor to 
handle temporary breakpoints when the G command is invoked.

Note  also that the map file that may optionally be  output  from 
the  linker,  LN,  is  useful for finding the locations of  entry 
points.

Example:  see example in section 1.12

1.9 Set module table Base address

Syntax:  B <addr> <offset>

     This command reads the module table entry from the specified 
     address and adjusts the SB and MOD tables  accordingly.  The 
     PC  is  set  to  the  value in the  module  table  plus  the 
     specified offset. This command must be issued prior to using 
     the  any of the execute or trace commands.  The module table 
     is automatically read when the monitor and the program to be 
     debugged is loaded.


1.10 Go execute program

Syntax:   G [<addr> [,<break>]]

     This command instructs the debugger to commence execution of 
     the  program.  Execution starts at the current PC unless  an 
     alternate  address  <addr>  is  supplied  by  the  user.   A 
     temporary breakpoint can be set at the <break> address. This 
     breakpoint  is  removed  when  it  is  hit.  Note  that  the      
     appropriate  CPU  registers must have been set up via the  B 
     command, see above.

Example:  see example in section 1.12
.pa
1.11 Trace execution

Syntax:     T [<length>]

     This command enables the user to trace a program.  If length 
     is  specified then <length> instructions are executed  prior 
     to  the  program  being exited and the current  CPU  control 
     registers and instruction being displayed.  The trace begins 
     from the current displayed PC.

Example:  see example in section 1.12

1.12 Example of a debugging session

In  this  example  we will debug the prime number  sieve  program 
written in C. The debugger will automatically start disassembling 
from the start of the program.  It is the user's responsibilty to 
determine  the  entry point of the  user  program.  For  programs 
written  in  C,  the user entry point is  '_main',  for  programs 
written in Fortran the user entry point is '_MAIN_'.

A>LOAD MON SIEVE                     ;tell DOS what we want to do
Definicon 32000 Symbolic Monitor 1Meg Version BETA
*l
Module name = START   , Symbol name = __MAIN_  
00807C ADDR    ARGB,R2
008080 MOVD    H'3FF,R1
008086 MOVB    H'E,R0
008089 SVC     
00808A MOVD    H'2020(SB),CHEAPL
008092 MOVD    H'2024(SB),CHEAPH
00809A BSR     H'812D
00809D CXP     H'1
00809F CXP     __strt   
0080A1 CXP     ___vers_ 
0080A3 CMPD    R0,H'6A
0080A9 BHS     H'80B9
*l
0080AB ADDR    H'80CB{PC},TOS
0080B1 CXP     _eprintf 
0080B3 CMPD    H'0,TOS
0080B9 ADDR    ARGB+H'4,TOS
0080BD MOVD    ARGB,TOS
0080C1 CXP     _main                    ;this is the call to the
0080C3 MOVQD   H'0,TOS                  ;prime program 
0080C5 CXP     _exit    
0080C7 CXP     __exit   
0080C9 BR      H'80C9
.pa
*g,._main                                ;Put the  breakpoint  at
                                         ;the user entry point
B=F
 PSR     PC     SP     FP     SB     US     IS   INTB  MOD
  40   8000  FB403      0   82A4      0  FB403   34C0   80
ENTER   [R3,R4,R5,R6,R7],H'4
*l
Module name = prime.c , Symbol name = _main    
008000 ENTER   [R3,R4,R5,R6,R7],H'4     ;It looks like the .A32
008003 ADDR    @H'A,-H'4(FP)            ; file.
008007 MOVQD   H'1,R2
008009 MOVQD   H'0,R7
00800B ADDR    @H'1FFF,R1
00800F ADDR    H'200F(SB),R0
008015 MOVB    R2,H'0(R0)
008018 ADDQD   -H'1,R1
00801A ADDQD   -H'1,R0
00801C CMPQD   H'0,R1
00801E BNE     H'8015
008020 MOVQD   H'0,R1
*g                                      ;run it to completion
Found 1899 primes                       ;a little while later
A>                                      ;It exits to DOS
.cw 12
.pa
1.13 Alphabetic command summary

B <address> <offset>

     This command reads the module table entry from the specified 
     <address> and adjusts the SB and MOD tables accordingly. The      
     PC  is  set  to the <value> in the  module  table  plus  the           
     specified  offset.  This command generally is not used since      
     all  the necessary registers are automatically set  up  when           
     the monitor and the progam to be debugged are loaded.

C <register | breakpoint | memory>=<value>

     This  command allows any of the registers to the CPU set  to 
     be altered including all the CPU registers,  the FPU and the 
     MMU  registers.  Below is the list of the register names and 
     what   they  correspond  to.   This  command   also   allows 
     breakpoints  to be set or cleared and memory locations to be 
     initialised with any data value.  Note that breakpoint 0 and 
     1 are only permissible if an MMU is installed, in which case 
     an  additional  option must be entered after the  breakpoint 
     address.  It  is either an 'R' or 'W' for break on  read  or 
     break  on  write access.  eg ' CBP0=5000 R ' would  cause  a 
     breakpoint to occur when memory location 5000 was read by  a 
     data  fetch.  An optional count can also be appended to  the 
     command,   eq CBP0=5000 R C=1A.  This example causes the MMU 
     to break the CPU on the 1A'th read of location 5000 hex.

     R0 - R7             General purpose CPU registers
     PSR                 The processor status register
     PC                  The program counter
     SP                  The stack pointer
     FP                  The frame pointer
     SB                  The static base register
     US                  The user stack
     IS                  The interrupt (system) stack
     INTB                The interrupt base table
     MOD                 The module table register

     FS                  The FPU status register
     F0  - F7            The  general  purpose  floating  point
                         registers

     MM                  The physical MMU status register
     EA                  The invalidate address register
     SC                  The sequential count registers (2 x 
                         16bit)
     BC                  The break count register
     PF0                 Profile register 0
     PF1                 Profile register 1
.pa
     BP0                 Breakpoint register 0
     BP1                 Breakpoint register 1

     TB0/TB1             These registers contain the Level 1 page
                         table   pointers   for  virtual   memory 
                         support and address translation
     MS                  Copy  of  MMU status register  on  entry 
                         into monitor, ie the user value of MSR

     HS                  The start of the available heap
     HE                  The end of the available heap
     HP                  The current heap pointer

     To  change the contents of a memory address,  three versions
     versions of the change command may be used as shown below.

     CMBaddr=value [V]      change a byte with optional verify
     CMWaddr=value [V]      change a word with optional verify
     CMDaddr=value [V]      change a double with optional verify

D [<address> [<length>]]

     This  command  displays the contents of the memory  starting 
     from  location  <address>  for  <length>  bytes.  The  ASCII 
     representation is displayed on the far right of the display. 
     If  no address is supplied then the display starts from  the       
     end of the previous display.  If the address is not supplied 
     then  8 lines of 16 bytes across of data will  be  displayed 
     from  the  last dump location or from the start of the  user 
     program.  Unless the length is supplied 8 lines of 16  bytes 
     will be displayed.

F <start_address> <end_address> <data>

     This  command  allows memory to be filled with  a  specified 
     data  pattern.  The fill command fills memory starting  from 
     the  specified <start address> through to the <end  address> 
     with  byte  sized  data.  The addresses are  assumed  to  be 
     inclusive.

G [<addr>] [,<break>]

     This command instructs the debugger to commence execution of 
     the  program.  Execution starts at the current PC or at  the 
     optional supplied address.  This command also allows a  soft 
     break  point  to be set at the <break>  address.  This  soft 
     breakpoint  is immediately removed the first time that it is 
     hit.
.pa
L [<address> [<length>]]

     This  command  disassembles  the  data  from  the  specified 
     <address>  and  displays  it  one line at a  time  with  the 
     address of the instruction.  The <length> is the hexadecimal      
     number of lines to display.

M <source_address> <destination_address> <length>

     This  command moves memory from the <source address> to  the      
     <destination address> for the specified <length>.

P <register | breakpoint | memory>

     This  command  will  display the contents of  the  specified 
     register,  breakpoint  or  memory  location.  The  register, 
     breakpoint,  and  memory  names are the same as  for  the  C 
     command.

Q

     This command terminates a debug session and returns the user 
     to MS/DOS.

RC

     This command displays the CPU control registers,  eg the PC, 
     SB,  MOD etc.  The current instruction is also  disassembled 
     and displayed below the registers.

R

     This command displays the general purpose CPU registers,  eg 
     R0, R1, R2 etc.

RF

     This command displays the FPU floating point registers. Note 
     an  FPU  register  must  be installed  in  the  DSI-32  card 
     otherwise the system will hang up.

RM

     This  command displays the MMU control registers.
.pa
S <addr>

     This command enables memory to be displayed a byte at a time 
     and to be modified accordingly.  The user may enter either a 
     byte,  word,  double or string variable. A string must begin 
     with either a single or double quote.  It is then entered in 
     a  byte  at  a time.  The next address to  be  displayed  is 
     automatically  adjusted  for  the  length  of  the  variable 
     entered.  To  go  back a single byte the minus sign  may  be 
     used, to terminate the command a single dot is required.

T [<length>]

     This command enables the user to trace a program.  If length 
     is  specified then <length> instructions are executed  prior 
     to  the  program  being exited and the current  CPU  control 
     registers and instruction being displayed.  The trace begins 
     from  the  current displayed PC.  Note that  <length>  is  a 
     hexadecimal number.

U <addr>

     This  command  performs  the  identical function  as  the  L 
     command,  it is included for compatibility with other  debug 
     packages.

