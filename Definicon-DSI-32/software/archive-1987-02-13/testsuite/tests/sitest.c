
/*
  !!DESCRIPTION!! C99 WCHAR test
  !!ORIGIN!!
  !!LICENCE!!     public domain
*/

/*
	sitest -- exercise features of C99 <stdint.h> and <inttypes.h>

	This source code has been placed into the PUBLIC DOMAIN by its author.

	last edit:	1999/11/05	gwyn@arl.mil

	Tries to accommodate pre-C99 versions of <inttypes.h>.

	Takes advantage of __Q8_* symbols defined by a particular
	implementation of <stdint.h>, but doesn't require them.

	NOTE:	This is not a thorough validation test of the facilities.
*/

#define NO_INTERNAL_WCHAR
/*#define STANDALONE*/

#include	<errno.h>
#include	<limits.h>		/* for CHAR_BIT */
#include	<stdio.h>
#include	<stddef.h>		/* for ptrdiff_t */
#include	<stdlib.h>
#include	<string.h>

#if !defined(STANDARD_C99) && !defined(STANDARD_CC65)

#error "this test checks C99 features, which are not available in the selected standard."

#else

#ifdef NO_WCHAR

#warn "this test checks C99 features, but NO_WCHAR is defined so the test will most definetly fails."

#endif

#include	<inttypes.h>		/* embeds <stdint.h> */

#include	<signal.h>		/* for sig_atomic_t */

#if	defined(INTMAX_MAX)		/* <inttypes.h> has C99 features */
#include	<wchar.h>
#endif

#include	<inttypes.h>		/* test idempotency */

#ifdef STANDALONE

FILE *outfile=NULL;
#define opentest(x) outfile=stdout;
#define closetest(x)

#else

#define OUTFILE  "sitest.out"
#define THISFILE "sitest"

/* always include this _after_ the above macros have been defined */
#include "testsuite.h"

#endif

#if	__STDC_VERSION__ >= 199901
#ifndef	__Q8_QT
#define	__Q8_QT	long long
#endif
#endif

#ifdef	PRIdMAX
#define	HAVE_PRIdMAX
#ifndef	__Q8_MT
#define	__Q8_MT	intmax_t
#endif
#else
#ifdef	PRIdLEAST64
#ifndef	__Q8_MT
#define	__Q8_MT	int_least64_t
#endif
#define	PRIdMAX	PRIdLEAST64
#else
#ifndef	__Q8_MT
#define	__Q8_MT	long
#endif
#define	PRIdMAX	"ld"
#endif
#endif

#ifdef	PRIuMAX
#define	HAVE_PRIuMAX
#define	U__Q8_MT	uintmax_t
#else
#ifdef	PRIuLEAST64
#define	U__Q8_MT	uint_least64_t
#define	PRIuMAX	PRIuLEAST64
#else
#define	U__Q8_MT	unsigned long
#define	PRIuMAX	"lu"
#endif
#endif

#define	STR_SUB(s)	# s
#define	STRINGIZE(s)	STR_SUB(s)	/* extra level to expand argument */

#if	defined(SCNo32) || defined(PRIo32)
static int32_t		int32;
#endif
static int_least16_t	intl16;
static uint_least16_t	uintl16;
static uint_fast16_t	uintf16;
static intmax_t		intmax;
static uintmax_t	uintmax;

