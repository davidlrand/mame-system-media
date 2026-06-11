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
#undef debug 

//#include "Profile.h"

#ifdef TRACE_TO_FILE
FILE* pTraceFile = NULL;
#endif
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

struct COM32 {
   short int sw86,rc86,sw32,rc32,
             qin86,qin32,qout86,qout32,
	     qpr86,qpr32,
	     dskrw,dskid,dskln;
    unsigned int dskdta;
    short int dskrc;
    unsigned int heapl, heaph, stackl, stackh;
    short int dskerr;
    unsigned int var1,var2,var3,var4,var5,var6,var7;
    short int var8;
    unsigned int locbuf,memtop,duartcnt,exit;
} __attribute__ ((packed)) *com32;

short int *SW86;


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



#ifdef debug
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
#ifdef debug
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

#ifdef debug
printf("\t(code) Now we load %d bytes into physical address %x\n",moddir->clen,moddir->caddr);
#endif

	
		mread(fi,offset,moddir->caddr,moddir->clen);
	    }
	    break;
	
	case 'L':

	    if ((offset=(long) moddir->lblk*512l)>0) {

#ifdef debug
printf("\t(link) Now we load %d bytes into physical address %x\n",moddir->llen,moddir->laddr);
#endif
	
		mread(fi,offset,moddir->laddr,moddir->llen);
	    }
	    break;

	case 'S':

	    if ((offset=(long) moddir->sbblk*512l)>0) {

#ifdef debug
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
    
#ifdef debug
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
#ifdef debug
    printf("Directory size is %d\n",sizeof(struct MODDIR));
    printf("Directory at file position %ld\n",(long)(gen->dirblk*512));
#endif
    curdir=totmod*64;
    
    directory = myalloc(curdir);
    modrec=(struct MODREC *)myalloc(3 * totmod * sizeof(struct MODREC));

    if (read(fi,directory,curdir)==-1)
	myerror(1);

#ifdef debug
    printf("Generating directory for %d modules, %d bytes\n",totmod,curdir);
#endif
    for (totmrec=curptr=t=0;curptr<totmod;curptr++) {
#ifdef debug
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

#ifdef debug
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
#if debug
printf("Loading %s...",namein);
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

static char *Version="$Revision: 1.5 $  $Date: 2017/07/20 23:31:29 $";

int main(int argc, char* argv[], char *envp[])
{
    uint32_t addresse,count,Start,End,oops;
    uint8_t daten;
    int i,j,startarg,x,tubecycles;
    char lastname[10240];
    struct timespec starttime, endtime;
    double elapsed;


    printf("Series 32000 Emulator %s\n",Version);
    if (argc < 2) { 
       printf("Usage: emul32k file <dump start> <dump count>\n");
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
	    }
	    if (startarg) break;
	} else {
	    End = load(argv[i]);
	    strcpy(lastname,argv[i]);
	}
    }
	           
    com32 = (struct COM32 *)(ns32016ram + 0x2000);
    com32->sw32 = 1; /* issue run command */
    SW86 = &com32->sw86;
    oops=0;
    printf("Starting execution\n");
    current_utc_time(&starttime);
    while (End) {
	tubecycles = n32016_exec(8);
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
		   break;
		case 1:
		   printf("BDOS request unsupported\n");
		   exit(1);
		case 2:
		case 3:
		   break;

		case 4:
		   switch(com32->dskrw) {
		    default:
		        printf("Unkown disk request %d\n",com32->dskrw);
			exit(1);
			break;
		    case 0:
			break;
		    case 1:
			com32->dskid = open(&ns32016ram[com32->dskdta],com32->dskid);
			com32->dskerr = errno;
			break;
		    case 2:
		        com32->dskid = creat(&ns32016ram[com32->dskdta],0644);
			com32->dskerr = errno;
			break;
		    case 3:
		        close(com32->dskid);
			com32->dskerr = errno;
			break;
		    case 4:
		        com32->dskdta = rename(&ns32016ram[com32->var2],&ns32016ram[com32->var1]);
			com32->dskerr = errno;
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
		        com32->dskrc = unlink(&ns32016ram[com32->dskdta]);
			com32->dskerr = errno;
			break;
		   }
		   break;

		case 5:
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
		    printf("Not sure how to do dostime yet\n");
		    break;

		case 15:
		    { unsigned char *s1 = ns32016ram+com32->var1;
		      unsigned char *s2 = ns32016ram+com32->var1+512;
		      char **ep = envp;
		      while (*ep && s1 < s2) {
			 if (strlen(*ep) <128) {
			     strcpy(s1,*ep);
			     s1 += strlen(*ep)+1;
			 }
			 ep++;
		      }
		      *s1++ = 0;
		      *s1++ = 0;
		      *s1++ = 0;
		      *s1++ = 0;
		    }
		    break;
		
		case 16:
		    com32->var1 = system(ns32016ram+com32->var1);
		    break;

		case 17:
		case 18:
		case 19:
		case 20:
		    printf("unsupported system call\n");
		    break;
	    }

	    com32->sw86 = 0;
	}

    }
    current_utc_time(&endtime);
   elapsed = (double)endtime.tv_sec + (double)(endtime.tv_nsec)/1000000000.0;
    elapsed -= (double)starttime.tv_sec + (double)(starttime.tv_nsec)/1000000000.0;
    if (elapsed == 0.0) elapsed = 1.0;

    printf("%1d instructions executed in %.6f seconds (%.2f MIPS).\n\n",oops,elapsed,(double)oops/1000000.0/elapsed);

    n32016_ShowRegs(0xFF);
    return 0;
}

