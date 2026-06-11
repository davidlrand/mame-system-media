
/*
  !!DESCRIPTION!! mod operator bug
  !!ORIGIN!!      testsuite
  !!LICENCE!!     Public Domain
  !!AUTHOR!!      marcas
*/

#define OUTFILE  "cc65101209.out"
#define THISFILE "cc65101209"

/* always include this _after_ the above macros have been defined */
#include "testsuite.h"

#include <stdlib.h>
#include <stdio.h>

int main(void) {

     int tmp;

    OPENTEST();

     for (tmp = 0; tmp<=12345; tmp++)
         if (!(tmp%1000)) fprintf(outfile, "%d mod 1000 is %d\n", tmp, tmp%1000);

    CLOSETEST();
    /* always return OK */
    return (0);
}

/*
results in  (vice x64)
0 mod 1000 is 0
232 mod 1000 is 0
1000 mod 1000 is 0

Interesting:

        1000    = $3E8
        232     = $E8

So testing with 999 gives:

0 mod 999 is 0
231 mod 999 is 0
999 mod 999 is 0

This seems to be systematic. 
*/