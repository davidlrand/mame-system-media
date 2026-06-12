#include <stdio.h>
#include <ctype.h>
#include "sys/types.h"
#include "sys/param.h"
#include "sys/filsys.h"
#include "sys/ino.h"
#include "aout.h"
/*
    Dave Rand
    72 Longfellow St.
    Thousand Oaks, CA  91360
    805-493-1987
    
    Version 1.4  Wed Aug  6 11:49:55 1986

    This program is based on rboot.c, and is used to serve as the
    I/O processor for the 32000 cards. It also invokes unix, and
    has limited unix file input available.
    
    Debug options are available when the program is compiled with
    DEBUG defined.

*/

#define LOADNAME	"unix"  /* Name of file we need to read */
#define CONFIGFILE	"unixconf" /* Name of config file */

#define MAXFBLOCK 1034	/* This would be a 1 meg file */
    			/* Warning: This value MUST ALWAYS be 10 plus
    			   a multiple of 256. This allows for the 10
    			   'root' entries in the directory, plus 256
    			   for each major block. Setting it otherwise
    			   may cause cancer and/or the premature collapse
    			   of the known universe.
    			 */

	/* To get to inode x, use lseek(fi,SEEKINODE(x),0) */
#define SEEKINODE(x)  ((x) - 1L) * 64L + 2048L
#define GETOFF(x)  (block[(x)] * 1024L)
#define SEEKUNIX(x)  (GETOFF((int)((x) / 1024L)) + ((x) % 1024L))

#ifdef DEBUG
int dosup = 0;		/* Don't display superblock info */
int dodir = 0;		/* Don't display directory info */
char *destname = NULL;	/* Don't create an output file */
int doboot= 1;		/* Don't display boot information */
#endif

int CPUtype;		/* CPU information */
char *cpumes[]= {
    "n 8088","n 8086"," NEC V20"," NECV30",
    "n 80188","n 80186","n Unknown","n 80286"
};

extern INIT(),MOVE_TO(),MOVE_ZTO(),DOIO();

main(argc,argv)
int argc;
char **argv;
{
    int x;
    char *doname,lbuf[128],tname[80],*fgets();
    FILE *fi,*fopen();
    long start,doread();

    CPUtype = getcpu();
    
    doname = CONFIGFILE;
    if ((fi = fopen(doname,"r")) == NULL) 
	abort("Can't locate unix configure file: %s\n",doname);
    
    while (fgets(lbuf,128,fi) != NULL) {
	if (instr(lbuf,"dosdisk 0 ") != -1) {
	    sscanf(lbuf,"%s %d %d %s",tname,&x,&x,tname);
	    if ((x = strlen(tname)) <2 || x > 64)
		abort("Root filename is invalid: %s\n",tname);
	    break;
	}
    }
    fclose(fi);
    
    if (x=INIT(0))
	abort("Board not present, or bad memory: %d\n",x);
    
    for (x=1; x<argc; x++) {
	if (argv[x][0] == '-')
	    switch(toupper(argv[x][1])) {
		case 'I':
		    fprintf(stderr,"CPU is a%s\n",cpumes[CPUtype]);
		    if (argv[x][2]!='\0') {
			sscanf(&argv[x][2],"%d",&CPUtype);
			fprintf(stderr,"CPU forced to a%s\n",cpumes[CPUtype]);
		    }
		    break;
#ifdef DEBUG
		case 'S':
		    dosup = 1;
		    break;
		    
		case 'D':
		    dodir = 1;
		    break;

		case 'O':
		    x++;
		    destname = argv[x];
		    break;

		case 'B':
		    doboot = 1;
		    break;
#endif
	    }
    }
    
    start = doread(tname,LOADNAME);
    MOVE_TO(&start,0x5cL,4);
    start &= 0xffff0000L;
    MOVE_TO(&start,0x58L,4);
    DOIO(start);
    UNINIT();
    exit(0);
}

