.pl 62
.UJ 0















                    DEFINICON SYSTEMS, INC.



                    DSI-32 COPROCESSOR BOARD
                   & MS/PC DOS SYSTEM SOFTWARE









        

     COMPILER REFERENCE MANUAL

     and

     C RUNTIME LIBRARY     




.pa
                            SECTION 1
                     Getting Started with C

1.1 System Requirements 

To  operate the Definicon Systems' C compiler,  your system  must 
satisfy the following requirements:

 o   A DSI-32000 coprocessor card,  with a minimum of 256K  bytes 
     of memory.

 o   An IBM compatible clone with a minimum of 96K bytes of  free 
     memory  exclusive  of  operating system  requirements  (256K 
     minimum of memory is recommended).

 o   Enough  disk space to contain the compiler,  the program  to      
     compile  and the output file (can be run from a floppy,  but 
     a hard disk is highly recommended).

1.2 Runtime Requirements 

To execute the compiled,  assembled and linked programs you  will 
need the following computer resources.

 o   A DSI-32000 coprocessor card,  with a minimum of 256K  bytes 
     of memory.

 o   An IBM compatible clone  with a minimum of 96K bytes of free 
     memory exclusive of operating system requirements.

Note   that   both  the  system  requirements  and  the   runtime 
requirements are for an absolutely minimum system.  A DSI-32 card 
with only 256K bytes of memory will severely restrict the size of 
functions  that can be compiled.  1M or 2M bytes of  memory  will 
enable very large functions to be compiled.  Also due to the size 
of the C compiler (around 200k bytes of executable code),  a hard 
disk drive,  or at the very least two floppy disk drives, will be 
particularly useful in retaining your sanity.

1.3 C Components 

The  Definicon  Systems' C compiler package consists  of  several 
components, some executable.  The executable components are the C 
compiler,  the  assembler  and  the linker.  The  C  system  also 
includes  six  special files that you can include in a C  program 
with  the  C '#include' directive.  These  special  files  enable 
programs  to be developed in a more portable fashion.   Table 1.1 
details the various components that are a part of the C system.
.pa
               Table 1.1 C System components
-----------------------------------------------------------------
|  Component  |  Disk File  |           Description             |
-----------------------------------------------------------------
|  compiler      CC.E32        the compiler module              |
|                CC.CMD        the CP/M command parser          |
|                CC.EXE        the MS/DOS command parser        |
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
|  #include      STDIO.H       various system dependent equates |
|  files         PORTAB.H      various portable definitions     |
|                ERRNO.H       definitions of various errors    |
|                MTH.H         definitions of various numbers   |
|                SETJMP.H      non-local program jump routine   |
|                CTYPE.H       various ASCII routines           |
|                                                               |
|  library       CLIB.O32      standard C functions (see runtime|
|                              section)                         |
|                                                               |
|  sample        HELLO.C       simple program                   |
|  programs      READ.ME       updated notes on C system and    |
|                              documentation                    |
-----------------------------------------------------------------
.pa
1.4 Minimum Working Disk 

To  compile  a program,  a diskette with the  following  programs 
forms a minimum configuration:

 o   CC.E32                 the compiler
 o   CC.EXE or CC.CMD       the command parser for MS/DOS or C/PM
 o   LOAD.EXE or LOAD.CMD   the DSI-32 program loader
 o   32IO.E32               the DSI-32 communications module
 o   any required .H files

To  assemble  and  link  the  program  ready  for  execution  the 
following programs form a minimum configuration:

 o   AS.E32                 the assembler  
 o   AS.CMD or AS.EXE       the command parser for the assembler
 o   AS32032.OPC            the opcode file for the assembler  
 o   LN.E32                 the linker
 o   LN.CMD or LN.EXE       the command parser for the linker
 o   CLIB.O32               C runtime library
 o   LOAD.EXE or LOAD.CMD   the DSI-32 program loader
 o   32IO.E32               the DSI-32 communications module
 o   LNC.D32                default file that controls the linker
                            and   specifies  the   libraries   to
                            search.
.pa
1.5 Compiling the demonstration program 

The  following  section describes how to  compile,  assemble  and 
link  the  demonstration  program  supplied  on  the  C  Language 
diskette.  The instructions assume that you are familiar with the 
operation of your operating system whether it is CCP/M or MS/DOS.

Prior  to  going  any further be sure to make a  backup  of  your 
master C diskette(s) and store the original(s) in a safe place.

1)   Create  a C working disk,  and an  assembler/linker  working      
     disk with the files as shown in Section 1.4.  If your system      
     has  a  hard disk then just move all the files  from  the  C 
     system diskette(s) to an area on the hard disk.
     
     With  your operating system disk in drive A and your C  work      
     disk  in  drive  B  which contains a  copy  of  the  HELLO.C 
     program,  you  are  ready  to compile and test  your  sample 
     program.  To look at the program you can type the program as 
     follows:

     B>TYPE HELLO.C

     main()
     {
          printf("Welcome to the world of 32 bit computers");
     }

2)  Now we are ready to compile the program.

     B>CC HELLO

     Note that it is not necessary to enter the .C extension when      
     invoking the compiler, the command parser performs that task      
     automatically. When the compiler has loaded and successfully 
     started  compiling your program it will display  its  signon 
     message.  At  the  end of the compilation the compiler  will 
     display any errors that have occurred and then return you to 
     the  operating  system prompt.  If no errors  have  occurred  
     then you are ready to assemble and link the program.

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

     The  linker  will link the program with the C libraries  and 
     produce a file that is executable on the DSI-32  board.  The 
     executable file will have a .E32 extension.

5)  Now we can load and run the program.

     B>LOAD HELLO

     Welcome to the world of 32 bit computers

     B>

     Note  that this example concentrated on linking a  single  C 
     program,  but  in  the  real world a typical C  program  may 
     consist of several separately compiled modules.  Below is an 
     example  of  how to link several modules to  form  a  single 
     executable  module.  In addition we will assume that we have 
     our own library, mylib.o32, that we want to search.

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
                    Operating the C compiler

2.1 General Overview 

The  Definicon  Systems' C compiler is a single  pass  optimizing 
compiler.  It  performs automatic register optimization and  will 
put  variables and pointers into registers depending  on  various 
parameters  such  as the lifetime of a variable and its  intended 
use (e.g.  if the address of a variable is taken at some stage in 
a  module,  then  the compiler will not place the variable  in  a 
register).  The C compiler supports all the standard features  of 
Berkeley 4.2 Unix portable C, including bit fields and enumerated 
data types.

