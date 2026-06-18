/* NSC 32000 ROM debugger.
 * Bruce Culbertson  Bob Krause
 *
 * Main, command table, expression parser, many command handlers.
 */

#include "debugger.h"
#if LSC
#include <stdio.h>
#include <unix.h>
#include <storage.h>
#endif

/* GCC considers "inline" to be keyword */
#define inline _inline

char help_str [] =
"Command arguments may be expressions.  Type HELP = for expression syntax.\n";

long debug = 0;
long defaultBase = 16;
#if !STANDALONE
char *fileBase = 0;			/* beginning of file buffer */
long fileTop = 0;			/* offset to end of file buffer */
#endif
unsigned char *Dot = 0;                 /* current point in virtual space */

struct MachState machState;

/* Command interpreters */
int baseConverter(), disassemble(), dump(), quitHandler(),
 edit(), help(), help_cmds(),
 search(), mode(), printBkpt(),
 stackTrace(), baud(),
 init_machState(), run(), single_step(),
 fill(), move(), set(), show(), gpr(), cpu(), fpu(), mmu(),
 scsiRaw(), scsiRead(), scsiWrite(), crc(), download();

#if !STANDALONE
int getfile(), putfile();
#endif /* STANDALONE */

struct cmd {				/* The commands, their names, help */
  int (*fn)();
  char *name;
  char *help;
};

