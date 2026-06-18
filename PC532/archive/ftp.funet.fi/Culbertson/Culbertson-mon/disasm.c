/* NSC 32000 ROM debugger.
 * Bruce Culbertson  Bob Krause
 *
 * Bob's disassembler
 */

#include "debugger.h"
#include "machine.h"
#include "dasm.h"
#if LSC
#include <stdio.h>
#include <io.h>
#include <unix.h>
#include <string.h>
#endif
#include "das32k.h"

#define GetShort(x, y)     (((x&0x80)>>7) + ((y&0x7)<<1))
#define GetGen0(x)         (((x)&0xf8)>>3)
#define GetGen1(x,y)       ((((x)&0xc0)>>6) + (((y)&0x7)<<2))
#define GetGenSI(x)       (int)((((x)&0x1c) == 0x1c) ? (((x)&0x3) + 1) : 0)
#define ScaledFields(input, reg, gen) \
            ((reg) = ((input)&0x7), \
             (gen) = (((input)&0xf8)>>3))

CONST unsigned char cpuRegTable [] = {
  REG_UPSR,  REG_DCR,   REG_BPC,     REG_DSR,
  REG_CAR,   REG_NIL,   REG_NIL,     REG_NIL,
  REG_FP,    REG_SP,    REG_SB,      REG_USP,
  REG_CFG,   REG_PSR,   REG_INTBASE, REG_MOD,
};

CONST unsigned char mmuRegTable [] = {
  REG_NIL,   REG_NIL,   REG_NIL,     REG_NIL,
  REG_NIL,   REG_NIL,   REG_NIL,     REG_NIL,
  REG_NIL,   REG_MCR,   REG_MSR,     REG_TEAR,
  REG_PTB0,  REG_PTB1,  REG_IVAR0,   REG_IVAR1,
};

int
formatOperand(operand, text)
struct operand *operand;
unsigned char text[];
{
    int need_comma, i, mask, textlen;

    switch (operand->o_mode) {

    case AMODE_REG :
    case AMODE_AREG :
    case AMODE_MREG :
        textlen = mySprintf(text, "%s", regTable[operand->o_reg0].name)
		+ PRINTFINC;
        break;

    case AMODE_MREL :
        textlen = mySprintf(text, "%ld(%ld(%s))",
            operand->o_disp1, operand->o_disp0,
            regTable[operand->o_reg0].name) + PRINTFINC;
        break;
        
    case AMODE_QUICK :
    case AMODE_IMM :
        textlen = mySprintf(text, "%ld", operand->o_disp0) + PRINTFINC;
        break;
        
    case AMODE_ABS :
        textlen = mySprintf(text, "@%ld", operand->o_disp0) + PRINTFINC;
        break;
        
    case AMODE_EXT :
        textlen = mySprintf(text, "ext(%ld)", operand->o_disp0) + PRINTFINC;
        if (operand->o_disp1)
            textlen += mySprintf(&text[textlen], "+%ld", operand->o_disp1) 
		+ PRINTFINC;
        break;
        
    case AMODE_TOS :
        strcat(text, "tos");
        textlen = 3;
        break;

    case AMODE_RREL :
    case AMODE_MSPC :
        textlen = mySprintf(text, "%ld(%s)", operand->o_disp0,
            regTable[operand->o_reg0].name) + PRINTFINC;
        break;
        
    case AMODE_REGLIST :
        strcpy(text, "[");
        textlen = 1;
        need_comma = 0;
        for (i = 0; i < 8; i++) {
            mask = (1<<i);
            if (operand->o_reg0&mask) {
                if (need_comma) {
                    strcat(&text[textlen], ",");
                    textlen++;
                }
                textlen += mySprintf(&text[textlen], "r%d", i) + PRINTFINC;
                need_comma = 1;
            }
        }
        strcat(&text[textlen], "]");
        textlen++;
        break;
        
    case AMODE_BLISTB :
        i = 7;
        goto bitlist;
        
    case AMODE_BLISTW :
        i = 15;
        goto bitlist;

    case AMODE_BLISTD :
        i = 31;

bitlist :
        strcpy(text, "B'");
        textlen = 2;
        for (; i >= 0; i--, textlen++) {
            mask = (1<<i);
            if (operand->o_disp0&mask)
                strcat(&text[textlen], "1");
            else
                strcat(&text[textlen], "0");
        }
        break;

    case AMODE_SOPT :
        i = operand->o_disp0>>1;
        textlen = mySprintf(text, "%s", sopt_table[i]) + PRINTFINC;
        break;

    case AMODE_CFG :
        strcpy(text, "[");
        textlen = 1;
        need_comma = 0;
        for (i = 0;i < 3;i++) {
            mask = 1<<i;
            if (operand->o_disp0&mask) {
                if (need_comma) {
                    strcat(&text[textlen], ",");
                    textlen++;
                }
                strcat(&text[textlen], cfg_table[i]);
                textlen++;
                need_comma = 1;
            }
        }
        strcat(&text[textlen], "]");
        break;

    case AMODE_CINV: {			/* print cache invalidate flags */
      char *p;
      int i, need_comma = 0;

      textlen = 0;
      for (i = 4, p = "AID"; i; i >>= 1, ++p)
	if (i & operand->o_disp0) {
	  if (need_comma)
	    text[textlen++] = ',';
	  text[textlen++] = *p;
	  text[textlen] = '\0';
	  need_comma = 1;
	}
      }
      break;

    case AMODE_INVALID :
        strcpy(text, "?");
        textlen = 1;
        break;
    }
    if (operand->o_iscale) {
        textlen += mySprintf(&text[textlen], "[r%d:", operand->o_ireg) 
	  + PRINTFINC;
        strcat(&text[textlen], scale_table[operand->o_iscale]);
        textlen++;
        strcat(&text[textlen], "]");
        textlen++;
    }
    operand->o_mode = AMODE_NONE;
    return(textlen);
}