long doread(s,load)
char *s,*load;
{
    int fi,x;
    unsigned int ldinode=0;
    long cur,rootbl,temp,start,readit();
    struct dinode inode;
    struct direct {
	unsigned short inode;
	char filename[14];
    } dirbl;
    
    if ((fi = open(s,0)) == -1) 
	abort("Can't open %s\n",s);
	
    rsuper(fi);		/* read the super block (just to check) */
    
    lseek(fi,SEEKINODE(ROOTINO),0);	/* Seek to root inode */
    if (read(fi,&inode,sizeof(inode)) != sizeof(inode))
	abort("Can't read root inode table\n",0);

    rootbl = *(long *)(inode.di_addr); /*get block of root directory*/
    rootbl &= 0x00ffffffL; 		/* Mask out the undesirables */
    cur = rootbl * 1024L;		/* Get address of root directory */

#ifdef DEBUG
    if (dodir)
	printf("Root directory size is %ld, and is at %ld (block %ld)\n",inode.di_size,cur,rootbl);
#endif
    
    temp = inode.di_size;

    do {		/* Scan each directory entry */
	lseek(fi,cur,0);
	if (read(fi,&dirbl,sizeof(dirbl)) != sizeof(dirbl))
	    abort("Can't read root directory!\n",0);
#ifdef DEBUG
	if (dirbl.inode > 1 && dodir) 
	    printf("%14s inode= %d\n",dirbl.filename,dirbl.inode);
#endif
	if (strncmp(dirbl.filename,load,14) == 0) {
	    ldinode = dirbl.inode;	/* If we found it, save it for later*/
	    break;
	}
	cur += sizeof(dirbl);		/* Index to next entry */
	temp -= sizeof(dirbl);
    } while (temp > 1L);
    
#ifdef DEBUG
    if (ldinode && (destname || doboot))	/* If we found it, load it */
	start = readit(fi,ldinode);
    else if (destname)
	abort("We did not find %s in the root directory\n",load);
#else
    if (ldinode)
	start =readit(fi,ldinode);
    else
	abort("Can't find %s in the unix root directory\n",load);
#endif
    close(fi);
    return(start);
}
	
long readit(fi,inode)
int fi,inode;
{
    struct dinode dino;
    struct filehdr fileh;
    struct aouthdr aouth;
    struct scnhdr scnh[3];
    long cur,*dbp,level0[13],fsize,addr,*block;
    int x,fo,bytes,consec,t;
    char temp[1024],*buffer,*malloc();
    
    if ((block = (long *)malloc(MAXFBLOCK * sizeof (long))) == 0)
	abort("Cannot allocate memory for block array\n",0);
    
    lseek(fi,SEEKINODE(inode),0);
    if (read(fi,&dino,sizeof(dino))!= sizeof(dino))
	abort("Can't read file's inode #%d\n",inode);
    
    fsize = dino.di_size;
#ifdef DEBUG
    printf("File's size is %ld\n",fsize);
#endif
	
    x = (int)(fsize / 1024L)+ 1; /* Estimate number of blocks */
    
    if (x > MAXFBLOCK) 
	abort("Sorry, we need %d levels to load this file.\n",x);

    for (x=0; x<13; x++) level0[x] = 0L;
    
    for (dbp = (long *)(dino.di_addr),x=0; x<13; x++) {
	if ((block[x] = level0[x] = (*dbp & 0x00ffffff)) == 0L)
	    break;
	(char *)dbp += 3;
    }
    if (level0[10]) {			/* If we need a level1 index */
	lseek(fi,level0[10]* 1024L,0);
	if (read(fi,&block[10],1024) != 1024)
	    abort("Failed read of first level block table\n",0);
    }
    if (level0[11]) {			/* If we need a level2 index */
	lseek(fi,level0[11] * 1024L,0);
	if (read(fi,temp,1024) != 1024)
	    abort("Failed read of second level block index\n",0);
	for (dbp = (long *)temp,x=0; x < 256; x++) {
	    if (dbp[x]) {
		lseek(fi,dbp[x] * 1024,0);
		if (read(fi,&block[266 + (x * 256)],1024) != 1024)
		    abort("Failed read of second level block table\n",0);
	    }
	}
    }
	
#ifdef DEBUG
    if (destname) {
	printf("Creating %s\n",destname);
	if ((fo = creat(destname,0644)) == -1) 
	    abort("Could not create output name %s\n",destname);
	
	for (x=0;fsize > 0; x++) {
	    lseek(fi,GETOFF(x),0);
	    bytes = (fsize > 1024L) ? 1024 : (int) fsize;
	    if (read(fi,temp,bytes) != bytes)
		abort("Failed during read of file\n");
	    if (write(fo,temp,bytes) != bytes)
		abort("Failed during write of file\n");
	    fsize -= (long)bytes;
	}
	close(fo);
    }

    if (doboot) {
#endif
	if ((buffer = malloc(32640)) == 0)
	    abort("Can't allocate enough memory!\n");
	fsize = dino.di_size;
	lseek(fi,SEEKUNIX(0L), 0);
	if (read(fi,&fileh,sizeof(fileh)) != sizeof(fileh) ||
	    read(fi,&aouth,sizeof(aouth)) != sizeof(aouth) ||
	    read(fi,&scnh[0],sizeof(scnh[0]) * 3) != (sizeof(scnh[0]) * 3))
	    abort("Failed on read of file header\n");
	if (fileh.f_magic != NS32WRMAGIC)
	    abort("File is not an executable 32000 image, type = %o (octal)\n"
	    	  ,fileh.f_magic);
	if (!(fileh.f_flags & F_EXEC))
	    abort("File is not executable\n");

#ifdef DEBUG
	printf("\nText is %ld bytes, Data is %ld bytes, BSS is %ld bytes.\n",
		aouth.tsize,aouth.dsize,aouth.bsize);
	printf("Entry point is %lx\n",aouth.entry);
#endif
	
	for (x=0; x<3; x++) {
#ifdef DEBUG
	    printf("Section %8s: phys=%8lx, virt=%8lx, size=%8ld, file=%8ld\n",
		    scnh[x].s_name, scnh[x].s_paddr, scnh[x].s_vaddr,
		    scnh[x].s_size, scnh[x].s_scnptr);
#endif
	    if (scnh[x].s_size) {
		fsize = scnh[x].s_size;
		cur = scnh[x].s_scnptr;
		addr = scnh[x].s_paddr;
		if (cur) {
		    while (fsize > 0) {
			/* First, find the number of consecutive blocks */
			t = (int)(cur / 1024L);
			for (consec=1; consec < 31; consec++, t++) 
			    if ((GETOFF(t) + 1024L) != GETOFF(t + 1)) 
				break;

			if ((bytes = 1024 - (int)(cur % 1024L)) == 1024)
			    bytes = consec * 1024;

			if (fsize < 1024L)
			    bytes = (int) fsize;
#ifdef DEBUG
printf("Reading %d bytes...\n",bytes);
#endif
			lseek(fi,SEEKUNIX(cur),0);
			if (read(fi,buffer,bytes) != bytes)
			    abort("Failed during boot of file\n");
	
			/*  At this point, we have 'bytes' bytes in the
			    buffer 'buffer'. We can now transfer them to
			    physical memory address 'addr'.
			*/
			MOVE_TO(buffer,addr,bytes);
			
			cur += (long) bytes;
			addr += (long) bytes;
			fsize -= (long) bytes;
		    }
		} else {

		    /* In this case, just block fill the memory from
		       'addr', for a count of 'fsize', with zero. This
		       is for the BSS area
		    */
		    MOVE_ZTO(addr,fsize);

		}
		    
	    }
		    
	}
    	free(buffer);
#ifdef DEBUG
    }
#endif
    free(block);
    return(aouth.entry);
}

