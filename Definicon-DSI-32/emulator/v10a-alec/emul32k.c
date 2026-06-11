// emul32k.c based on Pandora.c
//#include <stdint.h>
#include <inttypes.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <errno.h>

#include "32016.h"
#include "mem32016.h"
#include "defs.h"
#undef	DEBUG 
#undef	TRACE_SERVICE

//#include "Profile.h"

#ifdef TRACE_TO_FILE
FILE* pTraceFile = NULL;
#endif
/*
; 32000 MEMORY MAP
;
;       0000            BOOTSTRAP CODE
;       0020            I/O KERNEL MODULE TABLE
;       0030            CLIBR MODULE TABLE
;       0040            CRTLIB MODULE TABLE
;       0050            VIRTUAL HANDLER MODULE TABLE
;       0080            PROGRAM'S MODULE TABLE(S)
;       1F80            DEBUGGER MODULE TABLE
;       2000            I/O SPACE FOR 8086/32000 COMMUNICATION
;       2200            KERNEL
*/
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

/* HOST <--> 32000 communication area */
struct COM32 {
   short int	sw86,		/* HOST service request word */
		rc86,		/* HOST completion code */
		sw32,		/* 32000 service request word */
		rc32,		/* 32000 completion code */
		qin86,		/* 8086 display in queue pointer -unimplemented- */
		qin32,		/* 32000 display in queue pointer -unimplemented- */
		qout86,		/* 8086 display out queue pointer -unimplemented- */
		qout32,		/* 32000 display out queue pointer -unimplemented- */
		qpr86,		/* 8086 printer queue pointer -unimplemented- */ 
		qpr32,		/* 32000 printer queue pointer -unimplemented- */
		dskrw,		/* Disk service request */
		dskid,		/* Current file ID */
		dskln;		/* # of bytes to transfer */
    unsigned int dskdta;	/* Pointer to disk data */
    short int	dskrc;		/* Disk completion code */
    unsigned int heapl,		/* Heap low */
		heaph,		/* Heap high */
		stackl,		/* Stack low */
		stackh;		/* Stack high */
    short int dskerr;		/* Disk errno variable */
    unsigned int var1,		/* general variable passing variables */
		var2,
		var3,
		var4,
		var5,
		var6,
		var7;
    short int	var8;
    unsigned int locbuf,	/* local buffer pointer */
		memtop,		/* Top of memory */
		duartcnt,	/* actual DUART base */
		exit;		/* exit from 32io to multi-task */
} __attribute__ ((packed)) *com32;

/* 32000 uses shorts for "tm" structure members. sizeof(ltm)=20 bytes */
struct ltm {
        short tm_sec;
        short tm_min;
        short tm_hour;
        short tm_mday;
        short tm_mon;
        short tm_year;
        short tm_wday;
        short tm_yday;
        short tm_isdst;
        short tm_hsec;
} __attribute__ ((packed)) *ltm;


int tubecycles = 0;
int tube_irq = 0;

#ifdef __MACH__
#include <mach/clock.h>
#include <mach/mach.h>
#endif


void current_utc_time(struct timespec *ts) {

#ifdef __MACH__ // OS X does not have clock_gettime, use clock_get_time
  clock_serv_t cclock;
  mach_timespec_t mts;
  host_get_clock_service(mach_host_self(), CALENDAR_CLOCK, &cclock);
  clock_get_time(cclock, &mts);
  mach_port_deallocate(mach_task_self(), cclock);
  ts->tv_sec = mts.tv_sec;
  ts->tv_nsec = mts.tv_nsec;
#else
  clock_gettime(CLOCK_REALTIME, ts);
#endif

}


void tubeWrite(unsigned char Address, unsigned char Data)
{
   unsigned char temp = (Data < ' ') ? ' ' : Data;
//   PiTRACE("tubeWrite(%02X, %02X) %c\n", Address, Data, temp);
   
#ifdef TRACE_TO_FILE
   putchar(temp);
#endif
}

unsigned char tubeRead(unsigned char Address)
{
 //       PiTRACE("tubeRead(%02X)\n", Address);

   return 0x40;
}

void OpenTrace(const char *pFileName)
{
#ifdef TRACE_TO_FILE
   pTraceFile = fopen(pFileName, "wb");
#endif
}

void CloseTrace(void)
{
#ifdef TRACE_TO_FILE
   if (pTraceFile)
   {
      fclose(pTraceFile);
      pTraceFile = 0;
   }
#endif
}

