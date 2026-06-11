/*
  !!DESCRIPTION!! Example for adding tests, also tests the testsuite's util-routines themselves
  !!ORIGIN!!      testsuite
  !!LICENCE!!     Public Domain
  !!AUTHOR!!      Groepaz/Hitmen
*/

#define OUTFILE  "example.out"
#define THISFILE "example"

/* always include this _after_ the above macros have been defined */
#include "testsuite.h"

void subtest1(void)
{
    if(0)
    {
        PRINTERROR();
    }
    else
    {
        PRINTOK();
    }
}

void subtest2(void)
{
    if(1)
    {
        PRINTOK();
    }
    else
    {
        PRINTERROR();
    }
}

int main(void)
{
    OPENTEST();

    subtest1();
    subtest2();

    CLOSETEST();

    /* always return OK */
    return (0);
}
