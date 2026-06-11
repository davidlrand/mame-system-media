/*
  !!DESCRIPTION!! bit field bug
  !!ORIGIN!!      testsuite
  !!LICENCE!!     Public Domain
  !!AUTHOR!!      Johan Kotlinski
*/

#define OUTFILE  "cc65091020.out"
#define THISFILE "cc65091020"

/* always include this _after_ the above macros have been defined */
#include "testsuite.h"

#include <assert.h>

struct {
    int foo : 7;
    int bar : 4;
} baz = { 0, 2 };

int main() {
    char bar = baz.bar;

    OPENTEST();

    assert(2 == bar);

    fprintf(outfile,"it works :)\n");
    CLOSETEST();
    /* always return OK */
    return (0);
}

/* Assert fails. (SVN rev 4381) */