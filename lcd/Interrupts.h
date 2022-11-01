/*
 * Interrupts.h
 *
 *  Created on: 18 okt. 2022
 *      Author: camel
 */


#ifndef INTERRUPTS_H_
#define INTERRUPTS_H_

#ifdef __USE_CMSIS
#include "LPC17xx.h"
#endif

#include <cr_section_macros.h>

#include <stdio.h>
#include "GPIO.h"

void init_interrupts(void(*a)(), void(*b)(), void(*c)());


#endif /* INTERRUPTS_H_ */
