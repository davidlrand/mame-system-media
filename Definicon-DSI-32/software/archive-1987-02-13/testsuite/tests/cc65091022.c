/*
  !!DESCRIPTION!!
  !!ORIGIN!!      testsuite
  !!LICENCE!!     Public Domain
  !!AUTHOR!!      Johan Kotlinski
*/

#define OUTFILE  "cc65091022.out"
#define THISFILE "cc65091022"

/* always include this _after_ the above macros have been defined */
#include "testsuite.h"

/*
...gives "test.c(2): Error: Variable `foo' has unknown size" using -Cl.
Is it really unknown?

cc65 V2.13.0, SVN version: 4384
*/

int main() {
    char foo[] = { 0 };
    OPENTEST();

    fprintf(outfile,"it works :)\n");
    CLOSETEST();
    /* always return OK */
    return (0);
}
