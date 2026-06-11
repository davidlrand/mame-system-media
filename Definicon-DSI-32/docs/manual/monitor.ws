.pl 62

















                     DEFINICON SYSTEMS, INC.



                    DSI-32 COPROCESSOR BOARD
                   & MS/PC DOS SYSTEM SOFTWARE









        



     MON MONITOR USERS GUIDE

     and

     REFERENCE MANUAL








.pa
1.0 Monitor Overview

The  Definicon monitor,  MON,  allows the programmer to  debug  a 
32000  executable programs.  The debugger syntax is as similar to 
DEBUG  (under  MSDOS)  and SID (under C/CPM)  as  possible.  Most 
features  of  standard debuggers are available in  MON,  such  as 
program trace,  memory dump and code disassembly.  Extra features 
exist  in  MON  such as being able to  display   and  change  the 
contents of both the Floating Point Unit,  FPU,  and the optional 
Memory Management Unit, MMU.
When the MMU is installed two extra breakpoints are enabled which 
allow  the  programmer to set a breakpoint on either  a  read  or 
write  to a data location.  This is a powerful debugging  feature 
enables  a  programmer to locate the portion of code that may  be 
illegally overwriting some data location.
The  following sections show the syntax of the commands that  are 
supported  by MON as well as giving an example of a sample  debug 
session.

1.1 The Display memory command

Syntax:   D [<start addr> [<length>]]

The 32032 memory is dumped in hexadecimal and in ASCII  beginning 
with the <start address> for <length> bytes.  Any characters that 
do  not have a legal ASCII character representation are displayed 
as '.' in the ASCII portion of the display.

Example:
.cw 10
Definicon 32000 Monitor x.xx
*D3000
 003000 27 D8 83 36 6F 07 0E 0B 01 27 D0 8F F8 97 06 8F    ''..6o....'......'
 003010 F8 EF D4 2C 85 A6 00 01 04 1A 7B 5D D0 04 1F A8    '...,......{]....'
 003020 3C 0A 11 17 A8 3C 97 06 24 6F AD 30 ED A7 00 30    '<....<..$o.0...0'
 003030 7F 02 17 A8 80 8C 6F AD 80 80 ED A7 00 80 7F 02    '......o.........'
 003040 28 43 29 20 31 39 38 35 20 44 65 66 69 6E 69 63    '(C) 1985 Definic'
 003050 6F 6E 20 53 79 73 74 65 6D 73 20 49 6E 63 2E 20    'on Systems Inc. '
 003060 56 65 72 73 69 6F 6E 20 31 2E 30 61 2A 2A 2A 2A    'Version 1.0a****'
 003070 11 00 AF D4 8F FC 8F C8 00 EF D4 8F F8 62 0C 2B    '.............b.+'
*
*D3040 30
 003040 28 43 29 20 31 39 38 35 20 44 65 66 69 6E 69 63    '(C) 1985 Definic'
 003050 6F 6E 20 53 79 73 74 65 6D 73 20 49 6E 63 2E 20    'on Systems Inc. '
 003060 56 65 72 73 69 6F 6E 20 31 2E 30 61 2A 2A 2A 2A    'Version 1.0a****'
*
.cw12
.pa
1.2 Substitute memory

Syntax:   S <addr>

Display and substitute the hex data at <addr>
Valid inputs are:
Carriage returns alone (proceed to next byte), 
A  hex  number and <CR> (causes the previous memory value  to  be 
replaced  by  the  hex  number)
      -  (go back to previous location)
      . (finish this substitution session).

Example:
Definicon 32000 Monitor x.xx
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

Lists  the  contents of the memory as  32032  code,  starting  at 
<addr>.
 
Example:

