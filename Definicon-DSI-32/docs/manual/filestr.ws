                    Object File Structure

8.8.1 Introductions

     Object  files  are produced by assembler,  AS,  and  may  be 
manipulated  by the librarian,  LIB.  Object files are formed  of 
five distinct blocks:

     o    Directory
     o    Code
     o    Static Data
     o    Import Table
     o    Export Table

Each  block starts at a 512 byte boundary within the object  file 
and may occupy zero or more pages.  The first block is always the 
directory  block  and  the  rest of the  blocks  are  arbitrarily 
allocated by either that assembler or librarian.

8.8.1.1 Directory

     Each  directory  block contains information for  up  to  ten 
modules.  If  more  than ten modules are contained in the  ojbect 
file then further contiguous blocks are allocated.  The directory 
block is formed of three distinct sections:  a header section, an 
array of module information and a trailer (padding to 512 bytes).

   NAME   BITS           CONTENTS

   Header

     NEXTDIR     16    Pointer to next directory  block,  -1  if 
                       last block
     MODCOUNT    16    Number of modules in this block

Array of module information (maximum of 10 entries per block)

     MODNM       64    Module name, left justified, space padded
     MODTYPE      7    Language (c = C, p = pascal, f = fortran,
                       null (0hex) = assembler, b = basic) 
     STATUS       1    Module status (0=main, 1=not main)
     SUBTYPE      8    Language subtype (2nd letter after the '.'
                       in the module name, eg 'testprog.ca'  the 
                       letter 'a' will be the language subtype.
     STARTADDR   32    Execution start address (offset from code
                       start)
     CODELEN     32    Code size in bytes
     SBSIZE      32    Static Base data size in bytes
     SBBLK       16    Static base block number
     SYMLEN      16    Symbol table length (currently reserved)
.pa
     SBADDR      32    Static Base address (-1 if no SB data)
     CODEADDR    32    Code address (-1 if no code)
     EXPOBLK     16    Start block of export symbols
     IMPOBLK     16    Start block of import symbols
     CODEBLK     16    Start block of code
     SYMBLK      16    Start block of symbol table (reserved)

     EXPOLEN     16    Number of export symbols
     IMPOLEN     16    Number of import symbols

   NAME   BITS           CONTENTS
     
     FILLER     304    Reserved
     VERSION     16    Version number
     CHECK       56    Character string that contains 'CODEFIL'
     NULL         8    NULL character (0 hex)

8.8.1.2 Code

     The  code  block  contains  the actual program  code  to  be 
executed. The code blocks are contiguous.

8.8.1.3 Static Base data

     The static base data contains the data that is referenced by 
the  SB  register of the 32000 series  family.  The  static  base 
blocks are contiguous.

8.8.1.4 Import Table

     The  import  table blocks contain 20 of the following  items 
per 512 byte block.

   NAME   BITS           CONTENTS

     INAME      152    The import symbol name
     ISIZE       32    Size of data (used when symbol is common)  
     ITYPE        1    Symbol type (0 = data, 1 = entry point)
     ICOMM        1    Common flag (0 = not common, 1 = common)
     ISTAT        1    Static flag (0 = global symbol,
                       1 = local symbol)
     UNUSED      13    Reserved
.pa
8.8.1.5 Export Table

     The  export  table blocks contain 20 of the following  items 
per 512 byte block.

   NAME   BITS           CONTENTS

     ENAME      152    The export symbol name
     ESIZE       32    Byte offset within segment
     ESTAT        1    Static flag (0 = global symbol,
                       1 = local symbol)
     UNUSED       2    Reserved
     ETYPE        1    Type (0 = SB data, 1 = ABS data, 2 = PC,
                       3 = PC data 4 = PC) 
                       entry
     UNUSED      10    Reserved
.pa
                 Executable File Structure

8.8.2 Introduction

     Executable files are produced by linker,  An executable file 
is  typically  made up of serveral object files.  They  have  and 
extension  .E32  and  are  loaded into the DSI-32  board  by  the 
Definicon  Loader.  An executable file consists of five  distinct 
blocks.
     o    General Information
     o    Module directory
     o    Code
     o    Static data
     o    Link tables

Each  block starts at a 512 byte boundary within  the  executable 
file and may occupy zero or more pages. The first block is always 
the  general  information  block and the rest of the  blocks  are 
arbitrarily allocated by the linker.

8.8.2.1 General Information

     The  general information block contains information that  is 
common  to all modules.  Although also contained in this section, 
the  Global Module Table,  is not physically part of the  general 
information section.  It contains a copy of the module data  that 
enables   the   separate  modules  in  the  executable  file   to 
communicate and pass data between themselves.

   NAME   BITS           CONTENTS

     EXECID      16    7699 plus Version number
     DIRBLK      16    Pointer to first directory block
     HEAP_LOW    32    Heap low address
     HEAP_HIGH   32    Heap high address
     STACK_LOW   32    Stack low address
     STACH_HIGH  32    Stack high address
     MAIN        32    Main module number
     MODCOUNT    32    Number of modules in the module table
     MODADDR     32    Module table load address

   Global Module Table (MODCOUNT of these items)

     SBAD        32    Static Base data
     LINKAD      32    Link table address
     CODEAD      32    Code address
     UNUSED      32    Reserved
.pa
8.8.2.2 Module Directory

     The  Module Directories are contiguous arrays of  data,  one 
for each General Module Table entry (ie MODCOUNT of them).  Eight 
Module Directories are held in each 512 byte block.

   NAME   BITS           CONTENTS

     MODNM       64    Module name (left justified, space padded)
     MODTYPE      3    Reserved
     UNUSED      13    Reserved
     STRTADDR    32    Execution start address (offset from
                       code start)
     SLEN        32    Length of static base data in bytes
     LLEN        32    Length of link table in bytes
     CLEN        32    Length of code in bytes
     SYMLEN      32    Reserved
     SADDR       32    Static base data start address
     LADDR       32    Link table start address
     CADDR       32    Code start address
     LBLK        16    Link table start block number
     CBLK        16    Code start block number
     SYMBLK      16    Reserved
     SBLK        16    Static base start block number
     UNUSED     112    Reserved

8.8.2.3 Code

     The code blocks contain the actual code that belongs to  the 
module.

8.8.2.4 Static Base

     The  static base blocks contain the data that is  referenced 
by the series 32000 microprocessor's SB register.

8.8.2.5 Link Table

     The link table blocks store the linkage information for each 
module. The link table information allows the external addressing 
mode  of the series 32000 microprocessors to determine either the 
absolute  address  of a data item or the module and offset  of  a 
procedure in another module.