void n32016_dumpregs(char* pMessage)
{
   n32016_ShowRegs(0xFF);
}

void HandleTrap(void)
{
   n32016_dumpregs("HandleTrap() called");
}

void *myalloc(int size) {
    void *a;
    a = calloc(1,size);
    if (!a) { 
        printf("Can't allocate %d bytes!\n",size);
	exit(1);
    }
    return(a);
}

#ifdef DEBUG
pinfo(lmod)
struct MODDIR *lmod;
{
    int x;
    
    printf("Module ");
    
    for (x=0;x<8;x++) {
	putchar(lmod->modnm[x]);
    }
    printf(" Code start %d, Link start %d, Entry %x\n",lmod->cblk,lmod->lblk,lmod->strtaddr);
}
#endif
    
void sort(modrec,totmrec)
struct MODREC *modrec;
int totmrec;
{
    int compare();
#ifdef DEBUG
    printf("Sorting %d modules, each %lu bytes\n",totmrec,sizeof(struct MODREC));
#endif
    qsort(modrec,totmrec,sizeof(struct MODREC),compare);
}

void myerror(int err) {
   printf("Internal error %d\n",err);
   exit(1);
}
int compare(a,b)
struct MODREC *a,*b;
{
    return((a->record)-(b->record));
}


void ldblk(int fi,struct MODREC *modr,struct MODDIR *moddir)
{
    int t;
    long offset;
    unsigned char *ld;

    switch(modr->type) {
    
	case 'C':
	    if ((offset=(long) moddir->cblk*512l)>0) {

#ifdef DEBUG
printf("\t(code) Now we load %d bytes into physical address %x\n",moddir->clen,moddir->caddr);
#endif

	
		mread(fi,offset,moddir->caddr,moddir->clen);
	    }
	    break;
	
	case 'L':

	    if ((offset=(long) moddir->lblk*512l)>0) {

#ifdef DEBUG
printf("\t(link) Now we load %d bytes into physical address %x\n",moddir->llen,moddir->laddr);
#endif
	
		mread(fi,offset,moddir->laddr,moddir->llen);
	    }
	    break;

	case 'S':

	    if ((offset=(long) moddir->sbblk*512l)>0) {

#ifdef DEBUG
printf("\t(sb) Now we load %d bytes into physical address %x\n",moddir->slen,moddir->saddr);
#endif
		mread(fi,offset,moddir->saddr,moddir->slen);
	    
	    } else {
		if (strncmp(moddir->modnm,"$$$_COMM",8) == 0) 
		    zero_fill(moddir->saddr,moddir->slen);
	    }
	    break;
	    
	default:
	    printf("LOAD: Fatal error. Tried to load type \"%c\"\n",modr->type);
	    myerror(3);
    }
}


zero_fill(add,size)
long add,size;
{
    unsigned char *temp;
    int tsize;
    
    bzero(ns32016ram+add,size);
}

