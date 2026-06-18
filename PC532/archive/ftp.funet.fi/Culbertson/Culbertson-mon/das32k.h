/*
 * das32k.h
 */

/* Instruction types */
#define ITYPE_FMT0  0
#define ITYPE_FMT1  1
#define ITYPE_FMT2  2
#define ITYPE_FMT3  3
#define ITYPE_FMT4  4
#define ITYPE_FMT5  5
#define ITYPE_FMT6  6
#define ITYPE_FMT7  7
#define ITYPE_FMT8  8
#define ITYPE_FMT9  9
#define ITYPE_NOP   10
#define ITYPE_FMT11 11
#define ITYPE_FMT12 12
#define ITYPE_UNDEF 13
#define ITYPE_FMT14 14
#define ITYPE_FMT15 15

/* Table to determine the 'condition' field of an instruction */
CONST
char cond_table[16][3] = {
    "eq",       /* equal */
    "ne",       /* not equal */
    "cs",       /* carry set */
    "cc",       /* carry clear */
    "hi",       /* higher */
    "ls",       /* lower or same */
    "gt",       /* greater than */
    "le",       /* less than or equal */
    "fs",       /* flag set */
    "fc",       /* flag clear */
    "lo",       /* lower */
    "hs",       /* higher or same */
    "lt",       /* less than */
    "ge",       /* greater than or equal */
    "r",        /* unconditional branch */
    "??"        /* never branch (nop) */
};

#define IOL(x)      (((x)&0x3) + 1)
#define IOL_BYTE        1                       /* Byte (8-bits) */
#define IOL_WORD        2                       /* Word (16-bits) */
#define IOL_NONE        3                       /* Does not apply */
#define IOL_DOUBLE      4                       /* Double Word (32-bits) */

CONST
char iol_table[5][2] = {
    "?",                                    /* Should never appear */
    "b",                                    /* byte */
    "w",                                    /* word */
    "?",                                    /* undefined */
    "d"                                     /* double word */
};

CONST
char fol_table[2][2] = {
    "l",                                    /* long floating-point */
    "f"                                     /* standard floating-point */
};

CONST
char scale_table[5][2] = {
    "?",                                    /* no scaled indexing */
    "b",                                    /* byte */
    "w",                                    /* word */
    "d",                                    /* double-word */
    "q"                                     /* quad-word */
};

CONST
char cfg_table[4][2] = {
    "i",                                    /* vectored interrupt enable */
    "f",                                    /* floating point enable */
    "m",                                    /* memory management enable */
    "c"                                     /* custom slave enable */
};

CONST
char sopt_table[8][4] = {
    "",                                     /* no options */
    "b",                                    /* backward */
    "w",                                    /* while match */
    "b,w",                                  /* backward, while match */
    "?",                                    /* undefined */
    "b,?",                                  /* undefined */
    "u",                                    /* until match */
    "b,u"                                   /* backward, until match */
};

#define AREG_US     0x0         /* user stack */
#define AREG_FP     0x8         /* frame pointer */
#define AREG_SP     0x9         /* stack pointer */
#define AREG_SB     0xa         /* static base */
#define AREG_PSR    0xd         /* processor status register */
#define AREG_INTBASE    0xe         /* interrupt base */
#define AREG_MOD    0xf         /* module */

/* Floating-point operand length field masks */
#define FOL_SINGLE  0x1         /* Single Precision (32-bits) */
#define FOL_DOUBLE  0x0         /* Double Precision (64-bits) */

