.pl44








                     DEFINICON SYSTEMS, INC.



                    DSI-32 COPROCESSOR BOARD
                   & MS/PC DOS SYSTEM SOFTWARE




        

     C COMPILER USERS GUIDE

     and

     REFERENCE MANUAL
.pa









             Definicon Systems Compiler Users' Guide
                           C Language

                         October 1, 1984

This  document  describes  general characteristics  of  Definicon
                       Systems' compilers

               (c) 1984 Green Hills Software, Inc.
                (c) 1985 Definicon Systems, Inc.







The  Definicon  Systems Compilers are a ported version  of  Green 
Hills Compilers for the DSI-32 coprocessor board.  All references 
to  Definicon  Systems compilers for the  National  Semiconductor 
32032 are references to the ported version of the compilers.

                                        part# DOC20020-1.1
.pa
1    Introduction

Definicon  Systems  C  is  a complete  implementation  of  the  C 
programming language.   The basic C language is defined in "The C 
Programming  Language"  by Kernighan and Ritchie  (Prentice-Hall, 
1978).    However,   this  specification  is  very  terse,  often 
imprecise, occasionally misleading, and far from complete.  There 
is a standardization committee working on a precise definition of 
C but at this time there is no authoritative definition of the  C 
language.   The Portable C Compiler (PCC) is the most widely used 
implementation  of  C  and is the primary tool  for  implementing 
UNIX,  the largest and most important body of C code.  Therefore, 
Definicon  Systems has chosen to use PCC,  and in particular  the 
Berkeley 4.2 BSD VAX implementation,  as our definition of the  C 
language.

The Definicon Systems C language contains everything in the basic 
C  language,  as  well as all of the documented Western  Electric 
extensions,  and all of the undocumented features of the Berkeley 
compiler  used  in  implementing UNIX.   There  are  hundreds  of 
extensions  to the basic C language which are implemented in  all 
versions  of PCC.   Without these extensions it is impossible  to 
compile UNIX and many existing C applications programs.   Several 
of the most important of these extensions are listed  below,  but 
this is by no means a complete list.

If  you have a UNIX version of this compiler,  the  documentation 
provided  with  UNIX  (Kernighan  and  Ritchie  and  the  Western 
Electric  Extensions)  plus  this document  constitute  the  user 
documentation of the Definicon Systems compiler.

If  you  have  any  other  version  of  this  compiler  the  user 
documentation  consists  of Kernighan and Ritchie (which  may  be 
obtained from Definicon Systems if you do not have one) plus this 
document.


2    Additions to the basic C Language

2.1  Preprocessor

The Definicon Systems C compilers include a preprocessor which is 
functionally identical to the UNIX C preprocessor.  The basics of 
the  preprocessor are explained in Kernighan and Ritchie,  but as 
with the compiler,  the actual preprocessor is far more  complex.  
Unlike  PCC which depends on an initial text processing pass by a 
preprocessor   program,   the  Definicon  Systems   C   compilers 
preprocess the input program in the compiler itself.   This makes 
the  compilation process faster by requiring only one reading  of 
the source program and by running one less program.
.pa
2.2  Backslash v

Lower  case v is a special backslash character denoting  vertical 
tab.

2.3  void type

There is a type named void.   There are no operations defined  on 
the  type  void.   Void is used primarily in a cast  on  function 
calls to keep lint quiet.

2.4  __LINE__

__LINE__  is  a predefined preprocessor symbol whose value  is  a 
character  string  which consists of the ASCII representation  of 
the current line number within the current file.

2.5  __FILE__

__FILE__  is  a predefined preprocessor symbol whose value  is  a 
character  string which consists of the ASCII  representation  of 
the current file name.

2.6  Structures

Structures  may be assigned,  passed as parameters,  and returned 
from functions.

The  return  of  structures  from functions is  done  in  a  non-
reentrant fashion for compatibility with the PCC  implementation.  
Struct  return  values are returned by copying the  return  value 
into a static variable in the function.  A pointer to this static 
variable  is  returned  in  place of  the  struct.   The  calling 
function  then copies the static variable.   If an  interrupt  or 
signal  occurs after the functions returns but before the  caller 
had  time  to copy the return value,  and an interrupt or  signal 
called  the  function which was interrupted it  would  reuse  the 
static  return variable.   When the interrupted routine continued 
it  would  access  the value of the static variable  set  in  the 
interrupt  level  routine instead of the value which  would  have 
been accessed had there been no interrupt or signal.

2.7  Enumeration Type

There  is  an enumeration type similar to that  of  Pascal.   Its 
syntax is similar to that of the struct and union declarations.

<enum-specifier>:
	enum { <enum-list> }
	enum <identifier> { <enum-list> }
	enum <identifier>
