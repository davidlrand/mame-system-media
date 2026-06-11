
/*
  !!DESCRIPTION!! return values, implicit type conversion on return
  !!ORIGIN!!      cc65 devel list
  !!LICENCE!!     Public Domain
*/

#include <stdio.h>
#include <stdlib.h>

#include "util/output.c"
#define OUTFILE  "return.out"
#define THISFILE "return"

unsigned char val_char=0x76;
unsigned int val_int=0x5678;
unsigned long val_long=0x12345678;

int test1_int_char(void)
{
    return val_char;
}
int test1_int_int(void)
{
    return val_int;
}

int test2_int_char(void)
{
    return (int)val_char;
}
int test2_int_int(void)
{
    return (int)val_int;
}

long test1_long_char(void)
{
    return val_char;
}
long test1_long_int(void)
{
    return val_int;
}
long test1_long_long(void)
{
    return val_long;
}

long test2_long_char(void)
{
    return (long)val_char;
}
long test2_long_int(void)
{
    return (long)val_int;
}
long test2_long_long(void)
{
    return (long)val_long;
}

#define dotest(_n,_a,_v) \
    _n=_a; \
    fprintf(outfile,"%04lx %04lx,",(unsigned long)_n,(unsigned long)_v); \
    if(_n!=_v) fprintf(outfile,"failed\n"); \
    else fprintf(outfile,"ok\n")

int main(void)
{
int i;
unsigned long l;

	opentest(OUTFILE);

    fprintf(outfile,"\nwithout cast:\n");

    fprintf(outfile,"return int\n");

    dotest(i,test1_int_char(),0x76);
    dotest(i,test1_int_int(),0x5678);

    fprintf(outfile,"return long\n");

    dotest(l,test1_long_char(),0x76);
    dotest(l,test1_long_int(),0x5678);
    dotest(l,test1_long_long(),0x12345678);

   fprintf(outfile,"\nwith cast:\n");

   fprintf(outfile,"return int\n");

    dotest(i,test2_int_char(),0x76);
    dotest(i,test2_int_int(),0x5678);

    fprintf(outfile,"return long\n");

    dotest(l,test2_long_char(),0x76);
    dotest(l,test2_long_int(),0x5678);
    dotest(l,test2_long_long(),0x12345678);

	closetest(THISFILE);

        return (0);
}





