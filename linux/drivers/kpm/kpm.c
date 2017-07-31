/*
* drivers/kpm/kpm.c 
*
*  Copyright (C) 2017 Sun24.
*
* This file mainly contains the total functions which are used to construct
* KPM,the Kernel Protect Module.In this file,there will be a series of func-
* tions,and Sun24 will add functions step by step.
* This file must include kpm.h,functions delarations and struct definations
* all will be loacted in kpm.h
*/
#include <linux/kpm.h>

#define state	int
#define SECURE 	0	//secure state
#define DNAGER	1	//dangerous state

/*parameters which are used to represent current kerne state */
struct param_list	
{
	/* all param list*/
	int foo_param;
};

/* function declarations */
struct param_list gather();
state measure();
int xxx_handle();
int handle(state s);
int kpm_page_fault_hander();
/* main function for KPM */
void kpm_main()
{
	/* for set_pte_at() in handle_pte_fault  */
	unsigned int * p = (unsigned int *)STACK_TOP; 
	unsigned int * pp = (unsigned int *)((*p) + 16); //
	unsigned int ptep = *pp;  // pte for set
	unsigned int pte;
	__asm__ __volatile__ (
		"mov	%0,r11"
		:"=r"(pte)
	);
	if ((pte > KPM_PA_B) && (pte > KPM_PA_E)){
		goto jmp_out;
	}
	else{
		__asm__ __volatile__(
			"mov	r1,%0			\n\t"
			"mov	r0,%1			\n\t"
			"str	r1, [r0]"
			:
			:"r"(ptep),"r"(pte)
		);
	}


jmp_out:
	__asm__ __volatile__ (
		"pop 	{r0}				\n\t"	/*pop addr of out from kpm stack*/
		"bx 	r0"							/*branch to out,jump out from KPM*/
	);
}

/* get some functions addr,return to copy_xxx functions */
unsigned int * get_func_addrs()
{
	printk("get_func_addrs was called !\n");
	unsigned int * kpm_main_addr = &kpm_main;
	return kpm_main_addr;
}


/* gather kernel current state info for measure */
struct param_list gather()
{
	struct param_list params;
	printk("gather function\n");
	return params;
}

/*secure measure for kernel*/
state measure()
{
	//printk("measure function\n");
	return SECURE;
}

/* main handler */
int handle(state s)
{
	printk("handle function\n");
	return 0;
}
