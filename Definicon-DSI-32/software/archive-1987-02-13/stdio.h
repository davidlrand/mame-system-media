#include "portab.h"
#ifndef FILE
#define NULL	0
#define EOF	(-1)
#define BUFSIZ	512	/* buffer size for all IO buffers */
#define BLKSIZ	BUFSIZ
#define _NFILE	20
extern struct iobuf {
    UBYTE *io_next ;	/* next character to be read from buffer, next position
    			   to put a character in buffer */
    UBYTE *io_lid ;	/* last position with data in input buffer */
    UBYTE *io_top ;	/* position beyond buffer */
    UBYTE *io_base ;	/* start of buffer */
    short io_file ;	/* location in _iob array, also unix style file # */
    short io_flags ;	/* random info */
    short io_chan ;	/* Used to specify what channel, system dependant */
    short io_tmp ;	/* used by ungetc on unbuffered files */
} _iob[_NFILE];
#define _IO_READ	0x1	/* indicates iobuf is open for read */
#define _IO_WRITE	0x2	/* indicates iobuf is open for write */
#define _IO_ERROR	0x4	/* indicates some error has happenned */
#define _IO_EOF		0x8	/* indicates the end of file has been reached*/
#define _IO_DISK	0x20
#define _IO_BUF		0x40	/* has a system supplied buffer */
#define _IO_USRBUF	0x80	/* has a user supplied buffer */
#define _IO_STR		0x100	/* used by sprintf, maybe */
#define _OP_EOF	0x1000
#define FILE struct iobuf
#define stdin (&_iob[0])
#define stdout (&_iob[1])
#define stderr (&_iob[2])
#define getc(f)	(((f)->io_next >= (f)->io_lid)?_filbuf(f):(unsigned) *((f)->io_next++))
#define getchar() getc(stdin)
#define putc(ch,f) ((((f)->io_tmp=(ch))=='\n'||(f)->io_next>=(f)->io_top)?_flsbuf(f,(f)->io_tmp):(*((f)->io_next++)=(f)->io_tmp))
#define putb(ch,f) (((f)->io_next<(f)->io_top)?*((f)->io_next++)=(ch):_flsbuf(f,(ch)))
#define putchar(ch) putc(ch,stdout)
#define fileno(file)	((file)->io_file)
#define feof(file)	((file)->io_flags&_IO_EOF)
#define ferror(file)	((file)->io_flags&_IO_ERROR)
#define clearerr(file)	(file)->io_flags&=~(_IO_EOF|_IO_ERROR)
#endif
#define	MAX(x,y)   (((x) > (y)) ? (x) :  (y))
#define	MIN(x,y)   (((x) < (y)) ? (x) :  (y))
#define	abs(x)	((x) < 0 ? -(x) : (x))
FILE *fopen(),*fopena(),*fopenb(),*freopen(),*freopa(),*freopb();
