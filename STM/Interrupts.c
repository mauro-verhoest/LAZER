/*
===============================================================================
 Name        : Interrupts.c
 Author      : $(author)
 Version     :
 Copyright   : $(copyright)
 Description : main definition
===============================================================================
*/

#ifdef __USE_CMSIS
#include "LPC17xx.h"
#endif

#include <cr_section_macros.h>

#include <stdio.h>
#include "GPIO.h"

int counter = 0;

void init_interrupts(void(*a)(), void(*b)())
{
	init_GPIO(*a, *b);
}



// TODO: insert other include files here

// TODO: insert other definitions and declarations here





