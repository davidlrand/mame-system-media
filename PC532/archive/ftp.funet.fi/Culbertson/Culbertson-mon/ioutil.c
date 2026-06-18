/* NSC 32000 ROM debugger.
 * Bruce Culbertson  Bob Krause
 *
 * Code for more-like output.  Code to read and write files in UNIX and
 * MINIX versions.
 */

#include "debugger.h"
#if LSC
#  include <unix.h>
#  include "a.out.h"
#  include <storage.h>
#else
#  if !STANDALONE
#    include <fcntl.h>
#  endif
#  include "../../include/a.out.h"
#endif
#include "../../include/magic.h"
#include "../../include/conv.h"

#define CTRL(x) (x - 'A' + 1)	/* ASCII control code */
#define BEL 7

extern char *malloc();
long screenLength = 24;             /* number of lines on the screen */
int screenShown = 1;                /* # of lines printed since prompt */
int screenIgnore = 0;               /* on if discarding output */

morePrintf(increment, a0, a1, a2, a3, a4 ,a5, a6, a7, a8, a9)
int increment;
char *a0, *a1, *a2, *a3, *a4, *a5, *a6, *a7, *a8, *a9;
{
    unsigned int reply;
    
    if (screenIgnore)               /* if throwing away output */
        return;
    
    while (screenShown >= screenLength) {
        myPrintf("<MORE>...");
        reply = cooked_getc();
/*      myPrintf("\b\b\b\b\b\b\b\b\b");  */
        myPrintf("\r            \r");
        switch (reply) {
            case ' ' :                  /* ready for another screen full */
            screenShown = 1;
            break;
        
            case '\n':                  /* print just one more line */
            screenShown--;
            break;
        
	    case 'Q':
	    case 'q':
            case '\033' :               /* throw away output until next prompt */
            screenIgnore = 1;
            return;
            
            default :
            myPrintf("%c", BEL);
            continue;
        }
    }
    myPrintf(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9);
    screenShown += increment;
    
    return;
}

myprompt(buffer, length, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9)
char *buffer;
int length;
char *a0, *a1, *a2, *a3, *a4, *a5, *a6, *a7, *a8, *a9;
{
    int i;
    
    myPrintf(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9);
    for (i = 0; i < length -1; i++) {
        buffer[i] = (char)cooked_getc();
        if (buffer[i] == -1) {
            buffer[i] = '\0';
            break;
        } else if (buffer[i] == '\b') {
            if (i>0) {
                myPrintf("\b \b");
                i -= 2;                 /* delete this and previous char */
	    } else {
		cooked_putc (BEL);
		i -= 1;
	    }
        } else if (buffer[i] == CTRL('U')) {
	  while (i-- > 0) myPrintf("\b \b");
	  i = -1;
        } else {
            cooked_putc (buffer[i]);
            if (buffer[i] == '\n') {
                buffer[i +1] = '\0';
                break;
            }
        }
    }
    screenShown = 1;
    screenIgnore = 0;
    
    return;
}

#if !STANDALONE
printHdr(objHdr)
struct exec *objHdr;
{
    char str[12];
    int objFile = True;
    int objType;
    long magic;

    magic = WM2L(objHdr->a_magic);		/* magic now long */
    
