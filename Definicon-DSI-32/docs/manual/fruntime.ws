.pl 44








                     DEFINICON SYSTEMS, INC.



                    DSI-32 COPROCESSOR BOARD
                   & MS/PC DOS SYSTEM SOFTWARE




        

     FORTRAN RUNTIME LIBRARY     

     and

     COMPILER REFERENCE MANUAL






                                        part# DOC20080-1.1
.pa
                            SECTION 1
                  Getting Started with FORTRAN

1.1 System Requirements

To  operate the Definicon Systems' FORTRAN compiler,  your system 
must satisfy the following requirements:

 o   A DSI-32000 coprocessor card,  with a minimum of 256K  bytes 
     of memory.

 o   An IBM compatible clone with a minimum of 96K bytes of  free 
     memory  exclusive  of  operating system  requirements  (256K 
     minimum of memory recommended).

 o   Enough  disk space to contain the compiler,  the program  to      
     compile  and the output file (can be run  from  floppy,  but 
     hard disk highly recommended).

1.2 Runtime Requirements

To execute the compiled,  assembled and linked programs you  will 
need the following computer resources:

 o   A DSI-32000 coprocessor card,  with a minimum of 256K  bytes 
     of memory.

 o   An IBM compatible clone  with a minimum of 96K bytes of free 
     memory exclusive of operating system requirements.

Note   that   both  the  system  requirements  and  the   runtime 
requirements are for an absolutely minimum system.  A DSI-32 card 
with only 256K bytes of memory will severely restrict the size of 
function that can be compiled. 1M byte of memory will enable very 
large  functions  to be compiled.  Also due to the  size  of  the 
FORTRAN  compiler (around 200k bytes of executable code),  a hard 
disk drive,  or at the very least two floppy disk drives, will be 
particularly useful in retaining your sanity.

1.3 FORTRAN Components

The  Definicon  Systems'  FORTRAN compiler  package  consists  of 
several components,  some executable.   The executable components 
are  the FORTRAN compiler,  the assembler and the linker.   Table 
1.1 details the various components that are a part of the FORTRAN 
system.
.pa
               Table 1.1 FORTRAN System components
-----------------------------------------------------------------
|  Component  |  Disk File  |           Description             |
-----------------------------------------------------------------
|  compiler      FC.E32        the compiler module              |
|                FC.CMD        the CP/M command parser          |
|                FC.EXE        the MS/DOS command parser        |
|                LOAD.CMD      the CP/M program loader          |
|                LOAD.EXE      the MS/DOS program loader        |
|                                                               |
|  assembler     AS.E32 &      the enhanced compiler compatible |
|                AS.CMD/AS.EXE assembler                        |
|                AS32032.OPC   the opcode file for the assembler|
|                                                               |
|  linker        LN.E32 &      the enhanced compiler compatible |
|                LN.CMD/LN.EXE linker                           |
|                                                               |
|  libraries     CRTLIB.O32    standard C functions (see runtime|
|                              section)                         |
|                CLIB.O32      low level C routines             |
|                FLIB.O32      standard Fortran funtions.       |
|                                                               |
|  sample        HELLO.F       simple program                   |
|  programs      README.DOC    updated notes on FORTRAN system  |
|                              and documentation                |
-----------------------------------------------------------------
.pa
1.4 Minimum Working Disk

To  compile  a program,  a diskette with the  following  programs 
forms a minimum configuration:

 o   FC.E32                 the compiler
 o   FC.EXE or FC.CMD       the command parser for CP/M or MS/DOS
 o   LOAD.EXE or LOAD.CMD   the DSI-32 program loader
 o   32IO.E32               the DSI-32 communications module

To  assemble  and  link  the  program  ready  for  execution  the 
following programs form a minimum configuration:

 o   AS.E32                 the assembler  
 o   AS.CMD or AS.EXE       the command parser for the assembler 
 o   AS32032.OPC            the opcode file for the assembler
 o   LN.E32                 the linker
 o   LN.CMD or LN.EXE       the command parser for the linker
 o   CRTLIB.O32             C runtime library
 o   CLIB.O32               low level C functions
 o   FLIB.O32               Fortran runtime functions
 o   LOAD.EXE or LOAD.CMD   the DSI-32 program loader
 o   32IO.E32               the DSI-32 communications module
 o   LNF.D32                default file that controls the linker 
                            and   specifies  the   libraries   to 
                            include.
