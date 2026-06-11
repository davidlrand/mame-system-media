.pl44








                     DEFINICON SYSTEMS, INC.



                    DSI-32 COPROCESSOR BOARD
                   & MS/PC DOS SYSTEM SOFTWARE




        

     PASCAL COMPILER USERS GUIDE

     and

     REFERENCE MANUAL
.pa









             Definicon Systems Compiler Users' Guide
                         Pascal Language

                         October 1, 1984

  This document describes general characteristics of Definicon 
                       Systems' compilers

               (c) 1984 Green Hills Software, Inc.
                (c) 1985 Definicon Systems, Inc.







The  Definicon  Systems Compilers are a ported version  of  Green 
Hills Compilers for the DSI-32 coprocessor board.  All references 
to  Definicon  Systems compilers for the  National  Semiconductor 
32032 are references to the ported version of the compilers.

                                        part# DOC20040-1.1
.pa
1    Pascal Standards

There  are  currently two competing standards  for  Pascal.   The 
first  is  the  international effort  of  the  British  Standards 
Institute  (BSI)  and  the International  Standards  Organization 
(ISO), this standard will be referred to as the BSI/ISO standard.  
The second is the U.S.  effort of the American National Standards 
Institute  (ANSI) and the Institute of Electrical and  Electronic 
Engineers (IEEE),  this standard, ANSI/IEEE770X3.97-1983, will be 
referred to as the ANSI/IEEE standard.

The  BSI/ISO standard has two levels,  Level 1 is a  superset  of 
Level 0.

The  Definicon Systems Pascal compiler implements  the  ANSI/IEEE 
standard  and the BSI/ISO Level 0.   In the future the  Definicon 
Systems  Pascal  will implement BSI/ISO Level  1.   In  addition, 
Definicon  Systems  Pascal  implements  many  of  the  extensions 
present in the Berkeley 4.2BSD "pc" Pascal compiler.

The  documenation of the Definicon Systems Pascal consists of  an 
ANSI/IEEE Pascal definition plus this document.


2    Additions to the basic Pascal Language

2.1  Comment Delimiters

The  symbol  "(*" is equivalent to "{" and "*)" is equivalent  to 
"}".
.pa
2.2  Argc and Argv"

There  is an additional builtin function,  argc,  which takes  no 
arguments  and  returns an integer.   Argc returns the number  of 
command  line arguments.   The number of command  line  arguments 
includes the command name, so argc() is always greater than zero.

There is an additional builtin procedure,  argv,  which takes two 
arguments.  The first argument is an integer which is an argument 
number.   The second argument is a string variable.   Argv  reads 
the  command  line argument specified by the first argument  into 
the  string  variable  with null padding or  blank  extension  as 
appropriate.   It  is  illegal to attempt to access  an  argument 
number  greater  than  argc()-1.   Argument number  zero  is  the 
command name.

2.3  Set Implementation

Sets  of subranges of char,  boolean,  and enumeration types  are 
implemented  as  sets of the basetype.   This allows sets  to  be 
operated upon directly with in line code.

The above rule is inadequate for integer subrange  sets,  because 
the basetype is integer,  and a "set of integer" would require an 
exorbitant amount of memory.  Therefore, sets of integer type are 
implemented as sets of some subrange of integers.

When  the  -X56  (ISO compatibility) flag is  set,  all  sets  of 
integers are implemented as "set of 0..255".   When the -X56 flag 
is  not  set,  by  default,  sets of  integers  are  0..31.   The 
implemenation of "set of 0..31" is much more efficient than  "set 
of 0..255".

If  "set  of  0..31"  is  not  adequate  for  some  program,  the 
implementation  of  the sets may be changed by defining a set  of 
integer subrange with the name "intset".   This type intset  will 
become  the implementation of integer sets over the range of this 
type.

For example:

     type intset = set of 0..63;

will cause the compiler to implement all sets as 8 byte values.

2.4  Separate Compilation

2.4.1  External directive

A  new  identifier "external" is recognized as an alternative  to 
"forward" in procedure and function declarations.   It  specifies 
that  the  named  procedure or function exists  in  a  separately 
compiled module.

2.4.2  Declarations module

In every executable program there must be one file which consists 
of  a program statement and a main begin-end block.   This may be 
the  entire program or it may only be a part of  a  program.   If 
some  procedures or functions referenced in the main program  are 
declared  external  the procedures and functions must  be  linked 
with  the main program file to obtain a complete program.   These 
external functions may be written in Pascal, assembly language or 
any other programming language.
.pa
To  write  external procedures and functions in Pascal  create  a 
file,  called  a declarations module,  which consists only  of  a 
series of declarations.  It must not contain a program statement, 
a  main  begin-end block or a final period.   The procedures  and 
functions declared at the top level of a declarations module  and 
at  the  outermost  level  of the  program  module  are  declared 
external to the linker.

It  is also legal to declare constants,  types,  and variable  in 
declarations  modules.   Variable declared at the outer level  of 
declarations  modules  are also declared external to the  linker.  
Variables  declared  at the outer level of  program  modules  are 
local to the main program,  they are not external!

The same variable may be declared in several declarations modules 
with  each  name referring to the same variable.   If the  system 
linker  has  a limit to the symbol length,  this  limit  must  be 
observed  for  all external names.   Some versions of the  Pascal 
compiler  prepend an underscore or period to all external  names, 
this extra character must also be counted under any symbol length 
restrictions.

2.4.3  Relaxed Declaration Order

Standard Pascal requires all constants to be declared before  any 
types,  all  types to be declared before any variables,  and  all 
variables  to be defined before any procedures or functions.   To 
make  Pascal  easier  to use,  Definicon  Systems  Pascal  allows 
declarations  to be in any order,  provided that  the  definition 
always  appears  before  any reference to an  object  (except  as 
allowed under standard Pascal).
.pa
2.4.4  #include