#define FMT0_COND(x)    (((x)&0xf0)>>4)         /* Condition code for fmt 0 */
#define FMT1_OP(x)      (((x)&0xf0)>>4)         /* Op code for fmt 1 */
#define FMT2_OP(x)      (((x)&0x70)>>4)         /* Op code for fmt 2 */
#define FMT2_COND(x, y) (((x)>>7) + (((y)&0x80)<<1)) /* Condition code for fmt 2 */
#define FMT3_OP(x)      ((x)&0x7)               /* bits 2-4 of fmt 3 op code */
#define FMT4_OP(x)      (((x)&0x3c)>>2)         /* Op code for fmt 4 */
#define FMT5_OP(x)      (((x)&0x3c)>>2)             /* op code for fmt 5 */
#define FMT6_OP(x)      (((x)&0x3c)>>2)         /* op code for fmt 6 */
#define FMT7_OP(x)      (((x)&0x3c)>>2)         /* op code for fmt 7 */
#define FMT8_OP(x, y)   ((((x)&0xc0)>>6) + ((y)&0x4))
#define FMT8_REG(x)     (((x)&0x38)>>3)         /* register operand for fmt 8 */
#define FMT8_SU         1                       /* register value -> movsu */
#define FMT8_US         3                       /* register value -> movus */
#define FMT9_OP(x)      (((x)&0x38)>>3)         /* op code for fmt 9 */
#define FMT9_F(x)       (((x)&0x4)>>2)          /* float type for fmt 9 */
#define FMT11_OP(x)     (((x)&0x3c)>>2)         /* op code for fmt 11 */
#define FMT12_OP(x)     (((x)&0x3c)>>2)         /* op code for fmt 12 */
#define FMT11_F(x)      ((x)&01)                /* float type for fmt 11 */
#define FMT12_F(x)      ((x)&01)                /* float type for fmt 12 */
#define FMT14_OP(x)     (((x)&0x3c)>>2)         /* op code for fmt 14 */

#define GEN_R0      0   /* register 0 */
#define GEN_R1      1   /* register 1 */
#define GEN_R2      2   /* register 2 */
#define GEN_R3      3   /* register 3 */
#define GEN_R4      4   /* register 4 */
#define GEN_R5      5   /* register 5 */
#define GEN_R6      6   /* register 6 */
#define GEN_R7      7   /* register 7 */
#define GEN_RR0     8   /* register 0 relative */
#define GEN_RR1     9   /* register 1 relative */
#define GEN_RR2     10  /* register 2 relative */
#define GEN_RR3     11  /* register 3 relative */
#define GEN_RR4     12  /* register 4 relative */
#define GEN_RR5     13  /* register 5 relative */
#define GEN_RR6     14  /* register 6 relative */
#define GEN_RR7     15  /* register 7 relative */
#define GEN_FRMR    16  /* frame memory relative */
#define GEN_SPMR    17  /* stack memory relative */
#define GEN_SBMR    18  /* static memory relative */
#define GEN_RES     19  /* reserved for future use */
#define GEN_IMM     20  /* immediate */
#define GEN_ABS     21  /* absolute */
#define GEN_EXT     22  /* external */
#define GEN_TOS     23  /* top of stack */
#define GEN_FRM     24  /* frame memory */
#define GEN_SPM     25  /* stack memory */
#define GEN_SBM     26  /* static memory */
#define GEN_PCM     27  /* program memory */
#define GEN_SIB     28  /* scaled index, bytes */
#define GEN_SIW     29  /* scaled index, words */
#define GEN_SID     30  /* scaled index, double words */
#define GEN_SIQ     31  /* scaled index, quad words */

