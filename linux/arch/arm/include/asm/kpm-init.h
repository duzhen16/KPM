/*
* arch/arm/include/asm/kpm-init.h
*
*  Copyright (C) 2017 Sun24.
*
* This file contains some function definations,which caled by kpm_init() in init/main.c
*/
#include "../../mm/kpm-init.c"

extern void  __init  copy_page_dir();
extern void  __init  delete_kpm_entries();
extern void  __init  modify_kernel_pt();
extern void  __init  copy();

