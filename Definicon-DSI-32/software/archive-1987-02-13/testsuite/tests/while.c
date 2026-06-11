
/*
  !!DESCRIPTION!!
  !!ORIGIN!!      SDCC regression tests
  !!LICENCE!!     GPL, read COPYING.GPL
*/

#include <stdio.h>
#include <limits.h>

#include "util/output.c"
#define OUTFILE  "while.out"
#define THISFILE "while"

unsigned char success = 0;
unsigned char failures = 0;
unsigned char dummy = 0;

#ifdef SUPPORT_BIT_TYPES
bit bit0 = 0;
#endif
unsigned int aint0 = 0;
unsigned int aint1 = 0;
unsigned char achar0 = 0;
unsigned char achar1 = 0;


void
done ()
{

  dummy++;

}

void
while1 (void)
{
  unsigned char i = 10;

  do
    {
      achar0++;
    }
  while (--i);

  if (achar0 != 10)
    failures++;

}


int
main (void)
{

	opentest(OUTFILE);

  while1 ();


  success = failures;
  done ();
  fprintf(outfile,"failures: %d\n",failures);
	closetest(THISFILE);

    return (0);
}
