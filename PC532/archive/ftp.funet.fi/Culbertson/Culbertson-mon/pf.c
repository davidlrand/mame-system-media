/* NSC 32000 ROM debugger.
 * Bruce Culbertson  Bob Krause
 *
 * Printf replacement so you do not need libc.
 */

#include "debugger.h"

/* for justification */
#define NOJUST 0
#define LJUST  1
#define RJUST  2
#define ISNUM(x) (x >= '0' && x <= '9')

static int (*putc_func)();
int cooked_putc(), sprintf_putc();
static int pf_len;
static char *pf_bufp;

int
printf (fmt)
char *fmt;
{
  putc_func = cooked_putc;
  return xprintf (&fmt);
}

int
sprintf (buf, fmt)
char *buf, *fmt;
{
  putc_func = sprintf_putc;
  pf_bufp = buf;
  xprintf (&fmt);
  *pf_bufp = '\0';
  return pf_len;
}

int
printNumBase (num, base)
int base;
long num;
{
  if (num < 0 && base == 10) {
    cooked_putc ('-');
    num = -num;
  }
  putc_func = cooked_putc;
  return recur_print_num (num, base, 0);
}

int
xprintf (fmtp)
char **fmtp;
{
  register char *fmt = *fmtp;
  char *argp;
  int is_long;
  int len;
  int just;

  argp = (char *)fmtp +			/* machine dependent */
    sizeof (char *);
  pf_len = 0;
  for (; *fmt; ++fmt) {
    is_long = 0;
    just = NOJUST;
    len = 0;
    if (*fmt == '%') {
      ++fmt;
      if (!*fmt) break;			/* strange end of fmt string */
      if (*fmt == '-') {
	just = LJUST;
	++fmt;
        if (!*fmt) break;		/* strange end of fmt string */
      }
      if (ISNUM(*fmt)) {
	if (just == NOJUST) just = RJUST;
	do {
	  len = 10*len + *fmt++ - '0';
        } while (ISNUM(*fmt));
      }
      if (!*fmt) break; 		/* strange end of fmt string */
      if (*fmt == 'l') {
	is_long = 1;
	++fmt;
        if (!*fmt) break;		/* strange end of fmt string */
      }
      switch (*fmt) {
	case 'c':
	  print_char (*((int *)argp), just, len);
	  argp += sizeof (int *);
	  break;
	case 's':
	  print_str (*((char **)argp), just, len);
	  argp += sizeof (char *);
	  break;
	case 'd':
	  print_num (&argp, 10, is_long, just, len);
	  break;
	case 'x':
	  print_num (&argp, 16, is_long, just, len);
	  break;
	default:
	  pf_putc (*fmt);
      }
    } else pf_putc (*fmt);
  }
  return pf_len;
}

print_char (c, just, len)
int c, just, len;
{
  if (just == RJUST)
    while (--len > 0) pf_putc (' ');
  pf_putc (c);
  if (just == LJUST)
    while (--len > 0) pf_putc (' ');
}

print_num(argp, base, is_long, just, len)
char **argp;
int base, is_long, just, len;
{
  unsigned long val;
  int ret;

  if (is_long) {
    val = *((unsigned long *)*argp);
    *argp += sizeof (unsigned long);
  } else {
    val = *((unsigned int *)*argp);
    *argp += sizeof (unsigned int);
  }
  if ((val & 0x80000000) && base == 10){
    pf_putc ('-');
    val = -val;
  }
  ret = recur_print_num (val, base, just == RJUST? len: 0);
  if (just == LJUST)
    for (len -= ret; len > 0; --len) pf_putc (' ');
}

int
recur_print_num (val, base, len)
unsigned long val;
int base, len;
{
  int ret = 0;

  if (val >= base)
    ret = recur_print_num (val / base, base, len-1);
  else while (--len > 0) pf_putc (' ');
  pf_putc (*("0123456789abcdef" + val%base));
  return 1 + ret;
}

/* Print a string. */
print_str(strp, just, len)
char *strp;
int just, len;
{
  if (just != NOJUST) len -= strlen (strp);
  if (just == RJUST)
    while (len-- > 0) pf_putc (' ');
  for (; *strp; ++strp) pf_putc(*strp);
  if (just == LJUST)
    while (len-- > 0) pf_putc (' ');
}

/* Putc for xprintf.  Putc_func has been set up by (s)printf to do
 * the appropriate thing.  Also counts characters.
 */
pf_putc(x)
int x;
{
  ++pf_len;
  (*putc_func)(x);
}

/* Sends character to pf_buf for sprintf */
sprintf_putc(x)
int x;
{
  *pf_bufp++ = x;
}

/* Converts nl to cr-lf, sends character to io.  Probably all other
 * routines want to use this (rather than raw putch) for io.
 */
cooked_putc(x)
int x;
{
# if STANDALONE
  if (x == '\n') putch ('\r');
# endif
  putch(x);
}
