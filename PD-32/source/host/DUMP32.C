#include <stdio.h>
#include <ctype.h>
#define BUFSIZE 128


int CPUtype;		/* CPU information */
char *cpumes[]= {
    "n 8088","n 8086"," NEC V20"," NECV30",
    "n 80188","n 80186","n Unknown","n 80286"
};

main(argc, argv) int argc; char *argv[]; {

    unsigned char buffer[BUFSIZE];
    int loop, stat, i, j;
    long start=0L,loc;
    int fd,open();
    unsigned char ch;

    CPUtype = getcpu();

    if (argc > 1) {
	for (i=1; i < argc; i++) {
	    if (argv[i][0] == '-') {
		if (toupper(argv[i][1])=='I') {
		    printf("CPU is a%s\n",cpumes[CPUtype]);
		    if (argv[i][2]) {
			sscanf(&argv[i][2],"%d",&CPUtype);
			printf("CPU forced to a%s\n",cpumes[CPUtype]);
		    }
		}
	    }
	}
    }
		
    INIT();
    while (1) {
	printf("Please enter starting location? ");
	gets(buffer);
	if (strlen(buffer) == 0)
	    break;
	sscanf(buffer,"%lx ",&loc);
	while(!bdos(6,255)) {
	    MOVE_FROM(loc,buffer,BUFSIZE);
/*
if (argc < 2) {
*/
	    for (j=0; j < BUFSIZE; j=j+16) {
		printf("%06lx    ", loc);
		loc += 16L;
		for (i=0; i<16; i++) {
		    printf("%02x",(0xff & buffer[j + i]));
		    if (i & 1) printf(" ");
		    if (i == 7) printf(" ");
		}
		printf("   ");
		for (i=0; i<16; i++) {
		    ch = buffer[j + i];
		    if ((ch < ' ')|(ch > 'z'))
			printf(".");
		    else printf("%c",ch);
		}
		printf("\n");
	    }

/*
} else {
    loc += 128L;
    if (!(loc % 4096))
	printf("Location = %lx\r",loc);
    if (loc > 0x200000L)
	loc = 0L;
}
*/
	}
    }
}
#asm
readnxt_	proc	near
	ret
readnxt_	endp
	public	readnxt_,DoSerUn
DoSerUn	proc	near
	ret
DoSerUn	endp
	
#endasm

getcpu() {
    union {
	int ret;
	struct {
	    unsigned int flg_cpu : 3,
			 flg_ndp : 1,
			 flg_cerr: 1,
			 flg_nerr: 1;
	} ret1;
    } t;
    int CPUID();
    
    t.ret = CPUID();
    
    return(t.ret1.flg_cpu);
}
