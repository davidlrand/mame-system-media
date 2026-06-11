#include <stdio.h>
#include <ctype.h>
#include <errno.h>
#include "32kdef.h"

#define _OP_EOF 0x1000

int fi,open(),openx(),openxa();
#if PCDOS==0
long lseekb();
#endif

long lseek();

#define MAXARG 300
#define MAX 512
#define RDBUFSIZE 32640
#define MAXOPF 40
    
unsigned char *rdbuf_;		/* pointer to 32K buffer */
long rdoff_;		/* Current file offset */
int  rdbyt_;		/* Current number of bytes in buffer */
int  rdpos_;		/* Current offset into buffer */
unsigned char *buffer,*fixn;
long start;

static char *erm[]={
"Loader version 2.00  10/08/85 (c) 1985 Definicon Systems, Inc.",
"File too short! Possible non-32k file.",
"Seek error during program load. Possible non-32k file.",
"File read error during program load. File corrupt.",
"Could not allocate sufficient memory.",
"is not a 32000 executable file."
};

int ftype[MAXOPF];

/*
CSD-32000 Cross-Assembler V1.08 LISTING OF: LOADER.A32			PAGE 1

00000000 PC 6FAD20		     1  	LPRD	SB,@H'20
00000003 PC EDA70020		     2  	LPRW	MOD,H'20
00000007 PC 17A82C		     3  	MOVD	@H'2C,R0
0000000A PC 7F02		     4  	JUMP	R0
0 Assembly error(s) detected. 
*/

static unsigned char start32k[]={0x6f,0xad,0x20,0xed,0xa7,0x00,0x20,0x17,0xa8,0x2c,
			0x7f,0x02};
int _argc=0;
unsigned char *_argv[MAXARG];
unsigned char *lastnm;

extern VOID MOVE();
extern VOID MOVE1();
extern VOID INIT();
extern VOID EXEC();
extern VOID EXECT();
extern int GETSYS();


VOID error();
#if PCDOS != 1
VOID fix();
#endif

main(argc,argv)
int argc;
unsigned char *argv[];
{
    int time,x,mstat;
    unsigned char *tq,*myalloc();
    VOID dostat(),load();

printf("BETA TEST ONLY LOADER\n");

    for (x=0; x < MAXOPF; x++) 
	ftype[x] = 0;		/* All files in binary */

    ftype[0]=ftype[1]=ftype[2]=1;	/* Except STDIN, STDOUT, STDERR */
    for (x=argc; x>=0; x--) 
	strtol(argv[x]);
    /* Get local buffer space */
    
    buffer = myalloc(MAX);
    fixn = myalloc(30);
    lastnm = myalloc(30);
    rdbuf_ = myalloc(RDBUFSIZE);
    
    /* init 32k page zero */
    INIT();
    
    MOVE(start32k,0L,sizeof(start32k));
    
    /* Zero fill the communication area */
    tq=myalloc(4096);
    MOVE(tq,20L,4096);
    MOVE(tq,0x1000L,4096);
    MOVE(tq,8192L,4096);
    free(tq);
    
    mstat=time=0;

    load("32io");
#if debug
printf("\n");
#endif
    if (argc>1) {
	while (--argc) {
	    argv++;
	    if ((*argv[0]=='-') && (*(argv[0]+1)=='a')) {
		++argv;
		--argc;
		break;
	    }
	    if ((*argv[0]=='-')) {
		if (*(argv[0]+1)=='t')
		    time=TRUE;
		else if (*(argv[0]+1) == 'm')
		    mstat = TRUE;
	    } else {
		strcpy(lastnm,*argv);
		load(*argv);

#if debug
		printf("\n");
#endif
	    }
	}
	
	_argc=argc;
	for (x=0;x<=argc;x++)
	    _argv[x]=argv[x];
	
#if debug
	printf("\nStarting execution...\n\n");
#endif
	free(rdbuf_);
	if (time) 
	    EXECT();
	else 
	    EXEC();
	
	if (mstat) 
	    dostat();
	INIT();
    } else {
	printf(erm[0]);
	MOVE1(0x2267L,fixn,12);
	fixn[12]='\0';
	printf("\n32000 Kernel %s\n\n",fixn);
	printf("\nYou must supply a name to load.");
	exit(1);
    }
    
}