.pa
1.5 Compiling the demonstration program

The following section describes how to compile, assemble and link 
the  demonstration  program  supplied  on  the  FORTRAN  Language 
diskette.  The instructions assume that you are familiar with the 
operation of your operating system whether it is CP/M or MS/DOS.

Prior to following these instructions be sure to make a backup of 
your  master FORTRAN diskette(s) and store the original(s)  in  a 
safe place.

1)   Create  a  FORTRAN  working disk,  and  an  assembler/linker 
     working  disk with the files as shown in  Section  1.4.   If 
     your  system  has a hard disk then just move all  the  files 
     from  the FORTRAN system diskette(s) to an area on the  hard 
     disk.
     
     With your operating system disk in drive A and your  FORTRAN 
     work  disk  in drive B which contains a copy of the  HELLO.F 
     program,  you  are  ready to compile and  test  your  sample 
     program.  To look at the program you can type the program as 
     follows:

     B>TYPE HELLO.F

     PROGRAM HELLO
     WRITE (*,*) 'Welcome to the world of 32 bit computers'
     END
.pa
2)  Now we are ready to compile the program.

     B>FC HELLO

     Note that it is not necessary to enter the .F extension when      
     invoking the compiler, the command parser performs that task      
     automatically.    When   the   compiler   has   loaded   and 
     successfully  started compiling your program it will display 
     its  signon  message.  At  the end of  the  compilation  the 
     compiler will display any errors that have occurred and then 
     return you to the operating system prompt. If no errors have 
     occurred   then  you  are ready to  assemble  and  link  the 
     program.

3)  Now we are ready to assemble the program.

     B>AS HELLO

     Note that as for the compiler,  the assembler  automatically      
     searches  for  the .A32 extension.  Once the  assembler  has      
     successfully loaded,  it will display its signon message. As      
     each assembly pass progresses, the pass will be displayed on 
     the  console.  Any errors that are detected are displayed on 
     the console as they occur.

4)  Now we are ready to link the program.

     B>LN FILE=HELLO

     The linker will link the program with the FORTRAN  libraries 
     and  produce a file that is executable on the DSI-32  board. 
     The executable file will have a .E32 extension.
.pa
5)  Now we can load and run the program.

     B>LOAD HELLO

     Welcome to the world of 32 bit computers

     B>

     Note  that  this example concentrated on  linking  a  single 
     Fortran  program,  but  in the real world a typical  Fortran 
     program may consist of several separately compiled  modules. 
     Below is an example of how to link several modules to form a 
     single executable module. In addition we will assume that we 
     have our own library, mylib.o32, that we want to search.

     A>LN FILE=file1,file2,file3,file4 LIB=mylib

     The linker will first read all the symbol information  about 
     the  files into its tables and then after satisfying as many 
     of  the symbols as possible it will then search any  of  the 
     user   specified  libraries  to  resolve  any  import/export 
     references that have been left unresolved.
     Note  that to build a user library you must have  access  to 
     the Definicon librarian, LIB.
.pa
                            SECTION 2
                  Operating the FORTAN compiler

2.1 General Overview

The   Definicon  Systems'  FORTRAN  compiler  is  a  single  pass 
optimizing compiler.  It performs automatic register optimization 
and  will put variables and pointers into registers depending  on 
various  parameters  such as the lifetime of a variable  and  its 
intended use (e.g.  if the address of a variable is taken at some 
stage in a module,  then the compiler will not place the variable 
in a register).

- The sizes of various Fortran variables are listed below:
     integer                 32 bits
     logical                 32 bits
     real                    32 bits
     double precision        64 bits 
     character*1              8 bits
     integer*1                8 bits
     integer*2               16 bits
     integer*4               32 bits
     logical*1                8 bits
     logical*2               16 bits
     logical*4               32 bits 
     complex                 64 bits
     double complex         128 bits
 
- Program  size and data size are limited only by  the  available 
memory;  small model, medium model, large model, etc. limitations 
of  8088/8086/80286  IBM-PC clone systems do not exist  with  the 
National Semiconductor's 32000 series of 32 bit microprocessors. 
.pa
2.2 Compiler Command Line

The  command  line invokes the FORTRAN  compiler,  specifies  the 
source file to compile and passes any special instructions to the 
compiler.  The maximum compiler command line is 128 characters in 
length.  The form of the command line is shown below:

     B>FC    option_switches    source_filename

