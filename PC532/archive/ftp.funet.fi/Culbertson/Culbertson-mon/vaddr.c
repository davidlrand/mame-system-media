/* NSC 32000 ROM debugger.
 * Bruce Culbertson  Bob Krause
 *
 * Virtual address stuff.
 */

#include "debugger.h"
#include "machine.h"

#define PTE_VALID(p)	(1 == (p&1))

/* Macros for translating virtual addresses to physical addresses.
 */
#define IX1_SHIFT	22		/* index 1 position in vaddr */
#define IX1_MASK	0xffc00000	/* mask for index 1 bits in vaddr */
#define IX2_SHIFT	12		/* index 2 position in vaddr */
#define IX2_MASK	0x003ff000	/* mask for index 2 bits in vaddr */
#define OFFSET_MASK	0x00000fff	/* mask for offset bits in vaddr */
#define PFN_MASK	0xfffff000	/* mask for page frame num in pte */

#define IX1_4X(v)	/* four times IX1 */ \
			(((v) & IX1_MASK) >> (IX1_SHIFT-2))
#define IX2_4X(v)	/* four times IX2 */ \
			(((v) & IX2_MASK) >> (IX2_SHIFT-2))
#define OFFSET(v)	((v) & OFFSET_MASK)
#define PTE1_ADR(v,ptb)	((long *)((long)(ptb) | IX1_4X(v)))
#define PTE2_ADR(v,pte1) ((long *)((long)(pte1)&PFN_MASK | IX2_4X(v)))
#define PTE1(v,ptb)	(*PTE1_ADR(v,ptb))
#define PTE2(v,pte1)	(*PTE2_ADR(v,pte1))
#define PHYS(v,pte2)	(pte2&PFN_MASK | OFFSET(v))
#define TRANSLATE(p,v)  (PHYS(v, PTE2 (v, PTE1 (v, p))))

#define MMU_MCR_TU	0x1
#define MMU_MCR_TS	0x2
#define MMU_MCR_DS	0x4

int
getVaddr (p, c)
char **p;
long *c;
{
  long vaddr, ptb, getCurrentPtb();

  scan(p);
  if (**p != '(') return BAD_NUM;
  ++*p;
  if (GOT_NUM != getIntScan (p, &vaddr)) return BAD_NUM;
  scan(p);
  if (**p == ',') {
    ++*p;
    if (GOT_NUM != getIntScan (p, &ptb)) return BAD_NUM;
  } else ptb = getCurrentPtb();
  scan(p);
  if (**p != ')') return BAD_NUM;
  ++*p;
  return translateVaddr (vaddr, ptb, c);
}

translateVaddr (vaddr, ptb, c)
long vaddr, ptb, *c;
{
  long pte1, pte2;

  if (ptb == 0) *c = vaddr;		/* no translation */
  else {
    pte1 = PTE1 (vaddr, ptb);
    if (!PTE_VALID (pte1)) return BAD_NUM;
    pte2 = PTE2 (vaddr, pte1);
    if (!PTE_VALID (pte2)) return BAD_NUM;
    *c = PHYS (vaddr, pte2);
  }
  return GOT_NUM;
}

long
getCurrentPtb ()
{
  int user;

  user = machState.psr & PSR_U;
  if (machState.mcr & MMU_MCR_TU && user)
    return machState.mcr & MMU_MCR_DS? machState.ptb1: machState.ptb0;
  if (machState.mcr & MMU_MCR_TS && !user)
    return machState.ptb0;
  return 0;				/* no translation */
}