CONST
struct cmd cmd_tbl [] = {		/* Command Table */

#if STANDALONE
{ baud, "baud",
"Syntax: BAUD <rate> [<uart>].  Set UART baud rate.  Console UART is 0 and\n\
is the default."
},
#endif

{ printBkpt, "breakpoint",
"List software breakpoints.  Use SET to set breakpoints, SET to 0 to clear."
},

{ cpu, "cpu",
"Show CPU registers."
},

{ crc, "crc",
"Syntax: CRC <address> <length>.  Compute CCITT CRC on a section of memory."
},

{ disassemble, "disassemble",
"Syntax: DISASSEMBLE <address> <count>.  Disassembles <count> instructions\n\
starting at <address>."
},

{ download, "download",
"Syntax: DOWNLOAD <address>.  Download via serial line into memory.  Hit\n\
control-C to abort.  After transfer, hit <return> for status."
},

{ dump, "dump",
"Syntax: DUMP [<address> [<count>]].  Displays <count> bytes starting at\n\
<address>.  Default address is \".\" and default count is 16."
},

{ edit, "edit",
"Syntax: EDIT <address>.  Edits memory starting at <address>.  Give one or\n\
more values when prompted; the values will be placed in successive bytes.\n\
After hitting return, you will be prompted for another byte.  Editting stops\n\
when you hit return without a value.  Typing \"=\" instead of a new value\n\
causes the old value to be retained and advances the editor to the next byte."
},

{ fill, "fill",
"Syntax: FILL <address> <count> <pattern>.  Fills memory with byte pattern."
},

{ fpu, "fpu",
"Show FPU registers."
},

{ gpr, "gpr",
"Show general purpose registers."
},

{ help, "help",
"Syntax: HELP <command>.  Provides help for <command>."
},

{ init_machState, "initialize",
"Reset stacks, mod table, interrupt table, initialize machine registers\n\
to handy values."
},

{ mmu, "mmu",
"Show MMU registers."
},

{ mode, "mode",
"Print mode variables."
},

{ move, "move",
"Syntax: MOVE <source adr> <dest adr> <cnt>.  Move bytes in RAM."
},

#if !STANDALONE
{ quitHandler, "quit",
"Exit program."
},
#endif /* !STANDALONE */

#if STANDALONE
{ scsiRaw, "raw",
"Syntax: RAW <parameters> [<SCSI adr>].  Issues a command to a SCSI device.\n\
<Parameters> is the address of an array of eight pointers to the various\n\
SCSI buffers: data-out, data-in, command, status, dummy, dummy, message-out,\n\
message-in.  Default SCSI address is the variable scsi_adr."
},
#endif

#if !STANDALONE
{ getfile, "read",
"Syntax: READ <file> [<address>].  Reads a file into the buffer starting\n\
at <address> or 0 if no address is given."
},
#else
{ scsiRead, "read",
"Syntax: READ <block> <RAM address> [<len> [<SCSI adr> [<SCSI lun>]]].\n\
Read 1 or <len> blocks, starting at <block> into RAM at <RAM address> from\n\
SCSI device.  Default SCSI address and logical unit number are the variables\n\
scsi_adr and scsi_lun.  Block length is same as used by the SCSI device."
},
#endif /* STANDALONE */

{ run, "run",
"Syntax: RUN [<address>].  Start user program running.  Default address is\n\
current PC."
},

{ search, "search",
"Syntax: SEARCH <start> <cnt> <byte> [<byte> ...].  Searches from <start> to\n\
<start>+<cnt> for all occurrences of the given pattern.  Eventually quits if\n\
many matches."
},

{ set, "set",
"Syntax: SET <reg> <value>.  Set the value of a register or variable.  Use\n\
SHOW to get a complete list of register and variable names."
},

{ show, "show",
"List all registers and variables with their values."
},

{ single_step, "step",
"Syntax: STEP [<count>].  Execute one or <count> instructions from user\n\
program."
},

{ stackTrace, "trace",
"Syntax: TRACE <count> [<fp value>].  Display a trace of the top <count>\n\
stack frames, starting at FP or <fp value>."
},

#if !STANDALONE
{ putfile, "write",
"Syntax: WRITE <file> <address> <count>.  Writes <count> bytes to\n\
<file> from the buffer, starting at offset <address>."
},
#else
{ scsiWrite, "write",
"Syntax: WRITE <block> <RAM address> [<len> [<SCSI adr> [<SCSI lun>]]].\n\
Write 1 or <len> blocks, starting at <block> from RAM at <RAM address> to\n\
SCSI device.  Default SCSI address and logical unit number are the variables\n\
scsi_adr and scsi_lun.  Block length is same as used by the SCSI device."
},
#endif /* STANDALONE */

{ baseConverter, "=",
"Syntax: = <expression> [<base>].  Display the value of the expression in\n\
the given base or the default base.  Operators in order of precedence are:\n\
\t- b: w: d:\n\
\t* / % << >>\n\
\t+ -\n\
()'s may be used to override precedence.  b'101, d'99 and h'ff are binary,\n\
decimal, and hex numbers.  A number without a prefix has the default radix.\n\
Registers may be used in expressions.  The prefix r' specifies a register\n\
for ambigious cases, e.g. f0 is a hex number but r'f0 is a register.\n\
b: w: and d: fetch bytes, words, and doubles from memory.  '<character> is\n\
the ASCII value of <character>.  V(<adr>[,<ptb>]) translates a virtual\n\
address to a physical address."
},

{ help, "?",
"Prints a list of commands."
},
};
#define CMDLEN (sizeof (cmd_tbl) / sizeof (struct cmd))

extern char end;

main(argc, argv)
int argc;
char **argv;
{
#if !STANDALONE
#else /* STANDALONE */
  zero_bss();	/* note: this needs to happen before almost anything */
  copy_dataseg();
  db_setbaud (DEFAULT_BAUD, DEFAULT_UART);
  myPrintf ("\nNS32000 ROM Debugger\nVersion: %s\n", version);
  myPrintf ("RAM free above 0x%lx\n\n", (long)(&end));
#endif
#if UNIX
  ttopen();
  if (argc > 1) getfile (argv [1]);
#endif
  init_machState();
  initBreaks();
  command_loop();
#if UNIX
  ttclose();
#endif
#if !STANDALONE
  exit (0);
#endif /* STANDALONE */
}

command_loop ()
{
  CONST struct cmd *q, *find_cmd();
  char *p, inline [LNLEN];

  for (;;) {
    myprompt(inline, LNLEN, "Command (? for help): ");
    p = inline;
    scan (&p);
    if (*p == '\0' || *p == '\n') continue;
    /* if command is found in command table, call command handler */
    if (NULL != (q = find_cmd (&p))) (*(q->fn)) (p);
    else morePrintf (1, "Unknown command\n");
  }
}

