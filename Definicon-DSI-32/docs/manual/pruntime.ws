.pl62













                     DEFINICON SYSTEMS, INC.




                    DSI-32 COPROCESSOR BOARD
                   & MS/PC DOS SYSTEM SOFTWARE








        

     PASCAL RUNTIME LIBRARY     

     and

     COMPILER REFERENCE MANUAL









                                        part# DOC20070-1.1
.pa
                            SECTION 1
                   Getting Started with Pascal

1.1 System Requirements 

To  operate the Definicon Systems' Pascal compiler,  your system  must 
satisfy the following requirements.

 o   DSI-32000 coprocessor card,  with a minimum of 256K bytes of 
     memory.

 o   An IBM compatible clone with a minimum of 96K bytes of  free 
     memory exclusive of operating system requirements.

 o   Enough  disk space to contain the compiler,  the program  to      
     compile and the output file.

1.2 Runtime Requirements 

To  execute the compiled,  assembled and linked programs you will 
need the following computer resources:

 o   DSI-32000 coprocessor card,  with a minimum of 256K bytes of 
     memory.

 o   An IBM compatible clone  with a minimum of 96K bytes of free 
     memory exclusive of operating system requirements.

Note   that  both  the  system  requirements  and   the   runtime 
requirements are for an absolutely minimum system.  A DSI-32 card 
with  only 256K bytes of memory will severly restrict the size of 
functions  that can be compiled.  1M byte of memory  will  enable 
very large functions to be compiled.  Also due to the size of the 
Pascal  compiler (around 200k bytes of executable code),  a  hard 
disk drive,  or at the very least two floppy disk drives, will be 
particularly useful in retaining your sanity.

1.3 Pascal Components 

The  Definicon  Systems'  Pascal  compiler  package  consists  of 
several  components,  some executable.  The executable components 
are the Pascal compiler,  the assembler and the linker. Table 1.1 
depicts  the  various components that are a part  of  the  Pascal 
system.
.pa
               Table 1.1 Pascal System components
-----------------------------------------------------------------
|  Component  |  Disk File  |           Description             |
-----------------------------------------------------------------
|  compiler      PC.E32        the compiler module              |
|                PC.CMD        the CP/M command parser          |
|                PC.EXE        the MS/DOS command parser        |
|                LOAD.CMD      the CP/M program loader          |
|                LOAD.EXE      the MS/DOS program loader        |
|                                                               |
|  assembler     AS.E32        the enhanced compiler compatible |
|                AS.CMD/AS.EXE assembler                        |
|                AS32032.OPC   the opcode file for the assembler|
|                                                               |
|  linker        LN.E32        the enhanced compiler compatible |
|                LN.CMD/LN.EXE linker                           |
|                                                               |
|                                                               |
|  libraries     CLIB.O32      standard C functions             |
|                PLIB.O32      Pascal runtime functions         |
|                                                               |
|                                                               |
|  sample        HELLO.P       simple program                   |
|  programs      READ.ME       updated notes on Pascal system   |
|                              and documentation                |
-----------------------------------------------------------------

1.4 Minimum Working Disk 

To compile a program a diskette with the following programs forms 
a mimimum configuration.

 o   PC.E32                 the compiler
 o   PC.EXE or PC.CMD       the command parser for CP/M or MS/DOS
 o   LOAD.EXE or LOAD.CMD   the DSI-32 program loader
 o   32IO.E32               the DSI-32 communications module
 o   any required .H files
.pa
To  assemble  and  link  the  program  ready  for  execution  the 
following programs form a minimum configuration.

 o   AS.E32                 the assembler  
 o   AS.CMD or AS.EXE       command parser for the assembler
 o   AS32032.OPC            the opcode file for the assembler
 o   LN.E32                 the linker
 o   LN.CMD or LN.EXE       command parser for the linker 
 o   CLIB.O32               C runtime library
 o   PLIB.O32               Pascal runtime library
 o   LOAD.EXE or LOAD.CMD   the DSI-32 program loader
 o   32IO.E32               the DSI-32 communications module
 o   LNP.D32                default file that controls the linker 
                            and   specifies  the   libraries.

1.5 Compiling the demonstration program 

The following section describes how to compile, assemble and link 
the  demonstration  program  supplied on  the  Pascal   Langauage 
diskette.  The instructions assume that you are familiar with the 
operation of your operating system whether it be CP/M or MS/DOS.

