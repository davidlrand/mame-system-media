
/*
  !!DESCRIPTION!! forgetting to emit labels
  !!ORIGIN!!      Testsuite
  !!LICENCE!!     Public Domain
*/

/*
    http://www.cc65.org/mailarchive/2014-10/11673.html
    http://www.cc65.org/mailarchive/2014-10/11675.html
*/

/* #define STANDALONE */

#ifdef STANDALONE
#define outfile stdout
#define OPENTEST()
#define CLOSETEST()
#else
#define OUTFILE  "cc65141002.out"
#define THISFILE "cc65141002"
/* always include this _after_ the above macros have been defined */
#include "testsuite.h"
#endif

struct udata {
        int     (*u_sigvec[16])();
        int     u_argn;
        int     u_argn1;
};

struct udata udata;

#define sig (int)udata.u_argn
#define func (int (*)())udata.u_argn1

int _signal(void)
{
        if (func != 0) {
                goto nogood;
        }
        udata.u_sigvec[sig] = func;
        return 0;

nogood:
        return (-1);
}

int main(int n,char **args)
{
    OPENTEST();

    _signal();
    fprintf(outfile,"it works\n");

    CLOSETEST();

    return 0;
}