strtol(s)
unsigned char *s;
{
    while (*s)
	*s++ = tolower(*s);
}

VOID dostat() {
    long var1,var2,var3;
    double res,pc,tot;
    
    printf("\n\nMemory statistics          :   Bytes   Percent\n",'%');
    MOVE1(0x2020L,&var1,4); /* get the heap low address */
    MOVE1(0x2024L,&var2,4); /* get the heap high address */
    MOVE1(0x371fL,&var3,4); /* get CHEAPL */
    tot =  (double) var2 / 1024.0 ;
    res = (double) var1 / 1024.0 ;
    pc = res / tot * 100.0;
    printf("Size of program            : %7.2f K %6.2f%c\n",res,pc,'%');
/*
    res = (double) (var2 - var1) / 1024.0;
    pc = res / tot * 100.0;
    printf("Memory available to program: %7.2f K %6.2f%c\n",res,pc,'%');
*/
    res = (double) (var3 - var1) / 1024.0;
    pc = res / tot * 100.0;
    printf("Memory used by program     : %7.2f K %6.2f%c\n",res,pc,'%');
    res = (double) ((var2 - var1) - (var3 - var1)) / 1024.0;
    pc = res / tot * 100.0;
    printf("Free memory remaining      : %7.2f K %6.2f%c\n",res,pc,'%');
    printf("Total memory on card       : %7.2f K %6.2f%c\n",tot,100.0,'%');

}


VOID load(s)
unsigned char *s;
{
    struct GENINF *gen;
    unsigned char *namein,*fixname();
    VOID loadit();
    
    namein=fixname(s);
    
    fi=openx(namein,0);
    if (fi != -1) {
#if debug
printf("Loading %s...",namein);
#endif
    
	if (read(fi,buffer,sizeof(struct GENINF))==-1) 
	    error(1);
    
	gen = (struct GENINF *)buffer;
	
	if (gen->execid == 7700)
	    loadit(fi);
	else {
	    printf("\"%s\" ",namein);
	    error(5);
	}
	close(fi);
    } else {
	printf("\nFile %s not found.",namein);
	exit(1);
    }
}

unsigned char *fixname(s)
unsigned char *s;
{
    unsigned char *index();
    strcpy(fixn,s);
    if (index(fixn,'.')==NULL) {
	strcat(fixn,".e32");
    }
    return(fixn);
}

VOID loadit(fi)
int fi;
{
    struct GENINF *gen;
    struct MODDIR *mod,*directory;
    struct GMT *temp;
    struct MODREC *modrec;
    
    int t,curdir,curptr,totmrec;
    long totmod,realstart;
    unsigned char *myalloc();
    
    VOID sort(),ldblk();
    
    gen = (struct GENINF *)buffer;
    totmod=gen->modcount;
    
    /* Move general information to 32032 */
    MOVE(&gen->heap_low,0x2020L,16);

#ifdef debug
printf("Exec id=%d, Dirblk=%d, modcnt=%ld, modaddr=%ld\n",gen->execid,gen->dirblk,totmod,gen->modaddr);
#endif

    start=gen->modaddr;
    curdir=totmod;
    
    mod=(struct MODDIR *)myalloc(sizeof(struct MODDIR));

    lseek(fi,(long) (long)(gen->dirblk*512)+((gen->mainmod - 1L)*
		    sizeof(struct MODDIR)),0);
    
    if (read(fi,mod,sizeof(struct MODDIR))==-1) 
	error(1);

    realstart = mod->strtaddr;

    free(mod);
    
    lseek(fi,32L,0);

    temp=(struct GMT *)myalloc(16 * (int)totmod);
    if (read(fi,temp,16 * (int)totmod)== -1)
	error(1);

    temp->reserved = realstart + ((&temp[(int)(gen->mainmod - 1L)])->codead);

    (&temp[1])->reserved = gen->mainmod - 1L;
    
    MOVE(temp,start,16 * (int)totmod);
    start += 16L * totmod;
    
    free(temp);
    
    lseek(fi,(long) (gen->dirblk*512),0);
    curdir=totmod*64;
    
    directory=(struct MODDIR *)myalloc(curdir);
    modrec=(struct MODREC *)myalloc(3 * totmod * sizeof(struct MODREC));

    if (read(fi,directory,curdir)==-1)
	error(1);

    for (totmrec=curptr=t=0;curptr<totmod;curptr++) {
	modrec[t].record=directory[curptr].lblk;
	modrec[t].modoff=curptr;
	modrec[t].type='L';			/* Link information */
	t++;
	
	modrec[t].record=directory[curptr].cblk;
	modrec[t].modoff=curptr;
	modrec[t].type='C';			/* Code information */
	t++;
	
  
	modrec[t].record=directory[curptr].sbblk;
	modrec[t].modoff=curptr;
	modrec[t].type='S';
	t++;					/* Static base information */
    }
    if (t) {
	totmrec = t;
	sort(modrec,totmrec);
    }

    rdoff_ = -1;			/* Right now, we are nowhere */
    rdbyt_ = 0;				/* And we have no bytes in buffer */
    for (t=0;t<totmrec;t++) {

#ifdef debug
pinfo(&directory[modrec[t].modoff]);
#endif

	ldblk(fi,&modrec[t],&directory[modrec[t].modoff]);
    }
    free(modrec);
    free(directory);
}