Prior to following these instructions be sure to make a backup of 
your  master  Pascal diskette(s) and store the original(s)  in  a 
safe place.

1)   Create  a  Pascal  working  disk,  and  an  assembler/linker 
     working disk with the files as shown in Section 1.4. If your 
     system has a hard disk then just move all the files from the 
     Pascal system diskette(s) to an area on the hard disk.
     
     With  your operating system disk in drive A and your  Pascal  
     work  disk in drive B which contains a copy of  the  HELLO.P 
     program   you  are  ready to compile and  test  your  sample 
     program.  To look at the program you can type the program as 
     follows:

     B>TYPE HELLO.P

     program hello(input,output);
     begin
          writeln('Welcome to the world of 32 bit computers');
     end.

2)  Now we are ready to compile the program.

     B>PC HELLO

     Note that it is not necessary to enter the .P extension when 
     invoking the compiler, the command parser performs that task 
     automatically. When the compiler has loaded and successfully 
     started  compiling  your program it will display its  signon 
     message.  At  the end of the compilation the  compiler  will 
     display  any  errors that have occured and will then  return 
     you  to  the operating system prompt.  If  no   errors  have 
     occured   then  you  are  ready to  assemble  and  link  the      
     program.

3)  Now we are ready to assemble the program.

     B>AS HELLO

     Note  that as for the compiler,  the assembler automatically 
     searches  for  the .A32 extension.  Once the  assembler  has 
     successfully loaded it will display its signon  message.  As 
     each  pass  progresses,  the pass will be displayed  on  the 
     console.  Any  errors that are detected are displayed on the 
     console as they occur.

4)  Now we are ready to link the program.

     B>LN FILE=HELLO

     The  linker  will link the program with the Pascal libraries  and 
     produce a file that is executable on the DSI-32  board.  The 
     executable file will have a .E32 extension.

5)  Now we can load and run the program.

     B>LOAD HELLO

     Welcome to the world of 32 bit computers

     B>

     Note  that  this  example concentrated on linking  a  single 
     Pascal  program,  but  in the real world  a  typical  Pascal 
     program  may consist of several separately compiled modules. 
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

                           SECTION 2
                    Operating the Pascal compiler

2.1 General Overview 

The   Definicon  Systems'  Pascal  compiler  is  a  single   pass 
optimizing compiler.  It performs automatic register optimization 
and  will put variables and pointers into registers depending  on 
various  parameters  such as the lifetime of a variable  and  its 
intended use, (e.g. if the address of a variable is taken at some 
stage  in a module then the compiler will not place the  variable 
in a register).  Several other points are noteworthy of mention.

- The size of various Pascal variables is listed below:
     
     integer                 32 bits
     real                    64 bits
     maxint                  2**31-1
     minint                 -2**31 

- Program  size and data size are limited only by  the  available 
memory; small model, medium model, large model etc limitations of 
8088/8086/80268  IBM  clone type systems do not  exist  with  the 
National Semiconductor's 32000 series of 32 bit microprocessors. 
This  means  that  there is no need for INTEGER4  types  (4  byte 
integers), and the like.

2.2 Compiler Command Line 

The  command  line  invokes the Pascal  compiler,  specifies  the 
source file to compile and passes any special instructions to the 
compiler.  The maximum compiler command line is 128 characters in 
length.  The form of the command line is shown below:

B>PC    option_switches    source_filename

Note that the .P filetype extension does not have to be  supplied 
since the parser will automatically append the correct extension. 
If  an extension is supplied in the command line then  that  file 
will  be  compiled regardless of the .P convention.  Also  if  no 
extension  is supplied on the command line and a file that has no 
extension but has the right name exists then it will be compiled.

2.3 Stopping the Compiler 

The  Pascal  compiler may be stopped at any stage of  compilation 
by  entering a <CTRL>C (hold the CTRL key down and hit the C key) 
at the console.  The compiler will then immediately terminate and 
exit  back to the operating system.  Note that all programs  that 
run  on the DSI-32 card can be aborted in this way,  as  well  as 
assembler, linker and user programs.  Upon termination, all files 
that  are  open  are automatically closed.   Note also  that  any 
output files that are open will be closed in whatever state  they 
were in at the time control C was entered.  The output files will 
not be erased but they will probably contain incomplete data.

