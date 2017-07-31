/*
* include/linux/kpm.h 
*
*  Copyright (C) 2017 Sun24.
*
* This file is header file for KPM,Sun24 will define some functions declaration
* these functions are implemented in kpm.c
*/

#include <linux/const.h>
#include <asm/memory.h>

#define UL(x) _AC(x, UL)
/*
* These macros defined by Sun24 for describing KPM  
*          new kpm layout 
*/
#define KPM_BASE	UL(0x80600000)
#define KPM_DIR		KPM_BASE
#define KPM_END		UL(0x80700000)
#define KPM_TEXT    UL(0x80604000)
#define KPM_DATA    UL(0x806a0000)

#define KPM_PA_B	virt_to_phys(KPM_BASE)
#define KPM_PA_E	virt_to_phys(KPM_END)



#define RELOCATED_TEXT  UL(0x80100000)
#define RELOCATED_TEXT_END  KPM_END
#define RELOCATED_DATA  UL(0x80700000)
/* stack size = 8K */
#define STACK_BASE	KPM_END-4
#define STACK_TOP   STACK_BASE
#define STACK_BOT	UL(0x806fe000)

/* arcos used in this file */
#define PAGEDIR 	UL(0x80004000)
#define ENTRY_NR	4*1024  //4k 
#define AP2			1<<15

/* event type passed to KPM */
#define TYPE_MM_FAULT	UL(0x0) 	//type handle_mm_fault 
#define TYPE_SCTLR_IMP	UL(0x1)		//type system control register implemention
#define TYPE_TTBR_IMP	UL(0x2)		//type translation base register implemention

/* copied from arch/arm/include/asm/pgtable-hwdef.h */
#ifndef _ASMARM_PGTABLE_HWDEF_H
#define _ASMARM_PGTABLE_HWDEF_H

/*
 * Hardware page table definitions.
 *
 * + Level 1 descriptor (PMD)
 *   - common
 */
#define PMD_TYPE_MASK		(3 << 0)
#define PMD_TYPE_FAULT		(0 << 0)
#define PMD_TYPE_TABLE		(1 << 0)
#define PMD_TYPE_SECT		(2 << 0)
#define PMD_BIT4		(1 << 4)
#define PMD_DOMAIN(x)		((x) << 5)
#define PMD_PROTECTION		(1 << 9)	/* v5 */
/*
 *   - section
 */
#define PMD_SECT_BUFFERABLE	(1 << 2)
#define PMD_SECT_CACHEABLE	(1 << 3)
#define PMD_SECT_XN		(1 << 4)	/* v6 */
#define PMD_SECT_AP_WRITE	(1 << 10)
#define PMD_SECT_AP_READ	(1 << 11)
#define PMD_SECT_TEX(x)		((x) << 12)	/* v5 */
#define PMD_SECT_APX		(1 << 15)	/* v6 */
#define PMD_SECT_S		(1 << 16)	/* v6 */
#define PMD_SECT_nG		(1 << 17)	/* v6 */
#define PMD_SECT_SUPER		(1 << 18)	/* v6 */

#define PMD_SECT_UNCACHED	(0)
#define PMD_SECT_BUFFERED	(PMD_SECT_BUFFERABLE)
#define PMD_SECT_WT		(PMD_SECT_CACHEABLE)
#define PMD_SECT_WB		(PMD_SECT_CACHEABLE | PMD_SECT_BUFFERABLE)
#define PMD_SECT_MINICACHE	(PMD_SECT_TEX(1) | PMD_SECT_CACHEABLE)
#define PMD_SECT_WBWA		(PMD_SECT_TEX(1) | PMD_SECT_CACHEABLE | PMD_SECT_BUFFERABLE)
#define PMD_SECT_NONSHARED_DEV	(PMD_SECT_TEX(2))

/*
 *   - coarse table (not used)
 */

/*
 * + Level 2 descriptor (PTE)
 *   - common
 */
#define PTE_TYPE_MASK		(3 << 0)
#define PTE_TYPE_FAULT		(0 << 0)
#define PTE_TYPE_LARGE		(1 << 0)
#define PTE_TYPE_SMALL		(2 << 0)
#define PTE_TYPE_EXT		(3 << 0)	/* v5 */
#define PTE_BUFFERABLE		(1 << 2)
#define PTE_CACHEABLE		(1 << 3)

/*
 *   - extended small page/tiny page
 */
#define PTE_EXT_XN		(1 << 0)	/* v6 */
#define PTE_EXT_AP_MASK		(3 << 4)
#define PTE_EXT_AP0		(1 << 4)
#define PTE_EXT_AP1		(2 << 4)
#define PTE_EXT_AP_UNO_SRO	(0 << 4)
#define PTE_EXT_AP_UNO_SRW	(PTE_EXT_AP0)
#define PTE_EXT_AP_URO_SRW	(PTE_EXT_AP1)
#define PTE_EXT_AP_URW_SRW	(PTE_EXT_AP1|PTE_EXT_AP0)
#define PTE_EXT_TEX(x)		((x) << 6)	/* v5 */
#define PTE_EXT_APX		(1 << 9)	/* v6 */
#define PTE_EXT_COHERENT	(1 << 9)	/* XScale3 */
#define PTE_EXT_SHARED		(1 << 10)	/* v6 */
#define PTE_EXT_NG		(1 << 11)	/* v6 */

/*
 *   - small page
 */
#define PTE_SMALL_AP_MASK	(0xff << 4)
#define PTE_SMALL_AP_UNO_SRO	(0x00 << 4)
#define PTE_SMALL_AP_UNO_SRW	(0x55 << 4)
#define PTE_SMALL_AP_URO_SRW	(0xaa << 4)
#define PTE_SMALL_AP_URW_SRW	(0xff << 4)

#endif