#ifdef debug
pinfo(mod)
struct MODDIR *mod;
{
    int x;
    
    printf("Module ");
    
    for (x=0;x<8;x++) {
	putchar(mod->modnm[x]);
    }
    printf(" Code start %d, Link start %d, Entry %lx\n",mod->cblk,mod->lblk,mod->strtaddr);
}
#endif
    
VOID sort(modrec,totmrec)
struct MODREC *modrec;
int totmrec;
{
    int compare();
    qsort(modrec,totmrec,sizeof(struct MODREC),compare);
}

int compare(a,b)
struct MODREC *a,*b;
{
    return((a->record)-(b->record));
}


VOID ldblk(fi,modr,mod)
int fi;
struct MODREC *modr;
struct MODDIR *mod;
{
    int t;
    long offset;
    unsigned char *ld,*myalloc();

    switch(modr->type) {
    
	case 'C':
	    if ((offset=(long) mod->cblk*512l)>0) {

#ifdef debug
printf("\t(code) Now we load %ld bytes into physical address %lx\n",mod->clen,mod->caddr);
#endif

	
		mread(fi,offset,mod->caddr,mod->clen);
	    }
	    break;
	
	case 'L':

	    if ((offset=(long) mod->lblk*512l)>0) {

#ifdef debug
printf("\t(link) Now we load %ld bytes into physical address %lx\n",mod->llen,mod->laddr);
#endif
	
		mread(fi,offset,mod->laddr,mod->llen);
	    }
	    break;

	case 'S':

	    if ((offset=(long) mod->sbblk*512l)>0) {

#ifdef debug
printf("\t(sb) Now we load %ld bytes into physical address %lx\n",mod->slen,mod->saddr);
#endif
	
		mread(fi,offset,mod->saddr,mod->slen);
	    }
	    break;
	    
	default:
	    printf("Fatal error. Tried to load type \"%c\"\n",modr->type);
	    error(3);
    }
}

    
VOID error(x)
int x;
{
    printf(erm[x]);
    exit(1);
}

unsigned char *myalloc(x)
int x;
{
    unsigned char *t,*calloc();
    if ((t=calloc(x,1))==NULL) {
	printf("\n\n%d bytes: ",x);
	error(4);
    }
    return(t);
}


int _OPEN(s,x)
long s;
int x;
{
    unsigned char file[100];
    register int i;
    
    MOVE1(s,file,99);

#if debug
printf("Open request for \"%s\"\n",file);
#endif

#if PCDOS != 1
    fix(file);
#endif

    if (x & 4) 
	i = openx(file, x&3);
    else 
	i = openxa(file, x&3);
    
    if ((x & _OP_EOF) && (i > 0))
	lseek(i,0L,2);  /* Seek to end of file, if that's what he wants.. */
	
    return(i);
}

