#include "stdio.h"
#include "errno.h"
#include "ctype.h"
#define and &&
#define or  ||
#define not !
#define orif  ||
#define andif &&
#define elseif else if
#define forever for(;;)
#define DO_NOTHING
#define internal
typedef char *STRING ;
typedef char BOOLEAN ;
typedef int (*intfunc)();
#define TRUE  1
#define FALSE 0
#define alloc_len(pt)	(8*(*(((int *) pt)-1))-sizeof( int ))
#ifdef debug
extern int g_errno ;
#else
#define g_errno errno
#define g_open  open
#define g_creat creat
#define g_close close
#define g_read  read
#define g_write write
#define g_sbrk  sbrk
#endif