2.4 Error Messages 

All  error  messages are displayed during the  compilation.   The 
line number in which the error was detected and the type of error 
are displayed on the console.

2.5 Compiler Command Line Options 

Command  line option switches enable the user to control  certain 
functions of the compiler.  An option switch always starts with a 
minus  sign  and  follows with  certain  alphanumeric  sequences. 
Spaces  must not be left between any of the characters that  make 
up  an option switch,  but at least one space must  separate  one 
option  switch  from  another parameter.  Table 2.1  details  the 
available options and their function.

Command line options are entered as shown below:

B>PC -O1 -O2 -DDEBUG=OFF PROGRAM

Note 
      There is no case sensistivity for the command line options. 
The command line parser PC.CMD or PC.EXE converts symbol names to 
lowercase,  this  means that the above command line will  produce 
the  directive  to the compiler "#define debug =  off",  NOT  the 
directive "#define DEBUG = OFF". 
.pa
2.6 C runtime linkage

Since Pascal, C and Fortran use the same linkages, it is possible 
to make use of all of the C runtime functions from within Pascal. 
Indeed,  all of the Pascal runtime system is written in C.  The C 
runtime library manual may be ordered separately.  As an example, 
to  use  the  C  openb function,  to  open  a  binary  file,  the 
declaration:

function openb(var filename:char; ty:integer):integer;external;

would be used.

This allows more intimate contact with the operating system. Some 
delarations  may  become  tricky in Pascal (such as the  case  of 
printf),  and you may choose to write portions of your program in 
C to avoid this.  Pascal programs may call routines written in C, 
and C may call Pascal functions/procedures.
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
|    -istring      include file names that do not start with "/"|
|                  are searched for in the directory "string"   |
|                                                               |
|    -o1           attempt to optimize the program to be as fast|
|                  possible.  The Definicon  Pascal compiler    |
|                  generally performs many optimizations that   |
|                  are normally options on other compilers, so  |
|                  it may be counter productive to use this     |
|                  optimization switch as the code produced is  |
|                  around 10% larger.  Experimenting with this  |
|                  option will allow you to determine when it   |
|                  will prove to be useful                      |
|                                                               |
|    -o2           allow the optimizer to assume that memory    |
|                  locations do not change except by explicit   |
|                  stores.  That is, the optimizer is guaranteed|
|                  that no memory locations are I/O device      |
|                  registers that can change asynchronously     |
|                                                               |
|    -uname        Undefine the predefined preprocessor symbol  |
|                  "name". Equivalent to "#undef name".         |
|                                                               |
|    -x56          allow only ANSI/IEEE features.               |
|                                                               |
|    -x57          Generate bounds checking code for subranges  |
|                  and arrays.                                  |
|                                                               |
|    -x59          Turn off case sensitivity.                   |
|                                                               |
-----------------------------------------------------------------
.pa
                            SECTION 3
                     Pascal Runtime Library

ABS

Syntax:

     function abs(x);

Description:

ABS  returns the absolute value of X.  X must evaluate to a real, 
or integer expression. ABS returns the same type as X.

Examples:

     a := abs(4.952); (* a will equal 4.952 *)
     a := abs(-14.0); (* a will equal 14.0 *)

.pa
ARCTAN

Syntax:

     function arctan(x) : real;

Description:

ARCTAN returns the arctangent of X, expressed in radians. 

Examples:

     a := arctan(1.0);
     a := arctan(0.707);
.pa
CHR

Syntax:

     function chr(x) : char;

Description:

CHR returns the character with the ascii value of X.

Examples:

     writeln(chr(65)); (* Print an unppercase A *)
     
.pa
COS

Syntax:

     function cos(x) : char;

Description:

COS  returns  the  cosine of the angle X.  X may be  a  real,  or 
integer expression.

Examples:

     a := cos(12);
     a := cos(14.234); 
     
.pa
DISPOSE

Syntax:

     procedure dispose(var x:pointer);

Description:

DISPOSE  deallocates  space that NEW allocates.  Upon  return  of 
DISPOSE, the space allocate is returned to the free space pool.

Examples:

     dispose(q);

.pa
EOLN,_EOF

Syntax:

     function eoln : boolean;
     function eoln(var f:text) : boolean;
     function eof : boolean;
     function eof(var f:text) : boolean; 

Description:

