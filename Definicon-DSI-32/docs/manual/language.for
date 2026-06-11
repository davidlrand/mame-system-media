.pl44








                     DEFINICON SYSTEMS, INC.



                    DSI-32 COPROCESSOR BOARD
                   & MS/PC DOS SYSTEM SOFTWARE




        

     FORTRAN COMPILER USERS GUIDE

     and

     REFERENCE MANUAL
.pa









             Definicon Systems Compiler Users' Guide
                        FORTRAN Language

                         October 1, 1984

This document describes general characteristics of Definicon 
                        Systems' compilers

               (c) 1984 Green Hills Software, Inc.
                (c) 1985 Definicon Systems, Inc.







The  Definicon  Systems Compilers are a ported version  of  Green 
Hills Compilers for the DSI-32 coprocessor board.  All references 
to  Definicon  Systems compilers for the  National  Semiconductor 
32032 are references to the ported version of the compilers.

                                        part# DOC20030-1.1
.pa
1    Fortran Standard

The  Definicon  Systems  Fortran  compiler  implements  the  ANSI 
Fortran-77 (Full Language) standard.   It also implements all  of 
the  extensions to Fortran-77 documented in the Berkeley 4.2  BSD 
f77  documentation and many of the undocumented extensions in the 
4.2 BSD f77 implementation.   The documentation for the Definicon 
Systems Fortran compiler consists of the ANSI Fortran-77 standard 
plus this document.

1.1  Validation

For  Fortran validation,  the FORTRAN Compiler Validation  System 
Version  2.0 (1978) from the U.S.  Office of Software Development 
and  the  U.S.   Department  of  Commerce,   National   Technical 
Information Service was used.  Before each compiler release it is 
verified that the compiler correctly runs this validation system.  


2    Extensions to the 4.2 BSD F77 Documentation

The  4.2  BSD  f77 documentation is suitable  for  the  Definicon 
Systems compilers.   The same language,  switches,  and arguments 
are accepted, the code generated is compatible, and the libraries 
are identical.

All  of the enhancements to the Fortran-77 standard  included  in 
the  Berkeley  f77  implementation  have  been  included  in  the 
Definicon Systems implementation.

None  of the Violations of the Fortran-77 standard documented  in 
Section  3  of  the Berkeley f77 manual exist  in  the  Definicon 
Systems Fortran implementation.
The -lI66 command line argument generates proper printer carriage 
control operation on all files and terminals.

The Berkeley f77 restrictions on double precision alignment don't 
apply to the Definicon Systems Fortran.

The  Dummy Procedure Arguments restriction in Berkeley f77  don't 
apply to the Definicon Systems Fortran.

The  switches  -O  and  -g are compatible  in  Definicon  Systems 
Fortran.

The  4.2  BSD  Berkeley documentation  never  gives  the  calling 
sequence for the following additional builtin functions:

     integer*4 function iargc()

     subroutine getarg(arg_number, arg_value)
     integer*4 arg_number
     character*20 arg_value

     subroutine getenv(env_name, env_value)
     character*(*) env_name
     character*20 env_value

     integer function or(i1, i2)

     integer function and(i1, i2)

     integer function xor(i1, i2)

     integer function rshift(value, bits)

     integer function lshift(value, bits)

     integer function not(i1)


3    Illegal Programs

Floating  point computations are done at compile time to simulate 
floating   point   constant   computations.    Illegal   constant 
computations  may  cause unrecoverable floating point  errors  at 
compile time causing the compiler to crash.

The  list  in an assigned goto statement must  be  correct.   The 
optimizer  makes  use  of  the  list  to  determine  data   flow.  
Programmers  have  been known to put in dummy lists because  many 
compilers ignore the list,  this could cause unexpected  results.  
If  no list is present the optimizer assumes that the goto  could 
branch  to  any label which appears in any "assign" statement  in 
the program unit containing the assigned goto.  The assigned goto 
may not be used to jump from one program unit to another  program 
unit.  Jumps from assembly code to assigned labels will only work 
if extreme care is taken.

Excessive   subroutine  size  will  cause  the  compiler   memory 
allocation  to  grow  very  large.   Routines in  excess  of  one 
thousand  lines  may require more than a megabyte  of  memory  to 
compile.

Complex multiply,  add and subtract are done in line for  maximum 
speed.   Each  complex multiply generates at least four  floating 
point  multiplies,  two  floating point adds,  and two  temporary 
variables.   Routines containing in excess of one hundred complex 
operations may require more than a megabyte of memory.
The -w66 switch does nothing.  There are no Fortran-66 warnings.

Some  intrinsics (as allowed by the standard) cannot be passed as 
arguments to other procedures.  The compiler will not detect this 
error and an undefined reference will occur at link time.

Many  intrinsics  are  implemented as generics  even  though  the 
standard specifies that they are not generic.

No check is made for recursive statement functions.

Error messages follow the style of f77.   There is no  indication 
of the position within the line of an error.

I/O system error messages are quite verbose but often unhelpful.
