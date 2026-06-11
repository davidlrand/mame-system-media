
/*
  !!DESCRIPTION!! struct base address dereferencing bug
  !!ORIGIN!!      Testsuite
  !!LICENCE!!     Public Domain
*/

/* #define STANDALONE */

#ifdef STANDALONE
#include <stdio.h>
#define outfile stdout
#define OPENTEST()
#define CLOSETEST()
#else
#define OUTFILE  "cc65141022.out"
#define THISFILE "cc65141022"
/* always include this _after_ the above macros have been defined */
#include "testsuite.h"
#endif

struct yywork
{
        char verify, advance;
} yycrank[] =
{
        {0,0}
};

struct yysvf
{
        struct yywork *yystoff;
};

unsigned char fails = 0;

int main(int n, char **args)
{
    struct yysvf *yystate;
    struct yywork *yyt;

    OPENTEST();

    yystate->yystoff = yycrank;
    yyt = yystate->yystoff;
#ifdef STANDALONE
    fprintf(outfile,"yycrank: %04x\n", yycrank);
    fprintf(outfile,"yystate->yystoff: %04x\n", yystate->yystoff);
    fprintf(outfile,"yyt: %04x\n", yyt);
#endif
    if(yyt == yycrank) {
        fprintf(outfile,"yyt == yycrank (ok)\n");
    } else {
        fprintf(outfile,"yyt != yycrank (fail)\n");
        ++fails;
    }

    if(yyt == yystate->yystoff) {
        fprintf(outfile,"yyt == yystate->yystoff (ok)\n");
    } else {
        fprintf(outfile,"yyt != yystate->yystoff (fail)\n");
        ++fails;
    }

    if(yycrank == yystate->yystoff) {
        fprintf(outfile,"yycrank == yystate->yystoff (ok)\n");
    } else {
        fprintf(outfile,"yycrank != yystate->yystoff (fail)\n");
        ++fails;
    }

    fprintf(outfile,"fails: %d\n", fails);

    CLOSETEST();

    return 0;
}