int
formatAsm(insn, text)
struct insn *insn;
unsigned char text[];
{
    int i, textlen;

    textlen = mySprintf(&text[0], "%s\t", &insn->i_monic[0]) + PRINTFINC;

    for (i = 0; i < 4; i++) {
        if (insn->i_opr[i].o_mode == AMODE_NONE) {
            return(textlen);
        }
        if (i != 0) {
            strcat(text, ",");
            textlen++;
        }
        textlen += formatOperand(&insn->i_opr[i], &text[textlen]);
    }
    return(textlen);
}

disassemble(p)
char *p;
{
    char text[128];
    struct insn insn;
    int ret;
    long cnt;
    unsigned char *adr;

    ret = getIntScan (&p, &adr);
    if (ret == NO_NUM) {
        adr = Dot;
        cnt = 1;
    }
    else if (ret == BAD_NUM) {
	printf ("Bad address\n");
	return;
    }
    ret = getIntScan (&p, &cnt);
    if (ret == NO_NUM)
        cnt = 1;
    else if (ret == BAD_NUM) {
        morePrintf (1, "Bad count\n");
        return;
    }
    Dot = adr;
    while (cnt > 0) {
        int ate;
        
        initInsn (&insn);
        ate = dasm_ns32k(&insn, Dot + BASE);
        formatAsm(&insn, text);
        morePrintf(1, "%08lx\t%s\n", Dot, text);
        Dot += ate;
        cnt--;
    }
}

initInsn (insn)
struct insn *insn;
{
    insn->i_opr[0].o_mode = AMODE_NONE;
    insn->i_opr[0].o_iscale = 0;
    insn->i_opr[1].o_mode = AMODE_NONE;
    insn->i_opr[1].o_iscale = 0;
    insn->i_opr[2].o_mode = AMODE_NONE;
    insn->i_opr[2].o_iscale = 0;
    insn->i_opr[3].o_mode = AMODE_NONE;
    insn->i_opr[3].o_iscale = 0;
}

int
disp(machcode, result)
unsigned char *machcode;
long *result;
{
    if (!(*machcode&0x80)) {			/* one byte */
      *result =
	((*machcode&0x40)? 0xffffffc0L: 0) |
	(*machcode & 0x3f);
      return(1);
    } else if (!(*machcode&0x40)) {		/* two byte */
      *result =
        ((*machcode&0x20)? 0xffffe000L: 0) |
        (*machcode&0x1f) << 8 |
        *(machcode+1);
      return 2;
    } else {					/* four byte */
      *result = 
        ((*machcode&0x20)? 0xe0000000L: 0) |	/* bug fix 8/28 */
        (*machcode&0x1f) << 24 |		/* bug fix 7/21 */
        *(machcode+1) << 16 |
        *(machcode+2) << 8 |
        *(machcode+3);
      return(4);
    }
}

