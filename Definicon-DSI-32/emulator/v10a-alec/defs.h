#define BYTE_SWAP

//#define SWAP16 __builtin_bswap16
//#define SWAP32 __byte_swap_long_variable(x)
#define SWAP32(x) \
        ((((x) & 0xff000000) >> 24) | \
         (((x) & 0x00ff0000) >>  8) | \
         (((x) & 0x0000ff00) <<  8) | \
         (((x) & 0x000000ff) << 24))

typedef struct
   {
      uint8_t DoNotUse_u8_3;
      uint8_t DoNotUse_u8_2;
      uint8_t DoNotUse_u8_1;
      uint8_t u8;
   } use_byte_unsig;

typedef struct
   {
      int8_t DoNotUse_s8_3;
      int8_t DoNotUse_s8_2;
      int8_t DoNotUse_s8_1;
      int8_t s8;
   } use_byte_sig;

typedef struct
   {
      uint16_t DoNotUse_u16_1;
      uint16_t u16;
   } use_word_unsig;

typedef struct
   {
      int16_t DoNotUse_s16_1;
      int16_t s16;
   } use_word_sig;

typedef union
{
   uint32_t     u32;
   int32_t s32;

   use_byte_unsig usebyteu;   
   use_byte_sig usebytes;
   use_word_unsig usewordu;
   use_word_sig usewords;

} MultiReg;
struct GENINF {
    short int execid;
    short int dirblk;
    int heap_low;
    int heap_high;
    int stack_low;
    int stack_high;
    int mainmod;
    int modcount;
    int modaddr;
} __attribute__ ((packed));

struct GMT {
    int sbad;
    int linkad;
    int codead;
    int reserved;
} __attribute__ ((packed));

struct MODDIR {
    char modnm[8];
    short int modtype;
    int strtaddr;
    int slen;
    int llen;
    int clen;
    int symlen;
    int saddr;
    int laddr;
    int caddr;
    short int lblk;
    short int cblk;
    short int symblk;
    short int sbblk;
    char filler[14];
} __attribute__ ((packed));

struct MODREC {
    short int record;
    short int modoff;
    char type;
};

struct VHEAD {
    short int execid;
    int modst;
    int modbl;
    int modln;
    int codest;
    int codebl;
    int codeln;
    int heap_low;
    int heap_high;
    int stack_low;
    int stack_high;
    int entrymod;
    int entryadd;
} __attribute__ ((packed));

    
#define MAGIC1 7700
#define MAGIC2 7701

#define MEM_MB          4