/* Search cmd_tbl for 1) a command matching what the user typed or 2)
 * a unique command which is a superstring of what the user typed.
 */
CONST struct cmd *
find_cmd (p)
char **p;
{
  char token [LNLEN];
  CONST struct cmd *q, *substr;
  int multisub = 0;

  scanToken (p, token);
  for (q = cmd_tbl; q < cmd_tbl + CMDLEN; ++q)
    switch (myStrCmp (token, q->name)) {
      case CMP_MATCH:
	return q;
      case CMP_SUBSTR:
	++multisub;
	substr = q;
	break;
      case CMP_NOMATCH:
	break;
    }
  return (multisub == 1)? substr: NULL;
}

/* Scan next token from string pointed to by p.  Tokens are delimited
 * by white space.  Shift upper case chars to lower case.  Advance p
 * to point beyond token.
 */
scanToken (p, str)
char **p;
char *str;
{
  scan (p);
  while (**p != '\0' && **p != '\n' && **p != ' ' && **p != '\t')
    if (**p >= 'A' && **p <= 'Z') *str++ = (*(*p)++) - 'A' + 'a';
    else *str++ = *(*p)++;
  *str = '\0';
}

/* Advance p to point to non white space.
 */
scan (p)
char **p;
{
  while (**p == ' ' || **p == '\t') ++(*p);
}

#if UNIX
quitHandler(p)
char *p;
{
    ttclose();
    exit();
}
#endif

/* "?" command handler.  Print help and list of commands.
 */
help_cmds (p)
char *p;
{
    CONST struct cmd *q;

    morePrintf(1, help_str);
    morePrintf(1, "For additional help, type HELP <command>.  Commands are:\n");
    for (q = cmd_tbl; q < cmd_tbl + CMDLEN; ++q) 
        if ((q - cmd_tbl) % 5 == 4)
            morePrintf (1, "%s\n", q -> name);
        else
            morePrintf (0, "%-15s", q -> name);
    if ((q - cmd_tbl) % 5 != 0)
        morePrintf (1, "\n");
}

/* HELP command handler.  Print help on a particular command.
 */
help (p)
char *p;
{
  CONST struct cmd *q;

  if (NULL != (q = find_cmd (&p)))
    morePrintf (1, "%s\n", q -> help);
  else help_cmds (p);
}

/* DUMP command handler.  Hex dump routine.
 */
dump (p)
char *p;
{
    long adr, cnt, i, ret;
    char *q, printchars [24], *pch;

    switch (getIntScan (&p, &adr)) {
      case NO_NUM :
        adr = (long)Dot;
        cnt = 16;
        break;
      case BAD_NUM :
	myPrintf ("Bad argument\n");
        return;
      case GOT_NUM :
        ret = getIntScan(&p, &cnt);
        if (ret == NO_NUM) cnt = 16;
        else if (ret == BAD_NUM) {
	    myPrintf ("Bad argument\n");
            return;
	}
	break;
    }
    Dot = (unsigned char *)(adr + cnt);
    i = 0;
    pch = printchars;
    for (q = (char *)adr; q < (char *)(adr + cnt); ++q, ++i) {
        if ((i & 0xf) == 0) {
	    printhexbyt ((int)((long)q >> 24 & 0xff));
	    printhexbyt ((int)((long)q >> 16 & 0xff));
	    printhexbyt ((int)((long)q >> 8 & 0xff));
	    printhexbyt ((int)((long)q & 0xff));
	    morePrintf (0, " ");
        } else if ((i & 0xf) == 4 || (i & 0xf) == 8 ||(i & 0xf) == 12) {
            morePrintf(0, "| ");
        }
        printhexbyt (*(q + BASE));
        *pch++ = (*(q + BASE) >= 0x20 && *(q + BASE) <= 0x7e)?
          *(q + BASE): '.';
        morePrintf(0, " ");
        if ((i & 0xf) == 0xf) {             /* end of line */
            *pch = '\0';                      /* print printing characters */
            morePrintf(1, "%s\n", printchars);
            pch = printchars;
        }
    }
    if ((i & 0xf) != 0) {
        *pch = '\0';
        morePrintf (1, " %s\n", printchars);
    }
}  