The  functions EOF and EOLN return status on the end of  a  file, 
and  end  of  a  line,   respectively.  EOF  and  EOLN,  with  no 
parameters,  refer to the console. With parameters, they refer to 
the appropriate file specified.

Examples:

     while (not(eof(inp))) do begin ... end;
     if eoln then writeln('End of line');
.pa
EXP

Syntax:

     function exp(x) : real;

Description:

EXP returns the exponential of the real or integer quantity x.

Examples:

     a := exp(2.30258);
     a := exp(10); 
.pa
GET

Syntax:

     procedure get(var q: file variable);


Description:

GET  gets the next record from the file,  and places it into  the 
window variable.

Examples:

     get(q);
.pa
LN

Syntax:

     function ln(x) : real;

Description:

LN   returns  the  natural  logarithm  of  the  real  or  integer 
expression x.

Examples:

     a := ln(10);
.pa
NEW

Syntax:

     procedure new (var p : pointer; varients);

Description:

NEW allocates space for a record of the pointer's type,  and sets 
the pointer to this newly allocated space.  For variant  records, 
NEW allocates enough space for the largest record.

Examples:
     
     new(recp);
     new(recp, var1);
.pa
ODD

Syntax:

     function odd(x) : boolean;
     
Description:

ODD  returns  TRUE if the integer expression x is  odd,  else  it 
returns FALSE.

Examples:
     
     if odd(2) then writeln('Not possible error.');
     t := odd(q);
.pa
ORD

Syntax:

     function ord(x) : integer;

Description:

ORD  returns  the ordinal value of the expression x.  If x  is  a 
pointer type, then ORD returns the contents of the pointer.

Examples:
     
     type
          color = (red, green, blue, black);
     var
          q : color;

     begin
          q := green;
          writeln('Ordinal of green is ',ord(q)); 
     end.

.pa
PAGE

Syntax:

     procedure page(file variable);

Description:

PAGE  skips to the top of a new page when a text file is printing 
by placing a page-feed character (0x0c) in the file.  If no  file 
variable is specified, the standard output is assumed.

Examples:

     page;
     page(q);
.pa
PRED

Syntax:

     function pred(x) : scalar;

Description:

PRED returns the predecessor of the scalar expression x. 

Examples:

     a := pred(a);
     writeln('The letter before Z is',pred('Z'));          
.pa
PUT

Syntax:

     procedure put(file variable);

Description:

PUT  writes  the current window variable to the  file  associated 
with the file variable.

Examples:

     put(f);
.pa
READ,_READLN

Syntax:

     procedure read  (file variable, variable, variable..)
     procedure readln(file variable, variable, variable..)

Description:

READ  and READLN read data from the file associated with the file 
variable. If no file variable is supplied, then data is read from 
the standard input.  READLN always skips to the beginning of  the 
next line after the variable list has been satisfied.  READ never 
skips data.

Examples:

     read(a, b, c);
     readln(file, a, b, c);
.pa
RESET

Syntax:

     procedure reset(file variable, filename);

Description:

RESET  moves  the  file  pointer to the  beginning  of  the  file 
'filename'.

Examples:

     reset(inp, 'hello.p');
.pa
REWRITE

Syntax:

     procedure rewrite(file variable, filename);

Description:

REWRITE creates a new file on the disk, with the name 'filename', 
in preparation for writing data.

Examples:

     rewrite(outp, 'hello.p');
.pa
ROUND

Syntax:

     function round(x) : integer;

Description:

ROUND converts the real expression x to an integer by rounding up 
or down to the nearest integer value.

Examples:

     a := round(b);
     writeln(round(1.234));

.pa
SIN

Syntax:

     function sin(x) : real;

Description:

SIN returns the sine of the real or integer expression x.

Examples:

     a := sin(1.027);
.pa
SQR

Syntax:

     function sqr(x);

Description:

SQR  returns  the  square  of  x.  X may be  a  real  or  integer 
expression, and SQR returns the same type.

Examples:

     a := sqr(5)
     b := sqr(1.23);
.pa
SQRT

Syntax:

     function sqrt(x) : real;

Description:

SQRT returns the square root of the real or integer expression x.

Examples:

     a := sqrt(5)
     b := sqrt(1.23);
.pa
SUCC

Syntax:

     function succ(x) : scalar;

Description:

SUCC returns the value of the successor of x. 