If a "#include" appears at the start of a line followed by a file 
name  enclosed in single quotes (apostrophes),  the named file is 
read as input to the program.  At the end of the named file input 
is  taken  starting at the beginning of the  line  following  the 
#include.   This feature works exactly like #include in C.  It is 
intended  to  be used to include declarations common  to  several 
modules.

2.5  C Extensions

Several features of C have been added to Definicon Systems Pascal 
to make it more useful for systems programming.

2.5.1  Hexadecimal Constants

The  C  syntax  of  0x<hex digits> is  accepted  for  hexadecimal 
integer constants.

2.5.2  Case Sensitivity

For  compatibility  with C,  by default,  the  compiler  is  case 
sensitive.   For  example,  the  identifiers "ABC" and "abc"  are 
distinct.   Keywords  are  recognized only in  lower  case.   For 
example,  the identifier "BEGIN" is not a keyword.   This default 
is  not  compatible with either Pascal standard,  both  of  which 
specify that case is to be ignored.  The -X56 or -X59 switch will 
make the compiler conform to the standard interpretation of case.
.pa
2.5.3  Additional operators

The  most commonly used C operators have been added to  Definicon 
Systems Pascal.   Note that "/" and "/=" represent floating point 
division for integer operands, unlike in C.

      &
          The  unary  address  of  operator  takes  one  variable 
          operand and returns a pointer to its operand.
     ~
          The   one's  complement  operator  takes  one   integer 
          operand.
     &
          The  bitwise  logical  and operator takes  two  integer 
          operands.  Same precedence as "*".
     %
          The C modulo function takes two integer operands.  This 
          modulo  function is the natural remainder from  integer 
          divide supplied by the Target.   Generally the sign  of 
          the result is the sign of the second operand.
     >>
          The  right  shift operator takes two integer  operands.  
          The  first  operand is shifted right by the  number  of 
          bits specified by the second operand.   Same precedence 
          as "*".
     <<
          The  left  shift operator takes two  integer  operands.  
          The first operand is shifted left by the number of bits 
          specified  by the second operand.   Same precedence  as 
          "*".
     |
          The  bitwise  logical  or operator  takes  two  integer 
          operands.  Same precedence as "+".
     :=
          The assignment operator assigns the second operand into 
          the  first operand and returns the value of  the  first 
          operand.
     +=
          "a += b" is equivalent to "a := a + b".
     -=
          "a -= b" is equivalent to "a := a - b".
     *=
          "a *= b" is equivalent to "a := a * b".
     /=
          "a /= b" is equivalent to "a := a / b".
     %=
          "a %= b" is equivalent to "a := a % b".
     &=
          "a &= b" is equivalent to "a := a & b".
     |=
          "a |= b" is equivalent to "a := a | b".
     >>=
          "a >>= b" is equivalent to "a := a >> b".
     <<=
          "a <<= b" is equivalent to "a := a << b".

.pa
2.6  Input and Output

The  predefined  file  "input" is initially set to  the  standard 
input device.   The predefined file "output" is initially set  to 
the standard output device.  The standard input and output is the 
PC.

2.6.1  Interactive I/O

All  input  files are organized for  interactive  I/O.   In  many 
implementations  of  Pascal  the program will wait for  input  to 
become  available  on a file when it is  reset.   In  particular, 
since  "input"  is  reset by default before  the  program  starts 
executing,  many implementations will wait for a line to be typed 
before they will begin executing the user's program.   Under  the 
Definicon Systems I/O library resetting a file will not cause the 
program to wait for input to become available.   The program will 
only  wait  if an access is made to the file buffer  variable  or 
information is requested about the file, such as, eoln, or eof.

2.6.2  Default Field Widths

The default field width for the standard data types are given  in 
the following table.

	char		1
	integer		12
	Boolean		length of true or false
	real		25
	string		length of string
.pa
2.6.3  Second operand to reset and rewrite

The  built  in procedures reset and rewrite can have an  optional 
second parameter.   The second parameter must be a  string.   The 
string  is  interpreted as name of the file to  be  opened.   The 
string  is interpreted by the host operating system and therefore 
programs  using this feature may not be transportable to  systems 
with different file naming conventions.

2.7  Predefined Constants and Types

The   following   symbols  are  defined  as  if   the   following 
declarations  occurred just before the beginning of  each  source 
file.

     const
	maxint =	2147483648;
     type
	integer =	-2147483647..maxint;
	char =    	chr(0)..chr(127);

The type "real" is 64 bits.
.pa
2.8  New and Dispose

If  initial  values for tag fields are specified  as  "new",  the 
record  will be allocated with the minimum possible size for  the 
tag fields specified.   It is illegal to change tag fields set in 
this  way,  but the compiler does not check to see that it is not 
done.  If the tagfield is changed illegally, serious problems may 
occur because insufficient memory may have been allocated for the 
variant designated by the new tag field value.  Attempts to store 
into these new fields may store into adjacent memory allocated to 
an entirely unrelated variable.

2.9  Record Comparison

Record  comparison  is implemented.   However,  it is illegal  if 
either of the operands is not of the maximum variant size.  It is 
wise  to limit record assignment and comparison only  to  records 
which do not have variants.

3    Compile Time Options

The following compile time options are recognized by Pascal.

     -X56
          Allow only ANSI/IEEE features.  Generate error messages 
          for any non-standard constructs which are used.

     -X57
          Generate bounds checking code for subranges and arrays.

     -X59
          Turn off case sensitivity.
