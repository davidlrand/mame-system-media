
#ifndef  __GETCHAR_C__
#define  __GETCHAR_C__

#ifdef INFILE

/*
        reads a character from stdin, -1 on EOF
*/

/* get char from input */
int GETCHAR(void)
{
    return(fgetc(infile));
}

#endif /* INFILE */

#endif /* __GETCHAR_C__ */
