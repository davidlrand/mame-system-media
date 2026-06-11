/*
  !!DESCRIPTION!! variable argument lists
  !!ORIGIN!!      LCC 4.1 Testsuite
  !!LICENCE!!     own, freely distributeable for non-profit. read CPYRIGHT.LCC
*/

#include "testsuite.h"
#include "util/output.c"
#define OUTFILE  "stdarg.out"
#define THISFILE "stdarg"

#include <stdarg.h>

#ifndef NO_FUNCS_TAKE_STRUCTS
struct node
{
	int a[4];
} x =
{
#ifdef NO_SLOPPY_STRUCT_INIT
	{
#endif
		1,2,3,4
#ifdef NO_SLOPPY_STRUCT_INIT
	}
#endif
};
#endif

print(char *fmt, ...);

main()
{
	opentest(OUTFILE);
	print("test 1\n");
	print("test %s\n", "2");
	print("test %d%c", 3, '\n');
	print("%s%s %w%c", "te", "st", 4, '\n');
    #ifdef NO_FLOATS
		print("%s%s %f%c", "te", "st", (signed long) 5, '\n');
	#else
		print("%s%s %f%c", "te", "st", 5.0, '\n');
    #endif
	#ifndef NO_FUNCS_TAKE_STRUCTS
        print("%b %b %b %b %b %b\n", x, x, x, x, x, x);
	#endif
	closetest(THISFILE);
	return 0;
}

print(char *fmt, ...) {
	va_list ap;
	va_start(ap, fmt);
	for (; *fmt; fmt++)
	{
		if (*fmt == '%')
			switch (*++fmt) {
            case 'b': {
					#ifdef NO_FUNCS_TAKE_STRUCTS
                    	fprintf(outfile,"(1 2 3 4)");
					#else
                    	struct node x =
							va_arg(
								ap,
								struct node
								);
                    	fprintf(outfile,"(%d %d %d %d)", x.a[0], x.a[1], x.a[2], x.a[3]);
					#endif
                    break;
                    }
			case 'c':
				/* fprintf(outfile,"%c", va_arg(ap, char)); */
				fprintf(outfile,"%c", va_arg(ap, int));
				break;
			case 'd':
				fprintf(outfile,"%d", va_arg(ap, int));
				break;
			case 'w':
				/* fprintf(outfile,"%x", va_arg(ap, short)); */
				fprintf(outfile,"%x", va_arg(ap, int));
				break;
			case 's':
				fprintf(outfile,"%s", va_arg(ap, char *));
				break;
			case 'f':
            	#ifdef NO_FLOATS
					fprintf(outfile,"%ld.000000", va_arg(ap, signed long));
				#else
					fprintf(outfile,"%f", va_arg(ap, double));
            	#endif
				break;
			default:
				fprintf(outfile,"%c", *fmt);
				break;
			}
		 else
			fprintf(outfile,"%c", *fmt);
	}
	va_end(ap);
}
