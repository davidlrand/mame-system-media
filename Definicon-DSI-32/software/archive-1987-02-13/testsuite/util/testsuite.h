
#ifndef __TESTSUITE_H__
#define __TESTSUITE_H__

//#ifdef __CC65__
///* have the prototypes here instead of including the lib so we dont have to
//   worry about the right header being included
// */
///* stdlib.h */
//void __fastcall__ exit (int ret);
///* stdio.h */
//typedef struct _FILE FILE;
//extern FILE* stdin;
//extern FILE* stdout;
//FILE* __fastcall__ fopen (const char* name, const char* mode);
//int __fastcall__ fclose (FILE* f);
//int fprintf (FILE* f, const char* format, ...);
//int printf (const char* format, ...);
//#else
/* for the reference compile we should use its proper headers */
#include <stdio.h>
#include <stdlib.h>
//#endif

/* get char from input */
int GETCHAR(void);

#define PRINTERROR()    printerror(__LINE__)
#define PRINTOK()       printok(__LINE__)

#define OPENTEST() opentest(OUTFILE)
#define CLOSETEST() closetest(THISFILE)

void opentest(char *name);
void closetest(char *name);

void printerror(int line);
void printok(int line);

/* fixme: use target/host specific version of following files */

#include "targetconf.h"
#include "refconfig.h"

#if defined(__LINUX__) || defined(LINUX)
#define REFCC
#endif

/*
    fixme: these should be in a library or sth
*/

#ifdef REFCC
#define LOG_ERROR(x)

#else

#ifdef TARGET_RUN6502
extern void __fastcall__  run6502_logerror(unsigned char);
#define LOG_ERROR(x) run6502_logerror(x)
#endif

#ifdef TARGET_SIM65
#define LOG_ERROR(x)
#endif

#ifdef TARGET_C64
//#include "c64/dirtest.c" /* needed by output.c */
#define LOG_ERROR(x)
#endif

#endif

/* generic utility functions */
#include "output.c"
#include "getchar.c"

#endif /* __TESTSUITE_H__ */
