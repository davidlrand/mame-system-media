
/*
  !!DESCRIPTION!!
  !!ORIGIN!!      testsuite
  !!LICENCE!!     Public Domain
  !!AUTHOR!!      Johan Kotlinski
*/

#define OUTFILE  "cc65091007.out"
#define THISFILE "cc65091007"

/* always include this _after_ the above macros have been defined */
#include "testsuite.h"

/*
This produces the compiler error "test.c(9): Error: Assignment to const"
Shouldn't be an error, should it? baz is const, bar isn't.
*/

typedef struct {
    char foo;
} Bar;

int main() {
    Bar bar;
    Bar* const baz = &bar;

    OPENTEST();
    baz->foo = 1;

    fprintf(outfile,"it works :)\n");
    CLOSETEST();
    /* always return OK */
    return (0);
}