#if PCDOS != 1

VOID fix(s)
unsigned char *s;
{
    unsigned char *t,*t1;
    int len;
    
    t1=t=s;
    
    len=strlen(s);
    t1+=len;
    
    while ((*t1!='/') && (len != 0)) {
	len--;
	t1--;
    }
    if (len) 
	strcpy(t,++t1);
}
#endif

int _CREA(s,x)
long s;
int x;
{
    unsigned char file[100];
    register int i;
    
    MOVE1(s,file,99);

#if debug
printf("Create request for \"%s\"\n",file);
#endif

#if PCDOS != 1
    fix(file);
#endif

    if (x & 4) {
	i = creat (file, x&3);
	if (i > 0) ftype[i] = 0;
    } else {
	i = creat( file, x);
	if (i > 0) ftype[i] = 1;
    }
	
    return(i);
}
    
int _CLOS(x)
int x;
{
    static unsigned char eof=0x1a;

#if debug
printf("Close request for %d\n",x);
#endif
    
    if (ftype[x] == 2)
	write(x,&eof,1);
	
    ftype[x] = 0;
    return(close(x));
}

int _READ(fd,buffer,bytes)
int fd;
long buffer;
int bytes;
{
    unsigned char *tbuf,*myalloc();
    int tret;
    
#if debug
printf("Reading %d bytes from %d into %8lx\n",bytes,fd,buffer);
#endif

    tbuf=myalloc(bytes);
	
    if (ftype[fd])
	tret = reada(fd,tbuf,bytes);
    else
	tret = read(fd,tbuf,bytes);
	
    if (tret > 0) {
	MOVE(tbuf,buffer,bytes);
	errno = 0;
    }
	
    free(tbuf);
    return(tret);
}

reada(fd,tbuf,bytes)
int fd,bytes;
unsigned char *tbuf;
{
    int lbytes, lret, tbytes, endof;
    register unsigned char *pt1, *pt2;
    
    endof = tbytes = 0;
    lret = read(fd,tbuf,bytes);
    pt1 = pt2 = tbuf;
    
    while (lret > 0) {
	pt1 = pt2;
    
	for (lbytes = lret; lbytes > 0; lbytes--) {
	    if (ftype[fd] & 4 && lbytes == lret) {
		ftype[fd] &= 4;
		if (*pt1 == 0x0a) {
		    pt1++;
		    lbytes--;
		}
	    } else {
		if (*pt1 == 0x1a) {
		    endof = 1;
		    pt1 = tbuf + lret;
		    break;
		}
		if (*pt1 == 0x0d && lbytes > 1 && *(pt1 + 1) == 0x0a) {
		    lbytes--;
		    pt1++;
		} else {
		    if (*pt1 == 0x0d && lbytes == 1) {
			*pt1 = 0x0a;
			ftype[fd] |= 4; /* tell the world we will spcl case */
		    }
		}
		*pt2++ = *pt1++;
	    }
	}
	lret -= (pt1 - pt2);
	tbytes += lret;
	if (tbytes == bytes || fd < 3 || endof)
	    break;
	else 
	    lret = read(fd,pt2,bytes - tbytes);
    }
    return(tbytes);
}

    
int _WRIT(fd,buffer,bytes)
int fd;
long buffer;
int bytes;
{
    unsigned char *tbuf,*myalloc();
    int tret;
    
#if debug
printf("Writing %d bytes from %d into %8lx\n",bytes,fd,buffer);
#endif

    tbuf=myalloc(bytes);
    MOVE1(buffer,tbuf,bytes);
	
    if (ftype[fd])
	tret = writea(fd,tbuf,bytes);
    else
	tret = write(fd,tbuf,bytes);
	
    if (tret > 0) errno = 0;
    
    free(tbuf);
    return(tret);
}