- The sizes of various C variables are listed below:

     unsigned char            8 bits
              char            8 bits
     unsigned short int      16 bits
              short int      16 bits
     unsigned int            32 bits
              int            32 bits
     unsigned long           32 bits
              long           32 bits
              pointer        32 bits
              float          32 bits
              double         64 bits

- Program  size and data size are limited only by  the  available 
memory;  small model, medium model, large model, etc. limitations 
of  8088/8086/80286  IBM-PC clone systems do not exist  with  the 
National Semiconductor's 32000 series of 32 bit microprocessors. 

2.2 Compiler Command Line 

The  command line invokes the C compiler,  specifies  the  source 
file  to  compile  and  passes any special  instructions  to  the 
compiler.  The maximum compiler command line is 128 characters in 
length.  The form of the command line is shown below:

     B>CC    option_switches    source_filename

Note that the .C filetype extension does not have to be  supplied 
since the parser will automatically append the correct extension. 
If  an  extension is supplied in the command line then that  file 
will  be  compiled regardless of the .C convention.  Also  if  no 
extension is supplied on the command line and a file that has  no 
extension  but  has  the  right name  exists,  then  it  will  be 
compiled.
.pa
2.3 Stopping the Compiler 

The  C  compiler  may be stopped at any stage of  compilation  by 
entering a <CTRL>C at the console (hold the CTRL key down and hit 
the C key). The compiler will immediately terminate and exit back 
to the operating system.  Note that all programs that run on  the 
DSI-32  card  can be aborted in this way,  as well as  assembler, 
linker and user programs.  Upon termination,  all files that  are 
open  are automatically closed.  Note also that any output  files 
that  were open will be closed in whatever state they were in  at 
the time <CTRL>C was entered. The output files will not be erased 
but they will probably contain incomplete data.

2.4 Error Messages 

All  error  messages are displayed during the  compilation.   The 
line number in which the error was detected and the type of error 
are displayed on the console.

2.5 Compiler Command Line Options 

Command  line option switches enable the user to control  certain 
functions of the compiler.  An option switch always starts with a 
minus  sign  and  follows with  certain  alphanumeric  sequences. 
Spaces  must  not be left between the any of the characters  that 
make up an option switch but at least one space must separate  an 
option  switch from any other parameter.  Table 2.1  details  the 
available options and their function.

Command line options are entered as follows:

     B>CC -O1 -O2 -DDEBUG=OFF PROGRAM

Note 
      There is no case sensitivity for the command line  options. 
The command line parser CC.CMD or CC.EXE converts symbol names to 
lowercase,  this  means that the above command line will  produce 
the  directive  to the compiler "#define debug =  off",  NOT  the 
directive "#define DEBUG = OFF". 
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
|    -o2           allow the optimizer to assume that memory    |
|                  locations do not change except by explicit   |
|                  stores.  That is, the optimizer is guaranteed|
|                  that no memory locations are I/O device      |
|                  registers that can change asynchronously     |
|                                                               |
|    -uname        undefine the predefined preprocessor symbol  |
|                  "name".  Equivalent to "#undef name".        |
|                                                               |
|    -x55          make bit fields of int, short and char be    |
|                  signed.  The default is unsigned.            |
|                                                               |
|    -x56          allocate each enum type as the smallest size |
|                  which allows representation of all listed    |
|                  values (i.e. from the list "char", "short",  |
|                  "int", "unsigned char", "unsigned short" or  |
|                  "unsigned"). The default is to allocate "int"|
-----------------------------------------------------------------
.pa
                              SECTION 3
                         C Runtime Library


atoi,_atof,_atol,_strtol

The atoi,  atof,  atol, and strtol functions take an ASCII string 
representing  a  number and convert the string into  an  integer, 
double floating point or long integer number, respectively.

Declarations

     char      *string;    -- pointer to a null terminated string 
                              of  digits.    Leading  blanks  are 
                              ignored.    A leading sign (+,-) is 
                              allowed.
     char      **ptr;      -- if ptr is non zero,  it is treated
                              as  a  pointer to a return pointer,
                              where the location of the character
                              that  terminated the scan is to  be
                              stored. 
     int       iret;       -- atoi returns the string in integer
                              representation.
     long      lret;       -- atol returns the string  in  long 
                              representation.
     double    fret;       -- atof returns the string in double 
                              floating representation.

     int       atoi();
     double    atof();
     long      atol(), strtol();

Syntax

     iret   =  atoi(string);
     lret   =  atol(string);
     fret   =  atof(string);
     lret   =  strtol(string, ptr, base);

Notes
     Checking  for  overflow condition is not  handled  by  atoi, 
atof, atol or strtol.
.pa
brk,_sbrk

The brk and sbrk functions set the top of the program heap.   brk 
sets  the upper bound (above the current upper bound and referred 
to  as  the break) of the program heap to  a  specified  address.  
sbrk increases the heap by a specified number of bytes.

Declarations

     char      *addr;      -- the new break address
     int       incr;       -- amount to  increase  heap by  using
                              sbrk.
     int       bret;       -- 0 if brk is successful, -1 if not.
     char      *bheap      -- pointer   to   beginning   of  heap 
                              extension,    -1    if   sbrk    is 
                              unsuccessful.

     int       brk();
     char      *sbrk();

Syntax

     bret = brk(addr);
     bheap = sbrk(incr);
.pa
calloc,_malloc,_realloc,_free,_cfree

The calloc,  malloc,  realloc, free, and cfree functions are used 
to manipulate the allocation and deallocation of blocks  (spaces) 
of  memory  in the heap.   calloc (chunk allocation) returns  the 
starting  address  of a space allocated  for  an  array.   malloc 
(memory  allocation)  returns  the starting address for  a  space 
allocated on a word boundary. realloc (re-allocation) returns the 
starting address of a space that has been changed in size by  the 
specified amount.   free returns the specified space to the heap. 
cfree  actually  calls  the  free function and  is  included  for 
compatibilty only.

Declarations

     int       size        -- the number of bytes to allocate for 
                              the space.
     int       asize;      -- the  size  in bytes of  one  array 
                              element.
     int       narray;     -- the  number of array  elements  to 
                              allocate for the space.
     char      *sret       -- pointer to the allocated space.
     char      *addr       -- pointer  to the beginning  of  an 
                              existing space.

     char      *calloc(); *malloc(); *realloc();

Syntax

     sret   =  calloc(narray, asize);
     sret   =  malloc(size);
     sret   =  realloc(addr, size);
     free(addr);
     cfree(addr);

Notes

     When  realloc  is called,  the information in  the  original 
space is copied (if necessary) to the newly allocated space.   If 
there  is sufficient adjacent space,  realloc uses it as part  of 
the original space,  avoiding the costly copy operation.  Realloc 
frees the old space if it is not part of the new space.
.pa
clearn,_filln

