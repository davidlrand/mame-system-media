/*
 * machine.h
 */

/* These are indices into regTable.  Keep in sync!
 */
#define REG_R0  	0                       /* General Register 0 */
#define REG_R7		(REG_R0+7)              /* General Register 7 */
#define REG_F0		8                       /* Floating Point  0 */
#define REG_F7		(REG_F0+7)              /* Floating Point  7 */
#define REG_F15		(REG_F0+15)             /* Floating Point  15 */
#define REG_L0		24

#define REG_PC		32                      /* Program counter */
#define REG_USP		(REG_PC+1)
#define REG_ISP		(REG_PC+2)
#define REG_FP		(REG_PC+3)
#define REG_SB		(REG_PC+4)
#define REG_INTBASE	(REG_PC+5)
#define REG_MOD		(REG_PC+6)
#define REG_PSR		(REG_PC+7)
#define REG_UPSR	(REG_PC+8)
#define REG_DCR		(REG_PC+9)
#define REG_DSR		(REG_PC+10)
#define REG_CAR		(REG_PC+11)
#define REG_BPC		(REG_PC+12)
#define REG_CFG		(REG_PC+13)
#define REG_FIRST_CPU	REG_PC
#define REG_LAST_CPU	REG_CFG

#define REG_PTB0	46
#define REG_PTB1	(REG_PTB0+1)
#define REG_IVAR0	(REG_PTB0+2)
#define REG_IVAR1	(REG_PTB0+3)
#define REG_TEAR	(REG_PTB0+4)
#define REG_MCR		(REG_PTB0+5)
#define REG_MSR		(REG_PTB0+6)
#define REG_FIRST_MMU	REG_PTB0
#define REG_LAST_MMU	REG_MSR

#define REG_FSR		53
#define REG_SP		(REG_FSR+1)
#define REG_NIL		(REG_FSR+2)
#define REG_DOT		(REG_FSR+3)
#define REG_V1		(REG_FSR+4)
#define REG_V2		(REG_FSR+5)
#define REG_BKPT0	(REG_FSR+6)
#define REG_BKPT7	(REG_FSR+13)
#define REG_RADIX	(REG_FSR+14)
#define FIRST_MODE	REG_RADIX
#if STANDALONE
#  define LAST_MODE	(FIRST_MODE+4)
#else
#  define LAST_MODE	(FIRST_MODE+3)
#endif

#define PSR_C  		0x0001		/* Carry Flag */
#define PSR_T		0x0002		/* Trace Flag */
#define PSR_L		0x0004		/* Low Flag */
#define PSR_V		0x0010		/* 'V' Flag */
#define PSR_F		0x0020		/* 'F' Flag */
#define PSR_Z		0x0040		/* Zero Flag */
#define PSR_N		0x0080		/* Negative Flag */
#define PSR_U		0x0100		/* User Mode Flag */
#define PSR_S		0x0200		/* Stack Flag */
#define PSR_P		0x0400		/* Trace Trap Pending Flag */
#define PSR_I		0x0800		/* Interrupt Enable Flag */

#define CFG_I   0001                /* Interrupt Vectoring */
#define CFG_F   0002                /* Floating-Point */
#define CFG_M   0004                /* Memory Management */
#define CFG_C   0010                /* Custom Instruction Set */

#define FSR_TT  000007              /* Trap Type */
#define FSR_TT_UF   001         /* Underflow */
#define FSR_TT_OF   010         /* Overflow */
#define FSR_TT_DZ   011         /* Divide by Zero */
#define FSR_TT_II   100         /* Illegial Instruction */
#define FSR_TT_IO   101         /* Invalid Operation */
#define FSR_TT_IR   110         /* Inexact Result */
#define FSR_UEN 000010              /* Underflow Enable */
#define FSR_UF  000020              /* Undeflow Flag */
#define FSR_IEN 000040              /* Inexact Result Enable */
#define FSR_IF  000100              /* Inexact Result Flag */
#define FSR_RM  000600              /* Rounding Mode */
#define FSR_RM_V    000000          /* Nearest Value */
#define FSR_RM_Z    000200          /* Toward Zero */
#define FSR_RM_P    000400          /* Toward Positive Infinity */
#define FSR_RM_N    000600          /* Toward Negitive Infinity */
#define FSR_SWF 177000              /* NSC Software Use */

struct mod_desc {               /* Module Descriptor */
    char    *md_sbase;          /* Static Base Pointer */
    char    *md_lbase;          /* Link Base Pointer */
    char    *md_pbase;          /* Program Base Pointer */
    char    *md_resv;           /* Reserved for future use */
};

