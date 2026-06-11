/*
  !!DESCRIPTION!! minimal Program, checks if the Compiler and testsuite framework works
  !!ORIGIN!!      testsuite
  !!LICENCE!!     Public Domain
  !!AUTHOR!!      Groepaz/Hitmen
*/

#if 1
#include "testsuite.h"
#include "util/output.c"

#define OUTFILE  "minimal.out"
#define THISFILE "minimal"
#endif

int main(void)
{
#if 1
    opentest(OUTFILE);
    fprintf(outfile,"it works :)\n");
    closetest(THISFILE);
#endif
    /* always return OK */
    return (0);
}