writea(fd,tbuf,bytes)
int fd,bytes;
unsigned char *tbuf;
{
    int lret,lbytes,x;
    unsigned char *pt1,*pt2;
    static unsigned char cr = 0x0d;
    
    ftype[fd] = 2;
    pt1 = pt2 = tbuf;
	
    for (lbytes = bytes; lbytes > 0; lbytes--) {
	if (*pt1 == 0x0a) {
	    x = pt1 - pt2;
	    if ( (lret = write(fd,pt2,x)) != x)
		goto error;
	    if (write(fd,&cr,1) != 1)
		goto error;
	    pt2 = pt1;
	}
	pt1++;
    }
    lret = write(fd,pt2,(pt1 - pt2));
error:
    return(lret);
}

	
	
    

int _RENAM(file2,file1)
long file1,file2;
{
    unsigned char f1[100],f2[100];
    int ret;
    
    MOVE1(file1,f1,99);
    MOVE1(file2,f2,99);
    
#if debug
printf("Renaming \"%s\" to \"%s\"\n",f1,f2);
#endif
    
    if ((ret=open(f2,0)) != -1) {
	close(ret);
	ret = 1;
    } else {
	ret = rename(f1,f2);
    }
    return(ret);
}


long _SEEK(fd,offset,ptrname)
int fd;
long offset;
int ptrname;
{

    register long x;
    
#if PCDOS==0
    if (ftype[fd]) 
	x = lseek(fd,offset,ptrname);
    else
	x = lseekb(fd,offset,ptrname);
	
#endif

#if PCDOS==1
    x = lseek(fd,offset,ptrname);
#endif
    
    
#if debug
printf("Seeking in %d to %ld, with a mode of %d\n",fd,offset,ptrname);
#endif

    return(x);
}

long _TELL(fd)
int fd;
{
    register long x;

    x = _SEEK(fd,0L,1);
#if debug
printf("Telling location of %d = %ld\n",fd,x);
#endif

    return(x);
}


    

int _UNLI(s,l)
long s;
int l;
{
    unsigned char file[100];
    MOVE1(s,file,99);

#if debug
printf("Unlinking %s\n",file);
#endif

    return(unlink(file));
}
    
    

_ARGP(len,dta)
long dta;
int len;
{
    unsigned char *loc,*temp,*myalloc();
    int curoff=0,x;
    long *t1;

    loc=temp=myalloc(len);

    t1=(long *)loc;
    
    *t1++=(long) _argc + 1L;
    
    *t1++ = (long) curoff;
    curoff += strlen(lastnm)+1;
    
    for (x=0;x<_argc;x++) {
	*t1++ = (long) curoff;
	curoff+=strlen(_argv[x])+1;
    }
    
    temp=(unsigned char *)t1;
    strcpy(temp,lastnm);
    temp += strlen(lastnm)+1;
    
    for (x=0;x<_argc;x++) {
	if (_argv[x][0]=='-' &&
	   (_argv[x][1]=='d' || _argv[x][1]=='x' || _argv[x][1]=='e' ||
	    _argv[x][1]=='z' || _argv[x][1]=='s'))
	    _argv[x][1]=toupper(_argv[x][1]);
	if (strcmp(_argv[x],"-o1")==0)
	    strcpy(_argv[x],"-O ");
	if (strcmp(_argv[x],"-o2")==0)
	    strcpy(_argv[x],"-O2");
	strcpy(temp,_argv[x]);
	temp+=strlen(_argv[x])+1;
    }
    
    
    MOVE(loc,dta,len);
    free(loc);
    
}

