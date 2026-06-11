
/*
  !!DESCRIPTION!! basic ASCII character test
  !!ORIGIN!!      testsuite
  !!LICENCE!!     Public Domain
  !!AUTHOR!!      Groepaz/Hitmen
*/


#include "testsuite.h"
#include "util/output.c"
#define OUTFILE  "charset.out"
#define THISFILE "charset"

#include <stdio.h>

#if 0

/* this kind of line-continuation for strings doesnt work properly for cc65 */

const unsigned char characters[]={
	   /*0123456789abcdef0123456789abcdef*/
        /* iso646-us control-characters */
        "                                "      /* 00-1f */
        /* iso646-us printable characters */
        " !\"#$%&'()*+,-./"                     /* 20-2f !"#$%&'()*+,-./ */
        "0123456789"                            /* 30-39 0123456789      */
        ":;<=>?@"                               /* 3a-40 :;<=>?@         */
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"            /* 41-5a A-Z             */
        "[\\]^_`"                               /* 5b-60 [\]^_`          */
        "abcdefghijklmnopqrstuvwxyz"            /* 61-7a a-z             */
        "{|}~ "                                 /* 7b-7f {|}~            */
        /* iso8859-15 extended characters */
};

#endif

const unsigned char characters[]={
	   /*0123456789abcdef0123456789abcdef*/
        /* iso646-us control-characters */
		/* 00-1f */
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        /* iso646-us printable characters */
		/* 20-2f !"#$%&'()*+,-./ */
        ' ','!','"','#','$','%','&','\'','(',')','*','+',',','-','.','/',
		/* 30-39 0123456789      */
        '0','1','2','3','4','5','6','7','8','9',
		/* 3a-40 :;<=>?@         */
        ':',';','<','=','>','?','@',
		/* 41-5a A-Z             */
		'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z',
		/* 5b-60 [\]^_`          */
		'[','\\',']','^','_','`',
		/* 61-7a a-z             */
        'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z',
		/* 7b-7f {|}~            */
		'{','|','}','~',' '
        /* iso8859-15 extended characters */
};

void printchars(unsigned char a,unsigned char b){
        for(b++;a!=b;a++)
/*                fprintf(outfile, "%02x ",a); */
/*                fprintf(outfile, "%02x ",characters[a]); */
                fprintf(outfile, "%c",characters[a]);
        fprintf(outfile, "\n");
}

int main(void) {

		opentest(OUTFILE);

		fprintf(outfile, "characters:\n\n");
        printchars(0x61,0x7a);
        printchars(0x41,0x5a);
		fprintf(outfile, "numbers:\n\n");
        printchars(0x30,0x39);
		fprintf(outfile, "other:\n\n");
        printchars(0x20,0x2f);
        /*printchars(0x3a,0x40);*/
        printchars(0x3a,0x3f);
        /*printchars(0x5b,0x60);*/
        /*printchars(0x7b,0x7f);*/
		fprintf(outfile, "\n\n");
		fprintf(outfile, "slash:               '%c'\n",'/');
		fprintf(outfile, "backslash:           '%c'\n",'\\');
		fprintf(outfile, "curly braces open:   '%c'\n",'{');
		fprintf(outfile, "curly braces close:  '%c'\n",'}');
		fprintf(outfile, "square braces open:  '%c'\n",'[');
		fprintf(outfile, "square braces close: '%c'\n",']');
		fprintf(outfile, "underscore:          '%c'\n",'_');
		fprintf(outfile, "tilde:               '%c'\n",'~');
		fprintf(outfile, "pipe:                '%c'\n",'|');
		fprintf(outfile, "apostroph:           '%c'\n",'\'');
		fprintf(outfile, "single quote         '%c'\n",'`');
		fprintf(outfile, "xor                  '%c'\n",'^');
		fprintf(outfile, "at                   '%c'\n",'@');

		closetest(THISFILE);

		return(0);
}