Examples:

     a := succ(1);
     writeln('The letter after A is ',succ('A')); 
.pa
TRUNC

Syntax:

     function trunc(x) : integer;

Description:

TRUNC returns the integer value of the real or integer expression 
x.  TRUNC  does  not  round,  so all digits to the right  of  the 
decimal point are ignored.

Examples:

     a := trunc(1.0723);
.pa
WRITE,_WRITELN

Syntax:

     procedure write  (file variable, variable, variable..);
     procedure writeln(file variable, variable, variable..);

Description:

WRITE  and WRITELN place the data in the variable list  into  the 
file  associated  with  the  file  variable.  WRITELN  appends  a 
newline,  WRITE  does  not.  If  no file variable  is  specified, 
standard output is assumed.

Examples:

     writeln('Hello, world!');
     write(outp,'A=',a,'B=',b,'C=',c);
.pa
                            Section 4
                     Pascal Run Time Summary
abs
arctan
chr
cos
dispose
eoln, eof
exp
get
ln
new
odd
ord
page
pred
put
read, readln
reset
rewrite
round
sin
sqr
sqrt
succ
trunc
write, writeln
.pa
                            SECTION 5
                      Other Math Functions


floor,_fmod

The  floor  function  returns the largest integer  (as  a  double 
precision  number)  not  greater  than  the  argument.  The  fmod 
function  returns the floating point remainder of the division of 
or the two arguments passed to it.

Declarations

     integer   num;       -- the integer argument
     integer   ret;       -- the result
     real      x,y;       -- double precision arguments
     real      fnum;      -- the floating point argument
     real      fret       -- the floating point result

     function floor(x:real) : real; external;
     function fmod(x:real, y:real) : real; external;

Syntax

     fret  :=  floor(x);
     fret  :=  fmod(x, y);    /*  fret = x - floor(x/y) * y  */

Note

     Fmod  returns  zero if y is zero or if x/y  would  overflow. 
Otherwise  the returned number is fret with the same sign  as  x, 
such that x = iy + fret for some integer i, and |f| < |y|.
.pa
ceil

The  ceil  function  takes  a  double  precision  floating  point 
argument and returns the smallest integer greater or equal to the 
passed argument.

Declarations

     real      argument;   -- the double precision argument
     real      num;        -- the  returned  ceiling  of   the
                              argument

     function ceil(x:real) : real; external;

Syntax

     num   :=  ceil(argument);
.pa
cos,_sin,_tan,_acos,_asin,_atan,_atan2

The  trigonometric  functions  which  take an  angular  value  in 
radians and return a value are cos,  sin, tan, and their inverses 
are acos, asin and atan respectively. 

Declarations

     real      num;        -- a  double precision floating  point
                              number.
     real      x,y;        -- two double precision values
     real      angle;      -- a double  precision  floating point
                              number that expresses the  angle in
                              radians.
function sin(x:real): real -- trigonometric  sine   of  an  angle
                              expressed in radians.
function cos(x:real): real -- trigonometric  cosine  of an  angle
                              expressed in radians.
function tan(x:real): real -- trigonometric  tangent of an  angle
                              expressed in radians.
function asin(x:real):real -- returns the  arcsine  in  the range
                              -PI/2 to PI/2
function acos(x:real):real -- returns the arcscosine in the range
                              0 to PI
function atan(x:real):real -- returns the arctangent in the range
                              -PI/2 to PI/2
function atan2(y:real;
          x:real) : real   -- returns  the  arctangent of y/x  in
                              the  range  -PI to  PI,  using  the
                              signs   or   both   arguments    to
                              determine   the  quadrant  of   the
                              return value

Syntax

     fret   := cos(angle);
     fret   := sin(angle);
     fret   := tan(angle);
     angle  := acos(num);
     angle  := asin(num);  
     angle  := atan(num);
     angle  := atan2(y, x);

Note

     All the above functions test the range of the input argument 
and set the error EDOM (see errno.h file) in the errno  variable, 
if the number is illegal for the function. 
.pa
cosh,_sinh,_tanh

The cosh,  sinh,  and tanh return,  respectively,  the hyperbolic 
cosine, sine and tangent of their argument.

Declarations

     real      angle;      -- the  angle as a double  precision
                              floating point number
     real      num;        -- the double precision result

