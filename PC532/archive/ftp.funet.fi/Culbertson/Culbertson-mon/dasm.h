/*
 * dasm.h
 */

struct operand {
    int     o_mode;         /* address mode */
    int     o_reg0;         /* first register */
    int     o_reg1;         /* second register */
    long    o_disp0;        /* first displacment value */
    long    o_disp1;        /* second displacement value */
    int     o_iscale;       /* scaled indexing factor */
    int     o_ireg;         /* scaled indexing register */
};

struct insn {
    int     i_format;       /* instruction format */
    int     i_op;           /* operation code */
    char        i_monic[8];     /* mneumonic string */
    unsigned char   i_iol;          /* integer operand length */
    struct  operand i_opr[4];               /* operands */
};

/* Addressing modes (not the same format as in a machine instruction) */
#define AMODE_NONE      0   /* operand not used by instruction */
#define AMODE_REG       1   /* register */
#define AMODE_RREL      2   /* register relative */
#define AMODE_MREL      3   /* memory relative */
#define AMODE_IMM       4   /* immediate */
#define AMODE_ABS       5   /* absolute */
#define AMODE_EXT       6   /* external */
#define AMODE_TOS       7   /* top of stack */
#define AMODE_MSPC      8   /* memory space */
#define AMODE_REGLIST   9   /* a register list */
#define AMODE_QUICK     10  /* quick integer (-8 .. +7) */
#define AMODE_INVALID   11  /* invalid address mode found */
#define AMODE_AREG      12  /* CPU dedicated register */
#define AMODE_BLISTB    13  /* byte length bit list */
#define AMODE_BLISTW    14  /* word length bit list */
#define AMODE_BLISTD    15  /* double-word length bit list */
#define AMODE_SOPT      16  /* options in string instructions */
#define AMODE_CFG       17  /* configuration bits */
#define AMODE_MREG      18  /* memory management register */
#define AMODE_CINV	19  /* cache invalidate options */