Note that the .F filetype extension does not have to be  supplied 
since the parser will automatically append the correct extension. 
If  an  extension is supplied in the command line then that  file 
will  be  compiled regardless of the .F convention.  Also  if  no 
extension is supplied on the command line and a file that has  no 
extension  but  has  the  right name  exists,  then  it  will  be 
compiled.

2.3 Stopping the Compiler

The  FORTRAN compiler may be stopped at any stage of  compilation 
by entering a <CTRL>C at the console (hold the CTRL key down  and 
hit the C key).  The compiler will immediately terminate and exit 
back to the operating system.  Note that all programs that run on 
the DSI-32 card can be aborted in this way, as well as assembler, 
linker  and user programs.  Upon termination,  all files that are 
open  are automatically closed.  Note also that any output  files 
that  were open will be closed in whatever state they were in  at 
the time <CTRL>C was entered. The output files will not be erased 
but they will probably contain incomplete data.
.pa
2.4 Error Messages

All  error  messages are displayed during the  compilation.   The 
line number in which the error was detected and the type of error 
are displayed on the console.

2.5 Compiler Command Line Options

Command  line option switches enable the user to control  certain 
functions of the compiler.  An option switch always starts with a 
minus  sign  and  follows with  certain  alphanumeric  sequences. 
Spaces  must  not be left between the any of the characters  that 
make up an option switch but at least one space must separate  an 
option  switch from any other parameter.  Table 2.1  details  the 
available options and their function.

Command line options are entered as follows:

     B>FC -O1 -O2 PROGRAM
.pa
-----------------------------------------------------------------
|    Option    |              Description                       |
-----------------------------------------------------------------
|    -c            causes comments to be output with the pre-   |
|                  processor output                             |
|                                                               |
|    -dname        define "name" to the preprocessor with the   |
|                  value 1.  Equivalent to "#define name 1"     |
|                                                               |
|    -dname=string define "name" to the preprocessor with the   |
|                  value "string".  Equivalent to entering      |
|                  "#define name string" at the top of the file |
|                                                               |
|    -e            do not compile the program, just place the   |
|                  output from the preprocessor onto the console|
|                                                               |
|    -istring      included filenames that do not start with "/"|
|                  are searched for in the directory "string"   |
|                                                               |
|    -o1           attempt to optimize the program to be as fast|
|                  possible.  The Definicon System's C compiler |
|                  generally performs many optimizations that   |
|                  are normally options on other compilers, so  |
|                  it may be counter productive to use this     |
|                  optimization switch as the code produced is  |
|                  around 10% larger.  Experimenting with this  |
|                  option will allow you to determine when it   |
|                  will prove to be useful                      |
|                                                               |
-----------------------------------------------------------------
(continued next page)
.pa

(continued)
-----------------------------------------------------------------
|    -o2           allow the optimizer to assume that memory    |
|                  locations do not change except by explicit   |
|                  stores.  That is, the optimizer is guaranteed|
|                  that no memory locations are I/O device      |
|                  registers that can change asynchronously     |
|                                                               |
|    -uname        undefine the predefined preprocessor symbol  |
|                  "name".  Equivalent to "#undef name".        |
|                                                               |
-----------------------------------------------------------------
.pa
                     FORTRAN Runtime Library

TYPE*          FUNCTION       DEFINITION               ARGUMENTS*
-------------- -------------- ------------------------ ----------
INTEGER        INT(a)         convert a to INTEGER     any type
INTEGER        IFIX(a)        convert a to INTEGER     REAL*4
INTEGER        IDINT(a)       convert a to INTEGER     REAL*8
REAL*4         REAL(a)        convert a to REAL*4      any type
REAL*8         DREAL(a)       convert a to REAL*8      COMPLEX*16
REAL*4         FLOAT(a)       convert a to REAL*4      INTEGER
REAL*4         SNGL(a)        convert a to REAL*4      REAL*8
REAL*8         DBLE(a)        convert a to REAL*8      any type
COMPLEX*8      CMPLX(a)       convert a to COMPLEX*8   any type
COMPLEX*16     DCMPLX(a)      convert a to COMPLEX*16  any type
INTEGER        ICHAR(a)       convert a to INTEGER     CHARACTER
CHARACTER      CHAR(a)        convert a to CHARACTER   INTEGER

