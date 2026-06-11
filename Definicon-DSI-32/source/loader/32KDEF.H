struct GENINF {
    int execid;
    int dirblk;
    long heap_low;
    long heap_high;
    long stack_low;
    long stack_high;
    long mainmod;
    long modcount;
    long modaddr;
};

struct GMT {
    long sbad;
    long linkad;
    long codead;
    long reserved;
};

struct MODDIR {
    char modnm[8];
    int modtype;
    long strtaddr;
    long slen;
    long llen;
    long clen;
    long symlen;
    long saddr;
    long laddr;
    long caddr;
    int lblk;
    int cblk;
    int symblk;
    int sbblk;
    char filler[14];
};

struct MODREC {
    int record;
    int modoff;
    char type;
};
