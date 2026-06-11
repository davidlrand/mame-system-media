
/*
  !!DESCRIPTION!! check if character constants are translated correctly
  !!ORIGIN!!      cc65 bug report
  !!LICENCE!!     Public Domain
*/

#include <limits.h>
#include <ctype.h>
#define OUTFILE  "charconst.out"
#define THISFILE "charconst"

#include "testsuite.h"
#include "util/output.c"

void backslash(unsigned char c)
{

    fprintf(outfile,"%c : ",c);

    switch (c)
    {
	case 'b':
		c = '\b';
	case 'f':
		c = '\f';
	case 'n':
		c = '\n';
	case 'r':
		c = '\r';
	case 't':
		c = '\t';
	case 'v':
    #ifndef NO_BACKSLASH_V
        c = '\v';
    #else
        c = 0x0b;
    #endif
	}

    if(!isprint(c))
    {
        fprintf(outfile,"ok.\n");
    }
    else
    {
        fprintf(outfile,"failed.\n");
    }
}

void testbackslash(void)
{
        backslash('b');
        backslash('f');
        backslash('n');
        backslash('r');
        backslash('t');
        backslash('v');
}

int main(void)
{
	opentest(OUTFILE);

	testbackslash();

	closetest(THISFILE);

	return 0;
}
