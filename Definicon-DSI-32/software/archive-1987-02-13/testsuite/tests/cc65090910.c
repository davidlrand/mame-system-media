/*
  !!DESCRIPTION!! optimizer bug
  !!ORIGIN!!      testsuite
  !!LICENCE!!     Public Domain
  !!AUTHOR!!      Oliver Schmidt
*/

#define OUTFILE  "cc65090910.out"
#define THISFILE "cc65090910"

/* always include this _after_ the above macros have been defined */
#include "testsuite.h"

/*
After spending a "little" time I finally succeeded in isolating an
(maybe THE) optimizer bug causing Contiki to fail.

From my user perspective it is very interesting that the bug shows up
with compiler option -O but does _not_ show up with -Oi.
*/

unsigned htons(unsigned val)
{
	return (((unsigned) (val)) << 8) | (((unsigned) (val)) >> 8);
}

int main(void)
{
    OPENTEST();

    fprintf(outfile,"%x -> %x\n", 0x1234, htons(0x1234) & 0xffff);

    CLOSETEST();

    /* always return OK */
    return (0);
}
