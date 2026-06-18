/*
Date: Tue, 27 Feb 90 11:47:38 pst
From: Bruce Culbertson <culberts@hplwbc.hpl.hp.com>
To: pc532@daver.bungi.com
Subject: Re:  ROM Debugger -- download command..

John L. Connin <johnc%manatee%uunet@daver> writes:
> Dave, what data format / protocol does the ROM debugger 'download' command 
> expect ??   

Here is the download program which I run at the other end of the
serial line -- in my case, on an AT clone.  The download protocol
is explained in the comments in the program.  I just recently added
the download command to the monitor so let me know if you have
problems with it or if you wish it worked differently.

Bruce Culbertson
----------------------------------------------------------------------
*/
/* MS-DOS Program for downloading to the NSC32000 Monitor.  Use this as a
 * template for writing downloaders for other OS's.  Compile the MS-DOS
 * version with the Microsoft C compiler.
 * 
 * Bruce Culbertson  18 February 1990
 */

/* Instructions for use:
 *
 * machine      prompt, command, etc.
 * -------------------------------------------------------------------
 * 32000	Command (? for help): download <address>
 * MS-DOS	[exit terminal emulator]
 * MS-DOS	C> <this program> <file name to download>
 * MS-DOS	[re-enter terminal emulator]
 * 32000	[hit return to get status of download]
 * 32000	Command (? for help): ...
 *
 * At any point you can send control-C (e.g. using your terminal emulator)
 * to the 32000 monitor to abort the download and return to the monitor
 * prompt.
 */

/* Download protocol:
 *
 * <start mark> <length> <data> <CRC>
 *
 * Below, the sending machine is called SRC, receiving machine is DST.
 * Eight bit characters are used.
 *
 * Control-C (0x03) aborts the transfer.  This capability is nice
 * to have if, for example, length is garbled and the DST expects
 * billions of characters.  Since any byte of <length>, <data>,
 * or <CRC> could be control-C, we need to have a quote character.
 * I use ESC (0x1b).  Thus, control-C and ESC are sent as
 * {0x1b 0x03} and {0x1b 0x1b}, respectively.
 *
 * Start mark:
 *   This is a colon.  When SRC begins sending, DST loops until it sees
 *   the start mark.  Thus, if spurious characters are sent as the
 *   user switches from terminal emulator to download program on SRC,
 *   DST can ignore them.
 *
 * Length:
 *   Four bytes, least significant first.  The length is number of
 *   data bytes to be transfered, not including quote characters.
 *   The two CRC bytes are also not included in the length.
 *
 * Data:
 *   A byte is sent as a byte, with quoting if necessary.
 *
 * CRC:
 *   Two bytes, least significant first.  Use CCITT CRC generator
 *   polynomial (x^16 + x^12 + x^5 + 1).  Compute on data only (not
 *   length or start) and exclude quotes.  (This is the same CRC
 *   as computed by Minix's CRC command.)
 */

#include <stdio.h>
#include <fcntl.h>

#ifdef MSDOS
#  define OPEN_FLAGS    (O_RDONLY | O_BINARY)
#else
#  define OPEN_FLAGS    O_RDONLY
#endif

#define CCITT_GEN	0x11021		/* x^16 + x^12 + x^5 + 1 */
#define BUFSZ		0x1000
#define ESC		0x1b
#define CTL_C		0x03
#define DEFAULT_PORT    1

char buf[BUFSZ];
int port_num = DEFAULT_PORT;
int port;
long write_data(), write_header();