The  clearn  and  filln functions enable areas of  memory  to  be 
cleared  or filled respectively.  The clearn function initialises
the specified area of memory with NULLs, while the filln function 
initialises the memory with the specified data.

Declarations

     char      *ptr;      -- pointer to array to be filled
     char      data;      -- the data to fill the array with
     int       len;       -- the length of data to fill

     void      clearn(), filln();

Syntax

     clearn(len, ptr);
     filln(len, ptr, data);
.pa
close

The close function closes an open (opened via open or creat) file 
or  device.   Note,  you  must specify a file descriptor for  the 
close function, not a stream address.  To close a stream file use 
the fclose function. 

Declarations

     int       fd;         -- file  descriptor of a  valid  open 
                              file.
     int       ret;        -- 0 if close succeeds, -1 if it fails

     int       close();

Syntax

     ret   =   close(fd);
.pa
creat,_creata,_creatb

The creat,  creata and creatb functions create new files.   creat 
and  creata  both create ASCII files.   creatb creates  a  binary 
file.  Binary  files  are generally faster to access  than  ASCII 
files  because of computational overheads in handling the CR  and 
LF characters.

Declarations

     char      *filename;  -- filename of file to create.
     int       mode;       -- access mode, not used by the DSI-32 
                              interface (for UNIX compatibility).
     int       fd;         -- file  descriptor returned by creat, 
                              creata and creatb.

     int       creat(); creata(); creatb();

Syntax

     fd    =   creat(filename, mode);
     fd    =   creata(filename, mode);
     fd    =   creatb(filename, mode);

Notes

     creat  and  creata are functionally  equivalent.  Additional 
information  on  the creat,  creata and creatb functions  may  be 
found in The_C_Programming_Language by Kernighan and Ritchie.
.pa
exit

The exit function returns control to the operating system.   exit 
flushes  all  buffers,   closes  all  open  files  and  sets  the 
completion code.

Declarations

     int        code;    -- completion  code returned to  the 
                            operating system.

Syntax

     exit(code);

Notes

     exit  does not return a value and therefore does not need to 
be declared.
.pa
fclose,_fflush

The fflush function writes out to disk any data in a stream file.  
fclose does the same, but closes the file.

Declarations

     FILE      *stream;  -- pointer to a stream file.
     int       fret;     -- 0 if flush succeeds, -1 if it fails.

     int       fclose(), fflush();

Syntax

     fret   =  fclose(stream);
     fret   =  fflush(stream);

Notes

     Additional  information  on  streams is available  in  The_C 
Programming_Language by Kernighan and Ritchie.
.pa
ferror,_clearerr,_feof,_fileno

The  feof,  ferror,  clearerr  and fileno are used to  manipulate 
stream  files.   They  are macros  defined  in  STDIO.H.   ferror 
returns an error status on a stream.   clearerr clears any errors 
on the specified stream.   feof determines whether an end of file 
has  been  reached  on a stream or not.   fileno returns  a  file 
descriptor associated with the specified stream.

Declarations

     FILE      *stream;  -- stream to operate on.
     int       fret;     -- Non zero if at end of file, 0 if not.
     int       eret;     -- Non zero if error, 0 if no error.
     int       fd;       -- file  descriptor  associated  with  a
                            stream.

     int       feof(), ferror(), fileno();

Syntax

     fret   =  feof(stream);
     eret   =  ferror(stream);
     fd     =  fileno(stream);
     clearerr(stream);

Notes

     As with all other macros they must not be defined to  return 
anything.   Additional information on streams is available in The 
C_Programming_Language by Kernighan and Ritchie.  Also see fopen.
.pa
fopen,_fopena,_fopenb,_freopen,_freopa,_freopb

The fopen,  fopena,  fopenb,  freopen,  freopa,  freopb functions 
create  an  association  between a file or device and  a  stream.  
fopen and fopena both associate streams with ASCII files.  fopenb 
associates a stream with a binary file.  freopen and freopa close 
the  specified stream and open the specified ASCII file with same 
stream  pointer.  freopb  performs the same function  as  freopen 
except  on a binary file.  Note that binary files  are  generally 
faster  to access than ASCII files due to computational overheads 
in  processing  the CR and LF characters.  Note that  the  access 
parameter  in the open request enables the user to request binary 
mode processing of the data file.

Declarations

     char      *filename;  -- pointer  to  a  null  terminated 
                              filename.
     char      *access;    -- a  character  with  one  of  the 
                              following values:
                                   r  - read access
                                   w  - write access
                                   a  - append access
                                   rb - read binary
                                   wb - write binary
                                   ab - append binary
     int       fd;         -- file descriptor associated with an 
                              existing open file.
     FILE      *streamold; -- stream to be closed.
c     FILE      *stream;    -- stream pointer returned form open.

     FILE      *fopen(), *fopena(), *fopenb();
     FILE      *freopen(), *freopa(), *freopb();

Syntax

     stream  = fopen(filename, access);
     stream  = fopena(filename, access);
     stream  = fopenb(filename, access);
     stream  = freopen(filename, access, streamold);
     stream  = freopa(filename, access, streamold);
     stream  = freopb(filename, access, streamold);
.pa
fread,_fwrite

The fread and fwrite functions are used to move byte data between 
a  stream  and  memory.   fread reads from a  stream  to  memory.  
fwrite writes from memory to a stream.

Declarations

     char      *buffer;  -- pointer to a memory buffer.
     int       size;     -- size  in bytes of an item  in  the 
                            stream.
     int       rnitems;  -- requested number of items read  or 
                            written in the stream.
     int       nitems;   -- actual  number of items  read  or 
                            written in the stream.
     FILE      *stream;  -- stream to read from and write to.

     int       fread(), fwrite();

Syntax

     nitems  = fread(buffer, size, rnitems, stream);
     nitems  = fwrite(buffer, size, rnitems, stream);

Notes

     The  requested  number of items read or written may  not  be 
equal to the actual number of items read or written.   A value of 
0 is returned if an error or EOF occurs.
.pa
frexp

The  frexp  function  takes a passed double and a pointer  to  an 
integer  and  extracts  the mantissa of  the  passed  double  and 
returns  that as the function argument.  The exponent portion  of 
the  double gets placed in the integer such that original  passed 
double  value  =  mantissa * (2 ** exponent).  Where  "**"  means 
"raised to the power of exponent".

Declarations

     int       *exp;     -- pointer to exponent return variable
     double    value;    -- the passed double
     double    dret;     -- the returned mantissa

     double    frexp();

Syntax

     dret  =   frexp(value, &exp);