rsuper(fi)
int fi;
{
    struct filsys super;
        
    lseek(fi,SUPERB * 512L,0);

    if (read(fi,&super,sizeof(super)) != sizeof(super)) 
	abort("Can't read superblock.",0);
#ifdef DEBUG
    if (dosup) {
    printf("Superblock is %d bytes long\n\n\n",sizeof(super));
    printf("number of i-list blocks           = %d\n",super.s_isize);
    printf("Number of blocks in volume        = %ld\n",super.s_fsize);
    printf("Number of address in s_free       = %d\n",super.s_nfree);
    printf("number of i-nodes in s_inode      = %d\n",super.s_ninode);
    printf("lock during free list manipulation= %d\n",super.s_flock);
    printf("lock during i-list manipulation   = %d\n",super.s_ilock);
    printf("super block modified flag         = %d\n",super.s_fmod);
    printf("mounted read-only flag            = %d\n",super.s_ronly);
    printf("last super block update           = %ld\n",super.s_time);
    printf("device information                = %d %d %d %d\n",super.s_dinfo[0],super.s_dinfo[1],super.s_dinfo[2],super.s_dinfo[3]);
    printf("total free blocks                 = %ld\n",super.s_tfree);;
    printf("total free inodes                 = %d\n",super.s_tinode);
    printf("file system name                  = %.6s\n",super.s_fname);
    printf("file system pack name             = %.6s\n",super.s_fpack);
    printf("magic number (new file system)    = %lx\n",super.s_magic);
    printf("type of new file system           = %ld\n\n\n",super.s_type);
    
    }
#endif
    
    if (super.s_magic != FsMAGIC || super.s_type != Fs2b) 
	abort("Root file system is not a 1K file system!\n",0);
}

abort(s,m)
char *s,*m;
{
    printf("\n\nUNIX: ");
    printf(s,m);
    exit(1);
}

instr(s1,s2)			/* compute the offset of s2 in s1 */
char *s1,*s2;
{
    register char *t1,*t2,*t3;
	
    t3 = s1;
    
    while (*s1) {
	t1 = s1;
	t2 = s2;
	while (*t2)
	    if (*t1++ != *t2++)
		break;
	if (!*t2)
return(s1 - t3);			/* found it! return the difference */
	s1++;
    }
    return(-1);				/* Not found, so exit */
}

getcpu() {
    union {
	int ret;
	struct {
	    unsigned int flg_cpu : 3,
			 flg_ndp : 2,
			 flg_cerr: 1,
			 flg_nerr: 1;
	} ret1;
    } t;
    int CPUID();
    
    t.ret = CPUID();
    
    return(t.ret1.flg_cpu);
}