#define RDBUFSIZE 32*1024
unsigned char rdbuf_[RDBUFSIZE];
int rdoff_,rdbyt_,rdpos_;

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
		myerror(2);
	    rdbyt_=0;
	}
	if (rdbyt_==0) {
	    rdbyt_ = read(fd,rdbuf_,RDBUFSIZE);
	    rdpos_ = 0;
	}
	if (rdbyt_==-1)
	    myerror(3);
	
	locoff = offset - rdoff_;	/* This is where we want to be */
	    

	if (locoff < 0) {
	printf("\nWe can't back up in file! Offset=%ld,%d",offset,rdoff_);
	    exit(1);
	}
	
	if (locoff >= (long)rdbyt_) {
	    /* If want to read in the next block, go there */
	    rdoff_+= (long) rdbyt_;
	    rdbyt_=0;
	} else {
	    rdoff_+=locoff;		/* Position to correct offset */
	    rdbyt_-=(int)locoff;	/* And decrement byte count */
	    rdpos_+=(int)locoff;	/* and correct position offset */
	    lbytes = (bytes > (long)rdbyt_) ? rdbyt_ : (int)bytes;
	    memmove(ns32016ram+buffer,&rdbuf_[rdpos_],lbytes);
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


void loadit(int fi,struct GENINF *gen)
{
    struct MODDIR moddir,*directory;
    struct GMT *temp;
    struct MODREC *modrec;
    
    int t,curdir,curptr,totmrec;
    long totmod,realstart,start;
    
    
    totmod=gen->modcount;
    if (totmod <=0) {
        printf("Total module count can't be <1 (was %ld)\n",totmod);
	exit(1);
    }
    
#ifdef DEBUG
printf("Exec id=%d, Dirblk=%d, modcnt=%ld, modaddr=%d\n",gen->execid,gen->dirblk,totmod,gen->modaddr);
#endif

    /* Move general information to 32032 */
    memmove(ns32016ram+0x2020,&gen->heap_low,16);

    start=gen->modaddr;
    curdir=totmod;
    

    lseek(fi,(long) (long)(gen->dirblk*512)+((gen->mainmod - 1L)*
		    sizeof(struct MODDIR)),0);
    
    if (read(fi,&moddir,sizeof(struct MODDIR))==-1) 
	myerror(1);

    realstart = moddir.strtaddr;

    
    lseek(fi,32L,0);

    temp=(struct GMT *)myalloc(16 * (int)totmod);
    if (read(fi,temp,16 * (int)totmod)== -1)
	myerror(1);

    temp->reserved = realstart + ((&temp[(int)(gen->mainmod - 1L)])->codead);

    (&temp[1])->reserved = gen->mainmod - 1L;
    
    memmove(ns32016ram+start,temp,16*totmod);
    start += 16L * totmod;
    
    free(temp);
    
    lseek(fi,(long) (gen->dirblk*512),0);
#ifdef DEBUG
    printf("Directory size is %d\n",sizeof(struct MODDIR));
    printf("Directory at file position %ld\n",(long)(gen->dirblk*512));
#endif
    curdir=totmod*64;
    
    directory = myalloc(curdir);
    modrec=(struct MODREC *)myalloc(3 * totmod * sizeof(struct MODREC));

    if (read(fi,directory,curdir)==-1)
	myerror(1);

#ifdef DEBUG
    printf("Generating directory for %d modules, %d bytes\n",totmod,curdir);
#endif
    for (totmrec=curptr=t=0;curptr<totmod;curptr++) {
#ifdef DEBUG
        printf("rec %d, link=%d, code=%d, static=%d\n",curptr,directory[curptr].lblk,directory[curptr].cblk,directory[curptr].sbblk);
#endif
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

#ifdef DEBUG
	printf("ldblk %d\n",t);
pinfo(&directory[modrec[t].modoff]);
#endif
	
	ldblk(fi,&modrec[t],&directory[modrec[t].modoff]);
    }
    free(modrec);
    free(directory);
}
int load(char *src)
{
    struct GENINF gen;
    char namein[10240],*s;
    int x,l,fi;
    
    l = strlen(src);
    if (l >5000) {
        printf("Filename too long: %s\n",src);
	exit(1);
    }
    strcpy(namein,src);
    x = l-4;
    if (x <0 || strcmp(&namein[x],".e32"))
        strcat(namein,".e32");
    
    fi=open(namein,0);
    if (fi != -1) {
#ifdef TRACE_SERVICE
    printf("Loading %s\n",namein);
#endif
    
	if (read(fi,&gen,sizeof(struct GENINF))==-1) 
	    myerror(1);
    
	
	switch (gen.execid) {
	    case 7700:
		loadit(fi,&gen);
		break;
		
	    case 7701:
		/*
		MOVE(namein,0x80L,strlen(namein)+1);
		dovirt = TRUE;
		load("virt.e32",0);
		*/
		printf("We can't handle virtual executables\n");
		exit(1);
		break;
	    
	    default:
		printf("LOAD: \"%s\" ",namein);
		printf("Invalid execid %x\n",gen.execid);
		exit(1);
	}
	close(fi);
	
    } else {
	printf("\nLOAD: File %s not found.\n",namein);
	exit(1);
    }
    return(1);
}

filename_norm(char *s, char *d) {
    int i=0;
    while( s[i] ) {
	if (s[i]=='\\') {
	    d[i]='/';
	} else {
	    d[i]=tolower(s[i]);
	};
	i++;
    };
    d[i]='\000';
};

void dostat() {  /* ported from Definicon LOAD. Buggy */
    long var1,var2,var3;
    double res,pc,tot;

    fprintf(stderr,"\n\nMemory statistics          :    Bytes   Percent\n",'%');
    var1=com32->heapl;       /* get the heap low address */
    var2=com32->heaph;       /* get the heap high address */
    var3=ns32016ram[0x371f]; /* get CHEAPL See a 32io.map file to get CHEAPL aggress*/
    tot = (double) var2 / 1024.0 ;
    res = (double) var1 / 1024.0 ;
    pc = res / tot * 100.0;
    fprintf(stderr,"Size of program            : %8.2f K %6.2f%c\n",res,pc,'%');
/*
    res = (double) (var2 - var1) / 1024.0;
    pc = res / tot * 100.0;
    fprintf(stderr,"Memory available to program: %8.2f K %6.2f%c\n",res,pc,'%');
*/
    res = (double) (var3 - var1) / 1024.0;
    pc = res / tot * 100.0;
    fprintf(stderr,"Memory used by program     : %8.2f K %6.2f%c\n",res,pc,'%');
    res = (double) ((var2 - var1) - (var3 - var1)) / 1024.0;
    pc = res / tot * 100.0;
    fprintf(stderr,"Free memory remaining      : %8.2f K %6.2f%c\n",res,pc,'%');
    fprintf(stderr,"Total memory on card       : %8.2f K %6.2f%c\n",tot,100.0,'%');

}

int main(int argc, char* argv[], char *envp[])
{
    uint32_t addresse,count,Start,End,oops;
    uint8_t daten;
    int i,j,startarg,x;
    char lastname[10240];
    char filename1[10240];
    char filename2[10240];
    char *p;
    struct timespec starttime, endtime;
    double elapsed;
    /* for service call */
    time_t curtime;
    struct tm * timeinfo;

    int timing=0;
    int memuse=0;

    printf("Series 32000 Emulator %dMb Version 1.0a(lowercase) of 15 Aug 2017\n",MEM_MB);
    if (argc < 2) { 
       printf("Usage: load command[.e32] -a <argv>\n");
       exit(1);
    }
    n32016_init();
#ifdef INSTRUCTION_PROFILING
    memset(IP, 0, sizeof(IP));
#endif

#ifdef PANDORA_BASE
    n32016_reset_addr(PANDORA_BASE);
#else
    n32016_reset_addr(0);
#endif
    memmove(ns32016ram,start32k,sizeof(start32k));

//    OpenTrace("PandoraTrace.txt");
//       ProfileInit();
    load("32io.e32");

    startarg = 0;
    for (i=1; i < argc; i++) {
        if (argv[i][0] == '-') {
            switch(argv[i][1]) {
	       case 'a':
	       case 'b':
	           startarg = i+1;
		   break;
	       case 't':
		   timing=1;
		   break;
	       case 'm':
		   memuse=1;
		   break;
	    }
	    if (startarg) break;
	} else {
	    End = load(argv[i]);
	    strcpy(lastname,argv[i]);
	}
    }
	           
    com32 = (struct COM32 *)(ns32016ram + 0x2000);
    com32->sw32 = 1; /* issue run command */
    oops=0;
#ifdef TRACE_SERVICE
    printf("Starting execution\n");
#endif
    current_utc_time(&starttime);
    while (End) {
	tubecycles = 8;
	n32016_exec();
//	printf("%8X \n",pc);
	oops++;
	if (tubecycles<0) {break;}
	if (com32->sw86) {
	    switch (com32->sw86) {
		default:
		   printf("Unknown sw86 request %d\n",com32->sw86);
		   exit(1);
		   break;
	        case 0:
		   /* No requests */
		   break;
		case 1:
		   /* old BDOS request */
		   printf("BDOS request unsupported\n");
		   exit(1);
		case 2:
		case 3:
		   break;
		case 4:
		   /* Disk operation request */
		   switch(com32->dskrw) {
		    default:
		        printf("Unkown disk request %d\n",com32->dskrw);
			exit(1);
			break;
		    case 0:
			break;
		    case 1:
			/* Open disk file
			Pass:
				dskdta = Filename pointer
				dskid  = Open Mode, 0 = read, 1 = write, 2 = create
			Return:
				dskid = File id, (-1 => error)
			----------------------------------- */
			filename_norm(&ns32016ram[com32->dskdta],filename1);
#ifdef TRACE_SERVICE
			printf("open(\"%s\",%03o)",filename1,com32->dskid);
#endif
			com32->dskid = open(filename1,com32->dskid);
			com32->dskerr = errno;
#ifdef TRACE_SERVICE
			printf(" -> %d\n",com32->dskid);
#endif
			break;
		    case 2:
			/* Create a file
			Pass:
				dskdta = Filename pointer
				dskid  = Create mode (0 = ascii, 1 = binary)
					 -not supported on UNIX host -
			Returns:
				dskid = File id (-1 = error)
			---------------------------------- */
			filename_norm(&ns32016ram[com32->dskdta],filename1);
#ifdef TRACE_SERVICE
			printf("creat(\"%s\",0644)",filename1);
#endif
		        com32->dskid = creat(filename1,0644);
			com32->dskerr = errno;
#ifdef TRACE_SERVICE
			printf(" -> %d\n",com32->dskid);
#endif
			break;
		    case 3:
			/* Close disk file
			Pass:
				dskid = File id to close
			Return:
				dskrc = Return status (0 = good, -1 = error)
			------------------------------ */
#ifdef TRACE_SERVICE
			printf("close(%d)",com32->dskid);
#endif
		        com32->dskrc = close(com32->dskid);
			com32->dskerr = errno;
#ifdef TRACE_SERVICE
			printf(" -> %d\n",com32->dskrc);
#endif
			break;
		    case 4:
			/* Rename a file
			Pass:
				var2 = Source file string pointer
				var1 = Destination file string pointer
			Returns:
				dskdta = returns 0 if OK, non-zero if error
			-------------------------------- */
			filename_norm(&ns32016ram[com32->var1],filename1);
			filename_norm(&ns32016ram[com32->var2],filename2);
#ifdef TRACE_SERVICE
			printf("rename(\"%s\",\"%s\")",filename2,filename1);
#endif
		        com32->dskdta = rename(filename2,filename1);
			com32->dskerr = errno;
#ifdef TRACE_SERVICE
			printf(" -> %d\n",com32->dskid);
#endif
			break;
		    case 5:
		        com32->dskrc = read(com32->dskid,&ns32016ram[com32->dskdta],com32->dskln);
			com32->dskerr = errno;

			break;
		    case 6:
		        com32->dskrc = write(com32->dskid,&ns32016ram[com32->dskdta],com32->dskln);
			com32->dskerr = errno;

			break;
		    case 7:
		        com32->dskdta = lseek(com32->dskid,com32->dskdta,com32->dskln);
			com32->dskerr = errno;
			break;
		    case 8:
		        com32->dskdta = lseek(com32->dskid,0,SEEK_CUR);
			com32->dskerr = errno;
			break;
		    case 9:
			/* Erase disk file
			Pass:
				dskdta = Filename to erase
			Return:
				dskrc = Status (0 = good, -1 = error)
			---------------------------- */
			filename_norm(&ns32016ram[com32->dskdta],filename1);
#ifdef TRACE_SERVICE
			printf("unlink(\"%s\")",filename1);
#endif
		        com32->dskrc = unlink(filename1);
			com32->dskerr = errno;
#ifdef TRACE_SERVICE
			printf(" -> %d\n",com32->dskrc);
#endif
			break;
		   }
		   break;

		case 5:
		   /* Argument request (get command-line values)
			dskln  = Length of command buffer
			dskdta = Location to store the data
		    Returns:
			???(32000 R2) = Number of commands (argc)
		    --------------------------------------- */
		   {  unsigned int *ap1; unsigned char *s1;
		      int curoff;
		      ap1 = (unsigned int *)(ns32016ram + com32->dskdta);
		      j = com32->dskln;
		      if (startarg == 0  || startarg >argc)
		         i = 1;
		      else
		         i = argc-startarg + 1;
		      *ap1++ = i;j -= 4;
		      *ap1++ = 0; j -= 4;
		      curoff = strlen(lastname) + 1;

		      for (x=0; j > 4 && x <(i-1); x++, j-=4) {
		         *ap1++ = curoff;
			 curoff += strlen(argv[x+startarg]) + 1;
		      }
		      if (j < curoff) {
		          printf("Out of space for arguments\n");
			  exit(1);
		      }
		      s1 = (unsigned char *)ap1;
		      strcpy(s1,lastname);
		      s1 += strlen(lastname)+1;
		      for (x=0; x < (i-1); x++) {
		          strcpy(s1,argv[x+startarg]);
			  s1 += strlen(argv[x+startarg]) + 1;
		      }
		    }
		    break;

		case 6:
		    /* Exit */
		    End = 0;
		    break;

		case 7: 
		case 8:
		    printf("moveto/movefrom not supported\n");
		    break;

		case 9:
		case 10:
		    printf("port i/o not supported\n");
		    break;

		case 11:
		case 12:
		    printf("memory allocate/deallocate not supported\n");
		    break;

		case 13:
		    printf("8086 interrupts not supported\n");
		    break;

		case 14:
		    /* Get time
		    Pass:
			var1 = pointer to struct tm
		    Returns:
			local tm structure filled in
		    -------------------------------------- */
		    curtime = time(0);
		    timeinfo = localtime(&curtime); 
		    ltm = ( struct ltm *) &ns32016ram[com32->var1];
		    /* Translate to (short) tm struct (20 bytes) */
		    ltm->tm_sec  = (short) timeinfo->tm_sec; 
		    ltm->tm_min  = (short) timeinfo->tm_min;
		    ltm->tm_hour = (short) timeinfo->tm_hour;
		    ltm->tm_mday = (short) timeinfo->tm_mday;
		    ltm->tm_mon  = (short) timeinfo->tm_mon;
		    ltm->tm_year = (short) timeinfo->tm_year;
		    ltm->tm_wday = (short) timeinfo->tm_wday;
		    ltm->tm_yday = (short) timeinfo->tm_yday;
		    ltm->tm_isdst= (short) timeinfo->tm_isdst;
		    ltm->tm_hsec = 0; /* ???? */
		    break;

		case 15:
		    /* Get host environment
		    Pass:
			var1 = pointer to 512 bytes of memory
		    Returns:
		       Set of null-terminatad strings from the HOST environment.
		    -------------------------------------------- */
		    { unsigned char *s1 = ns32016ram+com32->var1;
		      unsigned char *s2 = ns32016ram+com32->var1+512-4;
		      char **ep = envp;
		      while (*ep && (s1 < s2)) {
		         if (((s1+strlen(*ep)+1) < s2) &&
				(strlen(*ep)< 128) ) { /* skip too long strings */
			    strcpy(s1,*ep);
			    s1 += strlen(*ep)+1;
			 };
			 ep++;
		      }
		      *s1++ = 0;
		      *s1++ = 0;
		      *s1++ = 0;
		      *s1++ = 0;
		    }

		    break;
		
		case 16:
		    /* Do the system() call (/bin/sh -c <cmd>)
		    Pass:
			var1 = pointer to the command line
		    Returns:
			??? 32000 R0 = result of system (nz if error)
		    -------------------------------------- */
		    filename_norm(ns32016ram+com32->var1,filename1);
#ifdef TRACE_SERVICE
		    printf("system(\"%s\")\n",filename1);
#endif
		    com32->var1 = system(filename1);
		    break;

		case 17:
		    /* Do exec() call
		    Pass:
			var1 = pointer to name of file
			var2 = pointer to pointer to command line args
		    Returns:
			??? 32000 R0 = -1 if failure.
		    -------------------------------------- */
		    printf("unsupported exec call\n");
		    break;
		case 18:
		    /* Do signal call */
		    printf("unsupported signal call\n");
		    break;
		case 19:
		    /* Do path service calls
		    var1 = pointer to string
		    var2 = 1 for mkdir
			 = 2 for rmdir
			 = 3 for chdir
			 = 4 for getcwd
		    ---------------------------------------- */
		   switch(com32->var2) {
		    default:
		        printf("Unkown path request %d\n",com32->var2);
			exit(1);
			break;
		    case 0:
			break;
		    case 1:
			printf("mkdir call\n");
			break;
		    case 2:
			printf("rmdir call\n");
			break;
		    case 3:
			printf("chdir call\n");
			break;
		    case 4:
			printf("getcwd call\n");
			break;
		    }
		case 20:
		    printf("unsupported system call\n");
		    break;
	    }

	    com32->sw86 = 0;
	}

    }
    if (timing){
        current_utc_time(&endtime);
        elapsed = (double)endtime.tv_sec + (double)(endtime.tv_nsec)/1000000000.0;
        elapsed -= (double)starttime.tv_sec + (double)(starttime.tv_nsec)/1000000000.0;
        if (elapsed == 0.0) elapsed = 1.0;

        printf("%1d instructions executed in %.6f seconds (%.2f MIPS).\n\n",oops,elapsed,(double)oops/1000000.0/elapsed);

    n32016_ShowRegs(0xFF);
    };

    if (memuse) {
        dostat();
    };
    return 0;
}

