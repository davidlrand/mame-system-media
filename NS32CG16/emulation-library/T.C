#include <portab.h>

struct _vsstr {
    WORD montype;
    WORD maxR, maxC;
    ULONG monsize;
    UBYTE attrib;
    UBYTE *locscreen;
    long remscreen;
};

UBYTE *page;

main() {
    struct _vsstr *vs,*vs1,*v_init();
    int time,time1;

    vs = v_init(50);
    
    page = vs->locscreen;

    time = timer(0);
    test();
    time = timer(0) - time;
    printf("\033ERun time was %d msec\n",time);
    v_lpr(vs,300);
    
}
    
    