REAL           AINT(a)        truncate a to REAL       REAL
REAL*8         DINT(a)        truncate a to REAL*8     REAL*8
REAL           ANINT(a)       round a to whole number  REAL
REAL*8         DNINT(a)       round a to whole number  REAL*8
INTEGER        NINT(a)        round a to INTEGER       REAL
INTEGER        IDNINT(a)      round a to INTEGER       REAL*8

any type       ABS(a)         absolute value of a      any type
INTEGER        IABS(a)        INTEGER absolute value   INTEGER
REAL*8         DABS(a)        REAL*8 absolute value    REAL*8
COMPLEX        CABS(a)        COMPLEX absolute value   COMPLEX
COMPLEX*16     CDABS(a)       COMPLEX*16 absolute val  COMPLEX*16

INTEGER/REAL*  MOD(a,b)       modulus of a/b           INTEGER/REAL
REAL*4         AMOD(a,b)      modulus of a/b           REAL*4
REAL*8         DMOD(a,b)      modulus of a/b           REAL*8
INTEGER/REAL   SIGN(a,b)      sign transfer of a to b  INTEGER/REAL
INTEGER        ISIGN(a,b)     INTEGER sign transfer    INTEGER
REAL*8         DSIGN(a,b)     REAL*8 sign transfer     REAL*8

INTEGER/REAL   DIM(a,b)       a-b positive difference  INTEGER/REAL
INTEGER        IDIM(a,b)      INTEGER pos. difference  INTEGER
REAL*8         DDIM(a,b)      REAL*8 pos. differnece   REAL*8

INTEGER/REAL   MAX(a,b,...)   maximum of a,b,...       INTEGER/REAL
INTEGER        MAX0(a,b,...)  INTEGER max of a,b,...   INTEGER
REAL*4         AMAX1(a,b,...) REAL*4 max of a,b,...    REAL*4
REAL*4         AMAX0(a,b,...) REAL*4 max of a,b,...    INTEGER
INTEGER        MAX1(a,b,...)  INTEGER max of a,b,...   REAL*4
REAL*8         DMAX1(a,b,...) REAL*8 max of a,b,...    REAL*8
INTEGER/REAL   MIN(a,b,...)   minimum of a,b,...       INTEGER/REAL
INTEGER        MIN0(a,b,...)  INTEGER min of a,b,...   INTEGER
REAL*4         AMIN1(a,b,...) REAL*4 min of a,b,...    REAL*4
REAL*4         AMIN0(a,b,...) REAL*4 min of a,b,...    INTEGER
INTEGER        MIN1(a,b,...)  INTEGER min of a,b,...   REAL*4
REAL*8         DMIN1(a,b,...) REAL*8 min of a,b,...    REAL*8

REAL*8         DPROD(a,b)     REAL*8 product of a * b  REAL*4

REAL*4         AIMAG(a)       imaginary part of a      COMPLEX*8
REAL*8         DIMAG(a)       imaginary part of a      COMPLEX*16
COMPLEX*8      CONJG(a)       conjugate of a           COMPLEX*8
COMPLEX*16     DCONJG(a)      conjugate of a           COMPLEX*16

REAL/COMPLEX   SQRT(a)        square root of a         REAL/COMPLEX
REAL*8         DSQRT(a)       REAL*8 square root of a  REAL*8
COMPLEX*8      CSQRT(a)       COMPLEX*8 square root    COMPLEX*8
COMPLEX*16     CDSQRT(a)      COMPLEX*16 square root   COMPLEX*16

REAL/COMPLEX   EXP(a)         e to the a power         REAL/COMPLEX
REAL*8         DEXP(a)        REAL*8 e to the a power  REAL*8
COMPLEX*8      CEXP(a)        COMPLEX*8 e to the a     COMPLEX*8
COMPLEX*16     CDEXP(a)       COMPLEX*16 e to the a    COMPLEX*16

REAL/COMPLEX   LOG(a)         natural logarithm of a   REAL/COMPLEX
REAL*4         ALOG(a)        REAL*4 natural logarithm REAL*4
REAL*8         DLOG(a)        REAL*8 natural logarithm REAL*8
COMPLEX*8      CLOG(a)        COMPLEX*8 natural log    COMPLEX*8
COMPLEX*16     CDLOG(a)       COMPLEX*16 natural log   COMPLEX*16
REAL           LOG10(a)       log base 10 of a         REAL
REAL*4         ALOG10(a)      REAL*4 log base 10 of a  REAL*4
REAL*8         DLOG10(a)      REAL*8 log base 10 of a  REAL*8