Definicon 32000 Monitor x.xx
*l4000
004000  ADDR    H'40BE{PC},R2
004004  MOVD    H'400,R1
00400A  MOVB    H'E,R0
00400D  SVC     
00400E  MOVD    H'2020(SB),EXT(H'4)+H'0
004016  MOVD    H'2024(SB),EXT(H'5)+H'0
00401E  CXP     H'3
004020  CXP     H'1
004022  ADDR    H'40C2{PC},TOS
004028  MOVD    H'40BE{PC},TOS
00402E  CXP     H'0
004030  MOVQD   H'0,TOS

.pa
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

Note: RM will cause a system that is configured for an MMU to the 
hang unless an MMU is present.

1.5 Change Registers

Syntax:   C<register>=<value>

Changes the contents specified register to the given hex value.

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
Definicon 32000 Monitor x.xx
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
Definicon 32000 Monitor x.xx
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

The  monitor  will  be  used  to examine  the  execution  of  the 
SIEVE.E32  benchmark on the distribution disks.  The comments  in 
lower case to the right of the monitor's output were added to the 
printout  to clarify what is happenning.  The addresses that  are 
shown below are hypothetical.

.cw 10
A>LOAD MON SIEVE                        ;tell DOS what we want to do
Definicon 32000 Monitor x.xx
*l                                      ;list the CMAIN code
004000  ADDR    H'40BE{PC},R2
004004  MOVD    H'400,R1
00400A  MOVB    H'E,R0
00400D  SVC     
00400E  MOVD    H'2020(SB),EXT(H'4)+H'0
004016  MOVD    H'2024(SB),EXT(H'5)+H'0
00401E  CXP     H'3
004020  CXP     H'1
004022  ADDR    H'40C2{PC},TOS
004028  MOVD    H'40BE{PC},TOS
00402E  CXP     H'0                     ;This is where the SIEVE.C code 
004030  MOVQD   H'0,TOS                 ; itself is entered.
*l                                      ;Lets look at more code
004032  CXP     H'2
004034  MOVD    TOS,R0
004036  CMPQD   H'0,R0
004038  BEQ     H'404E
00403A  BSR     H'4052
00403C  MOVB    H'A,H'0(R3)
004040  ADDR    H'4078{PC},R2
004046  SUBD    R2,R3
004048  MOVQW   H'2,R1
00404A  MOVB    H'9,R0
00404D  SVC     
00404E  MOVB    H'F,R0                  ;AHA..here is the final exit
004051  SVC                             ;back to DOS
.pa
*cbp2=402e                              ;Lets set a breakpoint where SIEVE
*g                                      ;is entered and execute to there
B=2                                     ;Oh..we hit breakpoint 2
 PSR     PC     SP     FP     SB     US     IS   INTB  MOD
   0   402E  17504      0   2000      0  17504  FE248   80
CXP     H'0                             ;and this is the instruction there
*t                                      ;lets trace to the start of SIEVE
 PSR     PC     SP     FP     SB     US     IS   INTB  MOD
   2   74A4  174FC      0  178B4      0  174FC  FE248   B0
ENTER   [R3,R4,R5],H'2000
*r                                      ;why not....we have the time...
      R0       R1       R2       R3       R4       R5       R6       R7
   178F8      C10 FFFFFFFF        0        0        0        0        0

*l                                      ;Lets see if this code really
0074A4  ENTER   [R3,R4,R5],H'2000       ;looks like our .A32 source
0074AA  MOVQD   H'1,R4
0074AC  MOVQD   H'0,R5
0074AE  MOVQD   H'0,R1
0074B0  MOVQB   H'1,-H'2000(FP)[R1:B]
0074B5  ADDQD   H'1,R1
0074B7  CMPD    H'1FFF,R1
0074BD  BGE     H'74B0
0074BF  MOVQD   H'0,R1
0074C1  CMPQB   H'0,-H'2000(FP)[R1:B]
0074C6  BEQ     H'74EC
0074CB  MOVD    R1,R0
*g                                      ;Sure does, lets execute it
Found 1899 primes                       ;a little while later
A>                                      ;Sure enough, it exits to DOS
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

