
/*
  !!DESCRIPTION!! unreachable code related bug
  !!ORIGIN!!      Testsuite
  !!LICENCE!!     Public Domain
*/

/*
    test2 and test3 will result in an endless loop (SVN version: 4974M)
*/

/* #define STANDALONE */

#ifdef STANDALONE
#define outfile stdout
#define OPENTEST()
#define CLOSETEST()
#else
#define OUTFILE  "cc65110211.out"
#define THISFILE "cc65110211"
/* always include this _after_ the above macros have been defined */
#include "testsuite.h"
#endif

static char upper[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

int test1(void)
{
    int res;
    unsigned char *p;

    p = upper;
    res = 0;

    while(*p) {
        if(*p < 0) {
            res = 1;
        }
        p++;
    }

    fprintf(outfile,"test1:ok\n");
    return res;
}

int test2(void)
{
    int res;
    unsigned char *p;

    p = upper;
    res = 0;

    while(*p) {
        if(*p++ < 0) {
            res = 1;
        }
    }

    fprintf(outfile,"test2:ok\n");
    return res;
}

int test3(void)
{
    int res;
    unsigned char *p;

    p = upper;
    res = 0;

    while(*p) {
        if(*++p < 0) {
            res = 1;
        }
    }

    fprintf(outfile,"test3:ok\n");
    return res;
}

int main(int n,char **args)
{
    OPENTEST();

    test1();
    test2();
    test3();

    fprintf(outfile,"it works :)\n");
    CLOSETEST();

    return 0;
}
