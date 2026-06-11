#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#define TRUE  1
#define FALSE 0
#include <errno.h>

#define NULLPTR (char *)0

#define MAXARG 100

/* extern int errno; */

#if MAKE == 1
#define COMPILER 1
#define NOCHECK 0
#define NAMEX "cc"
#define TYPE ".c"
#define TYPE1 ".c"
#endif

#if MAKE == 2
#define COMPILER 1
#define NOCHECK 0
#define NAMEX "pc"
#define TYPE ".p"
#define TYPE1 ".pas"
#endif

#if MAKE == 3
#define COMPILER 1
#define NOCHECK 0
#define NAMEX "fc"
#define TYPE ".f"
#define TYPE1 ".for"
#endif

#if MAKE == 4
#define COMPILER 0
#define NOCHECK 1
#define NAMEX "as"
#define TYPE ".a32"
#define TYPE1 ".a32"
#endif

#if MAKE == 5
#define COMPILER 0
#define NOCHECK 1
#define NAMEX "ln"
#define TYPE ".o32"
#define TYPE1 ".o32"
#endif

#if MAKE == 6
#define COMPILER 0
#define NOCHECK 1
#define NAMEX "lib"
#define TYPE ".o32"
#define TYPE1 ".o32"
#endif


#if MAKE == 7
#define COMPILER 1
#define NOCHECK 0
#define NAMEX "fortran"
#define NAMEX1 "ncode"
#define NAMEX2 "glinker"
#define TYPE ".f"
#define TYPE1 ".for"
#define TYPE2 ".i"
#define TYPE3 ".obj"
#endif


#define SW1 "-b"

#if MAKE == 7
#define SW3 " "
#else
#if COMPILER == 1
#define SW2 "-GE"
#define SW3 "-X22 -X88"
#else
#define SW2 " "
#endif
#endif

static char *NAME=NAMEX;

#if MAKE == 7
static char *NAME1=NAMEX1;
static char *NAME2=NAMEX2;
static char *SW2=" -i";
#endif

main(argc,argv)
int argc;
char **argv;
{
    char temp[100],coml[256],args[100];
    register int count,fi,doas,filarg,dow;
    char *arg[MAXARG];
    int append();
    
    
#if NOCHECK == 0
    if (argc<2) {
#if MAKE==7
	printf("usage: %s filename <options>\n",NAME);
#else
	printf("usage: %s <options> filename\n",NAME);
#endif
	exit(1);
    }
#endif
    doas = TRUE;
    dow  = FALSE;
    
    for (count=0;count<MAXARG;count++)
	arg[count]=NULLPTR;
    
    for (count=0;count<=argc;count++)
	arg[count]=argv[count];
	
    arg[argc]=NULLPTR;

#if NOCHECK == 0
    strcpy(temp,"");
    
    for (count=1;count<argc;count++) {
#if MAKE == 7
	if (arg[count][0]!='-' && arg[count][0]!='+' )
#else
	if (arg[count][0]!='-' )
#endif
	{
	    filarg = count;
	    strcpy(temp,arg[count]);
	}
	else
	{
#if MAKE != 7
	    arg[count][1]=toupper(arg[count][1]);
#endif
	    switch(toupper(arg[count][1]))
	    {
	    case 'S':
		doas = FALSE;
		break;
		
	    case 'O':
		if (toupper(arg[count][2]) == 'W')
		    dow = TRUE;
		break;
	    }
	}
    }
    
    if (strlen(temp)) {
	if ((fi=open(temp,0))==-1) {
	    replace(temp,TYPE);
	    if ((fi=open(temp,0))==-1) {
		replace(temp,TYPE1);
		if((fi=open(temp,0))==-1) {
		    printf("%s: Can't open \"%s\"\n",NAME,temp);
		    exit(1);
		}
	    }
	}
	printf("Compiling %s\n",temp);
	close(fi);
	arg[filarg]=temp;
#endif

	
#if COMPILER
	for (count = 1; count < argc; count++) {
	    strcpy(args,arg[count]);
	    strup(args);
	    if ((strcmp(args,"-S") == 0) || (strcmp(args,"-OW") == 0))
		strcpy(arg[count],"");
	}
#if MAKE == 7
	strcat(SW2,temp);
	replace(SW2,TYPE2);
#endif

#endif
	
#if !COMPILER
	execlp("LOAD","LOAD",NAME,SW1,arg[1],arg[2],arg[3],arg[4],arg[5],
	       arg[6],arg[7],arg[8],arg[9],NULLPTR);
#endif

#if COMPILER
	if (fi = execlp("load","load",NAME,SW1,SW2,SW3,arg[1],arg[2],
		arg[3],arg[4],arg[5],arg[6],arg[7],arg[8],arg[9],NULLPTR)) {
	    fi = wait();
	    printf("Global error code returned is %d",errno);
	    printf("Error in compiler. Return code = %d\n",fi);
	    exit(fi);
	}
#if MAKE == 7 /* this is the svs compiler code */
	    replace(temp,TYPE2);
	    if ((fi = open(temp,0)) == -1) {
		printf("Error in compiler. No output file.\n");
		exit(1);
	    }
	if (fi = execlp("load","load",NAMEX1,SW1,temp,NULLPTR)) {
	    fi = wait();
	    printf("Error in code generator. Return code = %d\n",fi);
	    exit(fi);
	}
	    replace(temp,TYPE3);
	    if ((fi = open(temp,0)) == -1) {
		printf("Error in code generator. No output file.\n");
		exit(1);
	    }

	if (fi = execlp("load","load",NAMEX2,SW1,temp,NULLPTR)) {
	    fi = wait();
	    printf("Error in object code generation. Return code = %d\n",fi);
	    exit(fi);
	}
	else
	{
		 /* if all went ok up to now, we're in the winners circle */
		/* so lets delete the ".obj" file which is now useless */
		unlink(temp);
	}

#else	/* OK back to normal code */
	if (doas) {
	    replace(temp,".a32");
	    if ((fi = open(temp,0)) == -1) {
		printf("Error in compiler. No output file.\n");
		exit(1);
	    }
	    close(fi);
	    
	    if (fi = execlp("load","load","as","-a",(dow) ? "-zw" : "-z",
			temp,NULLPTR)) {
		fi = wait();
		printf("Error in assembler. Return code = %d\n",fi);
		exit(fi);
	    }
	}
#endif
#endif
		
#if NOCHECK == 0
    } else {
	printf("%s: No filename specified",NAME);
	exit(1);
    }
#endif
    exit(0);
}
		
strup(s)
char *s;
{
    while (*s)
	*s++ = toupper(*s);
}

append(d,s)
char *d,*s;
{
    char *t;
    t = d;
    
    while (*d)
	d++;
    
    while (t < d) {
	d--;
	if (*d == '.')
	    break;
	if (*d == '/' || *d == '\\' || t == d) {
	    while (*d)
		d++; /* skip to end of string */
	    break;
	}
    }
	
    if (*d == '\0') {
	while (*s)
	    *d++ = *s++;
	*d = '\0';
    }
}

replace(d,s)
char *d,*s;
{
    char *t;
    t = d;
    
    while (*d)
	d++;
    
    while (t < d) {
	d--;
	if (*d == '.') 
	    break;
	if (*d == '/' || *d == '\\' || t == d) {
	    while (*d)
		d++; /* skip to end of string */
	    break;
	}
    }
	
    while (*s)
	*d++ = *s++;
    
    *d = '\0';
}