Notes

     The exponent is in the range -1023 through to +1023. Zero is 
returned if the input value is zero.  The mantissa is  normalized 
to  be  less that one and greater than or equal to  a  half.  The 
format  follows  the proposed IEEE standard for  binary  floating 
point arithmetic, Task P754.
     This  routine is useful for the "normalization" of  floating 
point quantities.
.pa
fseek,_ftell,_rewind

The  fseek,  ftell and rewind functions allow positioning of  the 
read/write  pointer in a stream.   fseek sets the pointer to  the 
specified  location.   ftell returns the current position of  the 
pointer  in  the  stream.   rewind  places  the  pointer  at  the 
beginning of the stream.

Declarations

     long      offset;   -- number of bytes to move pointer from
                            current position (signed value).
     int       offtype;  -- type of offset:
                                   0 - offset from start of file
                                   1 - offset from current location
                                   2 - offset from end of file
     FILE      *stream;  -- stream to position
     int       fret;     -- 0 if positioning succeeds,  -1 if it 
                            fails.

     int       fseek(), rewind();
     long      ftell();

Syntax

     fret   =  fseek(stream, offset, offtype);
     offset =  ftell(stream);
     fret   =  rewind(stream);
.pa
getc,_getchar,_fgetc,_getw,_getl

getc  and  getchar are macros that  read  characters.   They  are 
defined in STDIO.H.  fgetc, getw and getl are functions that read 
characters,  words and long integers from a stream.  getc reads a 
character from a stream.  getw reads a 16-bit word from a stream.  
getl reads a 32-bit long integer from a stream.   getchar reads a 
character  from  stdin  (the  standard  input).   fgetc  reads  a 
character  from  a stream.   getc is a macro and is  functionally 
equivalent to fgetc, which is a function.

Declarations

     FILE      *stream;  -- stream to read from.
     int       cret;     -- character returned from getc, fgetc 
                            and getchar.
     int       wret;     -- word returned from getw.
     long      lret;     -- 32-bit long integer  returned  from 
                            getl.

     int       fgetc(), getw();
     long      getl();

Syntax

     cret   =  getc(stream);
     cret   =  fgetc(stream);
     cret   =  getchar();
     wret   =  getw(stream);
     lret   =  getl(stream);

Notes

     Errors  returned  by the stream read functions may be  valid 
values.  Use feof and ferror to detect errors when using streams.  
Also  as in all other case of macros,  do not declare the  macros 
getc() and getchar().
.pa
getpid

The  getpid  function  is included for compatibility  with  UNIX.  
Under  UNIX,  getpid  returns  the  process  id.  In  the  DSI-32 
implementation a random number is returned.

Declarations

     int       pid;           -- process id. See above.

     int       getpid();

Syntax

     pid   =   getpid();
.pa
gets,_fgets

The  gets and fgets functions read strings from a  stream.   gets 
reads  a  string from stdin (the standard  input),  removing  the 
newline character at the end of the string.  fgets reads a string 
from  a stream and leaves the newline character in  the  returned 
string.

Declarations

     char      *cstring; -- pointer to a null terminated string.
     int       maxc;     -- maximum character count for fgets.
     FILE      *stream;  -- stream to read from.
     char      *rstring  -- return string (= cstring) if gets or fgets 
                            succeeds, null if fails.

     char      *gets(), *fgets();

Syntax

     rstring = gets(cstring);
     rstring = fgets(cstring, maxc, stream);

Notes

     The  string cstring holds the input string in both gets  and 
fgets.   Adequate space for an input line must be provided for by 
the  string  cstring.   Maxc  specifies  the  maximum  number  of 
characters  to  be  returned  in  fgets,  including  the  newline 
character.
.pa
index,_rindex,_strchr,_strrchr

The  index,  rindex,  strchr  and  strrchr functions  return  the 
location of a specified character in a string.   index and rindex 
search for the first and last occurrence of a substring within  a 
string respectively.  strchr and strrchr search for the first and 
last occurrence of a character within a string respectively.

Declarations

     char      c;          -- character to locate in string.
     char      *sbstring;  -- pointer to substring to search for.
     char      *cstring;   -- pointer to null terminated  string
                              to search.
     int       rchar;      -- offset corresponding to position of
                              substring   or   character   within
                              string or -1 if no match.
     char      *ptr;       -- pointer to the matched character
                              or  NULL  if the character  is  not 
                              found.
     char      *strchr(), strrchr();
     int       index(), rindex();

Syntax

     rchar  =  index(cstring, sbstring);
     ptr    =  strchr(cstring, c);
     rchar  =  rindex(cstring, sbstring);
     ptr    =  strrchr(cstring, c);
.pa
isalpha,_isupper,_islower,_isdigit,_isalnum,_isspace,_ispunct, 
isprint,_iscntrl,_isascii

These  functions return true or false if the condition  specified 
by their name is or is not met, respectively.

Declarations

     #include  <ctype.h>
     char      c;          -- character to be checked.
     int       cret;       -- 0  if condition is met,  -1 if  it
                              fails.

