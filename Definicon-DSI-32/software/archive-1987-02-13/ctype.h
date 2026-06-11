#define	_U	01
#define	_L	02
#define	_N	04
#define	_S	010
#define _P	020
#define _C	040
#define _X	0100
extern char _ctype_[];
#define isalpha(ch) (_ctype_[1+(ch)]&(_U|_L))
#define isupper(ch) (_ctype_[1+(ch)]&(_U))
#define islower(ch) (_ctype_[1+(ch)]&(_L))
#define isdigit(ch) (_ctype_[1+(ch)]&(_N))
#define isspace(ch) (_ctype_[1+(ch)]&(_S))
#define isxdigit(ch) (_ctype_[1+(ch)]&(_N|_X))
#define iscntl(ch)  (_ctype_[1+(ch)]&(_C))
#define isprint(ch) (!(_ctype_[1+(ch)]&(_C)))
#define isgraph(ch) (_ctype_[1+(ch)]&(_U|_L|_N|_P))
#define isalpha(ch) (_ctype_[1+(ch)]&(_U|_L))
#define isalnum(ch) (_ctype_[1+(ch)]&(_U|_L|_N))
#define ispunct(ch) (_ctype_[1+(ch)]&(_P))
#define isascii(ch) ((unsigned) (ch)<=0x7f)
#define	tolower(ch) (isupper(ch) ? (ch)+('a'-'A') : (ch))
#define	toupper(ch) (islower(ch) ? (ch)+('A'-'a') : (ch))
#define	toascii(ch) ((ch) & 0177)