CONST
unsigned char fmttab[128] = {
/* This is really an array of 256 nibbles.  The index is the first
 * byte of an instruction opcode.  The value in the array is the
 * instruction format corresponding to that first byte.
 *
 * group    1st byte        insn format     insn type
 */
(4+         /* 00000000 format 4    add .b    */
(4*16)),    /* 00000001 format 4    add .w    */
(1+         /* 00000010 format 1    bsr   */
(4*16)),    /* 00000011 format 4    add .l    */
(4+         /* 00000100 format 4    cmp .b    */
(4*16)),    /* 00000101 format 4    cmp .w    */
(13+        /* 00000110 format 19   undefined */
(4*16)),    /* 00000111 format 4    cmp .l    */
(4+         /* 00001000 format 4    bic .b    */
(4*16)),    /* 00001001 format 4    bic .w    */
(0+         /* 00001010 format 0    beq   */
(4*16)),    /* 00001011 format 4    bic .w    */
(2+         /* 00001100 format 2    addq .b   */
(2*16)),    /* 00001101 format 2    addq .w   */
(5+         /* 00001110 format 5          */
(2*16)),    /* 00001111 format 2    addq .l   */
(4+         /* 00010000 format 4    addc .b   */
(4*16)),    /* 00010001 format 4    addc .w   */
(1+         /* 00010010 format 1    ret   */
(4*16)),    /* 00010011 format 4    addc .l   */
(4+         /* 00010100 format 4    mov .b    */
(4*16)),    /* 00010101 format 4    mov .w    */
(15+        /* 00010110 format 15         */
(4*16)),    /* 00010111 format 4    mov .l    */
(4+         /* 00011000 format 4    or .b     */
(4*16)),    /* 00011001 format 4    or .w     */
(0+         /* 00011010 format 0    bne   */
(4*16)),    /* 00011011 format 4    or .l     */
(2+         /* 00011100 format 2    cmpq .b   */
(2*16)),    /* 00011101 format 2    cmpq .w   */
(14+        /* 00011110 format 14         */
(2*16)),    /* 00011111 format 2    cmpq .l */
(4+         /* 00100000 format 4    sub .b */
(4*16)),    /* 00100001 format 4    sub .w */
(1+         /* 00100010 format 1    cxp   */
(4*16)),    /* 00100011 format 4    sub .l    */
(4+         /* 00100100 format 4    addr .b   */
(4*16)),    /* 00100101 format 4    addr .w   */
(13+        /* 00100110 format 19   undefined */
(4*16)),    /* 00100111 format 4    addr .l   */
(4+         /* 00101000 format 4    and .b    */
(4*16)),    /* 00101001 format 4    and .w    */
(0+         /* 00101010 format 0    bcs   */
(4*16)),    /* 00101011 format 4    and .l    */
(2+         /* 00101100 format 2    spr .b    */
(2*16)),    /* 00101101 format 2    spr .w    */
(8+         /* 00101110 format 8          */
(2*16)),    /* 00101111 format 2    spr .l    */
(4+         /* 00110000 format 4    subc .b   */
(4*16)),    /* 00110001 format 4    subc .w   */
(1+         /* 00110010 format 1    rxp   */
(4*16)),    /* 00110011 format 4    subc .l   */
(4+         /* 00110100 format 4    tbit .b   */
(4*16)),    /* 00110101 format 4    tbit .w   */
(15+        /* 00110110 format 15         */
(4*16)),    /* 00110111 format 4    tbit .l   */
(4+         /* 00111000 format 4    xor .b    */
(4*16)),    /* 00111001 format 4    xor .w    */
(0+         /* 00111010 format 0    bcc   */
(4*16)),    /* 00111011 format 4    xor .l    */
(2+         /* 00111100 format 2    scond .b */
(2*16)),    /* 00111101 format 2    scond .w */
(9+         /* 00111110 format 9          */
(2*16)),    /* 00111111 format 2    scond .l */
(4+         /* 01000000 format 4    add .b    */
(4*16)),    /* 01000001 format 4    add .w    */
(1+         /* 01000010 format 1    rett      */
(4*16)),    /* 01000011 format 4    add .l    */
(4+         /* 01000100 format 4    cmp .b    */
(4*16)),    /* 01000101 format 4    cmp .w    */
(13+        /* 01000110 format 19   undefined */
(4*16)),    /* 01000111 format 4    cmp .l    */
(4+         /* 01001000 format 4    bic .b    */
(4*16)),    /* 01001001 format 4    bic .w    */
(0+         /* 01001010 format 0    bhi   */
(4*16)),    /* 01001011 format 4    bic .l    */
(2+         /* 01001100 format 2    acb .b    */
(2*16)),    /* 01001101 format 2    acb .w    */
(6+         /* 01001110 format 6          */
(2*16)),    /* 01001111 format 2    acb .l    */
(4+         /* 01010000 format 4    addc .b   */
(4*16)),    /* 01010001 format 4    addc .w   */
(1+         /* 01010010 format 1    reti      */
(4*16)),    /* 01010011 format 4    addc .l   */
(4+         /* 01010100 format 4    mov .b    */
(4*16)),    /* 01010101 format 4    mov .w    */
(15+        /* 01010110 format 15         */
(4*16)),    /* 01010111 format 4    mov .l    */
(4+         /* 01011000 format 4    or .b     */
(4*16)),    /* 01011001 format 4    or .w     */
(0+         /* 01011010 format 0    bis   */
(4*16)),    /* 01011011 format 4    or .l     */
(2+         /* 01011100 format 2    movq .b   */
(2*16)),    /* 01011101 format 2    movq .w   */
(13+        /* 01011110 format 16   undefined */
(2*16)),    /* 01011111 format 2    movq .l   */
(4+         /* 01100000 format 4    sub .b    */
(4*16)),    /* 01100001 format 4    sub .w    */
(1+         /* 01100010 format 1    save      */
(4*16)),    /* 01100011 format 4    sub .l    */
(4+         /* 01100100 format 4    addr .b   */
(4*16)),    /* 01100101 format 4    addr .w   */
(13+        /* 01100110 format 19   undefined */
(4*16)),    /* 01100111 format 4    addr .l   */
(4+         /* 01101000 format 4    and .b    */
(4*16)),    /* 01101001 format 4    and .w    */
(0+         /* 01101010 format 0    bgt   */
(4*16)),    /* 01101011 format 4    and .l    */
(2+         /* 01101100 format 2    lpr .b    */
(2*16)),    /* 01101101 format 2    lpr .w    */
(8+         /* 01101110 format 8          */
(2*16)),    /* 01101111 format 2    lpr .l    */
(4+         /* 01110000 format 4    subc .b   */
(4*16)),    /* 01110001 format 4    subc .w   */
(1+         /* 01110010 format 1    restore   */
(4*16)),    /* 01110011 format 4    subc .l   */
(4+         /* 01110100 format 4    tbit .b   */
(4*16)),    /* 01110101 format 4    tbit .w   */
(15+        /* 01110110 format 15         */
(4*16)),    /* 01110111 format 4    tbit .l   */
(4+         /* 01111000 format 4    xor .b    */
(4*16)),    /* 01111001 format 4    xor .w    */
(0+         /* 01111010 format 0    ble   */
(4*16)),    /* 01111011 format 4    xor .l    */
(3+         /* 01111100 format 3          */
(3*16)),    /* 01111101 format 3          */
(13+        /* 01111110 format 10   undefined */
(3*16)),    /* 01111111 format 3          */
(4+         /* 10000000 format 4    add .b    */
(4*16)),    /* 10000001 format 4    add .w    */
(1+         /* 10000010 format 1    enter     */
(4*16)),    /* 10000011 format 4    add .l    */
(4+         /* 10000100 format 4    cmp .b    */
(4*16)),    /* 10000101 format 4    cmp .w    */
(13+        /* 10000110 format 19   undefined */
(4*16)),    /* 10000111 format 4    cmp .l    */
(4+         /* 10001000 format 4    bic .b    */
(4*16)),    /* 10001001 format 4    bic .w    */
(0+         /* 10001010 format 0    bfs   */
(4*16)),    /* 10001011 format 4    bic .l    */
(2+         /* 10001100 format 2    addq .b   */
(2*16)),    /* 10001101 format 2    addq .w   */
(13+        /* 10001110 format 18   undefined */
(2*16)),    /* 10001111 format 2    addq .l   */
(4+         /* 10010000 format 4    addc .b   */
(4*16)),    /* 10010001 format 4    addc .w   */
(1+         /* 10010010 format 1    exit      */
(4*16)),    /* 10010011 format 4    addc .l   */
(4+         /* 10010100 format 4    mov .b    */
(4*16)),    /* 10010101 format 4    mov .w    */
(15+        /* 10010110 format 15         */
(4*16)),    /* 10010111 format 4    mov .l    */
(4+         /* 10011000 format 4    or .b     */
(4*16)),    /* 10011001 format 4    or .w     */
(0+         /* 10011010 format 0    bfc   */
(4*16)),    /* 10011011 format 4    or .l     */
(2+         /* 10011100 format 2    cmpq .b   */
(2*16)),    /* 10011101 format 2    cmpq .w   */
(13+        /* 10011110 format 13   undefined */
(2*16)),    /* 10011111 format 2    cmpq .l   */
(4+         /* 10100000 format 4    sub .b    */
(4*16)),    /* 10100001 format 4    sub .w    */
(10+        /* 10100010 format 1    nop       */
(4*16)),    /* 10100011 format 4    sub .l    */
(4+         /* 10100100 format 4    addr .b   */
(4*16)),    /* 10100101 format 4    addr .w   */
(13+        /* 10100110 format 19   undefined */
(4*16)),    /* 10100111 format 4    addr .l   */
(4+         /* 10101000 format 4    and .b    */
(4*16)),    /* 10101001 format 4    and .w    */
(0+         /* 10101010 format 0    blo   */
(4*16)),    /* 10101011 format 4    and .l    */
(2+         /* 10101100 format 2    spr .b    */
(2*16)),    /* 10101101 format 2    spr .w    */
(8+         /* 10101110 format 8          */
(2*16)),    /* 10101111 format 2    spr .l    */
(4+         /* 10110000 format 4    subc .b   */
(4*16)),    /* 10110001 format 4    subc .w   */
(1+         /* 10110010 format 1    wait      */
(4*16)),    /* 10110011 format 4    subc .l   */
(4+         /* 10110100 format 4    tbit .b   */
(4*16)),    /* 10110101 format 4    tbit .w   */
(15+        /* 10110110 format 15         */
(4*16)),    /* 10110111 format 4    tbit .l   */
(4+         /* 10111000 format 4    xor .b    */
(4*16)),    /* 10111001 format 4    xor .w    */
(0+         /* 10111010 format 0    bhs   */
(4*16)),    /* 10111011 format 4    xor .l    */
(2+         /* 10111100 format 2    scond .b */
(2*16)),    /* 10111101 format 2    scond .w */
(11+        /* 10111110 format 11         */
(2*16)),    /* 10111111 format 2    scond .l */
(4+         /* 11000000 format 4    add .b    */
(4*16)),    /* 11000001 format 4    add .w    */
(1+         /* 11000010 format 1    dia   */
(4*16)),    /* 11000011 format 4    add .l    */
(4+         /* 11000100 format 4    cmp .b    */
(4*16)),    /* 11000101 format 4    cmp .w    */
(13+        /* 11000110 format 19   undefined */
(4*16)),    /* 11000111 format 4    cmp .l    */
(4+         /* 11001000 format 4    bic .b    */
(4*16)),    /* 11001001 format 4    bic .w    */
(0+         /* 11001010 format 0    blt   */
(4*16)),    /* 11001011 format 4    bic .l    */
(2+         /* 11001100 format 2    acb .b    */
(2*16)),    /* 11001101 format 2    acb .w    */
(7+         /* 11001110 format 7          */
(2*16)),    /* 11001111 format 2    acb .l    */
(4+         /* 11010000 format 4    addc .b   */
(4*16)),    /* 11010001 format 4    addc .w   */
(1+         /* 11010010 format 1    flag      */
(4*16)),    /* 11010011 format 4    addc .l   */
(4+         /* 11010100 format 4    mov .b    */
(4*16)),    /* 11010101 format 4    mov .w    */
(15+        /* 11010110 format 15         */
(4*16)),    /* 11010111 format 4    mov .l    */
(4+         /* 11011000 format 4    or .b     */
(4*16)),    /* 11011001 format 4    or .w     */
(0+         /* 11011010 format 0    bqt   */
(4*16)),    /* 11011011 format 4    or .l     */
(2+         /* 11011100 format 2    movq .b   */
(2*16)),    /* 11011101 format 2    movq .w   */
(13+        /* 11011110 format 17   undefined */
(2*16)),    /* 11011111 format 2    movq .l   */
(4+         /* 11100000 format 4    sub .b    */
(4*16)),    /* 11100001 format 4    sub .w    */
(1+         /* 11100010 format 1    svc   */
(4*16)),    /* 11100011 format 4    sub .l    */
(4+         /* 11100100 format 4    addr .b   */
(4*16)),    /* 11100101 format 4    addr .w   */
(13+        /* 11100110 format 19   undefined */
(4*16)),    /* 11100111 format 4    addr .l   */
(4+         /* 11101000 format 4    and .b    */
(4*16)),    /* 11101001 format 4    and .w    */
(0+         /* 11101010 format 0    b     */
(4*16)),    /* 11101011 format 4    and .l    */
(2+         /* 11101100 format 2    lpr .b    */
(2*16)),    /* 11101101 format 2    lpr .w    */
(8+         /* 11101110 format 8          */
(2*16)),    /* 11101111 format 2    lpr .l    */
(4+         /* 11110000 format 4    subc .b   */
(4*16)),    /* 11110001 format 4    subc .w   */
(1+         /* 11110010 format 1    bpt   */
(4*16)),    /* 11110011 format 4    subc .l   */
(4+         /* 11110100 format 4    tbit .b   */
(4*16)),    /* 11110101 format 4    tbit .w   */
(15+        /* 11110110 format 15         */
(4*16)),    /* 11110111 format 4    tbit .l   */
(4+         /* 11111000 format 4    xor .b    */
(4*16)),    /* 11111001 format 4    xor .w    */
(10+        /* 11111010 format 0    nop   */
(4*16)),    /* 11111011 format 4    xor .l    */
(13+        /* 11111100 format 3    undefined */
(13*16)),   /* 11111101 format 3    undefined */
((12+       /* 11111110 format 12   new 532 */
13*16)) };  /* 11111111 format 3    undefined */

