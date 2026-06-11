/*
  !!DESCRIPTION!! verbose/debug version of yacc.c (if one fails and the other does not you most likely have a stack related problem)
  !!ORIGIN!!      LCC 4.1 Testsuite
  !!LICENCE!!     own, freely distributeable for non-profit. read CPYRIGHT.LCC
*/

/*#define STANDALONE*/

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#ifdef STANDALONE

FILE *outfile=NULL;
FILE *infile=NULL;
#define opentest(x) { outfile=stdout; infile=stdin; }
#define closetest(x) { fclose(outfile); fclose(infile); }
#define GETCHAR() (fgetc(infile))

#else

#define OUTFILE  "yaccdbg.out"
#define THISFILE "yaccdbg"
#define INFILE   "yaccdbg.in"

#endif

/* always include this _after_ the above macros have been defined */
#include "testsuite.h"

#define LEXDEBUG
#define YYDEBUG

#define YACCDBG
#include "yacc.c"
