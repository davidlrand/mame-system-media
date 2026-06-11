/*
  !!DESCRIPTION!! optimizer bug
  !!ORIGIN!!      testsuite
  !!LICENCE!!     Public Domain
  !!AUTHOR!!      Oliver Schmidt
*/

#define OUTFILE  "cc65090913.out"
#define THISFILE "cc65090913"

/* always include this _after_ the above macros have been defined */
#include "testsuite.h"

/*
> I found the problem and fixed it. cc65 treated a label as a statement, but
> the standard says, that a label is part of a statement. In a loop without
> curly braces like
>
>         while (foo < bar)
>             label:  ++foo;
>
> the following statement is the one that is looped over - and because cc65
> treated just the label as a statement, it created code that looped forever.

*/

int foo=0,bar=2;

int main(void)
{
    OPENTEST();

    while(foo<bar)
	    label: ++foo;

    fprintf(outfile,"foo: %d bar: %d\n",foo,bar);

    CLOSETEST();

    /* always return OK */
    return (0);
}
