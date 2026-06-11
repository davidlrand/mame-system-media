/*
  !!DESCRIPTION!! 
  !!ORIGIN!!      testsuite
  !!LICENCE!!     Public Domain
  !!AUTHOR!!
*/

#define OUTFILE  "cc65090726.out"
#define THISFILE "cc65090726"

/* always include this _after_ the above macros have been defined */
#include "testsuite.h"

struct Record {
  struct Record *PtrComp;
  int x;
};

typedef struct Record RecordType;
typedef RecordType *RecordPtr;

void Proc3(RecordPtr *PtrParOut)
{
	/* whatever */
}

void Proc1(RecordPtr PtrParIn)
{
#define  NextRecord  (*(PtrParIn->PtrComp))
	Proc3((RecordPtr *)NextRecord.PtrComp);
	Proc3(&NextRecord.PtrComp);
	Proc3(&PtrParIn->PtrComp->PtrComp);

#ifdef CAST_STRUCT_PTR
	Proc3((RecordPtr *) PtrParIn->PtrComp->PtrComp);
	Proc3((RecordPtr *) (*(PtrParIn->PtrComp)).PtrComp);
	Proc3((RecordPtr *) NextRecord.PtrComp);
#else
	Proc3(PtrParIn->PtrComp->PtrComp);
	Proc3((*(PtrParIn->PtrComp)).PtrComp);
	Proc3(NextRecord.PtrComp);
#endif
	
#undef   NextRecord
}

int main(void)
{
    OPENTEST();
    fprintf(outfile,"it works :)\n");
    CLOSETEST();

    /* always return OK */
    return (0);
}