/* EDIT command handler.  Interactively edit memory.
 */
edit (p)
char *p;
{
  char *q, inline [LNLEN];
  int  i, ret;
  long offset;

  switch (getIntScan (&p, &offset)) {
    case NO_NUM:
      offset = (long)Dot;
      break;
    case BAD_NUM:
      morePrintf (1, "Bad offset - %s\n", p);
      return;
    default:;
  }
  q = (char *)offset;
  for (;;) {
    myprompt (inline, LNLEN, "%x(%x): ", q, *(q + BASE) & 0xff);
    p = inline;
    scan (&p);
    if (*p == '\0' || *p == '\n') break;
    for (;;) {
      scan (&p);
      if (*p == '=') ++p;		/* = means leave old value */
      else {
        if (GOT_NUM != (ret = getIntScan (&p, &i))) break;
        *(q + BASE) = i;
      }
      ++q;
    }
    if (ret == BAD_NUM) morePrintf (1, "Bad number: %s\n", p);
  }
  Dot = (unsigned char *)q;
}

/* FILL command handler.  Fill memory with some value.
 */
fill (p)
char *p;
{
  long cnt, start, pattern;
  unsigned char *endp, *startp;

  if (GOT_NUM != getIntScan (&p, &start) ||
     GOT_NUM != getIntScan (&p, &cnt) ||
     GOT_NUM != getIntScan (&p, &pattern))
  {
    myPrintf ("Bad start address, count or pattern\n");
    return;
  }
  startp = (unsigned char *)start + BASE;
  for (endp = startp + cnt; startp < endp; ++startp)
    *(startp) = pattern;
}

/* MOVE command handler.  Move (really copy) a block of memory.
 */
move (p)
char *p;
{
  long cnt;
  unsigned char *start, *dest, *q;

  if (GOT_NUM != getIntScan (&p, &start) ||
     GOT_NUM != getIntScan (&p, &dest) ||
     GOT_NUM != getIntScan (&p, &cnt))
  {
    myPrintf ("Bad source, destination or count\n");
    return;
  }
  start += BASE;
  dest += BASE;
  if (start > dest)
    for (q = start; q < start + cnt;) *dest++ = *q++;
  else
    for (q = start + cnt, dest += cnt; q > start;)
      *--dest = *--q;
}

/* SEARCH command handler.  Search memory for a pattern.
 */
search (p)
char *p;
{
  char sbuf [LNLEN], *m, *n, *q, *first_fnd;
  long start, cnt;
  int len, found, i;

  if (GOT_NUM != getIntScan (&p, &start) ||
  GOT_NUM != getIntScan (&p, &cnt)) {
    myPrintf ("Bad start address or count\n");
    return;
  }
  found = len = 0;
  q = sbuf;
  while (q < sbuf + LNLEN && GOT_NUM == getIntScan (&p, &i)) {
    *q++ = i;
    ++len;
  }
  first_fnd = NULL;
  for (m = (char *)start; m < (char *)start + cnt; ++m) {
    if (*(m + BASE) == *sbuf) {
      for (n = m + 1, q = sbuf + 1; n - m < len && *(n + BASE) == *(q + BASE);
        ++n, ++q);
      if (n - m == len) {
	if (NULL == first_fnd) first_fnd = m;
        ++found;
        morePrintf (1, "0x%x\n", m);
      }
      if (found > 100) {
        morePrintf (1, "quitting...\n");
        break;
      }
    }
  }
  if (first_fnd != NULL) Dot = (unsigned char *)first_fnd;
}

/* Parse an expression from the passed string.  For consistency, all
 * command handlers should use this for parsing numeric arguments.
 * Advances *p past the current expression.
 *
 * Return True if got legal input, False otherwise.  Legal inputs are:
 *   allow simple expressions using + and -.
 *   . (return Dot)
 *   pc, fp, usp, or isp (return machState.xxx)
 *   '<character>
 *   <number> (default radix)
 *   b'<binary number>
 *   d'<decimal number>
 *   h'<hex number>
 */
