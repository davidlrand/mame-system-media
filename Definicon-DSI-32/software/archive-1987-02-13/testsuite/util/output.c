
#ifndef __OUTPUT_C__
#define __OUTPUT_C__

#include "testsuite.h"

#ifdef __CC65__
//#include <conio.h>
extern unsigned char _filetype;
#endif

/*
	globals needed when tests should load and execute the following test
	- currently only used by c64 target

	FIXME: get rid of this
*/
//#ifdef TARGET_C64
//static char mydirname[NAME_MAX+1]=".";
//static DIR *mydir;
//static struct dirent *mydirent;
//#endif

static FILE *outfile,*infile;

void opentest(char *name)
{
/*
	avoid warnings for those targets that do not use the
	name parameter.
*/
#ifdef TARGET_RUN6502
	name=name;
#endif

/*
	cc65 would normally create USR files as default for
	all cbm targets, this changes it to PRG
*/
#ifdef TARGET_C64
	_filetype='p';
#endif
	
/*
	open output file.

	"run6502" target writes to stdout !
*/

	outfile=stdout;
#ifndef TARGET_RUN6502
#ifndef TARGET_SIM65
	if(!(outfile=fopen(name,"w")))
	{
		printf("couldnt open file \"%s\"\n\r",name);
		exit(-1);
	}
#endif
#endif
	fprintf(outfile,"\n");

/*
	open input file.

	"run6502" target reads from stdin !
*/
	
	infile=stdin;

#ifndef TARGET_RUN6502
#ifndef TARGET_SIM65

#ifdef INFILE
	if(!(infile=fopen(INFILE,"r")))
	{
		printf("couldnt input file \"%s\"\n\r",INFILE);
		exit(-1);
	}
#endif

#endif
#endif
	
}
void closetest(char *name)
{

	if((outfile!=stdout)&(outfile!=NULL))
	{
		fprintf(outfile,"\n");
#ifndef TARGET_RUN6502
#ifndef TARGET_SIM65
		fclose(outfile);
#endif
#endif
	}

/*
	load and execute next text

	- currently specific for c64 target
*/
	
#ifdef TARGET_C64
#if 0
	mydir=opendir(mydirname);
	if(mydir==NULL)
	{
		printf("error: couldnt open directory.\n\r");
	}
	else
	{
		while((mydirent=readdir(mydir))!=NULL)
		{
/*printf(">%s:%s\n",mydirent->d_name,name);*/
			if(strcmp(mydirent->d_name,name)==0)
			{
/*printf("found %s:%s\n",mydirent->d_name,name);*/
				if((mydirent=readdir(mydir))!=NULL)
				{
/*
					{
					char len=strlen(mydirent->d_name);
						if((mydirent->d_name[len-4]!='.')&
						   (mydirent->d_name[len-3]!='.'))
						{
*/
						closedir(mydir);

						/*
						asm("sei");
						asm("jsr $fd15");
						asm("jsr $fda3");
						asm("jsr $ff5b");
						asm("cli");
						*/

						clrscr();
						printf("\n\r\n\rnew\n\r\n\r\n\rload\"%s\",8,1\n\r\n\r\n\r\n\r\n\r",mydirent->d_name);
						printf("new\n\r\n\r\n\rsys2061");
						gotoxy(0,0);
						*((char*)198)=8;
						*((char*)631)='\n';
						*((char*)632)='\n';
						*((char*)633)='\n';
						*((char*)634)='\n';
						*((char*)635)='\n';
						*((char*)636)='\n';
						*((char*)637)='\n';
						*((char*)638)='\n';
						exit(0);
/*
						}

					}
*/
					
				}

				break;

			}
			
		} 
		printf("all tests done.\n\r");
		closedir(mydir);
#endif
                *((char*)2)=2;
                ((void(*)())0x0002)();;
//                asm("jmp $0002");     /* this doesnt work with older compilers ^_^ */
//	}
#else
	name=name;
#endif
	
}

void printerror(int line)
{
    fprintf(outfile,"(%d): err\n",line);
}

void printok(int line)
{
    fprintf(outfile,"(%d): ok\n",line);
}

#endif /* __OUTPUT_C__ */
