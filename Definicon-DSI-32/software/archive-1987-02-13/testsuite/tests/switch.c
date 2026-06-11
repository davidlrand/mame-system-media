/*
  !!DESCRIPTION!! switch statement
  !!ORIGIN!!      LCC 4.1 Testsuite
  !!LICENCE!!     own, freely distributeable for non-profit. read CPYRIGHT.LCC
*/

#include <limits.h>
#define OUTFILE  "switch.out"
#define THISFILE "switch"

#include "testsuite.h"
#include "util/output.c"

#ifdef NO_IMPLICIT_FUNC_PROTOTYPES
testbig();
testbackslash();
backslash(int c);
f();
g();
h();
limit();

big(
# ifdef ASSUME_32BIT_UNSIGNED
	unsigned
# else
	unsigned long
# endif
x);

#endif

main()
{
	opentest(OUTFILE);

	testbackslash();
	f();
	g();
	h();
	testbig(); /* ! broken long int compare (?) */
	limit();   /* ! broken long int compare (?) */

	closetest(THISFILE);

	return 0;
}

testbig()
{
	#ifdef ASSUME_32BIT_INT
    	int i;
	#else
		signed long i;
	#endif
	       /*  2341234      2341234         2341234 */
	for (i = 0x1000000; i&0x7000000; i += 0x1000000) {
/*		printf("i = 0x%lx\n", i); */
		big(i);
	}
}

#ifdef NO_LOCAL_STRING_INIT
/*        static char _s[8]={"bfnrtvx"}; */
        static char _s[8]="bfnrtvx";
#endif

testbackslash()
{
        char *s;

#ifdef NO_STRINGS_IN_FOR
# ifndef NO_LOCAL_STRING_INIT
        char _s[8]={"bfnrtvx"};
# endif
        for (s=_s; *s; s++) {
#else
        for (s = "bfnrtvx"; *s; s++) {
#endif
			fprintf(outfile,"%c = %c\n", *s, backslash(*s));
        }
}

backslash(c)
{
	switch (c)
    {
	case 'b':
		return 'b';
	case 'f':
		return 'f';
	case 'n':
		return 'n';
	case 'r':
		return 'r';
	case 't':
		return 't';
	case 'v':
        return 'v';
	}

	return 'x';
}

f() {
	int i, x = 0, y;

	fprintf(outfile,"f:\n");
	for (i = 0; i <= 20; i++) {
		y = i;
		switch (i) {
		case 1: x = i; break;
		case 2: x = i; break;
		case 7: x = i; break;
		case 8: x = i; break;
		case 9: x = i; break;
		case 16: x = i; break;
		case 17: x = i; break;
		case 18: x = i; break;
		case 19: x = i; break;
		case 20: x = i; break;
		}
		fprintf(outfile,"x = %d\n", x);
	}
}

g() {
	int i;

	fprintf(outfile,"g:\n");
	for (i = 1; i <= 10; i++)
		switch (i) {
		case 1: case 2: fprintf(outfile,"1 %d\n", i); break;
		case 3: case 4: case 5: fprintf(outfile,"2 %d\n", i); break;
		case 6: case 7: case 8: fprintf(outfile,"3 %d\n", i);
		default:
			fprintf(outfile,"d %d\n", i); break;
		case 1001: case 1002: case 1003: case 1004:
			fprintf(outfile,"5 %d\n", i); break;
		case 3001: case 3002: case 3003: case 3004:
			fprintf(outfile,"6 %d\n", i); break;
	}
}

h()
{
	int i, n=0;

	fprintf(outfile,"h:\n");
	for (i = 1; i <= 500; i++)
		switch (i) {
		default: n++; continue;
		case 128: fprintf(outfile,"i = %d\n", i); break;
		case 16: fprintf(outfile,"i = %d\n", i); break;
		case 8: fprintf(outfile,"i = %d\n", i); break;
		case 120: fprintf(outfile,"i = %d\n", i); break;
		case 280: fprintf(outfile,"i = %d\n", i); break;
		case 264: fprintf(outfile,"i = %d\n", i); break;
		case 248: fprintf(outfile,"i = %d\n", i); break;
		case 272: fprintf(outfile,"i = %d\n", i); break;
		case 304: fprintf(outfile,"i = %d\n", i); break;
		case 296: fprintf(outfile,"i = %d\n", i); break;
		case 288: fprintf(outfile,"i = %d\n", i); break;
		case 312: fprintf(outfile,"i = %d\n", i); break;
		}
	fprintf(outfile,"%d defaults\n", n);
}

#ifdef NO_OLD_FUNC_DECL
        big(
#else
        big(x)
#endif

# ifdef ASSUME_32BIT_UNSIGNED
	unsigned
# else
	unsigned long
# endif

#ifdef NO_OLD_FUNC_DECL
        x) {
#else
        x; {
#endif

/*	printf("x = 0x%x\n", x); */

	switch(x&0x6000000){
	case -1:
	case -2:
	case 0x0000000:
		fprintf(outfile,"x = 0x%lx\n", x); break;
	case 0x2000000:
		fprintf(outfile,"x = 0x%lx\n", x); break;
	case 0x4000000:
		fprintf(outfile,"x = 0x%lx\n", x); break;
	default:
		fprintf(outfile,"x = 0x%lx (default)\n", x); break;
	}
}

limit() {
	int i;

	for (i = INT_MIN; i <= INT_MIN+5; i++)
/*	for (i = INT_MIN; i <  INT_MIN+6; i++) */
		switch (i) {
			case INT_MIN:	fprintf(outfile,"0\n"); break;
			case INT_MIN+1:	fprintf(outfile,"1\n"); break;
			case INT_MIN+2:	fprintf(outfile,"2\n"); break;
			case INT_MIN+3:	fprintf(outfile,"3\n"); break;
			case INT_MIN+4:	fprintf(outfile,"4\n"); break;
			default:     	fprintf(outfile,"5\n"); break;
		}
	for (i = INT_MAX; i >= INT_MAX-5; i--)
		switch (i) {
			case INT_MAX:	fprintf(outfile,"0\n"); break;
			case INT_MAX-1:	fprintf(outfile,"1\n"); break;
			case INT_MAX-2:	fprintf(outfile,"2\n"); break;
			case INT_MAX-3:	fprintf(outfile,"3\n"); break;
			case INT_MAX-4:	fprintf(outfile,"4\n"); break;
			default:	    fprintf(outfile,"5\n"); break;
		}
}