int
getIntScan (p, c)
char **p;
long *c;
{
  long accum, val;
  char op;
  int ret;

  ret = getIntScan1 (p, &accum);
  if (ret != GOT_NUM) return ret;
  for (;;) {
    scan (p);
    op = **p;
    if (op != '-' && op != '+') break;
    ++(*p);
    if (getIntScan1 (p, &val) != GOT_NUM) return BAD_NUM;
    accum += (op == '-')? -val: val;
  }
  *c = accum;
  return GOT_NUM;
}

/* Part of expression parser, parses one precedence level.
 */
int
getIntScan1 (p, c)
char **p;
long *c;
{
  long accum, val;
  char op;
  int ret;

  ret = getIntScan2 (p, &accum);
  if (ret != GOT_NUM) return ret;
  for (;;) {
    scan (p);
    op = **p;
    if (op != '*' && op != '/' && op != '%' && op != '<' && op != '>')
      break;
    if (op == '<' || op == '>') {
      if (op != *(*p + 1)) return BAD_NUM;
      ++(*p);
    }
    ++(*p);
    if (getIntScan2 (p, &val) != GOT_NUM) return BAD_NUM;
    switch (op) {
      case '*':
	accum *= val; break;
      case '/':
	accum /= val; break;
      case '%':
	accum %= val; break;
      case '<':
	accum <<= val; break;
      case '>':
	accum >>= val; break;
    }
  }
  *c = accum;
  return GOT_NUM;
}

/* Part of expression parser, parses one precedence level.
 */
int
getIntScan2 (p, c)
char **p;
long *c;
{
  long j, k;
  char c0, c1;

  scan (p);
  c0 = tolower(**p);
  c1 = tolower(*(*p+1));
  switch (c0) {
    case '\n':
    case '\0':
      return NO_NUM;
    case '-':					/* -exp */
      ++*p;
      j = getIntScan2 (p, &k);
      *c = -k;
      return j;
    case '(':					/* (exp) */
      ++*p;
      if (GOT_NUM != getIntScan (p, c))
        return BAD_NUM;
      scan (p);
      if (**p != ')')
        return BAD_NUM;
      ++*p;
      return GOT_NUM;
    case '\'':					/* ASCII code */
      ++*p;
      *c = *(*p)++;
      return GOT_NUM;
    case 'b':
      if (c1 == ':') {				/* b:adr, fetch char */
        *p += 2;
        j = getIntScan2 (p, &k);
        *c = CHAR_STAR(k+BASE);
        return j;
      }
      if (c1 == '\'') {				/* b'num, base = 2 */
        *p += 2;
	return getNum (p, c, 2L);
      }
      break;
    case 'w':					/* w:adr, fetch short */
      if (c1 == ':') {
        *p += 2;
        j = getIntScan2 (p, &k);
        *c = SHORT_STAR(k+BASE);
        return j;
      }
      break;
    case 'd':					/* d:adr, fetch long */
      if (c1 == ':') {
        *p += 2;
        j = getIntScan2 (p, &k);
        *c = LONG_STAR(k+BASE);
        return j;
      }
      if (c1 == '\'') {				/* d'num, base 10 */
        *p += 2;
	return getNum (p, c, 10L);
      }
      break;
    case 'h':					/* base 16 number */
      if (c1 == '\'') {
        *p += 2;
	return getNum (p, c, 16L);
      }
      break;
    case 'r':					/* r'<reg> */
      /* force "f0" to be interpretted as a reg instead of a number */
      if (c1 == '\'') {
        *p += 2;
	return getReg (p, c);
      }
      break;
    case 'v':					/* v(vaddr[,ptb]) */
      if (c1 == '(' || c1 == ' ' || c1 == '\t') {
	++*p;
	return getVaddr (p, c);
      }
      break;
  }
  if (getNum (p, c, defaultBase) == GOT_NUM) return GOT_NUM;
  return getReg (p, c);
}

/* Part of expression parser.  **p should point to a register name.
 * If so, *c is assigned the current value of the register.
 */
