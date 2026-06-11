/*
  !!DESCRIPTION!! div/mod test
  !!ORIGIN!!
  !!LICENCE!!     public domain
*/

#include <stdio.h>

#include "util/output.c"
#define OUTFILE  "divmod.out"
#define THISFILE "divmod"

void printc(signed char a,signed char b){
signed char x=a/b,y=a%b,z=a*b;
	fprintf(outfile,"%3d,%3d is %3d,%3d,%3d\n",a,b,x,y,z);
}
void prints(short a,short b){
short x=a/b,y=a%b,z=a*b;
	fprintf(outfile,"%3d,%3d is %3d,%3d,%3d\n",a,b,x,y,z);
}
void printl(long a,long b){
long x=a/b,y=a%b,z=a*b;
	fprintf(outfile,"%3ld,%3ld is %3ld,%3ld,%3ld\n",a,b,x,y,z);
}

int main(void) {
	opentest(OUTFILE);
	printl( 3,-2);
	printl(-3,-2);
	printl(-3, 2);
	printl( 3, 2);
	fprintf(outfile,"-\n");
	prints( 3,-2);
	prints(-3,-2);
	prints(-3, 2);
	prints( 3, 2);
	fprintf(outfile,"-\n");
	printc( 3,-2);
	printc(-3,-2);
	printc(-3, 2);
	printc( 3, 2);
	closetest(THISFILE);
    /* return OK */
    return(0);

}