CONST
char fmt1_table[16][8] = {
    "bsr",      /* branch to subroutine */
    "ret",      /* return from subroutine */
    "cxp",      /* call external procedure */
    "rxp",      /* return from external procedure */
    "rett",     /* return from trap */
    "reti",     /* return from interrupt */
    "save",     /* save general purpose registers */
    "restore",  /* restore general purpose registers */
    "enter",    /* enter new procedure context */
    "exit",     /* exit procedure context */
    "nop",      /* no operation */
    "wait",     /* wait fro interrupt */
    "dia",      /* diagnose */
    "flag",     /* flag trap */
    "svc",      /* supervisor call trap */
    "bpt"       /* breakpoint trap */
};

#define FMT1_BSR    0x0 /* branch to subroutine */
#define FMT1_RET    0x1 /* return from subroutine */
#define FMT1_CXP    0x2 /* call external procedure */
#define FMT1_RXP    0x3 /* return from external procedure */
#define FMT1_RETT   0x4 /* return from trap */
#define FMT1_RETI   0x5 /* return from interrupt */
#define FMT1_SAVE   0x6 /* save general purpose registers */
#define FMT1_RESTORE    0x7 /* restore general purpose registers */
#define FMT1_ENTER  0x8 /* enter new procedure context */
#define FMT1_EXIT   0x9 /* exit procedure context */
#define FMT1_NOP    0xa /* no operation */
#define FMT1_WAIT   0xb /* wait fro interrupt */
#define FMT1_DIA    0xc /* diagnose */
#define FMT1_FLAG   0xd /* flag trap */
#define FMT1_SVC    0xe /* supervisor call trap */
#define FMT1_BPT    0xf /* breakpoint trap */