int
getReg (p, c)
char **p;
long *c;
{
  char regbuf [80];
  struct regTable *r, *find_reg();

  scanreg (p, regbuf);				/* get register */
  if (NULL != (r = find_reg (regbuf))) {
    switch (r->type & T_LEN) {
      case T_CHAR:	*c = CHAR_STAR (r->ptr); break;
      case T_SHORT:	*c = SHORT_STAR (r->ptr); break;
      case T_LONG:	*c = LONG_STAR (r->ptr); break;
    }
    return GOT_NUM;
  }
  return BAD_NUM;
}

/* Part of expression parser.  **p should point to a number.
 * If so, *c is assigned the number.
 */
int
getNum (p, c, base)
char **p;
long *c, base;
{
  long j, k;

  if (-1 != (j = tohex (**p)) || j >= base) {	/* get number */
    ++*p;
    while (-1 != (k = tohex (**p)) && k < base) {
      ++*p;
      j = j * base + k;
    }
    *c = j;
    return GOT_NUM;
  } else return BAD_NUM;
}

/* Like getIntScan, but p is char * instead of char **.  See getIntScan.
 */
int
getInt (p, val)
long *val;
char *p;
{
  return getIntScan (&p, val);
}
  
/* Convert a char, interpretted as a hex digit, to an int.
 */
int
tohex (ch)
char ch;
{
  ch = toupper (ch);  /* bug fix WBC */
  if (ch >= '0' && ch <= '9') return ch - '0';
  if (ch >= 'A' && ch <= 'F') return ch - 'A' + 10;
  return -1;
}

/* Print a byte as two hex digits, 0-padded if necessary.
 */
printhexbyt (i)
int i;
{
  printhexnib ((i>>4) & 0xf);
  printhexnib (i & 0xf);
}

/* Print a nibble as a hex digit.
 */
printhexnib (i)
int i;
{
  if (i > 9)
    morePrintf (0, "%c", (char)('A' + i - 10));
  else
    morePrintf (0, "%c", (char)('0' + i));
}

#define ASCII_CH(x) (((x)>=0x20 && (x)<=0x7e)? (x): '.')
/* "=" command handler.  Kind of a calculator.
 */
baseConverter(p)
char *p;
{
  long base, val;

  if (getIntScan (&p, &val) != GOT_NUM) {
    myPrintf ("Bad value\n");
    return;
  }
  switch (getIntScan (&p, &base)) {
    case NO_NUM:
      base = defaultBase;
      break;
    case BAD_NUM:
      base = -1;		/* force error msg */
      break;
  }
  if (base < 2 || base > 16) {
    myPrintf ("Bad base\n");
    return;
  }
  printNumBase (val, (int)base);
  myPrintf (" \"%c%c%c%c\"\n",
    ASCII_CH((val>>24)&0xff),
    ASCII_CH((val>>16)&0xff),
    ASCII_CH((val>>8)&0xff),
    ASCII_CH((val>>0)&0xff));
}

/* TRACE command handler.  Trace back through stack frame chain,
 * printing return addresses.
 */
stackTrace (p)
char *p;
{
  long cnt, *fp;

  switch (getIntScan (&p, &cnt)) {
    case BAD_NUM:
      myPrintf ("Bad count\n");
      return;
    case NO_NUM:
      cnt = 1;
      break;
  }
  switch (getIntScan (&p, (long)(&fp))) {
    case BAD_NUM:
      myPrintf ("Bad fp value\n");
      return;
    case NO_NUM:
      fp = (long *)machState.fp;
      break;
  }
  while (cnt-- > 0) {
    myPrintf ("0x%lx\n", *(fp+1));
    fp = (long *)(*fp);
  }
}

#if STANDALONE
/* BAUD command handler. Change the baud rate.
 */
baud (p)
char *p;
{
  int rate, uart;

  switch (getIntScan (&p, &rate)) {
    case BAD_NUM:
    case NO_NUM:
      myPrintf ("Bad baud rate\n");
      return;
  }
  switch (getIntScan (&p, &uart)) {
    case BAD_NUM:
      myPrintf ("Bad uart number\n");
      return;
    case NO_NUM:
      uart = DEFAULT_UART;
      break;
  }
  db_setbaud (rate, uart);
}
#endif
