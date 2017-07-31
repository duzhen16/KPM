/*
* include/linux/kpm.h 
*
*  Copyright (C) 2017 Sun24.
*
* This file is header file for KPM,Sun24 will define some functions declaration
* these functions are implemented in kpm.c
*/

#include <linux/const.h>

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