main (argc, argv)
int argc;
char **argv;
{
  int fd;
  long crc, len;

  if (argc == 3) {
    if (1 != sscanf (argv[2], "%d", &port_num)) {
      fprintf (stderr, "Bad serial port, use 1 or 2\n");
      exit (-1);
    }
    --argc;
  }
  if (argc != 2) {
    fprintf (stderr, "usage: %s <file> [<serial port>]\n", argv[0]);
    exit (-1);
  }
  if (port_num == 1) port = 0x3f8;
  else if (port_num == 2) port = 0x2f8;
  else {
    fprintf (stderr, "Bad serial port, use 1 or 2\n");
    exit (-1);
  }
  if (0 > (fd = open (argv[1], OPEN_FLAGS))) {
    fprintf (stderr, "can not open %s\n", argv[1]);
    exit (-1);
  }
  init_port();
  len = write_header (fd);
  crc = write_data (fd);
  write_crc (crc);
  printf ("Length=%ld CRC=%ld\n", len, crc);
  restore_port();
  exit (0);
}

/* Write header.  Format is a colon followed by four byte length,
 * LSB first.  Length is the number of data bytes after quotes are
 * removed.
 */
long
write_header (fd)
int fd;
{
  long len, lseek();

  if (0 == (len = lseek (fd, 0L, 2))) {
    fprintf (stderr, "file length is zero\n");
    exit (-1);
  }
  lseek (fd, 0L, 0);
  write_ch (':');
  write_ch ((int)((len >> 0) & 0xff));
  write_ch ((int)((len >> 8) & 0xff));
  write_ch ((int)((len >> 16) & 0xff));
  write_ch ((int)((len >> 24) & 0xff));
  return len;
}

/* Write data.
 */
long
write_data (fd)
int fd;
{
  long len, crc = 0, update_crc();
  char *p;

  for (;;) {
    len = read (fd, buf, BUFSZ);
    if (-1 == len) {
      fprintf (stderr, "read failed\n");
      exit (-1);
    }
    if (len == 0) break;
    for (p = buf; p < buf + len; ++p) {
      write_ch (*p);
      crc = update_crc (crc, *p);
    }
  }
  return crc;
}

/* Write two CRC bytes, LSB first.
 */
write_crc (crc)
long crc;
{
  write_ch ((int)((crc >> 0) & 0xff));
  write_ch ((int)((crc >> 8) & 0xff));
}

/* Given old CRC and new character, return new CRC.  Uses standard
 * CCITT CRC generator polynomial.
 */
long
update_crc (crc, ch)
long crc;
int ch;
{
  int i;

  for (i = 0x80; i; i >>= 1) {
    crc = (crc << 1) | (i & ch? 1: 0);
    if (crc & 0x10000) crc ^= CCITT_GEN;
  }
  return crc;
}

/* Output a character.  If it is a CLT_C or ESC, then quote (preceed)
 * it with a ESC.
 */
write_ch (c)
int c;
{
  if (c == ESC || c == CTL_C)
    putch (ESC);
  putch (c);
}

#ifdef MSDOS
/* Write hardware directly since BIOS and DOS are not reliable.
 */
#define COM_WR      0
#define COM_RD      0
#define COM_IER     1
#define COM_CTL     3
#define COM_STAT    5
#define COM_CTL_VAL 3                   /* 8 bits, 1 stop, no parity */
#define COM_IER_VAL 0                   /* interrupts off */
#define COM_TX_RDY  0x20

int old_control, old_ier;

/* Output a character to the serial port.
 */
putch (c)
int c;
{
  int stat;
  
  for (;;) {
    stat = inp (port + COM_STAT);
    if (stat & COM_TX_RDY) break;
  }
  outp (port + COM_WR, c);
}

/* Initialize serial port and save old values.  Assume baud rate
 * already set.
 */
init_port()
{
  old_control = inp (port + COM_IER);
  old_ier = inp (port + COM_CTL);
  outp (port + COM_CTL, COM_CTL_VAL);
  outp (port + COM_IER, COM_IER_VAL);
}

/* Restore serial port to old configuration.
 */
restore_port()
{
  outp (port + COM_CTL, old_control);
  outp (port + COM_IER, old_ier);
}
#else
putch (c) {putchar (c);}
init_port(){}
restore_port(){}
#endif