decode_operand(buffer, byte, operand, iol)
unsigned char buffer[];
unsigned char byte;
struct operand *operand;
unsigned char iol;
{
    register int i, consumed = 0;
    long value;
    
    switch (byte) {
        
    case GEN_R0 :
    case GEN_R1 :
    case GEN_R2 :
    case GEN_R3 :
    case GEN_R4 :
    case GEN_R5 :
    case GEN_R6 :
    case GEN_R7 :
        operand->o_mode = AMODE_REG;
        operand->o_reg0 = REG_R0 + byte;
        break;

    case GEN_RR0 :
    case GEN_RR1 :
    case GEN_RR2 :
    case GEN_RR3 :
    case GEN_RR4 :
    case GEN_RR5 :
    case GEN_RR6 :
    case GEN_RR7 :
        operand->o_mode = AMODE_RREL;
        operand->o_reg0 = REG_R0 + (byte&0x7);
        goto one_disp;

    case GEN_FRMR :
        operand->o_reg0 = REG_FP;
        operand->o_mode = AMODE_MREL;
        goto two_disp;
        
    case GEN_SPMR :
        operand->o_reg0 = REG_SP;
        operand->o_mode = AMODE_MREL;
        goto two_disp;

    case GEN_SBMR :
        operand->o_reg0 = REG_SB;
        operand->o_mode = AMODE_MREL;
        goto two_disp;

    case GEN_IMM :
        operand->o_mode = AMODE_IMM;
	/* fix to sign extend */
	value = (*buffer & 0x80)? 0xffffffff: 0;
        for (i = 0; i < iol; i++) {
	    value = (value << 8) + buffer[i];
        }
        operand->o_disp0 = value;
        consumed = iol;
        break;
        
    case GEN_ABS :
        operand->o_mode = AMODE_ABS;
        goto one_disp;

    case GEN_EXT :
        operand->o_mode = AMODE_EXT;
        goto two_disp;

    case GEN_TOS :
        operand->o_mode = AMODE_TOS;
        break;

    case GEN_FRM :
        operand->o_mode = AMODE_MSPC;
        operand->o_reg0 = REG_FP;
        goto one_disp;

    case GEN_SPM :
        operand->o_mode = AMODE_MSPC;
        operand->o_reg0 = REG_SP;
        goto one_disp;

    case GEN_SBM :
        operand->o_mode = AMODE_MSPC;
        operand->o_reg0 = REG_SB;
        goto one_disp;

    case GEN_PCM :
        operand->o_mode = AMODE_MSPC;
        operand->o_reg0 = REG_PC;
        goto one_disp;
        
    default :
        operand->o_mode = AMODE_INVALID;
        break;
        
two_disp :
        consumed = disp(buffer, &operand->o_disp0);
        consumed += disp(&buffer[consumed], &operand->o_disp1);
        break;
        
one_disp :
        consumed = disp(buffer, &operand->o_disp0);
        break;
    }
    return(consumed);
}

int
gen(insn, buffer, mask, byte0, byte1)
struct insn *insn;
unsigned char buffer[];
int mask;				/* 1 to get gen1, 2 to get gen2 */
unsigned char byte0, byte1;
{
    int opr = 0, opr2, consumed = 0;
    unsigned char gen0, gen1;
    
    gen0 = GetGen0(byte1);		/* mask and shift gen fields */
    gen1 = GetGen1(byte0, byte1);	/* gen0 is really 1, gen1 is 2 */
    
    while (insn->i_opr[opr].o_mode != AMODE_NONE)
        opr++;

    if (mask&0x1) {
        if (insn->i_opr[opr].o_iscale = GetGenSI(gen0)) {
            ScaledFields(buffer[0], insn->i_opr[opr].o_ireg,
                gen0);
            consumed++;
        }
        opr2 = opr + 1;
    }
    else
        opr2 = opr;

    if (mask&0x2 &&
        (insn->i_opr[opr2].o_iscale = GetGenSI(gen1))) {
        ScaledFields(buffer[consumed], insn->i_opr[opr2].o_ireg,
            gen1);
        consumed++;
    }

    if (mask&0x1)
        consumed += decode_operand(&buffer[consumed], gen0,
            &insn->i_opr[opr], insn->i_iol);
    if (mask&0x2)
        consumed += decode_operand(&buffer[consumed], gen1,
            &insn->i_opr[opr2], insn->i_iol);

    return(consumed);
}       