CONST
char fmt2_table[7][5] = {
    "addq",     /* add quick */
    "cmpq",     /* compare quick */
    "spr",      /* save processor register */
    "s",        /* save condition as boolean */
    "acb",      /* add, compare and branch */
    "movq",     /* move quick */
    "lpr"       /* load processor register */
};

#define FMT2_ADDQ   0x0 /* add quick */
#define FMT2_CMPQ   0x1 /* compare quick */
#define FMT2_SPR    0x2 /* store processr oregister */
#define FMT2_SCOND  0x3 /* save condition as boolean */
#define FMT2_ACB    0x4 /* add, compare and branch */
#define FMT2_MOVQ   0x5 /* move quick */
#define FMT2_LPR    0x6 /* load processor register */

CONST
char fmt3_table[8][7] = {
    "cxpd",     /* call external procedure with descriptor */
    "bicpsr",   /* bit clear in PSR */
    "jump",     /* jump */
    "bispsr",   /* bit set in PSR */
    "??3??",    /* UNDEFINED */
    "adjsp",    /* adjust stack pointer */
    "jsr",      /* jump to subroutine */
    "case"      /* case branch */
};

#define FMT3_CXPD   0x0 /* call external procedure with descriptor */
#define FMT3_BICPSR     0x1 /* bit clear in PSR */
#define FMT3_JUMP   0x2 /* jump */
#define FMT3_BISPSR 0x3 /* bit set in PSR */
#define FMT3_UNDEF  0x4 /* UNDEFINED */
#define FMT3_ADJSP  0x5 /* adjust stack pointer */
#define FMT3_JSR    0x6 /* jump to subroutine */
#define FMT3_CASE   0x7 /* case branch */

