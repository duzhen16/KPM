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

/* xxx envent handler */
int xxx_handle()
{
	printk("xxx_handle function\n");
	return 0;
}

/* main handler */
int handle(state s)
{
	printk("handle function\n");
	return 0;
}