int
dasm_ns32k(insn, buffer)
struct insn *insn;
unsigned char buffer[];
{
    unsigned char byte0, byte1, byte2;
    int i, j;
    int consumed;
    
    insn->i_iol = IOL_NONE;         /* Don't assume any operand length */
    byte0 = buffer[0];      /* look at first byte in insn */
    consumed = 1;
    i = byte0 / 2;          /* get index into fmttab */
    if (byte0 % 2)
        j = ((fmttab[i]&0xf0)>>4);
    else
        j = (fmttab[i])&0x0f;

    insn->i_format = j;     /* set insn type */
    
    switch (j) {
        
    case ITYPE_FMT0 :
        insn->i_op = FMT0_COND(byte0);   /* condition code field */
        insn->i_monic[0] = 'b';
        insn->i_monic[1] = cond_table[insn->i_op][0];
        insn->i_monic[2] = cond_table[insn->i_op][1];
        insn->i_monic[3] = '\0';
        insn->i_opr[0].o_mode = AMODE_IMM;      /* MSPC implied */
        insn->i_opr[0].o_reg0 = REG_PC;
        consumed += disp(&buffer[consumed], &insn->i_opr[0].o_disp0);
        break;
        
    case ITYPE_FMT1 :
        insn->i_op = FMT1_OP(byte0);
        strcpy(insn->i_monic, fmt1_table[insn->i_op]);
        switch (insn->i_op) {

        case FMT1_CXP :
        case FMT1_RXP :
        case FMT1_RET :
        case FMT1_RETT :
            insn->i_opr[0].o_mode = AMODE_IMM;
            consumed += disp(&buffer[consumed],
                &insn->i_opr[0].o_disp0);
            break;
            
        case FMT1_BSR :
            insn->i_opr[0].o_mode = AMODE_IMM;      /* MSPC implied */
            insn->i_opr[0].o_reg0 = REG_PC;
            consumed += disp(&buffer[consumed],
                &insn->i_opr[0].o_disp0);
            break;
            
        case FMT1_SAVE :
        case FMT1_RESTORE :
        case FMT1_ENTER :
        case FMT1_EXIT :
            insn->i_opr[0].o_mode = AMODE_REGLIST;
            insn->i_opr[0].o_reg0 = buffer[consumed++];
	    if (insn->i_op == FMT1_EXIT ||
	    insn->i_op == FMT1_RESTORE)			/* WBC bug fix */
		reverseBits (&(insn->i_opr[0].o_reg0));
            if (insn->i_op == FMT1_ENTER) {
                insn->i_opr[1].o_mode = AMODE_IMM;      /* MSPC implied */
                insn->i_opr[1].o_reg0 = REG_PC;
                consumed += disp(&buffer[consumed],
                    &insn->i_opr[1].o_disp0);
            }
            break;
        }
        break;
        
    case ITYPE_FMT2 :
        byte1 = buffer[consumed++];
        insn->i_op = FMT2_OP(byte0);
        insn->i_iol = IOL(byte0);
        i = GetShort(byte0, byte1);
        strcpy(insn->i_monic, fmt2_table[insn->i_op]);
        switch (insn->i_op) {

        case FMT2_SCOND :
            strcat(insn->i_monic, cond_table[i]);
            break;

        case FMT2_ADDQ :
        case FMT2_CMPQ :
        case FMT2_ACB :
        case FMT2_MOVQ :
            if (i&0x08) {           /* negative quick value */
                insn->i_opr[0].o_disp0 = i;
                insn->i_opr[0].o_disp0 |= 0xfffffff0;
            }
            else
                insn->i_opr[0].o_disp0 = i;
            insn->i_opr[0].o_mode = AMODE_QUICK;
            break;
            
        case FMT2_LPR :
        case FMT2_SPR :
	    insn->i_opr[0].o_reg0 = cpuRegTable [i];
            insn->i_opr[0].o_mode = AMODE_AREG;
            break;
        }
        consumed += gen(insn, &buffer[consumed], 0x1, 0, byte1);
        strcat(insn->i_monic, iol_table[insn->i_iol]);
        if (insn->i_op == FMT2_ACB) {
            consumed += disp(&buffer[consumed],
                &insn->i_opr[2].o_disp0);
            insn->i_opr[2].o_mode = AMODE_IMM;
        }
        break;
        
    case ITYPE_FMT3 :
        insn->i_format = ITYPE_FMT3;
        byte1 = buffer[consumed++];
        insn->i_op = FMT3_OP(byte1);
        insn->i_iol = IOL(byte0);
        strcpy(insn->i_monic, fmt3_table[insn->i_op]);
        consumed += gen(insn, &buffer[consumed], 0x1, 0, byte1);
        switch (insn->i_op) {
            
        case FMT3_CXPD :
        case FMT3_JUMP :
        case FMT3_JSR :
            if (insn->i_iol != IOL_DOUBLE)
                insn->i_format = ITYPE_UNDEF;
            break;
            
        case FMT3_BICPSR :
        case FMT3_BISPSR :
            if (insn->i_iol == IOL_DOUBLE) {
                insn->i_format = ITYPE_UNDEF;
                break;
            }
            if (insn->i_opr[0].o_mode == AMODE_IMM) {
                if (insn->i_iol == IOL_BYTE)
                    insn->i_opr[0].o_mode = AMODE_BLISTB;
                else
                    insn->i_opr[0].o_mode = AMODE_BLISTW;
            }
        /* fall through */

        case FMT3_CASE :
        case FMT3_ADJSP :
            strcat(insn->i_monic, iol_table[insn->i_iol]);
            break;
                    
        case FMT3_UNDEF :
            insn->i_format = ITYPE_UNDEF;
            break;
            
        }
        break;
        
    case ITYPE_FMT4 :
        byte1 = buffer[consumed++];
        insn->i_op = FMT4_OP(byte0);
        insn->i_iol = IOL(byte0);
        strcpy(insn->i_monic, fmt4_table[insn->i_op]);
        consumed += gen(insn, &buffer[consumed], 0x3, byte0,
            byte1);
        if (insn->i_op == FMT4_ADDR) {
            if (insn->i_iol != IOL_DOUBLE)
                insn->i_format = ITYPE_UNDEF;
        }
        else
            strcat(insn->i_monic, iol_table[insn->i_iol]);
        break;
        
    case ITYPE_FMT5 :
        byte1 = buffer[consumed++];
        insn->i_op = FMT5_OP(byte1);
        if (insn->i_op > FMT5_SKPS) {
            insn->i_format = ITYPE_UNDEF;
            break;
        }
        strcpy(insn->i_monic, fmt5_table[insn->i_op]);
        byte2 = buffer[consumed++];
        insn->i_opr[0].o_disp0 = GetShort(byte1, byte2);
        insn->i_iol = IOL(byte1);
        switch (insn->i_op) {

        case FMT5_MOVS :
        case FMT5_CMPS :
        case FMT5_SKPS :
            if (insn->i_opr[0].o_disp0&0x1)
                strcat(insn->i_monic, "t");
            else
                strcat(insn->i_monic,
                    iol_table[insn->i_iol]);
            insn->i_opr[0].o_mode = AMODE_SOPT;
            break;

        case FMT5_SETCFG :
            insn->i_opr[0].o_mode = AMODE_CFG;
            break;
        }
        break;
        
    case ITYPE_FMT6 :
        byte1 = buffer[consumed++];
        byte2 = buffer[consumed++];
        insn->i_op = FMT6_OP(byte1);
	insn->i_iol = IOL(byte1);
        strcpy(insn->i_monic, fmt6_table[insn->i_op]);
        strcat(insn->i_monic, iol_table[insn->i_iol]);
	if (insn->i_op == FMT6_ROT ||
	  insn->i_op == FMT6_ASH ||
	  insn->i_op == FMT6_LSH)
	{
          insn->i_iol = 1;	/* shift and rotate special case */
        }
        if (insn->i_op == FMT6_UNDEF1 ||
            insn->i_op == FMT6_UNDEF2) {
            insn->i_format = ITYPE_UNDEF;
            break;
        }
        consumed += gen(insn, &buffer[consumed], 0x3, byte1, byte2);
        break;
        
    case ITYPE_FMT7 :
        byte1 = buffer[consumed++];
        byte2 = buffer[consumed++];
        insn->i_op = FMT7_OP(byte1);
        strcpy(insn->i_monic, fmt7_table[insn->i_op]);
        insn->i_iol = IOL(byte1);
        strcat(insn->i_monic, iol_table[insn->i_iol]);
        consumed += gen(insn, &buffer[consumed], 0x3, byte1, byte2);
        switch (insn->i_op) {
        
        case FMT7_MOVM :
        case FMT7_CMPM :
            consumed += disp(&buffer[consumed],
                &insn->i_opr[2].o_disp0);	/* WBC bug fix */
            insn->i_opr[2].o_mode = AMODE_IMM;
            break;
            
        case FMT7_INSS :
        case FMT7_EXTS :
            byte2 = buffer[consumed++];
            insn->i_opr[2].o_disp0 = ((byte2&0xe0)>>5);
            insn->i_opr[2].o_mode = AMODE_IMM;
            insn->i_opr[3].o_disp0 = ((byte2&0x1f) + 1);
            insn->i_opr[3].o_mode = AMODE_IMM;
            break;
            
        case FMT7_MOVZD :
        case FMT7_MOVXD :
            strcat(insn->i_monic, "d");
            break;
            
        case FMT7_UNDEF :
            insn->i_format = ITYPE_UNDEF;
            break;
        }
        break;
        
    case ITYPE_FMT8 :
        byte1 = buffer[consumed++];
        byte2 = buffer[consumed++];
        insn->i_op = FMT8_OP(byte0, byte1);
        strcpy(insn->i_monic, fmt8_table[insn->i_op]);
        insn->i_iol = IOL(byte1);
        switch (insn->i_op) {

        case FMT8_MOV :
            if (FMT8_REG(byte1) == FMT8_SU)
                strcat(insn->i_monic, "su");
            else if (FMT8_REG(byte1) == FMT8_US)
                strcat(insn->i_monic, "us");
            else
                strcat(insn->i_monic, "??");
            /* fall through */

        case FMT8_EXT :
        case FMT8_INS :
        case FMT8_CHECK :
        case FMT8_INDEX :
        case FMT8_FFS :
            strcat(insn->i_monic, iol_table[insn->i_iol]);
            /* fall through */

        case FMT8_CVTP :
            if (insn->i_op != FMT8_FFS) {
                insn->i_opr[0].o_reg0 =
                    REG_R0 + FMT8_REG(byte1);
                insn->i_opr[0].o_mode = AMODE_REG;
            }
            consumed += gen(insn, &buffer[consumed], 0x3, byte1,
                byte2);
            if (insn->i_op == FMT8_EXT ||
                insn->i_op == FMT8_INS) {
                consumed += disp(&buffer[consumed],
                    &insn->i_opr[3].o_disp0);
                insn->i_opr[3].o_mode = AMODE_IMM;
            }
            break;

        default :
            insn->i_format = ITYPE_UNDEF;
            break;
        }   
        break;
        
    case ITYPE_FMT9 :
        byte1 = buffer[consumed++];
        byte2 = buffer[consumed++];
        insn->i_op = FMT9_OP(byte1);
        strcpy(insn->i_monic, fmt9_table[insn->i_op]);
        insn->i_iol = IOL(byte1);
        i = FMT9_F(byte1);
        switch (insn->i_op) {

        case FMT9_MOV :
            strcat(insn->i_monic, iol_table[insn->i_iol]);
            strcat(insn->i_monic, fol_table[i]);
            consumed += gen(insn, &buffer[consumed], 0x3, byte1,
                byte2);
            if (insn->i_opr[1].o_mode == AMODE_REG)
                insn->i_opr[1].o_reg0 =
                    REG_F0 + (insn->i_opr[1].o_reg0 - REG_R0);
            break;

        case FMT9_LFSR :
            consumed += gen(insn, &buffer[consumed], 0x1, byte1, byte2);
            break;

        case FMT9_SFSR :
            consumed += gen(insn, &buffer[consumed], 0x2, byte1, byte2);
            break;

        case FMT9_MOVLF :
        case FMT9_MOVFL :
            consumed += gen(insn, &buffer[consumed], 0x3, byte1,
                byte2);
            if (insn->i_opr[0].o_mode == AMODE_REG)
                insn->i_opr[0].o_reg0 =
                    REG_F0 + (insn->i_opr[0].o_reg0 - REG_R0);
            if (insn->i_opr[1].o_mode == AMODE_REG)
                insn->i_opr[1].o_reg0 =
                    REG_F0 + (insn->i_opr[1].o_reg0 - REG_R0);
            break;

        case FMT9_ROUND :
        case FMT9_TRUNC :
        case FMT9_FLOOR :
            strcat(insn->i_monic, fol_table[i]);
            strcat(insn->i_monic, iol_table[insn->i_iol]);
            consumed += gen(insn, &buffer[consumed], 0x3, byte1,
                byte2);
            if (insn->i_opr[0].o_mode == AMODE_REG)
                insn->i_opr[0].o_reg0 =
                    REG_F0 + (insn->i_opr[0].o_reg0 - REG_R0);
            break;
        }
        break;

    case ITYPE_NOP :
        insn->i_op = NOP;
        strcpy(insn->i_monic, "nop");
        break;

    case ITYPE_FMT11 :
        byte1 = buffer[consumed++];
        byte2 = buffer[consumed++];
        insn->i_op = FMT11_OP(byte1);
        switch (insn->i_op) {

        case FMT11_ADD :
        case FMT11_MOV :
        case FMT11_CMP :
        case FMT11_SUB :
        case FMT11_NEG :
        case FMT11_DIV :
        case FMT11_MUL :
        case FMT11_ABS :
            strcpy(insn->i_monic, fmt11_table[insn->i_op]);
            strcat(insn->i_monic, fol_table[FMT11_F(byte1)]);
            consumed += gen(insn, &buffer[consumed], 0x3, byte1,
                byte2);
            if (insn->i_opr[0].o_mode == AMODE_REG)
                insn->i_opr[0].o_reg0 =
                    REG_F0 + (insn->i_opr[0].o_reg0 - REG_R0);
            if (insn->i_opr[1].o_mode == AMODE_REG)
                insn->i_opr[1].o_reg0 =
                    REG_F0 + (insn->i_opr[1].o_reg0 - REG_R0);
            break;

        default :
            insn->i_format = ITYPE_UNDEF;
            break;

        }
        break;
        
    case ITYPE_FMT12 :
        byte1 = buffer[consumed++];
        byte2 = buffer[consumed++];
        insn->i_op = FMT12_OP(byte1);
        switch (insn->i_op) {

        case FMT12_POLY :
        case FMT12_DOT :
        case FMT12_SCALB :
        case FMT12_LOGB :
            strcpy(insn->i_monic, fmt12_table[insn->i_op]);
            strcat(insn->i_monic, fol_table[FMT12_F(byte1)]);
            consumed += gen(insn, &buffer[consumed], 0x3, byte1,
                byte2);
            if (insn->i_opr[0].o_mode == AMODE_REG)
                insn->i_opr[0].o_reg0 =
                    REG_F0 + (insn->i_opr[0].o_reg0 - REG_R0);
            if (insn->i_opr[1].o_mode == AMODE_REG)
                insn->i_opr[1].o_reg0 =
                    REG_F0 + (insn->i_opr[1].o_reg0 - REG_R0);
            break;

        default :
            insn->i_format = ITYPE_UNDEF;
            break;

        }
        break;
        
    case ITYPE_UNDEF :
        strcpy(insn->i_monic, "???");
        break;
        
    case ITYPE_FMT14 :
        byte1 = buffer[consumed++];
        byte2 = buffer[consumed++];
        insn->i_op = FMT14_OP(byte1);
	insn->i_iol = 4;
        strcpy(insn->i_monic, fmt14_table[insn->i_op]);
        switch (insn->i_op) {

	case FMT14_CINV:
            insn->i_opr[0].o_disp0 = GetShort(byte1, byte2);
            insn->i_opr[0].o_mode = AMODE_CINV;
            consumed += gen(insn, &buffer[consumed], 0x1,
                byte1/* was 0*/, byte2);
            break;

        case FMT14_LMR :
        case FMT14_SMR :
            insn->i_opr[0].o_reg0 = mmuRegTable[GetShort(byte1, byte2)];
            insn->i_opr[0].o_mode = AMODE_REG;
            /* fall through */

        case FMT14_RDVAL :
        case FMT14_WRVAL :
            consumed += gen(insn, &buffer[consumed], 0x1,
                byte1/* was 0*/, byte2);
            break;
        }
        break;

    default :
        insn->i_format = ITYPE_UNDEF;
        strcpy(insn->i_monic, "???");
        break;
    }
    return(consumed);
}

/* Reverse the order of the bits in the LS byte of *ip.
 */
reverseBits (ip)
int *ip;
{
  int i, src, dst;

  src = *ip;
  dst = 0;
  for (i = 0; i < 8; ++i) {
    dst = (dst << 1) | (src & 1);
    src >>= 1;
  }
  *ip = dst;
}