int mread(fd,offset,buffer,bytes)
int fd;
long offset,buffer;
long bytes;
{
    long locoff;
    register int lbytes;
    
    while (bytes) {	/* While we still have bytes to load */
	if (rdoff_ == -1) {	/* we don't know where we are */
	    rdoff_ = offset;	/* Point to current location */
	    if (lseek(fd,offset,0)==-1)
		error(2);
	    rdbyt_=0;
	}
	if (rdbyt_==0) {
	    rdbyt_ = read(fd,rdbuf_,RDBUFSIZE);
	    rdpos_ = 0;
	}
	if (rdbyt_==-1)
	    error(3);
	
	locoff = offset - rdoff_;	/* This is where we want to be */
	    

	if (locoff < 0) {
	printf("\nWe can't back up in file! Offset=%ld,%ld",offset,rdoff_);
	    exit(1);
	}
	
	if (locoff > (long)rdbyt_) {
	    /* If want to read in the next block, go there */
	    rdoff_+= (long) rdbyt_;
	    rdbyt_=0;
	} else {
	    rdoff_+=locoff;		/* Position to correct offset */
	    rdbyt_-=(int)locoff;	/* And decrement byte count */
	    rdpos_+=(int)locoff;	/* and correct position offset */
	    lbytes = (bytes > (long)rdbyt_) ? rdbyt_ : (int)bytes;
	    MOVE(&rdbuf_[rdpos_],buffer,lbytes);
	    buffer+= (long) lbytes;
	    bytes -= (long) lbytes;
	    offset+= (long) lbytes;
	    rdoff_+= (long) lbytes;
	    rdbyt_-= lbytes;
	    rdpos_+= lbytes;
	}
    }
    return(1);
}


    

int openx(s,i)
register unsigned char *s;
int i;
{
    unsigned char lclnam[100], *index();
#if PCDOS ==1
    char *getenv(), *env;
#endif
    int r1;
    
    if (s[1]==':') {
	r1 = open(s,i);
	goto ret;
    }
    
    if ((r1=open(s,i))!=-1)
	goto ret;
    
#if PCDOS != 1
    lclnam[0]='A'+GETSYS();
    lclnam[1]=':';
    lclnam[2]='\0';
    strcat(lclnam,s);
    r1 = open(lclnam,i);
#endif
#if PCDOS == 1
    if (index(s,'/') == NULL && index(s,'\\')== NULL) {
	if ( (env = getenv("DSI")) != NULL) {
	    while (parse(lclnam,s,&env)) {
		if ((r1 = open(lclnam,i)) != -1) 
		    break;
	    }
	} else if ( (env = getenv("PATH")) != NULL) {
	    while (parse(lclnam,s,&env)) {
		if ((r1 = open(lclnam,i)) != -1)
		    break;
	    }
	}
    }
#endif
		
ret:
    if (r1 >= 0)
	ftype[r1] = 0;

    return(r1);
}

#if PCDOS == 1
parse(d,s,cd)
register char *d, *s;
char **cd;
{
    char *dir;
    
    if (**cd) {
	dir = *cd;
	while ((*dir != ';') && (*dir != '\0')) 
	    *d++ = *dir++;
	
	if (*dir) dir++;
	--d;
	if (*d != '/' && *d != '\\')
	    *++d = '\\';
	d++;
	
	while (*s) *d++ = *s++;
	
	*d = '\0';
	
	*cd = dir;
	
return(TRUE);
    }
return(FALSE);
}
#endif

	    

	
int openxa(s,i)
register unsigned char *s;
int i;
{
    unsigned char lclnam[100];
#if PCDOS ==1
    char *getenv(), *env;
#endif
    int r1;
    
    if (s[1]==':') {
	r1 = open(s,i);
	goto ret;
    }
    
    if ((r1=open(s,i))!=-1)
	goto ret;
    
	
#if PCDOS != 1
    lclnam[0]='A'+GETSYS();
    lclnam[1]=':';
    lclnam[2]='\0';
    strcat(lclnam,s);
    r1 = open(lclnam,i);
#endif
#if PCDOS == 1
    if (index(s,'/') == NULL && index(s,'\\')== NULL) {
	if ( (env = getenv("DSI")) != NULL) {
	    while (parse(lclnam,s,&env)) {
		if ((r1 = open(lclnam,i)) != -1) 
		    break;
	    }
	} else if ( (env = getenv("PATH")) != NULL) {
	    while (parse(lclnam,s,&env)) {
		if ((r1 = open(lclnam,i)) != -1)
		    break;
	    }
	}
    }
#endif
		
    
ret:
    if (r1 >= 0 )
	ftype[r1] = 1;
    return(r1);
}

	