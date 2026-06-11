/*
  !!DESCRIPTION!! 
  !!ORIGIN!!      testsuite
  !!LICENCE!!     Public Domain
  !!AUTHOR!!
*/

#define OUTFILE  "cc65080328.out"
#define THISFILE "cc65080328"

/* always include this _after_ the above macros have been defined */
#include "testsuite.h"

/*
The following code produces an 'Error: Incompatible pointer types' at
the last line when compiling with snapshot-2.11.9.20080316 without
optimizations. If I remove the struct inside f() it compiles fine ?!?

Best, Oliver
*/

void f(void){struct{int i;}d;}
struct{void(*p)(void);}s={f};

int main(void)
{
    OPENTEST();
    fprintf(outfile,"it works :)\n");
    CLOSETEST();
    /* always return OK */
    return (0);
}