CONST
char fmt4_table[16][5] = {
    "add",      /* add */
    "cmp",      /* compare */
    "bic",      /* bit clear */
    "?4?",      /* UNDEFINED */
    "addc",     /* add with carry */
    "mov",      /* move */
    "or",       /* or */
    "?4?",      /* UNDEFINED */
    "sub",      /* subtract */
    "addr",     /* compute effective address */
    "and",      /* and */
    "?4?",      /* UNDEFINED */
    "subc",     /* subtract with carry */
    "tbit",     /* test bit */
    "xor",      /* exclusive or */
    "?4?"       /* UNDEFINED */
};

#define FMT4_ADD    0x0 /* add */
#define FMT4_CMP    0x1 /* compare */
#define FMT4_BIC    0x2 /* bit clear */
#define FMT4_ADDC   0x4 /* add with carry */
#define FMT4_MOV    0x5 /* move */
#define FMT4_OR     0x6 /* or */
#define FMT4_SUB    0x8 /* subtract */
#define FMT4_ADDR   0x9 /* compute effective address */
#define FMT4_AND    0xa /* and */
#define FMT4_SUBC   0xc /* subtract with carry */
#define FMT4_TBIT   0xd /* test bit */
#define FMT4_XOR    0xe /* exclusive or */

CONST
char fmt5_table[4][7] = {
    "movs",     /* move string */
    "cmps",     /* compare string */
    "setcfg",   /* set configuration register */
    "skps"      /* skip string */
};

