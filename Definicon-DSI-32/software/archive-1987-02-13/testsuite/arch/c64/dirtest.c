
#if 1

/* define to compile test program */
/*#define TEST*/
/* define to compile test program for linux :=P */
/*#define LINUX*/

/*
	first test for posix directory routines for the c64
	kludges:
	-   currently uses cbm_open, which conflicts with standard i/o,
		which in turn makes it infact kindof unuseable. this can
		be easily changed however, since the only reason not to use
		open/read was that it currently appends ,u,r to filenames
	-   the offset in current dir stream should better be calculated
		from the values returned by "read".
	-   the type flag isnt filled in atm.	
	-   scandir/alphasort/versionsort is missing
	-   some bits are currently untested (ie, unused in the testprogram)
	27/02/2003 gpz	
*/

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>

#if defined(LINUX)
#include <sys/types.h>
#include <dirent.h>
#else

/*
	contents of dirent.h ...
*/

#include <cbm.h>

/* 16 will only be enough if NO subdirs are supported! */
/*#define NAME_MAX 16*/
#define NAME_MAX 128

struct dirent
{
	/* POSIX */
	char d_name[NAME_MAX+1];
};

typedef struct
{
	unsigned char fd;
	char name[NAME_MAX+1];
} DIR;

DIR *opendir(const char *name);
struct dirent *readdir(DIR *dir);
int closedir(DIR *dir);
int dirfd(DIR *dir);

/*-------------------------------------------------------------------------------------------
// functions access the directory via reading the "$" file to provide max. compatibility
//
// disc header:
//
// $07      ignored
// $01      quote
// $10      volume/subdir name
// $01      quote
// $06      disc id
// $01      = $00
//
// file entry:
//
// $02      ignored (basic-line link)
// $02      file length in 254-byte blocks (basic-line nr)
// $xx      spaces
//
// $01      quote if file-entry...                   | = 'b' if "blocks free"...
//          $00-$10  file-name                       |   $xx ignored
//          $01      quote                           |
//          $01      file-closed attrib (' 'or'*')   |   => file-length is free blocks
//          $03      file-type                       |
//          $01      file-protected attr. (' 'or'<') |
//          $xx      ignored                         |
//          $01      = $00 (basic-line end)          |
//-------------------------------------------------------------------------------------------*/

#include <conio.h>

DIR *opendir(const char *name)
{
/* making it local breaks the code! */
static unsigned char buffer[(8+16+1+7)+1];
static unsigned char fd;
static DIR *dir;
static unsigned char i;

/*printf("opendir: %s\n",name);*/

	dir=(DIR*) malloc(sizeof(DIR));
	if(dir!=NULL)
	{
		/* handle dir name, go into
		   subdirs on 1581 etc... */
		name=name;

		/* open dir "file" */
/*		if((fd=dir->fd=open("$",O_RDONLY))==-1)*/
		if(cbm_open(2, 8, CBM_READ, "$")!=0)
		{
			return(NULL);
		}

		fd=dir->fd=2;

		/* skip the disc header */

#if 0
//		read(fd,buffer,0x08);    /* some info */
//		read(fd,buffer,0x10);    /* disc name */
//		read(fd,buffer,0x01);    /* quote */
//		read(fd,buffer,0x07);    /* disc id + $00 */
		memset(buffer,0,(8+16+1+7));
		cbm_read(fd,buffer,(8+16+1+7));
		cprintf("opendir: buffer=%s\n\r",buffer+8+1);
#else


		memset(buffer,0,(8+16+1+7));
		cbm_read(fd,buffer,0x08);    /* some info */
/*cprintf("opendir: buffer=%s\n\r",buffer);*/
		memset(buffer,0,(8+16+1+7));
		cbm_read(fd,buffer,0x10);    /* disc name */
/*cprintf("opendir: buffer=%s\n\r",buffer);*/


/*cprintf("readdir:fd=%d\n\r",fd);*/
		
		/* read file entry */
		i=0;do
		{
			cbm_read(fd,buffer,0x01);
/*cprintf("%02x ",*buffer);*/

			++i;
			if(i>40) break;

		} while (buffer[0]!=0);

/*cprintf("headerlen: %d\n\r",i);*/


#endif
		
/*		printf("opendir: buffer=%s\n",buffer);*/

	}
	return(dir);

}

int closedir(DIR *dir)
{
	cbm_close(dir->fd);
	free(dir);
	return 0;
}

int dirfd(DIR *dir)
{
/*printf("dirfd: %d\n",dir->fd);*/
	return((int)dir->fd);
}

struct dirent *readdir(DIR *dir)
{

static unsigned char buffer[NAME_MAX+1];
static unsigned char temp;
static unsigned char i,ii;

static struct dirent entry;
static unsigned char fd;
static unsigned char ch;

again:

	 	fd=dirfd(dir);

/*cprintf("readdir:fd=%d offs=%d\n\r",fd,entry.d_off);*/

		/* basic line-link / file-length */
		cbm_read(fd,buffer,0x04);

		/* read file entry */
		i=0;do
		{
			cbm_read(fd,&ch,0x01);
/*cprintf("%02x ",ch);*/
			buffer[i]=ch;
			++i;

		} while (ch!=0);


/*cprintf("readdir:len: %d buffer=%s\n\r",i,buffer);*/

		/* skip until either quote (file) or b (blocks free => end) */
		i=0;ii=0;
		while(i==0){
			temp=buffer[ii];ii++;
			if(ii>16){
				/* something went wrong...this shouldnt happen! */
/*printf("readdir: args!\n");*/
				return(NULL);
			}
			else if(temp=='\"') i++;
			else if(temp=='b') {
/*printf("readdir: end\n");*/
				/* "blocks free" */
				return(NULL);
			}
		}

		/* process file entry */

		i=0;  temp=buffer[ii];ii++;
		while(temp!='\"'){

			if(temp=='.') goto again; /* HACK! a dot always means we dont want this entry! */
			
			entry.d_name[i]=temp;
			i++;
			temp=buffer[ii];ii++;
		}
		entry.d_name[i]=0;

		i=strlen(entry.d_name);


/*printf("readdir: %s\n\r",entry.d_name);*/

		return(&entry);

}
#endif

#endif
