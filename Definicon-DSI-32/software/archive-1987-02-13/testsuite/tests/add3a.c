
/*
  !!DESCRIPTION!! Addition tests - mostly int's
  !!ORIGIN!!      SDCC regression tests
  !!LICENCE!!     GPL, read COPYING.GPL
*/

#include <stdio.h>
#include <limits.h>

#define OUTFILE  "add3a.out"
#define THISFILE "add3a"

/* always include this _after_ the above macros have been defined */
#include "testsuite.h"

/*
  this test assumes:
  sizeof(long) == 4

  CAUTION: the wraparound behaviour is actually undefined, to get the "expected"
           behaviour with GCC, use -fwrapv or -fno-strict-overflow

  see: https://gcc.gnu.org/wiki/FAQ#signed_overflow
*/

#ifdef REFERENCE

/*
   make sure the reference output uses types with
   proper size
*/

#include <stdint.h>

int32_t long0 = 0;

#else

long long0 = 0;

#endif

void print(void)
{
#if defined(REFERENCE) && defined(REFCC_SIZEOF_LONG_64BIT)
    fprintf(outfile,"long0: %d\n", long0);
#else
    fprintf(outfile,"long0: %ld\n", long0);
#endif
}

int main(void)
{
    OPENTEST();

    long0 = 0x7f000000L;
    /* wrap around zero */
    print();
    long0 = long0 + 0x2000000L;
    if(long0 != -0x7f000000L) {
        fprintf(outfile,"failed!\n");
    }
    print();

    long0 = 0x7f000000L;
    /* wrap around zero */
    print();
    long0 = long0 + 0x2000000L;
    print();
    if(long0 != -0x7f000000L) {
        fprintf(outfile,"failed!\n");
    }
    print();

    CLOSETEST();

    return (0);
}
