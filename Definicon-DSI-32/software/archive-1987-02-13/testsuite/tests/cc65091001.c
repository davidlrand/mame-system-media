
/*
  !!DESCRIPTION!! invalid binary operation on pointer, should not compile
  !!ORIGIN!!      testsuite
  !!LICENCE!!     Public Domain
  !!AUTHOR!!
*/

#define OUTFILE  "cc65091001.out"
#define THISFILE "cc65091001"

/* always include this _after_ the above macros have been defined */
#include "testsuite.h"


/* > Gets stuck in an endless loop with -O. */

#include <assert.h>
#include <string.h>
typedef unsigned char U8;
char var = 0xf0;
char fn(char bar)
{
    char* ptr = (char*)0xf;
    var |= ptr; /* should throw an error here */
    while (var > bar)
        var <<= 1;
    return 0;
}
int main() {
    OPENTEST();

    fn(0x7f);
    assert(0);

    fprintf(outfile,"it works :)\n");
    CLOSETEST();
    /* always return OK */
    return (0);
}