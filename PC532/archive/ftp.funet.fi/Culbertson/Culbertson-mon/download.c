/* NSC 32000 ROM debugger.
 * Bruce Culbertson  Bob Krause
 *
 * Download code, CRC code.
 */

/* Download protocol:
 *
 * <start mark> <length> <data> <CRC>
 *
 * Sending machine is called SRC, receiving machine is DST.  Eight
 * bit characters are used.
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

#include "debugger.h"
#define GEN_CCITT	0x11021L
#define ESC		0x1b
#define CTLC		0x03
#define START		':'

unsigned long update_crc();

/* Compute CRC on memory.  This can be used to see if a chunk of
 * memory has been corrupted.
 */
crc (p)
char *p;
{
  unsigned long crc = 0, adr, len;
  unsigned char *cp;

  if (GOT_NUM != getIntScan (&p, &adr) ||
      GOT_NUM != getIntScan (&p, &len))
  {
    myPrintf ("Bad argument\n");
    return;
  }
  adr += BASE;
  for (cp = (unsigned char *)adr; cp < (unsigned char *)adr + len; ++cp)
    crc = update_crc (crc, *cp);
  myPrintf ("CRC: %d\n", crc);
}

/* Download via serial line
 */
download (p)
char *p;
{
  unsigned long crc, adr, len;
  unsigned char c;
  unsigned short xcrc;

  if (GOT_NUM != getIntScan (&p, &adr)) {
    myPrintf ("Bad argument\n");
    return;
  }
  adr += BASE;
  for (;;) {				/* get start character */
    c = getch();
    if (c == START) break;
    if (c == CTLC) return;
  }
  if (!get_buf ((unsigned char *)&len,	/* get len (assume big endian) */
    4L, &crc)) return;
  crc = 0;				/* crc on data only */
  if (!get_buf ((unsigned char *)adr,	/* get data */
    len, &crc)) return;
  if (!get_buf ((unsigned char *)&xcrc,	/* get crc (assume big endian) */
    2, &adr)) return;
  for (;;) {				/* let user return to terminal */
    c = getch();
    if (c == '\r' || c == '\n') break;	/* user hits <return> */
    if (c == CTLC) return;
  }
  if (crc == xcrc)			/* print status */
    myPrintf ("CRC ok, length = %d\n", len);
  else myPrintf ("CRC error, received %d, expected %d, length %d\n",
    xcrc, crc, len);
}

int
get_buf (adr, len, crc)
unsigned char *adr;
unsigned long len, *crc;
{
  unsigned char c;

  while (len > 0) {
    c = getch ();
    if (c == CTLC) return 0;		/* handle control-C */
    if (c == ESC) c = getch ();		/* handle quote */
    *adr++ = c;
    *crc = update_crc (*crc, c);	/* compute crc, skip quote */
    --len;
  }
  return 1;
}

unsigned long
update_crc (crc, new_ch)
unsigned long crc;
int new_ch;
{
  int i;

  for (i = 0x80; i; i >>= 1) {
    crc = (crc << 1) | (i & new_ch? 1: 0);
    if (crc & 0x10000) crc ^= GEN_CCITT;
  }
  return crc;
}
