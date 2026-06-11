
/*
  !!DESCRIPTION!! print character frequencies
  !!ORIGIN!!      LCC 4.1 Testsuite
  !!LICENCE!!     own, freely distributeable for non-profit. read CPYRIGHT.LCC
*/


/*
        cf - print character frequencies
*/

#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

#define INFILE   "cf.in"
#define OUTFILE  "cf.out"
#define THISFILE "cf"
#include "testsuite.h"

#ifndef NO_FLOATS
float f[0x100];
#else
signed f[0x100];
#endif

#ifdef NO_OLD_FUNC_DECL
int main(int argc,char **argv)
#else
main(argc, argv)
int argc;
char *argv[];
#endif
{
	int i, c, nc;
#ifndef NO_FLOATS
	float cutoff, atof();
#else
        signed cutoff;
#endif

	opentest(OUTFILE);

	if (argc <= 1)
#ifndef NO_FLOATS
		cutoff = 0.0;
#else
                cutoff = 0;
#endif
	else
#ifndef NO_FLOATS
		cutoff = atof(argv[1])/100;
#else
                cutoff = atoi(argv[1])/100;
#endif
	for (i = 0; i < 0x100; )
    {
#ifndef NO_FLOATS
		f[i++] = 0.0;
#else
        f[i++] = 0;
#endif
    }

    fprintf(outfile,"input:\n\n");
    
    nc = 0;
    while ((c = GETCHAR()) != -1)
    {
/*        fprintf(outfile,"[%02x]",c); */
        fprintf(outfile,"%c",c);
        f[c] += 1;
        nc++;
    }
    fprintf(outfile,"\n\ncount: %d\n\n",nc);

    /*
        now try to print a report in a way so that
        the order is somewhat independent from the
        target character set
    */

    fprintf(outfile,"a-z char:freq\n\n");

    /* first round ... lowercase characters */
	for (i = 0; i < 0x100; ++i)
    {
		if ((f[i]) && ((f[i]/nc) >= cutoff))
        {
			if ((i >= 'a') && (i <= 'z'))
            {
				fprintf(outfile,"%c", i);
#ifndef NO_FLOATS
                fprintf(outfile,":%.1f\n", 100*f[i]/nc);
#else
                fprintf(outfile,":%d\n", 100*f[i]/nc);
#endif
                f[i]=0;
            }
		}
    }

    fprintf(outfile,"A-Z char:freq\n\n");

    /* second round ... uppercase characters */
	for (i = 0; i < 0x100; ++i)
    {
		if ((f[i]) && ((f[i]/nc) >= cutoff))
        {
			if ((i >= 'A') && (i <= 'Z'))
            {
				fprintf(outfile,"%c", i);
#ifndef NO_FLOATS
                fprintf(outfile,":%.1f\n", 100*f[i]/nc);
#else
                fprintf(outfile,":%d\n", 100*f[i]/nc);
#endif
                f[i]=0;
            }
		}
    }

    fprintf(outfile,"0-9 char:freq\n\n");

    /* third round ... numbers */
	for (i = 0; i < 0x100; ++i)
    {
		if ((f[i]) && ((f[i]/nc) >= cutoff))
        {
			if ((i >= '0') && (i <= '9'))
            {
				fprintf(outfile,"%c", i);
#ifndef NO_FLOATS
                fprintf(outfile,":%.1f\n", 100*f[i]/nc);
#else
                fprintf(outfile,":%d\n", 100*f[i]/nc);
#endif
                f[i]=0;
            }
		}
    }

    fprintf(outfile,"isprint char:freq\n\n");

    /* second last round ... remaining printable characters */
	for (i = 0; i < 0x100; ++i)
    {
		if ((f[i]) && ((f[i]/nc) >= cutoff))
        {
			if(isprint(i))
            {
				fprintf(outfile,"%c", i);
#ifndef NO_FLOATS
                fprintf(outfile,":%.1f\n", 100*f[i]/nc);
#else
                fprintf(outfile,":%d\n", 100*f[i]/nc);
#endif
                f[i]=0;
            }
		}
    }

    fprintf(outfile,"rest char:freq\n\n");

    /* last round ... remaining non printable characters */
	for (i = 0; i < 0x100; ++i)
    {
		if ((f[i]) && ((f[i]/nc) >= cutoff))
        {
            if(i=='\n')
            {
				fprintf(outfile,"newline");
            }
            else
            {
				fprintf(outfile,"%03o", i);
            }
#ifndef NO_FLOATS
                fprintf(outfile,":%.1f\n", 100*f[i]/nc);
#else
                fprintf(outfile,":%d\n", 100*f[i]/nc);
#endif
		}
    }
	closetest(THISFILE);

	return 0;
}