    if (magic == EXEC_MAGIC) {
        morePrintf(0, "Magic = Executable, ");
        objType = 1;
    }
    else if (magic == RELOC_MAGIC) {
        morePrintf(0, "Magic = Relocatable, ");
        objType = 1;
    }
    else if (magic == AR_MAGIC) {
        morePrintf(0, "Magic = Archive");
        objType = 0;
    }
    else {
        morePrintf(1, "Magic = 0x%lx\n", magic);
        return;
    }
    if (objType) {
        long offset = sizeof(struct exec),
           entry, text, data, bss, trsize, drsize, sym, str;
                
        entry = WM2L(objHdr->a_entry);
        text = WM2L(objHdr->a_text);
        data = WM2L(objHdr->a_data);
        bss = WM2L(objHdr->a_bss);
        trsize = WM2L(objHdr->a_trsize);
        drsize = WM2L(objHdr->a_drsize);
        sym = WM2L(objHdr->a_sym);
        str = WM2L(objHdr->a_str);

        morePrintf(1, "EntryPoint = 0x%lx\n", entry);
        morePrintf(1, "\t\t\tOffset\tLength\n");
        if (text) {
            morePrintf(1, "  Text Segment\t-\t0x%lx\t0x%lx\n", offset,
              text);
            offset += text;
        }
        if (data) {
            morePrintf(1, "  Data Segment\t-\t0x%lx\t0x%lx\n", offset,
              data);
            offset += data;
        }
        if (bss) {
            morePrintf(1, "  BSS Segment\t-\t0x%lx\t0x%lx\n", offset,
              bss);
            offset += bss;
        }
        if (trsize) {
            morePrintf(1, "  Text Reloc. Table -\t0x%lx\t0x%lx\n", offset,
              trsize);
            offset += trsize;
        }
        if (drsize) {
            morePrintf(1, "  Data Reloc. Table -\t0x%lx\t0x%lx\n", offset,
              drsize);
            offset += drsize;
        }
        if (sym) {
            morePrintf(1, "  Symbol Table\t-\t0x%lx\t0x%lx\n", offset,
              sym);
            offset += sym;
        }
        if (str)
            morePrintf(2, "  String Table\t-\t0x%lx\t0x%lx\n\n", offset, str);
    }
    else {
        morePrintf(1, "archive info\n");
    }
}

#define DATA_SIZE 0x10000

getfile (p)
char *p;
{
    char filename [LNLEN], *malloc(), *q;
    int adr = 0, i;
    long foo;

    /* Do not use scanToken, it maps case */
    for (; *p == ' ' || *p == '\t'; ++p);
    for (q = filename; *p != '\0' && *p != '\n' && *p != '\r' && *p != ' ' &&
	*p != '\t';) *q++ = *p++;
    *q = '\0';
    if (filename[0] == '\0') {
        morePrintf(1, "Missing file name\n");
        return;
    }

    if (BAD_NUM == getIntScan (&p, &adr)) {
        morePrintf(1, "Bad address\n");
        return;
    }

    if ((i = open (filename, O_RDONLY))!= -1) {
        fileTop = lseek(i, 0L, 2);
        if (NULL == (fileBase = malloc (adr + fileTop))) {
          close (i);
          morePrintf(1, "Not enough memory\n");
          return;
        }
        lseek(i, 0L, 0);            /* rewind to beginning of file */
        morePrintf(1, "%d bytes read\n",
          read (i, fileBase + adr, (unsigned int)fileTop));
        close (i);
        fileTop += (long)fileBase;  /* fileTop is offset from fileBase to EOF */
        if (adr == 0) {
            struct exec *objHdr = (struct exec *)fileBase;
            printHdr(objHdr);
        }        
    } else
        morePrintf(1, "Could not open %s\n", filename);
    Dot = 0;
}

putfile (p)
char *p;
{
  char filename [LNLEN], cntstr [LNLEN], adrstr [LNLEN];
  int adr, cnt, i;

  if (3 != sscanf (p, "%s %s %s", filename, adrstr, cntstr) ||
  BAD_NUM == getInt (adrstr, &adr) || BAD_NUM == getInt (cntstr, &cnt)){
    morePrintf(1, "Missing or bad argument\n");
    return;
  }
  if (-1 != (i = open (filename, O_WRONLY | O_CREAT, 0777))) {
    morePrintf(1, "%d bytes written\n",
      write (i, fileBase + adr, cnt));
    close (i);
  } else morePrintf(1, "Could not open %s\n", filename);
}
#endif /* STANDALONE */

/* Does Unix style mapping of cr to nl.  Probably should be used for all
 * io rather than raw getch.
 */
int
cooked_getc()
{
  int ret;

  if ('\r' == (ret = getch())) return '\n';
  else return ret;
}