REAL/COMPLEX   SIN(a)         sine of a                REAL/COMPLEX
REAL*8         DSIN(a)        REAL*8 sine of a         REAL*8
COMPLEX*8      CSIN(a)        COMPLEX*8 sine of a      COMPLEX*8
COMPLEX*16     CDSIN(a)       COMPLEX*16 sine of a     COMPLEX*16
REAL/COMPLEX   COS(a)         cosine of a              REAL/COMPLEX
REAL*8         DCOS(a)        REAL*8 cosine of a       REAL*8
COMPLEX*8      CCOS(a)        COMPLEX*8 cosine of a    COMPLEX*8
COMPLEX*16     CDCOS(a)       COMPLEX*16 cosine of a   COMPLEX*16
REAL/COMPLEX   TAN(a)         tangent of a             REAL/COMPLEX
REAL*8         DTAN(a)        REAL*8 tangent of a      REAL*8
COMPLEX*8      CTAN(a)        COMPLEX*8 tangent of a   COMPLEX*8
COMPLEX*16     CDTAN(a)       COMPLEX*16 tangent of a  COMPLEX*16

REAL/COMPLEX   ASIN(a)        arc sine of a            REAL/COMPLEX
REAL*8         DASIN(a)       REAL*8 arc sine of a     REAL*8
COMPLEX*8      CASIN(a)       COMPLEX*8 arc sine of a  COMPLEX*8
COMPLEX*16     CDASIN(a)      COMPLEX*16 arc sine of a COMPLEX*16
REAL/COMPLEX   ACOS(a)        arc cosine of a          REAL/COMPLEX
REAL*8         DACOS(a)       REAL*8 arc cosine of a   REAL*8
COMPLEX*8      CACOS(a)       COMPLEX*8 arc cosine     COMPLEX*8
COMPLEX*16     CDACOS(a)      COMPLEX*16 arc cosine    COMPLEX*16
REAL/COMPLEX   ATAN(a)        arc tangent of a         REAL/COMPLEX
REAL*8         DATAN(a)       REAL*8 arc tangent of a  REAL*8
COMPLEX*8      CATAN(a)       COMPLEX*8 arc tangent    COMPLEX*8
COMPLEX*16     CDATAN(a)      COMPLEX*16 arc tangent   COMPLEX*16
REAL/COMPLEX   ATAN2(a,b)     arc tangent of a/b       REAL/COMPLEX
REAL*8         DATAN2(a,b)    REAL*8 arc tangent a/b   REAL*8
COMPLEX*8      CATAN2(a,b)    COMPLEX*8 arc tan a/b    COMPLEX*8
COMPLEX*16     CDATAN2(a,b)   COMPLEX*16 arc tan a/b   COMPLEX*16

REAL/COMPLEX   SINH(a)        hyperbolic sine of a     REAL/COMPLEX
REAL*8         DSINH(a)       REAL*8 hyperbolic sine   REAL*8
REAL/COMPLEX   COSH(a)        hyperbolic cos of a      REAL/COMPLEX
REAL*8         DCOSH(a)       REAL*8 hyperbolic cos    REAL*8
REAL/COMPLEX   TANH(a)        hyperbolic tan of a      REAL/COMPLEX
REAL*8         DTANH(a)       REAL*8 hyperbolic tan    REAL*8

LOGICAL        LGE(a,b)       a >= b?                  CHARACTER
LOGICAL        LGT(a,b)       a > b?                   CHARACTER
LOGICAL        LLE(a,b)       a <= b?                  CHARACTER
LOGICAL        LLT(a,b)       a < b?                   CHARACTER
LOGICAL        EOF(a)         is unit a at EOF?        INTEGER

-----------------------------------------

* - Types specified without a precision (i.e.  REAL as opposed to 
REAL*8)  indicate that the precision is not defined and therefore 
may  be of any precision.   Types specified in addition to  other 
types (i.e. REAL/COMPLEX) indicate that both types are applicable 
to the function or argument.   The term "any type" indicates that 
the function or argument is type independent.