#define FMT5_MOVS   0x0 /* move string */
#define FMT5_CMPS   0x1 /* compare string */
#define FMT5_SETCFG 0x2 /* set configuration register */
#define FMT5_SKPS   0x3 /* skip string */

CONST
char fmt6_table[16][6] = {
    "rot",      /* rotate */
    "ash",      /* arithmetic shift */
    "cbit",     /* clear bit */
    "cbiti",    /* clear bit interlocked */
    "??6??",    /* undefined */
    "lsh",      /* logical shift */
    "sbit",     /* set bit */
    "sbiti",    /* set bit interlocked */
    "neg",      /* negate */
    "not",      /* not */
    "??6??",    /* undefined */
    "subp",     /* subtract packed decimal */
    "abs",      /* absolute value */
    "com",      /* complement */
    "ibit",     /* invert bit */
    "addp"      /* add packed decimal */
};

#define FMT6_ROT    0x0 /* rotate */
#define FMT6_ASH    0x1 /* arithmetic shift */
#define FMT6_CBIT   0x2 /* clear bit */
#define FMT6_CBITI  0x3 /* clear bit interlocked */
#define FMT6_UNDEF1 0x4 /* undefined */
#define FMT6_LSH    0x5 /* logical shift */
#define FMT6_SBIT   0x6 /* s#define FMT6_NOT    0x9 /* not */
#define FMT6_UNDEF2 0xa /* undefined */
#define FMT6_SUBP   0xb /* subtract packed decimal */
#define FMT6_ABS    0xc /* absolute value */
#define FMT6_COM    0xd /* complement */
#define FMT6_IBIT   0xe /* invert bit */
#define FMT6_ADDP   0xf /* add packed decimal */

CONST
char fmt7_table[16][7] = {
    "movm",     /* move multiple */
    "cmpm",     /* compare multiple */
    "inss",     /* insert field short */
    "exts",     /* extract field short */
    "movxb",        /* move with sign-extention byte to word */
    "movzb",        /* move with zero-extention byte to word */
    "movz",         /* move with zero extention i to double */
    "movx",         /* move with sign-extention i to double */
    "mul",      /* multiply */
    "mei",      /* multiply extended integer */
    "?7?",      /* undefined */
    "dei",      /* divide extended integer */
    "quo",      /* quotient */
    "rem",      /* remainder */
    "mod",      /* modulus */
    "div"       /* divide */
};

#define FMT7_MOVM   0x0 /* move multiple */
#define FMT7_CMPM   0x1 /* compare multiple */
#define FMT7_INSS   0x2 /* insert field short */
#define FMT7_EXTS   0x3 /* extract field short */
#define FMT7_MOVXBW     0x4       /* move with sign-extention byte to word */
#define FMT7_MOVZBW     0x5       /* move with zero-extention byte to word */
#define FMT7_MOVZD      0x6       /* move with zero extention i to double */
#define FMT7_MOVXD  0x7 /* move with sign-extention i to double */
#define FMT7_MUL    0x8 /* multiply */
#define FMT7_MEI    0x9 /* multiply extended integer */
#define FMT7_UNDEF  0xa /* undefined */
#define FMT7_DEI    0xb /* divide extended integer */
#define FMT7_QUO    0xc /* quotient */
#define FMT7_REM    0xd /* remainder */
#define FMT7_MOD    0xe /* modulus */
#define FMT7_DIV    0xf /* divide */

CONST
char fmt8_table[8][6] = {
    "ext",      /* extract field */
    "cvtp",     /* convert to bit pointer */
    "ins",      /* insert field */
    "check",    /* bounds check */
    "index",    /* calculate index */
    "ffs",      /* find first set bit */
    "mov",      /* move supervisor to/from user space */
    "??8??"     /* undefined */
};

