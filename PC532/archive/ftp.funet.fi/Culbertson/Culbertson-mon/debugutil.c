/* NSC 32000 ROM debugger.
 * Bruce Culbertson  Bob Krause
 *
 * libc replacements
 */

#include "debugger.h"

/* Returns CMP_MATCH if strings are the same, CMP_SUBSTR if p1 is a
 * proper substring of p2, and CMP_NOMATCH if neither.
 */
int
myStrCmp (p1, p2)
register char *p1, *p2;
{
  while (*p1 == *p2 && *p1 != '\0') {
    ++p1;
    ++p2;
  }
  if (*p1 == '\0')
    return (*p2 == '\0')? CMP_MATCH: CMP_SUBSTR;
  return CMP_NOMATCH;
}

int
strlen(s)
register char *s;
{
char *s0 = s;

    while (*s++);
    return (s-s0-1);
}

char *strcpy(s1,s2)
register char *s1, *s2;
{
char *s = s1;

    while (*s1++ = *s2++);
    
    return (s);
}

char *strcat( s1, s2 )
char *s1, *s2;
{
    strcpy(s1+strlen(s1), s2);
    
    return (s1);
}

#if (STANDALONE & !LSC)
int
toupper(c)
char    c;
{
    return( (c>='a')&&(c<='z') ? (c-('a'-'A')) : c );
}

int
tolower(c)
char    c;
{
    return( (c>='A')&&(c<='Z') ? (c+('a'-'A')) : c );
}
#endif /* STANDALONE & !LSC */
