/*
* arch/arm/mm/kpm-init.c
*
*  Copyright (C) 2017 Sun24.
*
* This file contains some function inplementations,which caled by kpm_init() in init/main.c
*/

#include <linux/kpm.h>
#include "../../../drivers/kpm/kpm.c"
/*
* These functions defined here by Sun24 to be called by "static void __init kpm_init()" in init/main.c
* extern declaration in include/linux/kpm.h
*/
void in();
void out();

static void  __init  copy_page_dir()
{
	/* copies kernel page dir to KPM space */
	printk("copying kernel page dir....\n");
	int i;
	unsigned int * from = (unsigned int *)PAGEDIR;
	unsigned int * to   = (unsigned int *)KPM_DIR;
	for(i = 0; i < ENTRY_NR; i++){
		*to = *from;
		to += 1;
		from += 1;
	}
	/* kernel page dir range 0x80004000:0x80008000 */
	printk("copy ok\n");

	/* write KPM_DIR to ttbr1
	*  pkm dir VA : 0x8060,0000
	*  pkm dir PA : 0x6060,0000
	*  ttbr1 = PA | attribute : 0x6060,0000 | 0x006a 
	*/
	unsigned int p_k_d = KPM_DIR - 0x20000000;
	unsigned int ttbr1_val = p_k_d | 0x006a;  
	__asm__ __volatile__ (
		"MOV r0,%0			\n\t"
		"MCR p15, 0, r0, c2, c0, 1"
		:
		:"r"(ttbr1_val)
	);
	printk("write 0x%x to ttbr1 ok\n",ttbr1_val);
	
	return;
}

int do_copy(unsigned int *dest, unsigned int *src, unsigned int size)
{ 
	unsigned int c;
	for(c = 0; c < size ;c++){
		*(dest + c) = *(src + c);
	}
	return 0;
}
static void  __init  copy()
{
	/* do copy kpm_main() function to KPM_TEXT*/
	unsigned int * kpm_main_addr  = get_func_addrs();
	printk("copying kpm...\n");
	unsigned int * fkpm = (unsigned int *)KPM_TEXT;
	printk("kpm_main function is @ 0x%p\n",kpm_main_addr);
	do_copy(fkpm,kpm_main_addr,64); //how to determine size? 
	printk("copy ok\n");
	printk("KPM phy addr region is : 0x%x <-> 0x%x\n",KPM_PA_B,KPM_PA_E);
	return ;
}

static void  __init  delete_kpm_entries()
{
	/*
	this function deletes page dir entries which point to KPM space
	delete means to set entry [1:0]=0b00 to invaild descriptor reference ARM manual P1326.
	*/
	printk("deleting kpm entries in kernel dir...\n");
	unsigned int * entry = (unsigned int *)0x80006018;  //entry points to KPM region in kernel page dir
	printk("invaild entry @ 0x%p is 0x%x\t",entry,*entry);
	
	(* entry) &= 0xfffffffc;  //clear [1:0]=0b00;	
	printk("invaild ok,now is 0x%x\n",*entry);
	return ;
}
static void  __init  modify_kernel_pt()
{
	/*
	this function modifies 1) entries which point to kernel code to be Read-only for kernel
	kernel code locates at [RELOCATED_TEXT,RELOCATED_DATA),if we want to make ker`nel
	code Read-only for kernel,we need to set AP[2]=0b1 AP[1:0]=0b01,references from 
	ARM manual P1358.But we only need to set AP[2]=0b1 here,for AP[1:0] has already 
	been set. AP[2] is the 15th bit in the descriptor.
	entries points to kernel code are placed at [0x80006004,0x80006018),step = 4
	2)other mem area kernel data and user space to be PXN
	*/
	printk("modifying kernel code to be Read-only...\n");
	unsigned int * start = (unsigned int *)0x80006004;  //start entry points to kernel code region in kernel page dir
	unsigned int * end   = (unsigned int *)0x8000601c;  //end entry points to kernel code region in kernel page dir
	unsigned int * p =  start;
	unsigned int entry = * p;

	while(p < end){
		entry = * p;
		* p = entry | AP2;  //set AP[2]=0b1;	
		p = p + 1;
	}
	printk("modifying kernel data and user space to be PXN...\n");

	/* should be implemented */

	printk("modifying ok\n");
	return ;
}

void out()
{
	int ret;
	__asm__ __volatile__ (
		"mov	%0,r1				\n\t"	/*return value of KPM*/
		:"=r"(ret)
	);
	if (! ret){
		__asm__ __volatile__ (

			"mov    r0,#0x0 			\n\t"	/*write #0 to TTBCR.N to switch ttbr*/
			"isb						\n\t"
			"mcr    p15,0,r0,c2,c0,2	\n\t"
			"isb						\n\t"
			
			"pop 	{r0}				\n\t"   /*restore CPSR & enable interrupt*/
			"msr 	cpsr,r0				\n\t"

			"pop 	{lr,r3,r2,r1,r0}	\n\t"	/*restore ret addr of needle*/

			"mcr    p15,0,r0,c8,c7,0	\n\t"	/*TLB invalidate*/							
			"isb						\n\t"

			"bx 	lr"							/*branch to needle,kernel exution goes on*/
		);
	}else{
		printk("ERROR type is %d\n",ret);
		/*abort system*/
	}
	
}
/* come into KPM from kernel,do context switch,store current system state to shadow stack */
void in()
{	
	int event_type = 1;
	/* do something and ready to switch to KPM */
	unsigned int * out_addr = &out; 
	__asm__ __volatile__ (
		"push	{r0,r1,r2,r3,lr}	\n\t"   /*save context*/

		"mrs    r0,cpsr				\n\t"   /*save CPSR & disable interrupt*/
		"push   {r0}				\n\t"   
		"orr    r0,r0,#0x1c0	    \n\t"  
		"msr    cpsr,r0				\n\t"

		"mov    r0,#0x1 			\n\t"	/*write #1 to TTBCR.N to switch ttbr*/
		"isb						\n\t"
		"mcr    p15,0,r0,c2,c0,2	\n\t"
		"isb						\n\t"
	);
	__asm__ __volatile__ (
		"mov 	r0,%0				\n\t"   /*save addr of out*/
		"push	{r0}				\n\t"
		
		"mcr    p15,0,r0,c8,c7,0	\n\t"	/*TLB invalidate*/							
		"isb						\n\t"

		"mov  	r1,%2				\n\t"	/*pass event type to KPM*/

		"mov	r0,%1				\n\t"	/*branch to kpm*/	
		"bx 	r0"							
		:
		:"r"(out_addr),"r"(KPM_TEXT),"r"(event_type)
	);
}
EXPORT_SYMBOL(in);