<enum-list>:
	<enumeration-declaration>
	<enumeration-declaration> , <enum-list>

<enumeration-declaration>:
	<identifier>
	<identifier> = <constant-expression>


The  enumerated  type  name  may be the same as  the  name  of  a 
variable  in a scope but may not be the same as the name  of  any 
struct  or  union  in the  scope.   Each  enumeration-declaration 
declares  a  scalar  constant of  the  enumeration  type.   If  a 
constant  expressions  appears in an  enumeration-declaration  it 
specifies  the ordinal value of the identifier.   If no constant-
expression  is given in an enumeration-declaration the  value  is 
one   greater  than  the  value  of  the  previous   enumeration-
declaration,  unless  the  enumeration-declaration is  the  first 
enumeration-declaration in an enum-list in which case its ordinal 
value is zero.

Enum  types are signed by default for compatibility with PCC!   A 
compile  time  option  is  described  below  which  changes   the 
definition of enum types to a more rational form.


3    Conversion from other systems and compilers

3.1  Byte Ordering

The  MC68000,  Z8000,  and  IBM/370 arrange bytes within  a  word 
differently  than the VAX,  8086,  and NS32000.   This means that 
programs  that rely on the byte ordering,  e.g.  that  declare  a 
variable int in one module and char in another, may not work.

3.2  Registers

The  compiler  will  by default allocate automatic  variables  to 
registers where possible.  A Scalar or pointer variable generally 
qualifies  for  allocation to a register unless  its  address  is 
taken with the "&" operator.  A Floating point variable generally 
qualifies  for  allocation to a register in  architectures  which 
directly  support floating point arithmetic.   The compiler  will 
successfully   allocate  all  eligible  automatic  variables   to 
registers in all but the largest routines.

Variables with a storage class of "register" will be allocated to 
registers before "automatic" variables.
By  default there is no upper limit on how many variables will be 
allocated to registers. That is determined by how their lifetimes 
conflict.   Several  variables  may  be  allocated  to  the  same 
register, if their dynamic lifetimes do not overlap.

3.3  Implied register usage

The Definicon Systems C compiler register allocation algorithm is 
completely unlike that of PCC.  While this will have no effect on 
semantically  correct C programs,  some programs rely on  certain 
aspects of the PCC allocation scheme.

For  instance,  programs  relying on "register"  variables  being 
allocated  sequentially to pass hidden parameters will not  work. 
Hidden returns (using "return;" and expecting to get the value of 
the last expression) will not work either.

3.4  Memory allocation order

The Definicon Systems C compiler allocates memory variables based 
on their size, frequency of use, and other factors. Programs that 
rely on the ordering of variables within memory may not work.

3.5  Bit Fields

The  Definicon  Systems  C compiler supports signed  as  well  as 
unsigned bit fields.  For compatibility with most implementations 
of PCC,  by default, fields are unsigned even if a signed type is 
used  to declare the field.   Unsigned fields are recommended for 
most  applications as they are more efficient to access  on  most 
machines.   For compatibility with the VAX implementation of C, a 
compile time option, described below, is provided which specifies 
that  field whose type is signed is to be interpreted as a signed 
quantity.   The consequences of having signed fields can be  seen 
in the following example.

     {
	struct {int x:2;} y;
	y.x = 3;
	i = y.x; 
     }

In  this example if "x" is an unsigned field,  "i" will have  the 
value  of 3 at the end of the block.   However,  if signed fields 
are  accepted (as under the VAX PCC compiler) "i" will  have  the 
value -1 at the end of the block.

3.6  Extern and Common

In  PCC the default storage class for a variable declared in  the 
outer scope is "common".  That is, the variable will be allocated 
separate  from  this module,  it will be allocated with the  same 
initial address as all other variables of the same name  declared 
in  the  outer scope of other modules with the  "common"  storage 
class, and the size of the variable allocated will be the size of 
the largest of the "common" variables of that name.   In PCC, the 
storage  class  "extern" defines a variable to be a reference  to 
the  "common"  variable of that name.   If there is  an  "extern" 
declaration  for  a  name  there must be at  least  one  "common" 
declaration  of  that name in the program.   There  may  be  many 
"extern"  and  "common" declarations of the same name.   The  PCC 
model  for  "extern"  and "common"  is  supported  the  Definicon 
Systems C compiler.

In some target environments "common" is not implemented, or it is 
implemented   very   poorly.    In  those   cases   a   different 
interpretation  is  made  for  the  default  storage  class.   In 
particular,  if  a  variable is declared "extern" in  one  module 
there  must be exactly one declaration of a variable of the  same 
name  and  type  with the default storage class  in  exactly  one 
module  in  the  same  program.    There  may  be  many  "extern" 
declarations  for  the  variable.   This interpretation  for  the 
default  storage class seems to fit the definition  in  Kernighan 
and Ritchie better than the PCC definition.

