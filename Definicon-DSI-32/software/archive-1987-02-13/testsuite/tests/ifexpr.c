
/*
  !!DESCRIPTION!! if/then, ? operator, compares
  !!ORIGIN!!      cc65 devel list
  !!LICENCE!!     Public Domain
*/

#include <stdio.h>
#include <limits.h>

#include "util/output.c"
#define OUTFILE  "ifexpr.out"
#define THISFILE "ifexpr"

void t1a(void)
{

    int a = -0x5000;

    fprintf(outfile,a < 0x5000 ? "ok\n" : "error\n");

}

void t1b(void)
{
    int a = -0x5000;
        if(a<0x5000)
        {
            fprintf(outfile,"ok\n");
        }
        else
        {
            fprintf(outfile,"error\n");
        }

}


int main(void)
{

	opentest(OUTFILE);

    t1a();t1b();

	closetest(THISFILE);

    return (0);
}