int
main()	{
	int	status = 0;		/* exit status to be returned */

    OPENTEST();
	
	/* <stdint.h> features: */

	fprintf(outfile,"CHAR_BIT=%u\n", (unsigned)CHAR_BIT );
	fprintf(outfile,"sizeof(char)=%u\n", (unsigned)sizeof(char));	/* s.b. 1 */
	fprintf(outfile,"sizeof(short)=%u\n", (unsigned)sizeof(short));
	fprintf(outfile,"sizeof(int)=%u\n", (unsigned)sizeof(int));
	fprintf(outfile,"sizeof(long)=%u\n", (unsigned)sizeof(long));
#ifdef	__Q8_QT
	fprintf(outfile,"sizeof(long long)=%u\n", (unsigned)sizeof(__Q8_QT));
#else
	fprintf(outfile,"*** long long isn't defined ***\n");
#endif
	fprintf(outfile,"sizeof(intmax_t)=%u\n", (unsigned)sizeof(intmax_t));
	fprintf(outfile,"sizeof(ptrdiff_t)=%u\n", (unsigned)sizeof(ptrdiff_t));
	fprintf(outfile,"sizeof(size_t)=%u\n", (unsigned)sizeof(size_t));
	fprintf(outfile,"sizeof(sig_atomic_t)=%u\n", (unsigned)sizeof(sig_atomic_t));
	fprintf(outfile,"sizeof(wchar_t)=%u\n", (unsigned)sizeof(wchar_t));
#if	defined(WINT_MAX) || __STDC_VERSION__ >= 199901
	fprintf(outfile,"sizeof(wint_t)=%u\n", (unsigned)sizeof(wint_t));
#else
	fprintf(outfile,"*** wint_t isn't defined ***\n");
	status = EXIT_FAILURE;
#endif
#ifdef	INT8_MAX
	fprintf(outfile,"sizeof(int8_t)=%u\n", (unsigned)sizeof(int8_t));
	fprintf(outfile,"sizeof(uint8_t)=%u\n", (unsigned)sizeof(uint8_t));
#endif
#ifdef	INT9_MAX
	fprintf(outfile,"sizeof(int9_t)=%u\n", (unsigned)sizeof(int9_t));
	fprintf(outfile,"sizeof(uint9_t)=%u\n", (unsigned)sizeof(uint9_t));
#endif
#ifdef	INT12_MAX
	fprintf(outfile,"sizeof(int12_t)=%u\n", (unsigned)sizeof(int12_t));
	fprintf(outfile,"sizeof(uint12_t)=%u\n", (unsigned)sizeof(uint12_t));
#endif
#ifdef	INT16_MAX
	fprintf(outfile,"sizeof(int16_t)=%u\n", (unsigned)sizeof(int16_t));
	fprintf(outfile,"sizeof(uint16_t)=%u\n", (unsigned)sizeof(uint16_t));
#endif
#ifdef	INT18_MAX
	fprintf(outfile,"sizeof(int18_t)=%u\n", (unsigned)sizeof(int18_t));
	fprintf(outfile,"sizeof(uint18_t)=%u\n", (unsigned)sizeof(uint18_t));
#endif
#ifdef	INT24_MAX
	fprintf(outfile,"sizeof(int24_t)=%u\n", (unsigned)sizeof(int24_t));
	fprintf(outfile,"sizeof(uint24_t)=%u\n", (unsigned)sizeof(uint24_t));
#endif
#ifdef	INT32_MAX
	fprintf(outfile,"sizeof(int32_t)=%u\n", (unsigned)sizeof(int32_t));
	fprintf(outfile,"sizeof(uint32_t)=%u\n", (unsigned)sizeof(uint32_t));
#endif
#ifdef	INT36_MAX
	fprintf(outfile,"sizeof(int36_t)=%u\n", (unsigned)sizeof(int36_t));
	fprintf(outfile,"sizeof(uint36_t)=%u\n", (unsigned)sizeof(uint36_t));
#endif
#ifdef	INT40_MAX
	fprintf(outfile,"sizeof(int40_t)=%u\n", (unsigned)sizeof(int40_t));
	fprintf(outfile,"sizeof(uint40_t)=%u\n", (unsigned)sizeof(uint40_t));
#endif
#ifdef	INT48_MAX
	fprintf(outfile,"sizeof(int48_t)=%u\n", (unsigned)sizeof(int48_t));
	fprintf(outfile,"sizeof(uint48_t)=%u\n", (unsigned)sizeof(uint48_t));
#endif
#ifdef	INT60_MAX
	fprintf(outfile,"sizeof(int60_t)=%u\n", (unsigned)sizeof(int60_t));
	fprintf(outfile,"sizeof(uint60_t)=%u\n", (unsigned)sizeof(uint60_t));
#endif
#ifdef	INT64_MAX
	fprintf(outfile,"sizeof(int64_t)=%u\n", (unsigned)sizeof(int64_t));
	fprintf(outfile,"sizeof(uint64_t)=%u\n", (unsigned)sizeof(uint64_t));
#endif
#ifdef	INT72_MAX
	fprintf(outfile,"sizeof(int72_t)=%u\n", (unsigned)sizeof(int72_t));
	fprintf(outfile,"sizeof(uint72_t)=%u\n", (unsigned)sizeof(uint72_t));
#endif
#ifdef	INT128_MAX
	fprintf(outfile,"sizeof(int128_t)=%u\n", (unsigned)sizeof(int128_t));
	fprintf(outfile,"sizeof(uint128_t)=%u\n", (unsigned)sizeof(uint128_t));
#endif
	fprintf(outfile,"sizeof(int_least8_t)=%u\n", (unsigned)sizeof(int_least8_t));
	fprintf(outfile,"sizeof(uint_least8_t)=%u\n", (unsigned)sizeof(uint_least8_t));
	fprintf(outfile,"sizeof(int_least16_t)=%u\n", (unsigned)sizeof(int_least16_t));
	fprintf(outfile,"sizeof(uint_least16_t)=%u\n", (unsigned)sizeof(uint_least16_t));
	fprintf(outfile,"sizeof(int_least32_t)=%u\n", (unsigned)sizeof(int_least32_t));
	fprintf(outfile,"sizeof(uint_least32_t)=%u\n", (unsigned)sizeof(uint_least32_t));
#ifdef	INT_LEAST64_MAX
	fprintf(outfile,"sizeof(int_least64_t)=%u\n", (unsigned)sizeof(int_least64_t));
	fprintf(outfile,"sizeof(uint_least64_t)=%u\n", (unsigned)sizeof(uint_least64_t));
#else
	fprintf(outfile,"*** uint_least64_t isn't defined ***\n");
	status = EXIT_FAILURE;
#endif
#ifdef	INT_LEAST128_MAX
	fprintf(outfile,"sizeof(int_least128_t)=%u\n", (unsigned)sizeof(int_least128_t));
	fprintf(outfile,"sizeof(uint_least128_t)=%u\n",
		(unsigned)sizeof(uint_least128_t));
#endif
	fprintf(outfile,"sizeof(int_fast8_t)=%u\n", (unsigned)sizeof(int_fast8_t));
	fprintf(outfile,"sizeof(uint_fast8_t)=%u\n", (unsigned)sizeof(uint_fast8_t));
	fprintf(outfile,"sizeof(int_fast16_t)=%u\n", (unsigned)sizeof(int_fast16_t));
	fprintf(outfile,"sizeof(uint_fast16_t)=%u\n", (unsigned)sizeof(uint_fast16_t));
	fprintf(outfile,"sizeof(int_fast32_t)=%u\n", (unsigned)sizeof(int_fast32_t));
	fprintf(outfile,"sizeof(uint_fast32_t)=%u\n", (unsigned)sizeof(uint_fast32_t));
#ifdef	INT_FAST64_MAX
	fprintf(outfile,"sizeof(int_fast64_t)=%u\n", (unsigned)sizeof(int_fast64_t));
	fprintf(outfile,"sizeof(uint_fast64_t)=%u\n", (unsigned)sizeof(uint_fast64_t));
#else
	fprintf(outfile,"*** int_fast64_t isn't defined ***\n");
	status = EXIT_FAILURE;
#endif
#ifdef	INT_FAST128_MAX
	fprintf(outfile,"sizeof(int_fast128_t)=%u\n", (unsigned)sizeof(int_fast128_t));
	fprintf(outfile,"sizeof(uint_fast128_t)=%u\n", (unsigned)sizeof(uint_fast128_t));
#endif
#if	defined(INTPTR_MAX)
	fprintf(outfile,"sizeof(intptr_t)=%u\n", (unsigned)sizeof(intptr_t));
#if	defined(UINTPTR_MAX)
	fprintf(outfile,"sizeof(uintptr_t)=%u\n", (unsigned)sizeof(uintptr_t));
#else
	fprintf(outfile,"*** intptr_t is defined but uintptr_t isn't ***\n");
	status = EXIT_FAILURE;
#endif
#elif	defined(UINTPTR_MAX)
	fprintf(outfile,"sizeof(uintptr_t)=%u\n", (unsigned)sizeof(uintptr_t));
	fprintf(outfile,"*** uintptr_t is defined but intptr_t isn't ***\n");
	status = EXIT_FAILURE;
#else
	fprintf(outfile,"*** neither intptr_t nor uintptr_t is defined ***\n");
	status = EXIT_FAILURE;
#endif
#ifdef	INTMAX_MAX
	fprintf(outfile,"sizeof(intmax_t)=%u\n", (unsigned)sizeof(intmax_t));
	fprintf(outfile,"sizeof(uintmax_t)=%u\n", (unsigned)sizeof(uintmax_t));
#else
	fprintf(outfile,"*** intmax_t isn't defined ***\n");
	status = EXIT_FAILURE;
#endif

#ifdef	INT8_MAX
	fprintf(outfile,"INT8_MIN=%"PRIdMAX"\n", (__Q8_MT)INT8_MIN);
	fprintf(outfile,"INT8_MAX=%"PRIdMAX"\n", (__Q8_MT)INT8_MAX);
	fprintf(outfile,"UINT8_MAX=%"PRIuMAX"\n", (U__Q8_MT)UINT8_MAX);
#endif
#ifdef	INT9_MAX
	fprintf(outfile,"INT9_MIN=%"PRIdMAX"\n", (__Q8_MT)INT9_MIN);
	fprintf(outfile,"INT9_MAX=%"PRIdMAX"\n", (__Q8_MT)INT9_MAX);
	fprintf(outfile,"UINT9_MAX=%"PRIuMAX"\n", (U__Q8_MT)UINT9_MAX);
#endif
#ifdef	INT12_MAX
	fprintf(outfile,"INT12_MIN=%"PRIdMAX"\n", (__Q8_MT)INT12_MIN);
	fprintf(outfile,"INT12_MAX=%"PRIdMAX"\n", (__Q8_MT)INT12_MAX);
	fprintf(outfile,"UINT12_MAX=%"PRIuMAX"\n", (U__Q8_MT)UINT12_MAX);
#endif
#ifdef	INT16_MAX
	fprintf(outfile,"INT16_MIN=%"PRIdMAX"\n", (__Q8_MT)INT16_MIN);
	fprintf(outfile,"INT16_MAX=%"PRIdMAX"\n", (__Q8_MT)INT16_MAX);
	fprintf(outfile,"UINT16_MAX=%"PRIuMAX"\n", (U__Q8_MT)UINT16_MAX);
#endif
#ifdef	INT18_MAX
	fprintf(outfile,"INT18_MIN=%"PRIdMAX"\n", (__Q8_MT)INT18_MIN);
	fprintf(outfile,"INT18_MAX=%"PRIdMAX"\n", (__Q8_MT)INT18_MAX);
	fprintf(outfile,"UINT18_MAX=%"PRIuMAX"\n", (U__Q8_MT)UINT18_MAX);
#endif
#ifdef	INT24_MAX
	fprintf(outfile,"INT24_MIN=%"PRIdMAX"\n", (__Q8_MT)INT24_MIN);
	fprintf(outfile,"INT24_MAX=%"PRIdMAX"\n", (__Q8_MT)INT24_MAX);
	fprintf(outfile,"UINT24_MAX=%"PRIuMAX"\n", (U__Q8_MT)UINT24_MAX);
#endif
#ifdef	INT32_MAX
	fprintf(outfile,"INT32_MIN=%"PRIdMAX"\n", (__Q8_MT)INT32_MIN);
	fprintf(outfile,"INT32_MAX=%"PRIdMAX"\n", (__Q8_MT)INT32_MAX);
	fprintf(outfile,"UINT32_MAX=%"PRIuMAX"\n", (U__Q8_MT)UINT32_MAX);
#endif
#ifdef	INT36_MAX
	fprintf(outfile,"INT36_MIN=%"PRIdMAX"\n", (__Q8_MT)INT36_MIN);
	fprintf(outfile,"INT36_MAX=%"PRIdMAX"\n", (__Q8_MT)INT36_MAX);
	fprintf(outfile,"UINT36_MAX=%"PRIuMAX"\n", (U__Q8_MT)UINT36_MAX);
#endif
#ifdef	INT40_MAX
	fprintf(outfile,"INT40_MIN=%"PRIdMAX"\n", (__Q8_MT)INT40_MIN);
	fprintf(outfile,"INT40_MAX=%"PRIdMAX"\n", (__Q8_MT)INT40_MAX);
	fprintf(outfile,"UINT40_MAX=%"PRIuMAX"\n", (U__Q8_MT)UINT40_MAX);
#endif
#ifdef	INT48_MAX
	fprintf(outfile,"INT48_MIN=%"PRIdMAX"\n", (__Q8_MT)INT48_MIN);
	fprintf(outfile,"INT48_MAX=%"PRIdMAX"\n", (__Q8_MT)INT48_MAX);
	fprintf(outfile,"UINT48_MAX=%"PRIuMAX"\n", (U__Q8_MT)UINT48_MAX);
#endif
#ifdef	INT60_MAX
	fprintf(outfile,"INT60_MIN=%"PRIdMAX"\n", (__Q8_MT)INT60_MIN);
	fprintf(outfile,"INT60_MAX=%"PRIdMAX"\n", (__Q8_MT)INT60_MAX);
	fprintf(outfile,"UINT60_MAX=%"PRIuMAX"\n", (U__Q8_MT)UINT60_MAX);
#endif
#ifdef	INT64_MAX
	fprintf(outfile,"INT64_MIN=%"PRIdMAX"\n", (__Q8_MT)INT64_MIN);
	fprintf(outfile,"INT64_MAX=%"PRIdMAX"\n", (__Q8_MT)INT64_MAX);
	fprintf(outfile,"UINT64_MAX=%"PRIuMAX"\n", (U__Q8_MT)UINT64_MAX);
#endif
#ifdef	INT72_MAX
	fprintf(outfile,"INT72_MIN=%"PRIdMAX"\n", (__Q8_MT)INT72_MIN);
	fprintf(outfile,"INT72_MAX=%"PRIdMAX"\n", (__Q8_MT)INT72_MAX);
	fprintf(outfile,"UINT72_MAX=%"PRIuMAX"\n", (U__Q8_MT)UINT72_MAX);
#endif
#ifdef	INT128_MAX
	fprintf(outfile,"INT128_MIN=%"PRIdMAX"\n", (__Q8_MT)INT128_MIN);
	fprintf(outfile,"INT128_MAX=%"PRIdMAX"\n", (__Q8_MT)INT128_MAX);
	fprintf(outfile,"UINT128_MAX=%"PRIuMAX"\n", (U__Q8_MT)UINT128_MAX);
#endif
	fprintf(outfile,"INT_LEAST8_MIN=%"PRIdMAX"\n", (__Q8_MT)INT_LEAST8_MIN);
	fprintf(outfile,"INT_LEAST8_MAX=%"PRIdMAX"\n", (__Q8_MT)INT_LEAST8_MAX);
	fprintf(outfile,"UINT_LEAST8_MAX=%"PRIuMAX"\n",
		(U__Q8_MT)UINT_LEAST8_MAX);
	fprintf(outfile,"INT_LEAST16_MIN=%"PRIdMAX"\n", (__Q8_MT)INT_LEAST16_MIN);
	fprintf(outfile,"INT_LEAST16_MAX=%"PRIdMAX"\n", (__Q8_MT)INT_LEAST16_MAX);
	fprintf(outfile,"UINT_LEAST16_MAX=%"PRIuMAX"\n",
		(U__Q8_MT)UINT_LEAST16_MAX);
	fprintf(outfile,"INT_LEAST32_MIN=%"PRIdMAX"\n", (__Q8_MT)INT_LEAST32_MIN);
	fprintf(outfile,"INT_LEAST32_MAX=%"PRIdMAX"\n", (__Q8_MT)INT_LEAST32_MAX);
	fprintf(outfile,"UINT_LEAST32_MAX=%"PRIuMAX"\n",
		(U__Q8_MT)UINT_LEAST32_MAX);
#ifdef	INT_LEAST64_MAX
	fprintf(outfile,"INT_LEAST64_MIN=%"PRIdMAX"\n", (__Q8_MT)INT_LEAST64_MIN);
	fprintf(outfile,"INT_LEAST64_MAX=%"PRIdMAX"\n", (__Q8_MT)INT_LEAST64_MAX);
	fprintf(outfile,"UINT_LEAST64_MAX=%"PRIuMAX"\n", (U__Q8_MT)UINT_LEAST64_MAX);
#endif
#ifdef	INT_LEAST128_MAX
	fprintf(outfile,"INT_LEAST128_MIN=%"PRIdMAX"\n", (__Q8_MT)INT_LEAST128_MIN);
	fprintf(outfile,"INT_LEAST128_MAX=%"PRIdMAX"\n", (__Q8_MT)INT_LEAST128_MAX);
	fprintf(outfile,"UINT_LEAST128_MAX=%"PRIuMAX"\n", (U__Q8_MT)UINT_LEAST128_MAX);
#endif
	fprintf(outfile,"INT_FAST8_MIN=%"PRIdMAX"\n", (__Q8_MT)INT_FAST8_MIN);
	fprintf(outfile,"INT_FAST8_MAX=%"PRIdMAX"\n", (__Q8_MT)INT_FAST8_MAX);
	fprintf(outfile,"UINT_FAST8_MAX=%"PRIuMAX"\n",
		(U__Q8_MT)UINT_FAST8_MAX);
	fprintf(outfile,"INT_FAST16_MIN=%"PRIdMAX"\n", (__Q8_MT)INT_FAST16_MIN);
	fprintf(outfile,"INT_FAST16_MAX=%"PRIdMAX"\n", (__Q8_MT)INT_FAST16_MAX);
	fprintf(outfile,"UINT_FAST16_MAX=%"PRIuMAX"\n",
		(U__Q8_MT)UINT_FAST16_MAX);
	fprintf(outfile,"INT_FAST32_MIN=%"PRIdMAX"\n", (__Q8_MT)INT_FAST32_MIN);
	fprintf(outfile,"INT_FAST32_MAX=%"PRIdMAX"\n", (__Q8_MT)INT_FAST32_MAX);
	fprintf(outfile,"UINT_FAST32_MAX=%"PRIuMAX"\n",
		(U__Q8_MT)UINT_FAST32_MAX);
#ifdef	INT_FAST64_MAX
	fprintf(outfile,"INT_FAST64_MIN=%"PRIdMAX"\n", (__Q8_MT)INT_FAST64_MIN);
	fprintf(outfile,"INT_FAST64_MAX=%"PRIdMAX"\n", (__Q8_MT)INT_FAST64_MAX);
	fprintf(outfile,"UINT_FAST64_MAX=%"PRIuMAX"\n", (U__Q8_MT)UINT_FAST64_MAX);
#endif
#ifdef	INT_FAST128_MAX
	fprintf(outfile,"INT_FAST128_MIN=%"PRIdMAX"\n", (__Q8_MT)INT_FAST128_MIN);
	fprintf(outfile,"INT_FAST128_MAX=%"PRIdMAX"\n", (__Q8_MT)INT_FAST128_MAX);
	fprintf(outfile,"UINT_FAST128_MAX=%"PRIuMAX"\n", (U__Q8_MT)UINT_FAST128_MAX);
#endif
#ifdef	INTPTR_MAX
	fprintf(outfile,"INTPTR_MIN=%"PRIdMAX"\n", (__Q8_MT)INTPTR_MIN);
	fprintf(outfile,"INTPTR_MAX=%"PRIdMAX"\n", (__Q8_MT)INTPTR_MAX);
#endif
#ifdef	UINTPTR_MAX
	fprintf(outfile,"UINTPTR_MAX=%"PRIuMAX"\n", (U__Q8_MT)UINTPTR_MAX);
#endif
#ifdef	INTMAX_MAX
	fprintf(outfile,"INTMAX_MIN=%"PRIdMAX"\n", (__Q8_MT)INTMAX_MIN);
	fprintf(outfile,"INTMAX_MAX=%"PRIdMAX"\n", (__Q8_MT)INTMAX_MAX);
	fprintf(outfile,"UINTMAX_MAX=%"PRIuMAX"\n", (U__Q8_MT)UINTMAX_MAX);
#endif
#ifdef	PTRDIFF_MAX
	fprintf(outfile,"PTRDIFF_MIN=%"PRIdMAX"\n", (__Q8_MT)PTRDIFF_MIN);
	fprintf(outfile,"PTRDIFF_MAX=%"PRIdMAX"\n", (__Q8_MT)PTRDIFF_MAX);
#endif
#ifdef	SIG_ATOMIC_MAX
#if	SIG_ATOMIC_MIN < 0
	fprintf(outfile,"SIG_ATOMIC_MIN=%"PRIdMAX"\n", (__Q8_MT)SIG_ATOMIC_MIN);
	fprintf(outfile,"SIG_ATOMIC_MAX=%"PRIdMAX"\n", (__Q8_MT)SIG_ATOMIC_MAX);
#else
	fprintf(outfile,"SIG_ATOMIC_MIN=%"PRIuMAX"\n", (U__Q8_MT)SIG_ATOMIC_MIN);
	fprintf(outfile,"SIG_ATOMIC_MAX=%"PRIuMAX"\n", (U__Q8_MT)SIG_ATOMIC_MAX);
#endif
#endif
#ifdef	SIZE_MAX
	fprintf(outfile,"SIZE_MAX=%"PRIuMAX"\n", (U__Q8_MT)SIZE_MAX);
#endif

#ifdef	WCHAR_MAX
#if	WCHAR_MIN < 0
	fprintf(outfile,"WCHAR_MIN=%"PRIdMAX"\n", (__Q8_MT)WCHAR_MIN);
	fprintf(outfile,"WCHAR_MAX=%"PRIdMAX"\n", (__Q8_MT)WCHAR_MAX);
#else
	fprintf(outfile,"WCHAR_MIN=%"PRIuMAX"\n", (U__Q8_MT)WCHAR_MIN);
	fprintf(outfile,"WCHAR_MAX=%"PRIuMAX"\n", (U__Q8_MT)WCHAR_MAX);
#endif
#endif
#ifdef	WINT_MAX
#if	WINT_MIN < 0
	fprintf(outfile,"WINT_MIN=%"PRIdMAX"\n", (__Q8_MT)WINT_MIN);
	fprintf(outfile,"WINT_MAX=%"PRIdMAX"\n", (__Q8_MT)WINT_MAX);
#else
	fprintf(outfile,"WINT_MIN=%"PRIuMAX"\n", (U__Q8_MT)WINT_MIN);
	fprintf(outfile,"WINT_MAX=%"PRIuMAX"\n", (U__Q8_MT)WINT_MAX);
#endif
#endif

	/*
		7.18.4	Macros for integer constants
	*/

	/* INTn_C for n=8 and 16 were at one point unimplementable
	   on most platforms, so they're treated as "optional": */
#ifdef	INT8_C
	if ( INT8_C(-123) != -123 )
		fprintf(outfile,"*** INT8_C(-123) produced %"PRIdMAX" ***\n",
		       (__Q8_MT)INT8_C(-123)
		      );
	if ( UINT8_C(123) != 123 )
		fprintf(outfile,"*** UINT8_C(123) produced %"PRIuMAX" ***\n",
		       (U__Q8_MT)UINT8_C(123)
		      );
#endif
#ifdef	INT16_C
	if ( INT16_C(-12345) != -12345 )
		fprintf(outfile,"*** INT16_C(-12345) produced %"PRIdMAX" ***\n",
		       (__Q8_MT)INT16_C(-12345)
		      );
	if ( UINT16_C(12345) != 12345 )
		fprintf(outfile,"*** UINT16_C(12345) produced %"PRIuMAX" ***\n",
		       (U__Q8_MT)UINT16_C(12345)
		      );
#endif
	if ( INT32_C(-123456789) != -123456789 )
		fprintf(outfile,"*** INT32_C(-123456789) produced %"PRIdMAX" ***\n",
		       (__Q8_MT)INT32_C(-123456789)
		      );
	if ( UINT32_C(123456789) != 123456789 )
		fprintf(outfile,"*** UINT32_C(123456789) produced %"PRIuMAX" ***\n",
		       (U__Q8_MT)UINT32_C(123456789)
		      );
#ifdef	INT_LEAST64_MAX
	if ( INT64_C(-1234567890123456789) != -1234567890123456789 )
		fprintf(outfile,"*** INT64_C(-1234567890123456789) produced %"PRIdMAX
		       " ***\n",
		       (__Q8_MT)INT64_C(-1234567890123456789)
		      );
	if ( UINT64_C(1234567890123456789) != 1234567890123456789 )
		fprintf(outfile,"*** UINT64_C(1234567890123456789) produced %"PRIuMAX
		       " ***\n",
		       (U__Q8_MT)UINT64_C(1234567890123456789)
		      );
#endif
#ifdef	INTMAX_MAX
	if ( INTMAX_C(-1234567890123456789) != -1234567890123456789 )
		fprintf(outfile,"*** INTMAX_C(-1234567890123456789) produced %"PRIdMAX
		       " ***\n",
		       (__Q8_MT)INTMAX_C(-1234567890123456789)
		      );
	if ( UINTMAX_C(1234567890123456789) != 1234567890123456789 )
		fprintf(outfile,"*** UINTMAX_C(1234567890123456789) produced %"PRIuMAX
		       " ***\n",
		       (U__Q8_MT)UINTMAX_C(1234567890123456789)
		      );
#endif

	/* <inttypes.h> features: */

#if	__STDC_VERSION__ >= 199901
	fprintf(outfile,"sizeof(imaxdiv_t)=%u\n", (unsigned)sizeof(imaxdiv_t));
#endif

	/*
		7.8.1	Macros for format specifiers
	*/

	{
	/* scanf these strings */
	static const char	in_dn[] = "Z119bZ";
	static const char	in_dmo[] = "Z-0119bZ";
	static const char	in_dspx[] = "Z \t\n +0X119bZ";
	static const char	in_dsmx[] = "Z \t\n -0x119bZ";
	static const char	in_dsn[] = "Z \t\n 119bZ";
	static const char	in_dp[] = "Z+119bZ";
	static const char	in_dpx[] = "Z+0X119bz";

	/* sprintf into this */
	static char		buffer[1024];

#if 1

#define	SCAN(buf,fs,var,exp)	if ( sscanf(buf, "Z%" fs, &var) != 1 ) \
					{ \
					fprintf(outfile,"***%s=",fs, STR_SUB(fs) \
					       " failed ***\n" \
					      ); \
					status = EXIT_FAILURE; \
					} \
				else if ( var != (exp) ) \
					{ \
					fprintf(outfile,"***%s=",fs,  STR_SUB(fs) \
					       " should be: " STR_SUB(exp) \
					       ", was: %" fs " ***\n", var \
					      ); \
					status = EXIT_FAILURE; \
					} \
				else	/* for trailing semicolon */

#define	PRINT(fs,var,exp)	if ( sprintf(buffer, "%" fs, var ) <= 0 ) \
					{ \
					fprintf(outfile,"***%s=",fs, STR_SUB(fs) \
					       " failed ***\n" \
					      ); \
					status = EXIT_FAILURE; \
					} \
				else if ( strcmp(buffer, STR_SUB(exp)) != 0 ) \
					{ \
					fprintf(outfile,"***%s=",fs,  STR_SUB(fs) \
					       " should be: " STR_SUB(exp) \
					       ", was: %s ***\n", buffer \
					      ); \
					status = EXIT_FAILURE; \
					} \
				else	/* for trailing semicolon */

#else
								 
#define	SCAN(buf,fs,var,exp)
#define	PRINT(fs,var,exp)

#endif
								 
#ifdef	SCNo32

	SCAN(in_dn, SCNo32, int32, 9);

#endif
#ifdef	PRIo32
	PRINT(PRIo32, int32, 11);
#endif
	SCAN(in_dmo, SCNiLEAST16, intl16, -9);
	SCAN(in_dspx, SCNdLEAST16, intl16, 0);
	SCAN(in_dsmx, SCNiLEAST16, intl16, -4507);
	PRINT(PRIdLEAST16, intl16, -4507);
	PRINT(PRIiLEAST16, intl16, -4507);
	SCAN(in_dsn, SCNxLEAST16, uintl16, 4507);
	PRINT(PRIoLEAST16, uintl16, 10633);
	PRINT(PRIuLEAST16, uintl16, 4507);
	PRINT(PRIxLEAST16, uintl16, 119b);
	PRINT(PRIXLEAST16, uintl16, 119B);
	SCAN(in_dp, SCNxFAST16, uintf16, 4507);
	PRINT(PRIxFAST16, uintf16, 119b);
#ifdef	SCNdMAX
	SCAN(in_dp, SCNdMAX, intmax, 119);
#endif
#ifdef	PRIiMAX
	PRINT(PRIiMAX, intmax, 119);
#endif
#ifdef	SCNoMAX
	SCAN(in_dpx, SCNoMAX, uintmax, 0);
#endif
#ifdef	PRIxMAX
	PRINT(PRIxMAX, uintmax, 0);
#endif
	/* Obviously there should be a much larger battery of such tests. */
	}

#if	defined(INTMAX_MAX)		/* <inttypes.h> has C99 features */
	/*
		7.8.2	Functions for greatest-width integer types
	*/

	{
	static struct
		{
		intmax_t	input;
		intmax_t	expect;
		}	abs_data[] =
		{
#ifdef	INT8_MAX
	  { INT8_MAX,		INT8_MAX,   },
	  { -INT8_MAX,		INT8_MAX,   },
	 { 	UINT8_MAX,		UINT8_MAX,  },
#endif

#if 0
  
#ifdef	INT16_MAX
	 { 	INT16_MAX,		INT16_MAX,  },
	 { 	-INT16_MAX,		INT16_MAX,  },
	 { 	UINT16_MAX,		UINT16_MAX, },
#endif
#ifdef	INT32_MAX
	 { 	INT32_MAX,		INT32_MAX,  },
	 { 	-INT32_MAX,		INT32_MAX,  },
#ifdef	INT_LEAST64_MAX			/* else might support only 32 bits */
	 { 	UINT32_MAX,		UINT32_MAX, },
#endif
#endif
#ifdef	INT64_MAX
	 { 	INT64_MAX,		INT64_MAX,  },
	 { 	-INT64_MAX,		INT64_MAX,  },
#endif
	 { 	INT_LEAST8_MAX,		INT_LEAST8_MAX,      },
	 { 	-INT_LEAST8_MAX,	INT_LEAST8_MAX,      },
	 { 	UINT_LEAST8_MAX,	UINT_LEAST8_MAX,     },
	 { 	INT_LEAST16_MAX,	INT_LEAST16_MAX,     },
	 { 	-INT_LEAST16_MAX,	INT_LEAST16_MAX,     },
	 { 	UINT_LEAST16_MAX,	UINT_LEAST16_MAX,    },
	 { 	INT_LEAST32_MAX,	INT_LEAST32_MAX,     },
	 { 	-INT_LEAST32_MAX,	INT_LEAST32_MAX,     },
#ifdef	INT_LEAST64_MAX
	 { 	UINT_LEAST32_MAX,	UINT_LEAST32_MAX,    },
	 { 	INT_LEAST64_MAX,	INT_LEAST64_MAX,     },
	 { 	-INT_LEAST64_MAX,	INT_LEAST64_MAX,     },
#endif
	 { 	INT_FAST8_MAX,		INT_FAST8_MAX,       },
	 { 	-INT_FAST8_MAX,	INT_FAST8_MAX,           },
	 { 	UINT_FAST8_MAX,	UINT_FAST8_MAX,          },
	 { 	INT_FAST16_MAX,	INT_FAST16_MAX,          },
	 { 	-INT_FAST16_MAX,	INT_FAST16_MAX,      },
	 { 	UINT_FAST16_MAX,	UINT_FAST16_MAX,     },
	 { 	INT_FAST32_MAX,	INT_FAST32_MAX,          },
	 { 	-INT_FAST32_MAX,	INT_FAST32_MAX,      },
#ifdef	INT_FAST64_MAX
	 { 	UINT_FAST32_MAX,	UINT_FAST32_MAX,     },
	 { 	INT_FAST64_MAX,	INT_FAST64_MAX,          },
	 { 	-INT_FAST64_MAX,	INT_FAST64_MAX,      },
#endif
#ifdef	INTPTR_MAX
	 { 	INTPTR_MAX,		INTPTR_MAX,              },
	 { 	-INTPTR_MAX,		INTPTR_MAX,          },
#endif
#ifdef	UINTPTR_MAX
	 { 	UINTPTR_MAX,		UINTPTR_MAX,         },
#endif
	 { 	INTMAX_MAX,		INTMAX_MAX,              },
#ifdef	PTRDIFF_MAX
	 { 	PTRDIFF_MAX,		PTRDIFF_MAX,         },
#endif
#ifdef	SIG_ATOMIC_MAX
	 { 	SIG_ATOMIC_MAX,		SIG_ATOMIC_MAX,      },
#if	SIG_ATOMIC_MIN < 0
	 { 	-SIG_ATOMIC_MAX,	SIG_ATOMIC_MAX,      },
#endif
#endif
#ifdef	SIZE_MAX
	 { 	SIZE_MAX,		SIZE_MAX,                },
#endif
#ifdef	WCHAR_MAX
	 { 	WCHAR_MAX,		WCHAR_MAX,               },
#if	WCHAR_MIN < 0
	 { 	-WCHAR_MAX,		WCHAR_MAX,               },
#endif
#endif
#ifdef	WINT_MAX
	 { 	WINT_MAX,		WINT_MAX,                },
#if	WINT_MIN < 0
	 {  -WINT_MAX,		WINT_MAX,                },
#endif
#endif
	 { 	127,				127,                 },
	 { 	-127,				127,                 },
	 { 	128,				128,                 },
	 { 	-127-1,				128,                 },
	 { 	255,				255,                 },
	 { 	-256+1,				255,                 },
	 { 	256,				256,                 },
	 { 	-256,				256,                 },
	 { 	32767,				32767,               },
	 { 	-32767,				32767,               },
	 { 	32768,				32768,               },
	 { 	-32767-1,			32768,               },
	 { 	65535,				65535,               },
	 { 	-65536+1,			65535,               },
	 { 	65536,				65536,               },
	 { 	-65536,				65536,               },
	 { 	2147483647,			2147483647,          },
	 { 	-2147483647,			2147483647,      },
	 { 	2147483648,			2147483648,          },
	 { 	-2147483647-1,			2147483648,      },
#ifdef	INT_LEAST64_MAX			/* else might support only 32 bits */
	 { 	4294967295,			4294967295,          },
	 { 	-4294967296+1,			4294967295,      },
	 { 	4294967296,			4294967296,          },
	 { 	-4294967296,			4294967296,      },
	 { 	9223372036854775807,		9223372036854775807,    },
	 { 	-9223372036854775807,		9223372036854775807,    },
	 { 	1234567890123456789,		1234567890123456789,    },
	 { 	-1234567890123456789,		1234567890123456789,    },
#endif
	 { 	1,				1,                                  },
	 { 	-1,				1,                                  },
	 { 	2,				2,                                  },
	 { 	-2,				2,                                  },
	 { 	10,				10,                                 },
	 { 	-10,				10,                             },
	 { 	16,				16,                                 },
	 { 	-16,				16,                             },
#endif
		/* Other test cases can be added here. */
	 { 	0,		0	/* terminates the list */              },
		},	*adp = abs_data;

	do	{
		if ( (intmax = imaxabs(adp->input)) != adp->expect )
			{
			fprintf(outfile,"*** imaxabs(%"PRIdMAX") failed; should be: %"
			       PRIdMAX", was: %"PRIdMAX" ***\n",
			       adp->input, adp->expect, intmax
			      );
			status = EXIT_FAILURE;
			}
//		} while ( adp++->input != 0 );
		} while ( (adp++)->input != 0 );
	}

	{
	imaxdiv_t	result;
	static struct
		{
		intmax_t	numer;
		intmax_t	denom;
		intmax_t	exp_quot;
		intmax_t	exp_rem;
		}	div_data[] =
		{
	{	0, 1,				0, 0,   },
#if 0
	{	0, -1,				0, 0,   },
	{	0, 2,				0, 0,   },
	{	0, -2,				0, 0,   },
	{	0, 5,				0, 0,   },
	{	0, -5,				0, 0,   },
	{	1, 1,				1, 0,   },
	{	1, -1,				-1, 0,  },
	{	1, 2,				0, 1,   },
	{	1, -2,				0, 1,   },
	{	1, 5,				0, 1,   },
	{	1, -5,				0, 1,   },
	{	-1, 1,				-1, 0,  },
	{	-1, -1,				1, 0,   },
	{	-1, 2,				0, -1,  },
	{	-1, -2,				0, -1,  },
	{	-1, 5,				0, -1,  },
	{	-1, -5,				0, -1,  },
	{	2, 1,				2, 0,   },
	{	2, -1,				-2, 0,  },
	{	2, 2,				1, 0,   },
	{	2, -2,				-1, 0,  },
	{	2, 5,				0, 2,   },
	{	2, -5,				0, 2,   },
	{	-2, 1,				-2, 0,  },
	{	-2, -1,				2, 0,   },
	{	-2, 2,				-1, 0,  },
	{	-2, -2,				1, 0,   },
	{	-2, 5,				0, -2,  },
	{	-2, -5,				0, -2,  },
	{	17, 5,				3, 2,   },
	{	-17, -5,			3, -2,  },
	{	17, -5,				-3, 2,  },
	{	-17, 5,				-3, -2, },
	{	2147483647, 1,			2147483647, 0,         },
	{	-2147483647, 1,			-2147483647, 0,        },
	{	2147483648, 1,			2147483648, 0,         },
	{	-2147483647-1, 1,		-2147483647-1, 0,      },
	{	2147483647, 2,			1073741823, 1,         },
	{	-2147483647, 2,			-1073741823, -1,       },
	{	2147483648, 2,			1073741824, 0,         },
	{	-2147483647-1, 2,		-1073741824, 0,        },
#ifdef	INT_LEAST64_MAX			/* else might support only 32 bits */
	{	4294967295, 1,			4294967295, 0,         },
	{	-4294967296+1, 1,		-4294967296+1, 0,      },
	{	4294967296, 1,			4294967296, 0,         },
	{	-4294967296, 1,			-4294967296, 0,        },
	{	4294967295, -1,			-4294967296+1, 0,      },
	{	-4294967296+1, -1,		4294967295, 0,         },
	{	4294967296, -1,			-4294967296, 0,        },
	{	-4294967296, -1,		4294967296, 0,         },
	{	4294967295, 2,			2147483647, 1,         },
	{	-4294967296+1, 2,		-2147483647, -1,       },
	{	4294967296, 2,			2147483648, 0,         },
	{	-4294967296, 2,			-2147483647-1, 0,      },
	{	4294967295, 2147483647,		2, 1,              },
	{	-4294967296+1, 2147483647,	-2, -1,            },
	{	4294967296, 2147483647,		2, 2,              },
	{	-4294967296, 2147483647,	-2, -2,            },
	{	4294967295, -2147483647,	-2, 1,             },
	{	-4294967296+1, -2147483647,	2, -1,             },
	{	4294967296, -2147483647,	-2, 2,             },
	{	-4294967296, -2147483647,	2, -2,             },
	{	4294967295, 2147483648,		1, 2147483647,     },
	{	-4294967296+1, 2147483648,	-1, -2147483647,   },
	{	4294967296, 2147483648,		2, 0,              },
	{	-4294967296, 2147483648,	-2, 0,             },
	{	4294967295, -2147483647-1,	-1, 2147483647,    },
	{	-4294967296+1, -2147483647-1,	1, -2147483647,},
	{	4294967296, -2147483647-1,	-2, 0,             },
	{	-4294967296, -2147483647-1,	2, 0,              },
	{	9223372036854775807, 1,		9223372036854775807, 0,         },
	{	-9223372036854775807, 1,	-9223372036854775807, 0,        },
	{	9223372036854775807, 2,		4611686018427387903, 1,         },
	{	-9223372036854775807, 2,	-4611686018427387903, -1,       },
#endif
#endif
		/* There should be a much larger battery of such tests. */
	{	0, 0,		0, 0 },	/* 0 denom terminates the list */
		},	*ddp;

#if 0
	for ( ddp = div_data; ddp->denom != 0; ++ddp )
		if ( (result = imaxdiv(ddp->numer, ddp->denom)).quot
		     != ddp->exp_quot || result.rem != ddp->exp_rem
		   )	{
//			fprintf(outfile,"*** imaxdiv(%"PRIdMAX",%"PRIdMAX
//			       ") failed; should be: (%"PRIdMAX",%"PRIdMAX
//			       "), was: (%"PRIdMAX",%"PRIdMAX") ***\n",
//			       ddp->numer, ddp->denom, ddp->exp_quot,
//			       ddp->exp_rem, result.quot, result.rem
//			      );
			fprintf(outfile,"err:imaxdiv(%"PRIdMAX",%"PRIdMAX
			       ") = (%"PRIdMAX",%"PRIdMAX
			       "), is: (%"PRIdMAX",%"PRIdMAX")\n",
			       ddp->numer, ddp->denom, ddp->exp_quot,
			       ddp->exp_rem, result.quot, result.rem
			      );
			status = EXIT_FAILURE;
			}
#endif
	}
	
	{
	char		*endptr;
	wchar_t		*wendptr;
	static char	saved[64];	/* holds copy of input string */
	static wchar_t	wnptr[64];	/* holds wide copy of test string */
	static int	warned;		/* "warned for null endptr" flag */
	register int	i;
	static struct
		{
		char *		nptr;
		int		base;
		intmax_t	exp_val;
		int		exp_len;
		}	str_data[] =
		{
	{	"", 0,				0, 0,      },
	{	"", 2,				0, 0,      },
	{	"", 8,				0, 0,      },
	{	"", 9,				0, 0,      },
	{	"", 10,				0, 0,      },
	{	"", 16,				0, 0,      },
	{	"", 36,				0, 0,      },
	{	"0", 0,				0, 1,      },
	{	"0", 2,				0, 1,      },
	{	"0", 8,				0, 1,      },
	{	"0", 9,				0, 1,      },
	{	"0", 10,			0, 1,      },
	{	"0", 16,			0, 1,      },
	{	"0", 36,			0, 1,      },
	{	"+0", 0,			0, 2,      },
	{	"+0", 2,			0, 2,      },
	{	"+0", 8,			0, 2,      },
	{	"+0", 9,			0, 2,      },
	{	"+0", 10,			0, 2,      },
	{	"+0", 16,			0, 2,      },
	{	"+0", 36,			0, 2,      },
	{	"-0", 0,			0, 2,      },
	{	"-0", 2,			0, 2,      },
	{	"-0", 8,			0, 2,      },
	{	"-0", 9,			0, 2,      },
	{	"-0", 10,			0, 2,      },
	{	"-0", 16,			0, 2,      },
	{	"-0", 36,			0, 2,      },
	{	"Inf", 0,			0, 0,      },
	{	"Inf", 2,			0, 0,      },
	{	"Inf", 8,			0, 0,      },
	{	"Inf", 9,			0, 0,      },
	{	"Inf", 10,			0, 0,      },
	{	"Inf", 16,			0, 0,      },
	{	"Inf", 36,			24171, 3,  },
	{	"+Inf", 0,			0, 0,      },
	{	"+Inf", 2,			0, 0,      },
	{	"+Inf", 8,			0, 0,      },
	{	"+Inf", 9,			0, 0,      },
	{	"+Inf", 10,			0, 0,      },
	{	"+Inf", 16,			0, 0,      },
	{	"+Inf", 36,			24171, 4,  },
	{	"-Inf", 0,			0, 0,      },
	{	"-Inf", 2,			0, 0,      },
	{	"-Inf", 8,			0, 0,      },
	{	"-Inf", 9,			0, 0,      },
	{	"-Inf", 10,			0, 0,      },
	{	"-Inf", 16,			0, 0,      },
	{	"-Inf", 36,			-24171, 4, },
	{	"inf", 0,			0, 0,      },
	{	"inf", 2,			0, 0,      },
	{	"inf", 8,			0, 0,      },
	{	"inf", 9,			0, 0,      },
	{	"inf", 10,			0, 0,      },
	{	"inf", 16,			0, 0,      },
	{	"inf", 36,			24171, 3,  },
	{	"+inf", 0,			0, 0,      },
	{	"+inf", 2,			0, 0,      },
	{	"+inf", 8,			0, 0,      },
	{	"+inf", 9,			0, 0,      },
	{	"+inf", 10,			0, 0,      },
	{	"+inf", 16,			0, 0,      },
	{	"+inf", 36,			24171, 4,  },
	{	"-inf", 0,			0, 0,      },
	{	"-inf", 2,			0, 0,      },
	{	"-inf", 8,			0, 0,      },
	{	"-inf", 9,			0, 0,      },
	{	"-inf", 10,			0, 0,      },
	{	"-inf", 16,			0, 0,      },
	{	"-inf", 36,			-24171, 4, },
	{	"119b8Z", 0,			119, 3,         },
	{	"119bZ", 0,			119, 3,             },
	{	"-0119bZ", 0,			-9, 4,          },
	{	" \t\n 0X119bZ", 0,		4507, 10,       },
	{	" \t\n +0X119bZ", 0,		4507, 11,   },
	{	" \t\n -0x119bZ", 0,		-4507, 11,  },
	{	" \t\n 119bZ", 0,		119, 7,         },
	{	"+119bZ", 0,			119, 4,         },
	{	"+0X119bz", 0,			4507, 7,        },
	{	"119b8Z", 2,			3, 2,           },
	{	"119bZ", 2,			3, 2,               },
	{	"-0119bZ", 2,			-3, 4,          },
	{	" \t\n 0X119bZ", 2,		0, 5,           },
	{	" \t\n +0X119bZ", 2,		0, 6,       },
	{	" \t\n -0x119bZ", 2,		0, 6,       },
	{	" \t\n 119bZ", 2,		3, 6,           },
	{	"+119bZ", 2,			3, 3,           },
	{	"+0X119bz", 2,			0, 2,           },
	{	"119b8Z", 8,			9, 2,           },
	{	"119bZ", 8,			9, 2,               },
	{	"-0119bZ", 8,			-9, 4,          },
	{	" \t\n 0X119bZ", 8,		0, 5,           },
	{	" \t\n +0X119bZ", 8,		0, 6,       },
	{	" \t\n -0x119bZ", 8,		0, 6,       },
	{	" \t\n 119bZ", 8,		9, 6,           },
	{	"+119bZ", 8,			9, 3,           },
	{	"+0X119bz", 8,			0, 2,           },
	{	"119b8Z", 9,			10, 2,          },
	{	"119bZ", 9,			10, 2,              },
	{	"-0119bZ", 9,			-10, 4,         },
	{	" \t\n 0X119bZ", 9,		0, 5,           },
	{	" \t\n +0X119bZ", 9,		0, 6,       },
	{	" \t\n -0x119bZ", 9,		0, 6,       },
	{	" \t\n 119bZ", 9,		10, 6,          },
	{	"+119bZ", 9,			10, 3,          },
	{	"+0X119bz", 9,			0, 2,           },
	{	"119b8Z", 10,			119, 3,         },
	{	"119bZ", 10,			119, 3,         },
	{	"-0119bZ", 10,			-119, 5,        },
	{	" \t\n 0X119bZ", 10,		0, 5,       },
	{	" \t\n +0X119bZ", 10,		0, 6,       },
	{	" \t\n -0x119bZ", 10,		0, 6,       },
	{	" \t\n 119bZ", 10,		119, 7,         },
	{	"+119bZ", 10,			119, 4,         },
	{	"+0X119bz", 10,			0, 2,           },
	{	"119b8Z", 16,			72120, 5,       },
	{	"119bZ", 16,			4507, 4,        },
	{	"-0119bZ", 16,			-4507, 6,       },
	{	" \t\n 0X119bZ", 16,		4507, 10,   },
	{	" \t\n +0X119bZ", 16,		4507, 11,   },
	{	" \t\n -0x119bZ", 16,		-4507, 11,  },
	{	" \t\n 119bZ", 16,		4507,8,         },
	{	"+119bZ", 16,			4507, 5,        },
	{	"+0X119bz", 16,			4507, 7,        },
	{	"119b8Z", 36,			62580275, 6,    },
	{	"119bZ", 36,			1738367, 5,     },
	{	"-0119bZ", 36,			-1738367, 7,                 },
	{	" \t\n 0X119bZ", 36,		1997122175, 11,          },
	{	" \t\n +0X119bZ", 36,		1997122175, 12,          },
	{	" \t\n -0x119bZ", 36,		-1997122175, 12,         },
	{	" \t\n 119bZ", 36,		1738367, 9,                  },
	{	"+119bZ", 36,			1738367, 6,                  },
	{	"+0X119bz", 36,			1997122175, 8,               },
	 	/* There should be a much larger battery of such tests. */
	{	"127", 0,			127, 3,                          },
	{	"-127", 0,			-127, 4,                         },
	{	"128", 0,			128, 3,                          },
	{	"-128", 0,			-127-1, 4,                       },
	{	"255", 0,			255, 3,                          },
	{	"-255", 0,			-255, 4,                         },
	{	"256", 0,			256, 3,                          },
	{	"-256", 0,			-255-1, 4,                       },
	{	"32767", 0,			32767, 5,                        },
	{	"-32767", 0,			-32767, 6,                   },
	{	"32768", 0,			32768, 5,                        },
	{	"-32768", 0,			-32767-1, 6,                 },
	{	"65535", 0,			65535, 5,                        },
	{	"-65535", 0,			-65536+1, 6,                 },
	{	"65536", 0,			65536, 5,                        },
	{	"-65536", 0,			-65536, 6,                   },
	{	"2147483647", 0,		2147483647, 10,              },
	{	"-2147483647", 0,		-2147483647, 11,             },
	{	"2147483648", 0,		2147483648, 10,              },
	{	"-2147483648", 0,		-2147483647-1, 11,           },
	{	"4294967295", 0,		4294967295, 10,              },
	{	"-4294967295", 0,		-4294967296+1, 11,           },
	{	"4294967296", 0,		4294967296, 10,              },
	{	"-4294967296", 0,		-4294967296, 11,                        },
	{	"9223372036854775807", 0,	9223372036854775807, 19,            },
	{	"-9223372036854775807", 0,	-9223372036854775807, 20,           },
	{	"1234567890123456789", 0,	1234567890123456789, 19,            },
	{	"-1234567890123456789", 0,	-1234567890123456789, 20,           },
	{	"1", 0,				1, 1,                                       },
	{	"-1", 0,			-1, 2,                                      },
	{	"2", 0,				2, 1,                                       },
	{	"-2", 0,			-2, 2,                                      },
	{	"10", 0,			10, 2,                                      },
	{	"-10", 0,			-10, 3,                                     },
	{	"16", 0,			16, 2,                                      },
	{	"-16", 0,			-16, 3,                                     },
		/* Other test cases can be added here. */
	{	NULL, 0,	0, 0 },	/* terminates the list */
		},	*sdp;

	for ( sdp = str_data; sdp->nptr != NULL ; ++sdp )
		{
		/*
			7.8.2.3	The strtoimax and strtoumax functions
		*/

		strcpy(saved, sdp->nptr);

		errno = 0;		/* shouldn't be changed */

		if ( (intmax = strtoimax(sdp->nptr, &endptr, sdp->base))
		  != sdp->exp_val
		   )	{
			int	save = errno;

			fprintf(outfile,"*** strtoimax(%s,,%d) failed; should be: %"
			       PRIdMAX", was: %"PRIdMAX" ***\n", sdp->nptr,
			       sdp->base, sdp->exp_val, intmax
			      );
			status = EXIT_FAILURE;
			errno = save;
			}
		else if ( endptr != sdp->nptr + sdp->exp_len )
			{
			int	save = errno;

			fprintf(outfile,"*** strtoimax(%s,,%d) returned wrong endptr"
			       " ***\n", sdp->nptr, sdp->base
			      );
			status = EXIT_FAILURE;
			errno = save;
			}

		if ( errno != 0 )
			{
			fprintf(outfile,"*** strtoimax modified errno ***\n");
			status = EXIT_FAILURE;
			}

		if ( strcmp(sdp->nptr, saved) != 0 )
			{
			fprintf(outfile,"*** strtoimax modified its input ***\n");
			status = EXIT_FAILURE;
			strcpy(saved, sdp->nptr);
			}

		if ( sdp->exp_val >= 0 )	/* else some sign extension */
			{
			errno = 0;	/* shouldn't be changed */

			if ( (uintmax = strtoumax(sdp->nptr, &endptr, sdp->base
						 )
			     ) != sdp->exp_val
			   )	{
				int	save = errno;

				fprintf(outfile,"*** strtoumax(%s,,%d) failed; "
				       "should be: %"PRIuMAX", was: %"PRIuMAX
				       " ***\n", sdp->nptr, sdp->base,
				       sdp->exp_val, uintmax
				      );
				status = EXIT_FAILURE;
				errno = save;
				}
			else if ( endptr != sdp->nptr + sdp->exp_len )
				{
				int	save = errno;

				fprintf(outfile,"*** strtoumax(%s,,%d) returned wrong "
				       "endptr ***\n", sdp->nptr, sdp->base
				      );
				status = EXIT_FAILURE;
				errno = save;
				}

			if ( errno != 0 )
				{
				fprintf(outfile,"*** strtoumax modified errno ***\n");
				status = EXIT_FAILURE;
				}

			if ( strcmp(sdp->nptr, saved) != 0 )
				{
				fprintf(outfile,"*** strtoumax"
				       " modified its input ***\n"
				      );
				status = EXIT_FAILURE;
				strcpy(saved, sdp->nptr);
				}
			}

		/* tests for null endptr */

#define	WARN()	if (!warned) warned = 1, fprintf(outfile,"*** Using null endptr: ***\n")

		warned = 0;
		errno = 0;		/* shouldn't be changed */

		if ( (intmax = strtoimax(sdp->nptr, (char **)NULL, sdp->base))
		  != sdp->exp_val
		   )	{
			int	save = errno;

			WARN();
			fprintf(outfile,"*** strtoimax(%s,NULL,%d) failed; "
			       "should be: %"PRIdMAX", was: %"PRIdMAX" ***\n",
			       sdp->nptr, sdp->base, sdp->exp_val, intmax
			      );
			status = EXIT_FAILURE;
			errno = save;
			}

		if ( errno != 0 )
			{
			WARN();
			fprintf(outfile,"*** strtoimax modified errno ***\n");
			status = EXIT_FAILURE;
			}

		if ( strcmp(sdp->nptr, saved) != 0 )
			{
			WARN();
			fprintf(outfile,"*** strtoimax modified its input ***\n");
			status = EXIT_FAILURE;
			strcpy(saved, sdp->nptr);
			}

		if ( sdp->exp_val >= 0 )	/* else some sign extension */
			{
			errno = 0;	/* shouldn't be changed */

			if ( (uintmax = strtoumax(sdp->nptr, (char **)NULL,
						  sdp->base
						 )
			     ) != sdp->exp_val
			   )	{
				int	save = errno;

				WARN();
				fprintf(outfile,"*** strtoumax(%s,NULL,%d) failed; "
				       "should be: %"PRIuMAX", was: %"PRIuMAX
				       " ***\n", sdp->nptr, sdp->base,
				       sdp->exp_val, uintmax
				      );
				status = EXIT_FAILURE;
				errno = save;
				}

			 if ( errno != 0 )
				{
				WARN();
				fprintf(outfile,"*** strtoumax modified errno ***\n");
				status = EXIT_FAILURE;
				}

			 if ( strcmp(sdp->nptr, saved) != 0 )
				{
				WARN();
				fprintf(outfile,"*** strtoumax"
				       " modified its input ***\n"
				      );
				status = EXIT_FAILURE;
				strcpy(saved, sdp->nptr);
				}
			}

		/*
			7.8.2.4	The wcstoimax and wcstoumax functions
		*/

		for ( i = 0; i < 64; ++i )
			if ( (wnptr[i] = sdp->nptr[i]) == '\0' )
				break;

		errno = 0;		/* shouldn't be changed */

		if ( (intmax = wcstoimax(wnptr, &wendptr, sdp->base))
		  != sdp->exp_val
		   )	{
			int	save = errno;

			fprintf(outfile,"*** wcstoimax(%s,,%d) failed; should be: %"
			       PRIdMAX", was: %"PRIdMAX" ***\n", sdp->nptr,
			       sdp->base, sdp->exp_val, intmax
			      );
			status = EXIT_FAILURE;
			errno = save;
			}
		else if ( wendptr != wnptr + sdp->exp_len )
			{
			int	save = errno;

			fprintf(outfile,"*** wcstoimax(%s,,%d) returned wrong endptr"
			       " ***\n", sdp->nptr, sdp->base
			      );
			status = EXIT_FAILURE;
			errno = save;
			}

		if ( errno != 0 )
			{
			fprintf(outfile,"*** wcstoimax modified errno ***\n");
			status = EXIT_FAILURE;
			}

		for ( i = 0; i < 64; ++i )
			if ( wnptr[i] != sdp->nptr[i] )
				{
				fprintf(outfile,"*** wcstoimax modified its input ***\n"
				      );
				status = EXIT_FAILURE;

				for ( ; i < 64; ++i )
					if ( (wnptr[i] = sdp->nptr[i]) == '\0' )
						break;

				break;
				}
			else if ( wnptr[i] == '\0' )
				break;

		if ( sdp->exp_val >= 0 )	/* else some sign extension */
			{
			errno = 0;	/* shouldn't be changed */

			if ( (uintmax = wcstoumax(wnptr, &wendptr, sdp->base)
			     ) != sdp->exp_val
			   )	{
				int	save = errno;

				fprintf(outfile,"*** wcstoumax(%s,,%d) failed; "
				       "should be: %"PRIuMAX", was: %"PRIuMAX
				       " ***\n", sdp->nptr, sdp->base,
				       sdp->exp_val, uintmax
				      );
				status = EXIT_FAILURE;
				errno = save;
				}
			else if ( wendptr != wnptr + sdp->exp_len )
				{
				int	save = errno;

				fprintf(outfile,"*** wcstoumax(%s,,%d) returned wrong "
				       "endptr ***\n", sdp->nptr, sdp->base
				      );
				status = EXIT_FAILURE;
				errno = save;
				}

			if ( errno != 0 )
				{
				fprintf(outfile,"*** wcstoumax modified errno ***\n");
				status = EXIT_FAILURE;
				}

			for ( i = 0; i < 64; ++i )
				if ( wnptr[i] != sdp->nptr[i] )
					{
					fprintf(outfile,"*** wcstoumax"
					       " modified its input ***\n"
					      );
					status = EXIT_FAILURE;

					for ( ; i < 64; ++i )
						if ( (wnptr[i] = sdp->nptr[i])
						  == '\0'
						   )
							break;

					break;
					}
				else if ( wnptr[i] == '\0' )
					break;
			}

		/* tests for null endptr */

		warned = 0;
		errno = 0;		/* shouldn't be changed */

		if ( (intmax = wcstoimax(wnptr, (wchar_t **)NULL, sdp->base))
		  != sdp->exp_val
		   )	{
			int	save = errno;

			WARN();
			fprintf(outfile,"*** wcstoimax(%s,NULL,%d) failed; should be: %"
			       PRIdMAX", was: %"PRIdMAX" ***\n", sdp->nptr,
			       sdp->base, sdp->exp_val, intmax
			      );
			status = EXIT_FAILURE;
			errno = save;
			}

		if ( errno != 0 )
			{
			WARN();
			fprintf(outfile,"*** wcstoimax modified errno ***\n");
			status = EXIT_FAILURE;
			}

		for ( i = 0; i < 64; ++i )
			if ( wnptr[i] != sdp->nptr[i] )
				{
				WARN();
				fprintf(outfile,"*** wcstoimax modified its input ***\n"
				      );
				status = EXIT_FAILURE;

				for ( ; i < 64; ++i )
					if ( (wnptr[i] = sdp->nptr[i])
					  == '\0'
					   )
						break;

				break;
				}
			else if ( wnptr[i] == '\0' )
				break;

		if ( sdp->exp_val >= 0 )	/* else some sign extension */
			{
			errno = 0;	/* shouldn't be changed */

			if ( (uintmax = wcstoumax(wnptr, (wchar_t **)NULL,
						  sdp->base
						 )
			     ) != sdp->exp_val
			   )	{
				int	save = errno;

				WARN();
				fprintf(outfile,"*** wcstoumax(%s,NULL,%d) failed; "
				       "should be: %"PRIuMAX", was: %"PRIuMAX
				       " ***\n", sdp->nptr, sdp->base,
				       sdp->exp_val, uintmax
				      );
				status = EXIT_FAILURE;
				errno = save;
				}

			 if ( errno != 0 )
				{
				WARN();
				fprintf(outfile,"*** wcstoumax modified errno ***\n");
				status = EXIT_FAILURE;
				}

			for ( i = 0; i < 64; ++i )
				if ( wnptr[i] != sdp->nptr[i] )
					{
					WARN();
					fprintf(outfile,"*** wcstoumax"
					       " modified its input ***\n"
					      );
					status = EXIT_FAILURE;

					for ( ; i < 64; ++i )
						if ( (wnptr[i] = sdp->nptr[i])
						  == '\0'
						   )
							break;

					break;
					}
				else if ( wnptr[i] == '\0' )
					break;
			}
		}

	/*
		7.8.2.3	The strtoimax and strtoumax functions (continued)
	*/

	if ( (intmax = strtoimax("1234567890123456789012345678901234567890"
				 "1234567890123456789012345678901234567890"
				 "1234567890123456789012345678901234567890"
				 "1234567890123456789012345678901234567890"
				 "1234567890123456789012345678901234567890"
				 "1234567890123456789012345678901234567890",
				 &endptr, 0
				)
	     ) != INTMAX_MAX || errno != ERANGE
	   )	{
		fprintf(outfile,"*** strtoimax failed overflow test ***\n");
		status = EXIT_FAILURE;
		}

	if ( (intmax = strtoimax("+1234567890123456789012345678901234567890"
				 "1234567890123456789012345678901234567890"
				 "1234567890123456789012345678901234567890"
				 "1234567890123456789012345678901234567890"
				 "1234567890123456789012345678901234567890"
				 "1234567890123456789012345678901234567890",
				 &endptr, 0
				)
	     ) != INTMAX_MAX || errno != ERANGE
	   )	{
		fprintf(outfile,"*** strtoimax failed +overflow test ***\n");
		status = EXIT_FAILURE;
		}

	if ( (intmax = strtoimax("-1234567890123456789012345678901234567890"
				 "1234567890123456789012345678901234567890"
				 "1234567890123456789012345678901234567890"
				 "1234567890123456789012345678901234567890"
				 "1234567890123456789012345678901234567890"
				 "1234567890123456789012345678901234567890",
				 &endptr, 0
				)
	     ) != INTMAX_MIN || errno != ERANGE
	   )	{
		fprintf(outfile,"*** strtoimax failed -overflow test ***\n");
		status = EXIT_FAILURE;
		}

	if ( (uintmax = strtoumax("1234567890123456789012345678901234567890"
				 "1234567890123456789012345678901234567890"
				 "1234567890123456789012345678901234567890"
				 "1234567890123456789012345678901234567890"
				 "1234567890123456789012345678901234567890"
				 "1234567890123456789012345678901234567890",
				 &endptr, 0
				)
	     ) != UINTMAX_MAX || errno != ERANGE
	   )	{
		fprintf(outfile,"*** strtoumax failed overflow test ***\n");
		status = EXIT_FAILURE;
		}

	if ( (uintmax = strtoumax("+1234567890123456789012345678901234567890"
				 "1234567890123456789012345678901234567890"
				 "1234567890123456789012345678901234567890"
				 "1234567890123456789012345678901234567890"
				 "1234567890123456789012345678901234567890"
				 "1234567890123456789012345678901234567890",
				 &endptr, 0
				)
	     ) != UINTMAX_MAX || errno != ERANGE
	   )	{
		fprintf(outfile,"*** strtoumax failed +overflow test ***\n");
		status = EXIT_FAILURE;
		}

	if ( (uintmax = strtoumax("-1234567890123456789012345678901234567890"
				 "1234567890123456789012345678901234567890"
				 "1234567890123456789012345678901234567890"
				 "1234567890123456789012345678901234567890"
				 "1234567890123456789012345678901234567890"
				 "1234567890123456789012345678901234567890",
				 &endptr, 0
				)
	     ) != UINTMAX_MAX || errno != ERANGE
	   )	{
		fprintf(outfile,"*** strtoumax failed -overflow test ***\n");
		status = EXIT_FAILURE;
		}

	/*
		7.8.2.4	The wcstoimax and wcstoumax functions (continued)
	*/

#ifdef NO_INTERNAL_WCHAR
		fprintf(outfile,"NO_INTERNAL_WCHAR\n");
#else

	if ( (intmax = wcstoimax(L"1234567890123456789012345678901234567890"
				 L"1234567890123456789012345678901234567890"
				 L"1234567890123456789012345678901234567890"
				 L"1234567890123456789012345678901234567890"
				 L"1234567890123456789012345678901234567890"
				 L"1234567890123456789012345678901234567890",
				 &wendptr, 0
				)
	     ) != INTMAX_MAX || errno != ERANGE
	   )	{
		fprintf(outfile,"*** wcstoimax failed overflow test ***\n");
		status = EXIT_FAILURE;
		}

	if ( (intmax = wcstoimax(L"+1234567890123456789012345678901234567890"
				 L"1234567890123456789012345678901234567890"
				 L"1234567890123456789012345678901234567890"
				 L"1234567890123456789012345678901234567890"
				 L"1234567890123456789012345678901234567890"
				 L"1234567890123456789012345678901234567890",
				 &wendptr, 0
				)
	     ) != INTMAX_MAX || errno != ERANGE
	   )	{
		fprintf(outfile,"*** wcstoimax failed +overflow test ***\n");
		status = EXIT_FAILURE;
		}

	if ( (intmax = wcstoimax(L"-1234567890123456789012345678901234567890"
				 L"1234567890123456789012345678901234567890"
				 L"1234567890123456789012345678901234567890"
				 L"1234567890123456789012345678901234567890"
				 L"1234567890123456789012345678901234567890"
				 L"1234567890123456789012345678901234567890",
				 &wendptr, 0
				)
	     ) != INTMAX_MIN || errno != ERANGE
	   )	{
		fprintf(outfile,"*** wcstoimax failed -overflow test ***\n");
		status = EXIT_FAILURE;
		}

	if ( (uintmax = wcstoumax(L"1234567890123456789012345678901234567890"
				 L"1234567890123456789012345678901234567890"
				 L"1234567890123456789012345678901234567890"
				 L"1234567890123456789012345678901234567890"
				 L"1234567890123456789012345678901234567890"
				 L"1234567890123456789012345678901234567890",
				 &wendptr, 0
				)
	     ) != UINTMAX_MAX || errno != ERANGE
	   )	{
		fprintf(outfile,"*** wcstoumax failed overflow test ***\n");
		status = EXIT_FAILURE;
		}

	if ( (uintmax = wcstoumax(L"+1234567890123456789012345678901234567890"
				 L"1234567890123456789012345678901234567890"
				 L"1234567890123456789012345678901234567890"
				 L"1234567890123456789012345678901234567890"
				 L"1234567890123456789012345678901234567890"
				 L"1234567890123456789012345678901234567890",
				 &wendptr, 0
				)
	     ) != UINTMAX_MAX || errno != ERANGE
	   )	{
		fprintf(outfile,"*** wcstoumax failed +overflow test ***\n");
		status = EXIT_FAILURE;
		}

	if ( (uintmax = wcstoumax(L"-1234567890123456789012345678901234567890"
				 L"1234567890123456789012345678901234567890"
				 L"1234567890123456789012345678901234567890"
				 L"1234567890123456789012345678901234567890"
				 L"1234567890123456789012345678901234567890"
				 L"1234567890123456789012345678901234567890",
				 &wendptr, 0
				)
	     ) != UINTMAX_MAX || errno != ERANGE
	   )	{
		fprintf(outfile,"*** wcstoumax failed -overflow test ***\n");
		status = EXIT_FAILURE;
		}
#endif // NO_INTERNAL_WCHAR
	}
#endif	/* defined(INTMAX_MAX) */

	if ( status != 0 )
		fprintf(outfile, "sitest failed.\n");

    CLOSETEST();
	
	return status;
}

#endif