If the second method is followed,  a program can be ported to any 
implementation  of C.   The first method is more convenient  when 
using  include files and is the only method used in  UNIX.   Most 
UNIX  programs cannot be ported unchanged to target  environments 
that do not support common.

3.7  Unsigned Char and Unsigned Short

These data types are not in the C manual.   They are supported by  
the  Definicon Systems C compiler as well as many implementations 
of  PCC.   The VAX implementation of PCC will sometimes  evaluate 
them  differently  from the Definicon Systems C  in  expressions.  
For example, the negation operator may subtract an unsigned short 
from 2**16 under PCC (this seems like a bug) and from 2**32 under 
Definicon Systems C (this seems correct).

3.8  Value of Assignment Operators

The statement

     {int i; char c; c = 100; i = c *= 2;}

will  set  'i'  to 200 with some compilers and -55  with  others.  
This is a case of using small data types as a modulo function and 
should be avoided.  The C-VAX and C-32000 compilers implement the 
modulo function.

3.9  Evaluation Order

The  Definicon  Systems  C expression  evaluation  order  is  not 
identical to the PCC ordering, although it may appear deceptively 
similar.   A  noteworthy  case  is the use of the  '?:'  operator 
within routine calls. Definicon Systems C evaluates the question-
mark operator before the call, keeping the result in a temporary.  
The call:

     foo(b?i:i+i, i++);

will evaluate differently under the PCC compiler.

3.10 Case Statement

PCC has a strange interpretation of the "case" statement.   It is 
permissible for the "case" statement corresponding to a  "switch" 
statement to be buried within a complex statement.   That is,  it 
is  legal  to  jump  inside another  statement  with  a  "switch" 
statement.   The  following switch statement will  compile  under 
PCC,  but  will  generate  an error message under  the  Definicon 
Systems C at the line "case 2:".
.pa
     switch (i) {
     case 1:
	if (j) {
		case 2:
		i = 3;
	}
     }

The  only known use of this construct is in the source  code  for 
PCC!

3.11 asm Statement

The asm statement for in-line assembly code is somewhat different 
than the asm statement in PCC.   In particular,  in the Definicon 
Systems  compiler  the  asm  statement can  be  used  anywhere  a 
statement can appear.  In PCC, asm statements are also allowed to 
appear in declarations and between functions.

Since  the  code generated by the Definicon Systems  compiler  is 
substantially   different  than  the  code  generated  by   other 
compilers it is usually necessary to modify most asm statements.

The  asm statement is not supported in compilers  which  generate 
object code directly!

3.12 Detection

Many  of  the  problems  mentioned above  can  be  detected  with 
lint(1).    You should look for variables used before definition, 
routines  using  return  and  return(e),   nonportable  character 
operations,  evaluation order undefined, and routines whose value 
is  used but not set.   Lint is not able to detect programs  that 
rely  on the allocation order of memory variables,  or that  rely 
upon   the  arithmetic  characteristics  of  short  data   types. 
Furthermore,  since  lint does not do actual data flow  analysis, 
the absence of a message does not imply the absence of a problem.


4    Switches

The following compile time options are recognized by C.

-E   Do not compile the program,  instead place the output of the 
     preprocessor  on the standard output file.   This is  useful 
     for   debugging   preprocessor   macros.    The   integrated 
     preprocessor  cannot  generate  output as fast as  the  UNIX 
     "cpp" program, so use "cpp" for big jobs.

-C   If  this  option  is  given,  comments  are  output  in  the 
     preprocessor output.   The default is to strip comments from 
     the output.

-Dname     Define  "name" to the preprocessor with the  value  1.  
     This is equivalent to putting "#define name 1" at the top of 
     the source file.

-Dname=string   Define "name" to the preprocessor with the  value 
     "string".   This  is  equivalent to  putting  "#define  name 
     string" at the top of the source file.

-Uname     Undefine  the predefined preprocessor  symbol  "name".  
     This  is  equivalent to putting "#undef name" at the top  of 
     the source file.
.pa
-X6  Allocate each enum type as the smallest size predefined type 
     which  allows representation of all listed values (that  is, 
     from the list:  "char",  "short",  "int",  "unsigned  char", 
     "unsigned  short",   or  "unsigned").   The  default  is  to 
     allocate as an "int".

-X55 Make  fields of type int,  short,  and char be signed.   The 
     default is for all fields to be unsigned.


5    C Runtime Libraries

The   C  Runtime  library  includes  the  standard   mathematical 
functions,  character string functions,  and I/O functions.   The 
functions specified in Kernighan and Ritchie have their arguments 
as  given there,  other functions are as in UNIX.
