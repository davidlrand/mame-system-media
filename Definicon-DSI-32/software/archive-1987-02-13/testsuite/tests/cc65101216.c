
/*
  !!DESCRIPTION!! division bug
  !!ORIGIN!!      testsuite
  !!LICENCE!!     Public Domain
  !!AUTHOR!!      Stefan Wessels
*/

#define OUTFILE  "cc65101216.out"
#define THISFILE "cc65101216"

/* always include this _after_ the above macros have been defined */
#include "testsuite.h"

/*
The output from the code below is:
a / b = 6

Shouldn't that be 0?
*/

#include <stdio.h>
#define b 10000
char a;
int main()
{
        char c;

    OPENTEST();

        a = 100;
        c = a / b;
        fprintf(outfile, "a / b = %d", c);

    CLOSETEST();
    /* always return OK */
    return (0);
}