function cosh(x:real) : real; external;
function sinh(x:real) : real; external;
function tanh(x:real) : real; external;

Syntax

     num  :=  cosh(angle);
     num  :=  sinh(angle);
     num  :=  tanh(angle);
.pa
erf,_erfc

The  erf  and  erfc  functions  return  the  error  function  and 
complementary error functions respectively.
The  erf  function returns the error function of the argument  as 
defined by:
                               x
          erf(x) = 2/sqrt(PI)    e-t**2 dt
                               0

The erfc function returns the complementary error function of the 
argument as defined by:

          erfc(x) = 1.0 - erf(x)

Declarations

     real      x;          -- the double precision argument
     real      num;        -- the double precision result

function erf(x:real) : real; external;
function erfc(x:real) : real; external;

Syntax

     num   :=   erf(x);
     num   :=   erfc(x);
.pa
exp,_log,_log10,_pow,_sqrt

The  exp,  log,  log10,  pow and sqrt functions take the  natural 
exponential,  the natural loqarithm,  the base 10 logarithm,  the 
power and the square root of the argument(s) respectively.

Declarations

     real      res;        -- the double precision result
     real      x,y;        -- two double precision arguments
function exp(x:real): real -- the  value of the constant e raised
                              to   the   specified  argument   (e
                              = 2.7182812845905)
function log(x:real): real -- the  logarithm  base e of a  number
                              (natural logarithm).
function log10(x:real):real - the logarithm base 10 of a number.
function pow(x:real):real  -- take the value of x to the power y
function sqrt(x:real):real -- the square root of a number.

Syntax

     res   :=  exp(x);
     res   :=  log(x);
     res   :=  log10(x);
     res   :=  pow(x, y);     /*   ie res = x ** y   */
     res   :=  sqrt(x);
.pa
j0,_j1,_jn,_y0,_y1,_yn

The j0 and j1 functions return Bessel functions of the first kind 
or orders 0 and 1 respectively. Jn returns the Bessel function of 
x of the first kind of order n.
Y0  and  y1 return Bessel functions of x of the  second  kind  of 
orders 0 and 1 respectively.  Yn returns the Bessel function of x 
of the second kind of order n. The value of x must be positive.

Declarations

     real      x;          -- the double precision argument
     integer   n;          -- the order of the Bessel function
     real      ret;        -- the double precision result

function j0(x:real) : real; external;
function j1(x:real) : real; external;
function jn(n:integer; x:real) : real; external;
function y0(x:real) : real; external;
function y1(x:real) : real; external;
function yn(n:integer; x:real) : real; external;

Syntax

     ret   :=   j0(x);
     ret   :=   j1(x);
     ret   :=   jn(n, x);
     ret   :=   y0(x);
     ret   :=   y1(x);
     ret   :=   yn(n, x);

Note

     Non-positive  arguments  cause  y0,  y1 and yn  to  set  the 
variable errno to EDOM.
     Arguments  that are too large cause y0,  y1 and yn to return 
zero and to set the variable errno to ERANGE.
.pa
                            SECTION 6
                          Math Summary

     floor(x)       ;Returns the largest integer not larger than x
     fmod(x,y)      ;Returns the x-floor(x/y)*y

     ceil(x)        ;Returns the smallest integer not smaller than x

     cos(x)         ;cosine of x
     sin(x)         ;sine of x
     tan(x)         ;tangent of x
     acos(x)        ;arc cosine of x
     asin(x)        ;arc sine of x
     atan(x)        ;arc tangent of x
     atan2(y,x)     ;arc tangent of y/x with overflow protection,
                    ;and  checks to see that the result is -PI to
                    ;PI

     cosh(x)        ;hyperbolic cosine of x
     sinh(x)        ;hyperbolic sine of x
     tanh(x)        ;hyperbolic tangent of x

     erf(x)         ;error function at x
     erfc(x)        ;complimentary error function at x

     exp(x)         ;e^x
     log(x)         ;natural log of x
     log10(x)       ;log base 10 of x
     pow(x,y)       ;x^y
     sqrt(x)        ;square root of x

     j0(x)          ;0th bessel function at x
     j1(x)          ;1st bessel function at x
     jn(n,x)        ;nth bessel function at x
     y0(x)          ;0th bessel function of second kind at x
     y1(x)          ;1st bessel function of second kind at x
     yn(n,x)        ;nth bessel function of second kind at x

