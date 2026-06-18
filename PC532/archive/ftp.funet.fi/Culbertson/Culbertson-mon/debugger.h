#if GCC
#  define CONST const
#else
#  define CONST
#endif

#if STANDALONE
#define BASE 0
#define DEFAULT_BAUD	9600
#define DEFAULT_UART	0		/* right for pc532 */
#define getch() db_fgetc(DEFAULT_UART)
#define putch(x) db_fputc(x, DEFAULT_UART)
#else
#define BASE ((long)(fileBase))
#endif

#if LSC
#define PRINTFINC -1
#endif

#if GCC
#define PRINTFINC 0
#define NULL 0L
#define myPrintf printf
#define mySprintf sprintf
#endif

#if UNIX
#define PRINTFINC 0
#define NULL 0
#define myPrintf printf
#define mySprintf sprintf
#define getch ttgetc
#define putch ttputc
#endif

/* Machine State */
struct MachState {		     /* offset (keep in sync with resume.s) */
    unsigned long r[8];              /* 0	general registers */
    unsigned long pc;                /* 32	program counter */
    unsigned long sb;                /* 36	static base */
    unsigned long fp;                /* 40	frame pointer */
    unsigned long usp;               /* 44	user stack pointer */
    unsigned long isp;               /* 48	interrupt stack pointer */
    unsigned long intbase;           /* 52	interrupt base */
    unsigned short mod;              /* 56	module */
    unsigned short psr;              /* 58	processor status register */

    unsigned long ptb0;              /* 60	page table base 0 */
    unsigned long ptb1;              /* 64	page table base 1 */
    /* skip ivar0, ivar1 since write only */
    unsigned long tear;              /* 68  read only */
    unsigned long mcr;               /* 72 */
    unsigned long msr;               /* 76 */

    unsigned long dcr;		     /* 80 */
    unsigned long dsr;		     /* 84 */
    unsigned long car;		     /* 88 */
    unsigned long bpc;		     /* 92 */

    unsigned long fsr;               /* 96	floating point stat register */
    union {			     /* 100	floating point registers */
      unsigned long l[16];
      double d[8];
    } f;
    unsigned long cfg;		     /* 164 */
};
extern struct MachState machState;
extern long defaultBase;
                
#define NO_NUM 0
#define GOT_NUM 1
#define BAD_NUM 2
#define NOTRACE 0
#define TRACE 1
#define True 1
#define False 0
#define LNLEN 256

/* Stuff for myStrCmp()
 */
#define CMP_NOMATCH	0
#define CMP_MATCH	1
#define CMP_SUBSTR	2

extern char *fileBase,              /* beginning of file buffer */
       version[];		    /* date of make */
extern unsigned char *Dot;          /* current point in virtual space */
extern long fileTop;                /* offset to end of file buffer */

extern long screenLength;            /* number of lines on the screen */
extern int screenShown;             /* # of lines printed since prompt */
extern int screenIgnore;            /* on if discarding output */
extern long debug;		    /* turn on debugging printf's */
extern long scsiAdr, scsiLun;	    /* SCSI defaults */

#if LSC
#define swaps(x)    {short T = (((x)&0xff)<<8);\
                     (x) = (((x)>>8)|T);}
#define swapl(x)    {short T1 = (((x)&0xff)<<24),\
                           T2 = (((x)&0xff00)<<8),\
                           T3 =(((x)&0xff0000)>>8),\
                           T4 = (((x)&0xff000000)>>24);\
                     (x) = (T1|T2|T3|T4);}
#else
#define swaps(x)
#define swapl(x)
#endif

/* for newreg.c */
#define T_CHAR  0
#define T_SHORT 1
#define T_LONG  2
#define T_LEN	3
#define T_PSR	4	/* print PSR bits */
#define T_DECI	8	/* print in base 10 */

#define REGCHAR(c) ((c >= 'a' && c <= 'z') || \
                    (c >= 'A' && c <= 'Z') || \
                    (c >= '0' && c <= '9') || \
		    c == '.' || c == '_')
#define CHAR_STAR(adr)	(*((unsigned char *)(adr)))
#define SHORT_STAR(adr)	(*((unsigned short *)(adr)))
#define LONG_STAR(adr)	(*((unsigned long *)(adr)))

struct regTable {
  char *name;
  char *ptr;
  short type;
};

extern struct regTable regTable [];

#define NUM_SW_BKPT	8
#define INSN_BPT	((unsigned char)0xF2)

struct bkpt {
  unsigned char *adr;
  unsigned char insn;
};

extern struct bkpt bkpt[];