Syntax

     cret   =  isalpha(c); -- is c an alpha character (a-z, A-Z)
     cret   =  isupper(c); -- is c uppercase (A-Z)
     cret   =  islower(c); -- is c lowercase (a-z)
     cret   =  isdigit(c); -- is c a digit (0-9)
     cret   =  isalnum(c); -- is c alphanumeric (a-z, A-Z, 0-9)
     cret   =  isspace(c); -- is c a white space character  (SP,
                               TAB, CR, LF, VT, FF)
     cret   =  ispunct(c); -- is  c  a  punctuation  character
                              (! " # $ % & ' ( ) * + , - . / : ;
                               < = > ? @ [ \ ] ^ _ ` { | } ~)
     cret   =  isprint(c); -- is c a printable character
     cret   =  iscntrl(c); -- is c a control character
                              all characters less 0x20, and DEL.
     cret   =  isascii(c); -- is c an ASCII character (between 0
                              and 128)

Notes

     These are macros defined in CTYPE.H and therefore,  must not 
be declared in the user source.
.pa
lseek,_tell

lseek  and  tell are similar to fseek and ftell except that  they 
function on file descriptors instead of streams.

Declarations

     long      offset;    -- number of bytes to move pointer from
                             current position (signed value).
     int       offtype;   -- type of offset:
                                   0 - offset from start of file
                                   1 - offset from current locn
                                   2 - offset from end of file
     int       fd;        -- file to position
     long      fret;      -- 0 if positioning succeeds, -1 if it
                             fails.

     long      lseek(), tell();

Syntax

     fret   =  lseek(fd, offset, offtype);
     offset =  tell(fd);
.pa
mktemp

The  mktemp  function makes a temporary filename.   Use  this  in 
conjunction  with  creat,  creata  or creatb to create  a  unique 
temporary file.   The template is a character string containing a 
user  defined prefix followed by six x characters (upper case  or 
lower  case).   The  x's  are replaced by  a  unique  number  and 
returned in the original template character string.

Declarations

     char      *fstring;   -- pointer  to  a  null   terminated
                              filename template.

     char      *mktemp();

Syntax

     fstring = mktemp(fstring);
.pa
open,_opena,_openb

The open and opena functions open existing ASCII files and return 
a  file  descriptor for the opened file.   openb  opens  existing 
binary files and return a file descriptor for the opened file.
Note  that  the setting bit 2 of the mode parameter  informs  the 
open  and opena functions that binary mode file processing is  to 
occur thus making them equivalent to the openb function.

Declarations

     char      *filename;  -- pointer  to a null  terminated
                              filename.
     int       mode;       -- access mode which is one  of  the
                              following:
                                   0 - read access
                                   1 - write access
                                   2 - read/write access
                                   4 - read binary
                                   5 - write binary
                                   6 - read/write binary
     int       fd;         -- file descriptor returned by open,
                              opena or openb -1 if it fails.

     int       open(), opena(), openb();

Syntax

     fd    =   open(filename, mode);
     fd    =   opena(filename, mode);
     fd    =   openb(filename, mode);
.pa
perror

The  perror function writes an error message corresponding to the 
last  system  error  to  stderr  (the  standard  error   output).  
External  variable  errno contains the value of the  last  system 
error  code which is used by perror to determine the last  system 
error message.

Declarations

     include   <errno.h>
     char      *pstring; -- pointer to a null terminated prefix.

Syntax

     perror(pstring);

Notes

     pstring  is  a string which is written prior  to  the  error 
message.   See  ERRNO.H  for a description of the  defined  error 
codes and messages.
.pa
printf,_fprintf,_sprintf,_eprintf

The  printf,  fprintf,  sprintf  and eprintf interpret  a  format 
string and arguments to output to the standard output,  a stream, 
memory, and standard error respectively.  The number of arguments 
must be equal to or greater to the number of format operators  in 
the format string, otherwise undefined results may occur.  Format 
operators  are  represented as a % (percent)  sign,  followed  by 
optional  formatting  symbols,  followed  by a  single  character 
operator designator.  A space or equivalent white space character 
separates character operator designators.

The  following reserved formatting symbols and digit strings  may 
be used between the percent sign and a conversion character. Note 
that  the conversion character must appear as the last  character 
in the conversion list.

Formatting symbols consist of the following:

     -          - (minus sign) left justify conversion in field
     w.p        - (w,   p  =  decimal  digit)  field  width   and
                  precision. precision is optional
     L or l     - specifies a 32-bit value
     
The conversion character must be one of the following characters:

     d    - convert binary argument to decimal

     o    - convert binary argument to octal

     x    - convert binary argument to lowercase hexadecimal

     X    - convert binary argument to uppercase hexadecimal

     u    - convert binary argument to unsigned decimal

     s    - use argument as  a pointer to a NULL terminated ASCII
            string

     c    - argument is a single ASCII character

     %    - print a % character

     f    - convert a  float or double number to  ASCII  decimal.
            The precision string controls the number of decimals.

     e    - same as f, except converts to scientific notation

     g    - convert float or double numbers using the d,  f, or e
            conversion  character  depending on which yields  the
            full precision with shortest output string.
Declarations
     char      *format;    -- pointer to a null terminated format
                              descriptor string.
     char      *dstring;   -- pointer  to  a  null   terminated
                              destination string for sprintf.
     FILE      *stream;    -- destination stream for fprintf.
     int       nret;       -- number of characters output.
     int       arg1, arg2,..., argN;
                           -- N  arguments of type  corresponding
                              to  respective  character  operator
                              designator.

     int       printf(), fprintf(), sprintf(), eprintf();

Syntax
     nret   =  printf(format, arg1, arg2,..., argN);
     nret   =  fprintf(stream, format, arg1, arg2,..., argN);
     nret   =  sprintf(dstring, format, arg1, arg2,..., argN);
     nret   =  eprintf(format, arg1, arg2,..., argN);

Notes

     A complete discussion of printf, fprintf and sprintf appears 
in The_C_Programming_language by Kernighan and Ritchie.
.pa
putc,_putchar,_fputc,_putw,_putl

putc  and  putchar  are  macros defined  in  STDIO.H  that  write 
characters to a stream.  fputc,  putw and putl are functions that 
write  characters,  words and long integers to  a  stream.   putc 
writes  a character to a stream.   putw writes a 16-bit word to a 
stream.   putl writes a 32-bit long integer to a stream.  putchar 
writes a character to stdout (the standard output).  fputc writes 
a  character to a stream.   putc is a macro and  is  functionally 
identical to fputc, which is a function.

Declarations

     char      c;          -- character to write to stream.
     int       w;          -- word to write to stream.
     long      l;          -- 32-bit  long  integer to  write to
                              stream.
     FILE      *stream;    -- stream to write to.
     int       cret;       -- character written by putc, fputc or
                              putchar, -1 if an error occurs.
     int       wret;       -- word  written by putw,  -1  if  an
                              error occurs.
     long      lret;       -- 32-bit  long integer  written  by
                              putl, -1 if an error occurs.

     int       putchar(), fputc(), putw();
     long      putl();

Syntax

     cret   =  putc(c, stream);
     cret   =  fputc(c, stream);
     cret   =  putchar(c);
     wret   =  putw(w, stream);
     lret   =  putl(l, stream);

Notes

     Errors  returned by the stream write functions may be  valid 
values.  Use feof and ferror to detect errors when using streams.
.pa
puts,_fputs

The  puts  and fputs functions write strings out to  stdout  (the 
standard  output)  and a specified output  stream,  respectively.  
puts appends a newline character at the end of the output  string 
whereas fputs does not.

Declarations

     char     *cstring;  -- pointer to null terminated  character
                            string.
     FILE      *stream;
     int       sret;     -- last  character output,  or -1  if
                            output fails.

     int       puts(), fputs();

Syntax

     sret   =  puts(cstring);
     sret   =  fputs(stream, cstring);
.pa
qsort

The  qsort function sorts a table of data,  using the quick  sort 
algorithm.  Base  is  a  pointer to the base of the  data  to  be 
sorted,  normally the first element of an array of pointers.

Declarations

     void qsort(base, number, size, compare);

     char *base;
     unsigned int number, size;
     int (*compare)();

Syntax

     qsort(base, number, size, compare);

Notes

The  order  in the output of elements which compare as  equal  is 
undefined.

The  compare  function must return an integer that is less  than, 
equal  to,  or  greater  than zero,  depending  on  the  agument 
comparison of a < b, a == b, or a > b.

A sample sort for a list of strings would be:

     char *str[100];
     int compare();

     qsort(str, 100, sizeof(char *), compare)

and the compare would be:

compare(a,b)
char **a, **b;
{
     return(strcmp(*a, *b));
}


.pa

rand,_srand

The rand and srand functions return pseudo-random numbers.  srand 
initializes  the random number generator with the  user  supplied 
seed value and returns a random number.   rand returns subsequent 
random numbers.

Declarations

     int       seed;      -- initial seed value chosen by user.
     int       rret;      -- random value returned by generator.

     int       rand(), srand();

Syntax

     rret   =  srand(seed);
     rret   =  rand(seed);

Note

     The rand function returns and pseudo random number between 0 
and 2147483649, ie (2 ** 31 - 1).
.pa
read,_write

The  read  function reads input from an open file specified by  a 
file  descriptor.   The  write  function  writes  output  to  the 
specified file descriptor.

Declarations

     int       fd;        -- the  file  descriptor  defining  the
                             file.
     char      *mbuffer;  -- pointer to memory buffer. 
     unsigned   n;        -- number of bytes to  read.  Maximum 
                             number is 32640 characters.
     int       ret;       -- number of bytes transfered, -1 if an
                             error occurs.

     int       read();
     int       write();

Syntax

     ret   =  read(fd, mbuffer, n);
     ret   =  write(fd, mbuffer, n);
.pa
rename

The rename function takes two strings,  the first is the old name 
of the file and the second is the new name that the file is to be 
renamed  to.  rename returns an error if it fails to perform  its 
function.

Declarations

     char      *oldname;  -- the old name of the file
     char      *newname;  -- the new name of the file
     int       ret;       -- return status. 1 if error else 0.

     int       rename();

Syntax

     ret   =  rename(oldname, newname);
.pa
scanf,_fscanf,_sscanf

The scanf, fscanf and sscanf functions read input from stdin (the 
standard  input),   a  stream,  and  a  null  terminated  string, 
respectively.    The  input  is  interpreted  according  to   the 
specified  format descriptor string and placed into the remaining 
arguments to the function.

The  arguments may be of any type corresponding to the conversion 
specifications  in the format descriptor string.   The number  of 
conversion specifications in the format descriptor string must be 
less  than  or equal to the number of arguments that  follow  the 
descriptor.

A conversion specification consists of a percent  sign,  followed
by  an  optional  * (asterisk),  followed by  a  format  operator
designator.   The * specifies that the data is not to be assigned
to the argument.   Format specification designators are separated
by a space or equivalent white space character.

Format operator designators consist of the following:

     %  -  a  single  percent sign matches  the  the  input,  no
           conversion is performed.

     h  -  convert  short ASCII integer string  to  binary,  the
           corresponding  argument must be a pointer to  a  short
           integer.

     d  -  convert  decimal ASCII integer string to  binary  and
           store the result where the argument points.

     o  -  convert octal ASCII integer string to binary and store
           the result where the corresponding argument points.          

     x  -  convert hexadecimal ASCII integer string to binary and
           store  the  result  where the  corresponding  argument
           points.

     s  -  a character string, ending with a space, is input. The
           argument  pointer is assumed to point to  a  character
           array  which is large enough to contain the string and 
           a trailing NULL, which are concatenated.

     c  -  stores a single ASCII character including spaces where
           the argument points.

     e  -  convert scientific notation ASCII string to binary

     f  -  convert  standard  numeric notation ASCII  string  to
           binary
Declarations

     char      *format;   -- pointer to a null terminated  format
                             descriptor string.
     char      *sstring;  -- pointer to a null terminated source
                             string for sscanf.
     FILE      *stream;   -- source stream for fscanf.
     int       nret;      -- number of conversion specifications
                             converted.
     int       *arg1,*arg2,...,*argN;
                          -- pointers  to  arguments of any  type
                             corresponding  to format descriptors
                             string.

     int       scanf(), fscanf(), sscanf();

Syntax

     nret   =  scanf(format, arg1, arg2,..., argN);
     nret   =  fscanf(stream, format, arg1, arg2,..., argN);
     nret   =  sscanf(sstring, format, arg1, arg2,..., argN);

Notes

     Additional information on scanf,  fscanf,  and sscanf can be 
found in The_C_Programming_Language by Kernighan and Ritchie.
.pa
setbuf

The setbuf function may be invoked after a stream has been opened 
but before and read or write access has been performed. It causes 
the  array  pointed to by 'buffer' to be used as  the  read/write 
buffer instead of an automatically allocated buffer.  If 'buffer' 
is a NULL pointer then read/write will be totally unbuffered.

A  constant BUFSIZ,  defined in the <stdio.h> file  controls  the 
size of the array.

Declarations

     char      buffer[BUFSIZ]; -- The buffer to be used for all
                                 read/write to the stream file
     FILE      stream;         -- The stream file

     void      setbuf();

Syntax

     setbuf(stream, buffer);
.pa
setjmp,_longjmp

The  setjmp and longjmp functions are used in programs which  use 
nonlocal  goto  statements.   setjmp  stores the  existing  local 
program  environment prior to a nonlocal goto  statement.  setjmp 
also stores the location at which execution is to continue  after 
a longjmp call.   longjmp simulates a return from a nonlocal goto 
statement to the location saved when setjmp was called.   longjmp 
returns a status value to setjmp upon return.

Declarations

     #include  <setjmp.h>
     jmp_buf   env;      -- environment saved by setjump.
     int       sret;     -- returned  value  after  a  return  to
                            setjmp  environment.  Initially, this
                            value  is  0  until  longjmp  changes
                            it to the specified value.
     int       ljret;    -- the  return  value  longjmp sets  the
                            setjump to.

     int       setjmp();

Syntax

     The  setjmp,  longjmp  syntax  involves more than  a  simple 
     call.   First,  setjmp  is  called to save the  environment.  
     Then  the nonlocal goto statement is executed.   Finally the 
     longjmp  call is made to  return execution to  where  setjmp 
     was initially called.
     
     .
     .
     sret = setjmp(env);
     .
     .
     goto xxxx;
     .
     .
     xxxx:
     .
     .
     longjmp(env, ljret);
     .
     .

Notes

     env  is invalid if a return from a procedure or function  is 
executed.   jmp_buf is defined in SETJMP.H.   longjmp itself does 
not return a value and therefore is not declared.
strcat,_strncat,_strcmp,_strncmp,_strcpy,_strncpy,_strlen
strsave,_bufcpy
 
The strcat,  strncat,  strcmp,  strncmp, strcpy, strncpy, strlen, 
strsave  and  bufcpy functions are used  to  manipulate  strings.  
Functions  prefixed  with "strn" specify a number  of  characters 
upon which to perform the string operation.   Functions without a 
"strn"  prefix perform the string operations up to the first null 
character  in the string argument(s).   strcat  concatenates  two 
strings.   strcmp compares two strings.  strcpy copies one string 
into another.  strlen returns the length of a string (there is no 
strnlen  function).  The strsave function calls the system memory 
allocator  function for memory equal to the length of the  string 
and  then copies the string to that memory.  The bufcpy  function 
copies  a  source string to a destination string  for  the  exact 
length  specified and does not append a NULL,  it is  useful  for 
copying binary data.

Declarations

     char      *string1;   -- destination string pointer.
     char      *string2;   -- source string pointer.
     int       n;          -- number of characters to operate on
                              in "strn" prefixed functions.
     char      *catret;    -- points to the concatenated results
                              of strcat or strncat.
     int       cmpret;     -- results of comparison:
                              < 0 if string1 < string2
                              = 0 if string1 = string2
                              > 0 if string1 > string2
     char      *cpyret;    -- pointer  to  the copied results  of
                              strcpy or strncpy.
     int       lenret;     -- number  of characters determined by
                              strlen.

     char      *strcat(), strncat(), strcpy(), strncpy(), strsave;
     int       strcmp(), strncmp(), strlen();
     VOID      bufcpy();

Syntax

     catret  = strcat(string1, string2);
     catret  = strncat(string1, string2, n);
     cmpret  = strcmp(string1, string2);
     cmpret  = strncmp(string1, string2, n);
     cpyret  = strcpy(string1, string2);
     cpyret  = strncpy(string1, string2, n);
     lenret  = strlen(string1);
     cpuret  = strsave(string1);
               bufcpy(string1, string2, n);

Notes

     catret = strcat(string1,  string1) has undefined results and 
should  not be used.  Generally the program will overwrite all of 
memory causing the DSI-32 to crash.
  
     strncpy  truncates  the result string if the value of  n  is 
less  than  the length of the source  string.  Also,  the  result 
string  will not be null terminated,  this may cause other string 
functions to fail.

     strncpy will null pad the result string if n is greater than 
the length of the source string.
.pa
swab

The  swab function copies one memory space to another,  reversing 
high order and low order bytes.

Declarations

     char      *source;    -- pointer to bytes to be copied.
     char      *dest;      -- pointer to space to receive bytes.
     int       n;          -- number of bytes to copy (must be an
                              even number of bytes).
     int       sret;       -- 0 in all cases.

     int       swab();

Syntax

     sret   =  swab(source, dest, n);

Note

     The  swab function does not take into account the  direction 
of the memory move.  Also the source and destination buffers must 
be different and non overlapping.
.pa
timer,_times

times   fills  a  structure  with  the  current   time-accounting 
information.  timer  either returns the total elapsed time  (both 
user and system),  or if the supplied pointer is non-zero,  fills 
the  address with the information.  The times are returned,  with 
both  functions,  in  milliseconds.  Both functions  require  the 
optional  NS32202 Interrupt Control Unit (ICU) to be  mounted  in 
the DSI-32.

Declarations

     struct time_acct {
          int user_time;
          int system_time;
          int c_user_time;
          int c_system_time;
     } buffer;

     void times(buffer);

     int elap;
     int timer(a);

Syntax

     times(&buffer);

     elap = timer(0);
     timer(&elap);

Notes

User  time  is  the number of  milliseconds  of  non-I/O  related 
processing.  This  is  the actual time spent executing  the  user 
program.  System  time is the number of milliseconds wasted while 
waiting for the host IBM to complete I/O requests.

Note  that  since there are no child processes (as the  operating 
system  is  not multi-tasking) in  the  DSI-32,  c_user_time  and 
c_system_time  are  always  zero.  Timer can be passed  either  a 
pointer to an int,  or zero. If zero, it then returns the current 
elapsed  time.  If  it is passed an  address,  then  the  current 
elapsed time is placed in the supplied address.
.pa
toascii,_tolower,_toupper

The  tolower  and  toupper functions manipulate  ASCII  character 
strings by converting a single character to the respective  case.  
toascii is included for compatibility with other UNIX systems.

Declarations

     #include  <ctype.h>

     char      c;         -- the character to convert.
     int       toret;     -- the converted character returned.

Syntax

     toret  =  toupper(c);
     toret  =  tolower(c);

Notes

     The  tolower,  toupper and toascii functions are defined  as 
macros in CTYPE.H and therefore must not be declared.
.pa
ungetc

The  ungetc  function  pushes  a character back  onto  the  input 
stream.

Declarations

     char       c;        -- the  character to replace  in  the
                             stream.
     FILE       *stream;  -- the stream to receive the  replaced
                             character.
     int       uret;      -- the character c if ungetc succeeds,
                             -1 if it fails.

     int       ungetc();

Syntax

     uret   =  ungetc(c, stream);
.pa
unlink

The  unlink function erases the specified  file.   The  specified 
file  can  be  defaulted to the current directory,  or can  be  a 
completely specified filename with drive specifier and path.

Declarations

     char      *filename;     -- pointer   to   null   terminated
                                 filename.
     int       uret;          -- 0 if unlink erases the file, -1
                                 if it fails.

     int       unlink();

Syntax

     uret   =  unlink(filename);
.pa
                            SECTION 4
                       C function summary

     atoi, atof, atol, strtol
     brk, sbrk
     calloc, malloc, realloc, free, cfree
     clearn, filln
     close
     creat, creata, creatb
     exit
     fclose, fflush
     ferror, clearerr, feof, fileno
     fopen, fopena, fopenb, freopen, freopa, freopb
     fread, fwrite
     frexp
     fseek, ftell, rewind
     getc, getchar, fgetc, getw, getl
     getpid
     gets, fgets
     index, rindex, strchr, strrchr
     isalpha, isupper, islower, isdigit, isalnum, isspace,
          ispunct, isprint, iscntrl, isascii
     lseek, tell
     mktemp
     open, opena, openb
     perror
     printf, fprintf, sprintf, eprintf
     putc, putchar, fputc, putw, putl
     puts, fputs
     qsort
     scanf, fscanf, sscanf
     rand, srand
     read, write
     rename
     setbuf
     setjmp, longjmp
     strcat, strncat, strcmp, strncmp, strcpy, strncpy, strlen,
          strsave, bufcpy
     swab
     timer, times
     toascii, tolower, toupper
     ungetc
     unlink
.pa
                            SECTION 5
                         Math functions


abs,_fabs,_floor,_fmod

The  abs  function returns the absolute value of a integer  while 
the fabs function returns the absolute value of a floating  point 
number.  The  abs and fabs functions are actually implemented  as 
macros in STDIO.H, and therefore they must not be declared in the 
user source. The floor function returns the largest integer (as a 
double precision number) not greater than the argument.  The fmod 
function  returns the floating point remainder of the division of 
or the two arguments passed to it.

Declarations

     int       num;       -- the integer argument
     int       ret;       -- the result
     double    x,y;       -- double precision arguments
     double    fnum;      -- the floating point argument
     double    fret       -- the floating point result

     double    floor(), fmod();

Syntax

     ret   =   abs(num);
     fret  =   fabs(fnum);
     fret  =   floor(x);
     fret  =   fmod(x, y);    /*  fret = x - floor(x/y) * y  */

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

     double    argument;   -- the double precision argument
     double    num;        -- the  returned  ceiling  of   the
                              argument

     double    ceil();

Syntax

     num   =   ceil(argument);
.pa
cos,_sin,_tan,_acos,_asin,_atan,_atan2

The  trigonometric  functions  which  take an  angular  value  in 
radians and return a value are cos,  sin, tan, and their inverses 
are acos, asin and atan respectively. 

Declarations

     double    num;        -- a  double precision floating  point
                              number.
     double    x,y;        -- two double precision values
     double    angle;      -- a double  precision  floating point
                              number that expresses the  angle in
                              radians.
     double    sin():      -- trigonometric  sine   of  an  angle
                              expressed in radians.
     double    cos();      -- trigonometric  cosine  of an  angle
                              expressed in radians.
     double    tan();      -- trigonometric  tangent of an  angle
                              expressed in radians.
     double    asin();     -- returns the  arcsine  in  the range
                              -PI/2 to PI/2
     double    acos();     -- returns the arcscosine in the range
                              0 to PI
     double    atan();     -- returns the arctangent in the range
                              -PI/2 to PI/2
     double     atan2();   -- returns  the  arctangent of y/x  in
                              the  range  -PI to  PI,  using  the
                              signs   or   both   arguments    to
                              determine   the  quadrant  of   the
                              return value

Syntax

     fret   =  cos(angle);
     fret   =  sin(angle);
     fret   =  tan(angle);
     angle  =  acos(num);
     angle  =  asin(num);  
     angle  =  atan(num);
     angle  =  atan2(y, x);

Note

     All the above functions test the range of the input argument 
and set the error EDOM (see errno.h file) in the errno  variable, 
if the number is illegal for the function. 
.pa
cosh,_sinh,_tanh

The cosh,  sinh,  and tanh return,  respectively,  the hyperbolic 
cosine, sine and tangent of their argument.

Declarations

     double    angle;      -- the  angle as a double  precision
                              floating point number
     double    num;        -- the double precision result

     double    cosh(), sinh(), tanh();

Syntax

     num  =  cosh(angle);
     num  =  sinh(angle);
     num  =  tanh(angle);
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

     double    x;          -- the double precision argument
     double    num;        -- the double precision result

     double    erf(), erfc();

Syntax

     num   =   erf(x);
     num   =   erfc(x);
.pa
exp,_log,_log10,_pow,_sqrt

The  exp,  log,  log10,  pow and sqrt functions take the  natural 
exponential,  the natural loqarithm,  the base 10 logarithm,  the 
power and the square root of the argument(s) respectively.

Declarations

     double    res;        -- the double precision result
     double    x,y;        -- two double precision arguments
     double    exp();      -- the  value of the constant e raised
                              to   the   specified  argument   (e
                              = 2.7182812845905)
     double    log();      -- the  logarithm  base e of a  number
                              (natural logarithm).
     double    log10();    -- the logarithm base 10 of a number.
     double    pow();      -- take the value of x to the power y
     double    sqrt();     -- the square root of a number.

Syntax

     res   =   exp(x);
     res   =   log(x);
     res   =   log10(x);
     res   =   pow(x, y);     /*   ie res = x ** y   */
     res   =   sqrt(x);
.pa
gamma

This function returns the natural logarithm of the absolute value 
of  the  gamma function.  The sign is returned  in  the  external 
integer  signgam.  The argument supplied to the function may  not  
be a non-positive integer.

An example of how a program might calculate the gamma function is 
shown below:

     if ((y = gamma(x)) > LN_MAXDOUBLE)
          error();
     y = signgam * exp(y);

This function returns ln(| (x)|), where


          (x) =    e-ttx-1dt
                 0

Declarations

     double    x;           -- the double precision argument
     double    ret;         -- the double precision result
     extern    int signgam; -- the external variable that
                               contains sign

     double    gamma();

Syntax

     ret  =    gamma(x);
.pa
j0,_j1,_jn,_y0,_y1,_yn

The j0 and j1 functions return Bessel functions of the first kind 
or orders 0 and 1 respectively. Jn returns the Bessel function of 
x of the first kind of order n.
Y0  and  y1 return Bessel functions of x of the  second  kind  of 
orders 0 and 1 respectively.  Yn returns the Bessel function of x 
of the second kind of order n. The value of x must be positive.

Declarations

     double    x;          -- the double precision argument
     int       n;          -- the order of the Bessel function
     double    ret;        -- the double precision result

     double    j0(), j1(), jn(), y0(), y1(), yn();

Syntax

     ret   =   j0(x);
     ret   =   j1(x);
     ret   =   jn(n, x);
     ret   =   y0(x);
     ret   =   y1(x);
     ret   =   yn(n, x);

Note

     Non-positive  arguments  cause  y0,  y1 and yn  to  set  the 
variable errno to EDOM.
     Arguments  that are too large cause y0,  y1 and yn to return 
zero and to set the variable errno to ERANGE.
.pa
                            SECTION 6
                         C math summary

     abs(x)         ;Returns absolute value of x (int)
     fabs(x)        ;Returns absolute value of x (double)
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

     gamma(x)       ;Returns the log of the absolute value of the
                    ;gamma  function,  the  sign is  returned  in
                    ;extern integer signgam

     j0(x)          ;0th bessel function at x
     j1(x)          ;1st bessel function at x
     jn(n,x)        ;nth bessel function at x
     y0(x)          ;0th bessel function of second kind at x
     y1(x)          ;1st bessel function of second kind at x
     yn(n,x)        ;nth bessel function of second kind at x

