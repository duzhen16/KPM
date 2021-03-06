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

/* arcos used in this file */
#define PAGEDIR 	UL(0x80004000)
#define ENTRY_NR	4*1024  //4k 
#define AP2			1<<15

/* event type passed to KPM */
#define TYPE_SET_PTE	1	//type handle_mm_fault 
#define TYPE_SCTLR_IMP	2	//type system control register implemention
#define TYPE_TTBR_IMP	3	//type translation base register implemention