#define FMT8_EXT    0x0 /* extract field */
#define FMT8_CVTP   0x1 /* convert to bit pointer */
#define FMT8_INS    0x2 /* insert field */
#define FMT8_CHECK  0x3 /* bounds check */
#define FMT8_INDEX  0x4 /* calculate index */
#define FMT8_FFS    0x5 /* find first set bit */
#define FMT8_MOV    0x6 /* move supervisor to/from user space */
#define FMT8_UNDEF  0x7 /* undefined */

CONST
char fmt9_table[8][6] = {
    "mov",      /* move converting integer to floating point */
    "lfsr",     /* load floating-point status register */
    "movlf",    /* move long floating to floating */
    "movfl",    /* move floating to long floating */
    "round",    /* round floating to integer */
    "trunc",    /* truncate floating to integer */
    "sfsr",     /* store floating-point status register */
    "floor"     /* floor floating to integer */
};

#define FMT9_MOV    0x0 /* move converting integer to floating point */
#define FMT9_LFSR   0x1 /* load floating-point status register */
#define FMT9_MOVLF  0x2 /* move long floating to floating */
#define FMT9_MOVFL  0x3 /* move floating to long floating */
#define FMT9_ROUND  0x4 /* round floating to integer */
#define FMT9_TRUNC  0x5 /* truncate floating to integer */
#define FMT9_SFSR   0x6 /* store floating-point status register */
#define FMT9_FLOOR      0x7     /* floor floating to integer */

#define NOP             0xff;   /* catch all nop instruction */

CONST
char fmt11_table[16][4] = {
    "add",      /* add floating */
    "mov",      /* move floating */
    "cmp",      /* compare floating */
    "?f?",      /* undefined */
    "sub",      /* subtract floating */
    "neg",      /* negate floating */
    "?f?",      /* undefined */
    "?f?",      /* undefined */
    "div",      /* divide floating */
    "?f?",      /* undefined */
    "?f?",      /* undefined */
    "?f?",      /* undefined */
    "mul",      /* multiply floating */
    "abs",      /* absolute value floating */
    "?f?",      /* undefined */
    "?f?"       /* undefined */
};

CONST
char fmt12_table[16][6] = {
    "?f?",      /* 0 undefined */
    "?f?",      /* 1 undefined */
    "poly",     /* 2 */
    "dot",      /* 3 */
    "scalb",    /* 4 */
    "logb",     /* 5 */
    "?f?",      /* 6 undefined */
    "?f?",      /* 7 undefined */
    "?f?",      /* 8 undefined */
    "?f?",      /* 9 undefined */
    "?f?",      /* 10 undefined */
    "?f?",      /* 11 undefined */
    "?f?",      /* 12 undefined */
    "?f?",      /* 13 undefined */
    "?f?",      /* 14 undefined */
    "?f?",      /* 15 undefined */
};

#define FMT11_ADD   0x0 /* add floating */
#define FMT11_MOV   0x1 /* move floating */
#define FMT11_CMP   0x2 /* compare floating */
#define FMT11_UNDEF1    0x3 /* undefined */
#define FMT11_SUB   0x4 /* subtract floating */
#define FMT11_NEG   0x5 /* negate floating */
#define FMT11_UNDEF2    0x6 /* undefined */
#define FMT11_UNDEF3    0x7 /* undefined */
#define FMT11_DIV   0x8 /* divide floating */
#define FMT11_UNDEF4    0x9 /* undefined */
#define FMT11_UNDEF5    0xa /* undefined */
#define FMT11_UNDEF6    0xb /* undefined */
#define FMT11_MUL   0xc /* multiply floating */
#define FMT11_ABS   0xd /* absolute value floating */
#define FMT11_UNDEF7    0xe /* undefined */
#define FMT11_UNDEF8    0xf /* undefined */

#define FMT12_POLY	2
#define FMT12_DOT	3
#define FMT12_SCALB	4
#define FMT12_LOGB	5

CONST
char fmt14_table[][6] = {
    "rdval",    /* validate address for reading */
    "wrval",    /* validate address for writing */
    "lmr",      /* load memory managemnet register */
    "smr",      /* store memory management register */
    "???",
    "???",
    "???",
    "???",
    "???",
    "cinv",
};

#define FMT14_RDVAL 0x0 /* validate address for reading */
#define FMT14_WRVAL 0x1 /* validate address for writing */
#define FMT14_LMR   0x2 /* load memory managemnet register */
#define FMT14_SMR   0x3 /* store memory management register */
#define FMT14_CINV  0x9 /* cache invalidate */
