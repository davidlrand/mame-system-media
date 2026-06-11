/*
  !!DESCRIPTION!! 
  !!ORIGIN!!      testsuite
  !!LICENCE!!     Public Domain
  !!AUTHOR!!
*/

#define OUTFILE  "cc65090111.out"
#define THISFILE "cc65090111"

/* always include this _after_ the above macros have been defined */
#include "testsuite.h"

/*
cc65 doesn't compile this, if i use the "-O"-option.
but it works with "while(!0)"; instead of  "for(;;);"

i'm using cl65 V2.12.9 win

----
#include <stdint.h>

int main(void)
{
 static uint8_t x = 0;
 static uint8_t y = 0;

    for (x = 0; x < 16; ++x)
    {
     y = y + 1;
    }
    for(;;);
}
*/

#include <stdint.h>

int test(void)
{
 static uint8_t x = 0;
 static uint8_t y = 0;

    for (x = 0; x < 16; ++x)
    {
     y = y + 1;
    }
    for(;;);
}

int main(void)
{
    OPENTEST();
    fprintf(outfile,"it works :)\n");
    CLOSETEST();

    /* always return OK */
    return (0);
}
