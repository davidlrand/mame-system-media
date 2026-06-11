
/*
  !!DESCRIPTION!! Cast to char
  !!ORIGIN!!      Piotr Fusik
  !!LICENCE!!     PD
*/

#define OUTFILE  "casttochar.out"
#define THISFILE "casttochar"

/* always include this _after_ the above macros have been defined */
#include "testsuite.h"

int f1(int i, int j) {
    return (signed char) (i + 1) == j;
}

int f2(int i, int j) {
    return (unsigned char) (i + 1) == j;
}

int f3(int i, int j) {
    return (char) (i + 1) == j;
}

int main(void)
{
    OPENTEST();

    fprintf(outfile,"f1: got :%04x ", f1(0x1234, 0x35));
    if(f1(0x1234, 0x35) == 0) {
        fprintf(outfile,"- failed");
    }
    fprintf(outfile,"\n");

    fprintf(outfile,"f2: got :%04x ", f2(0x1234, 0x35));
    if(f2(0x1234, 0x35) == 0) {
        fprintf(outfile,"- failed");
    }
    fprintf(outfile,"\n");

    fprintf(outfile,"f3: got :%04x ", f3(0x1234, 0x35));
    if(f3(0x1234, 0x35) == 0) {
        fprintf(outfile,"- failed");
    }
    fprintf(outfile,"\n");

    CLOSETEST();

    return (0);
}
