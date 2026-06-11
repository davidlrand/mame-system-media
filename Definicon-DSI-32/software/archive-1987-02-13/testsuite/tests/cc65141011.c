
/*
  !!DESCRIPTION!! equality problem
  !!ORIGIN!!      Testsuite
  !!LICENCE!!     Public Domain
*/

/*
    different result depending on whether constant is on left or right side

    http://www.cc65.org/mailarchive/2014-10/11680.html
    http://www.cc65.org/mailarchive/2014-10/11683.html
*/

/* #define STANDALONE */

#ifdef STANDALONE
#define outfile stdout
#define OPENTEST()
#define CLOSETEST()
#else
#define OUTFILE  "cc65141011.out"
#define THISFILE "cc65141011"
/* always include this _after_ the above macros have been defined */
#include "testsuite.h"
#endif

int main(int n, char **args)
{
    unsigned char joy_state = 0x7e;

    OPENTEST();

    fprintf(outfile,"bad: %u\n", 0 == (joy_state & 1) );
    fprintf(outfile,"good: %u\n", (joy_state & 1) == 0 );

    CLOSETEST();

    return 0;
}
