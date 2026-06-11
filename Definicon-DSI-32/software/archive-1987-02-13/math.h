#include "errno.h"
#define ieee_float
#define internal static
#define and	&&
#define or	||
#define not	!
#define FALSE	0
#define TRUE	1
#define PI	3.14159265358979323846
#define TWO_PI	6.28318530717958647693
#define TWO_OVER_PI	.63661977236758134307553505 /* 3490 */
#define PI4	0.78539816339744830961566084 /* 5819 */
#define PI2	1.57079632679489661923
#define E	2.71828182845904523536
#define LOG10	2.30258509299404568402
#define LOG2	0.69314718055994530942
#define LOGS2PI	0.91893853320467274178
#define MUL	1.1283791670955126	/* 2/sqrt(PI), for erf */
#define HUGE_VAL 1.79e+308
#define TINY_VAL 2.20e-308
#define LOGHUGE  709.778
#define LOGTINY  -708.396
extern int errno ;
#define MAXE 21.
#define EXPBASE	1023
typedef struct {
    unsigned dbl_frac2 : 32 ;
    unsigned dbl_frac1 : 20 ;
    unsigned dbl_exp : 11 ;
    unsigned dbl_sign : 1 ;
} DBLE ;
double sin(), cos(), tan(), cotan();
double asin(), acos(), atan(), atan2();
double ldexp(), frexp(), modf();
double floor(), ceil(), fabs();
double log(), log10(), exp(), sqrt(), pow();
double sinh(), cosh(), tanh();
double j0(), j1(), jn();
double y0(), y1(), yn();
double gamma();
