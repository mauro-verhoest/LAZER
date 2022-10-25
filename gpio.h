#ifndef GPIO_H
#define GPIO_H

#ifdef __USE_CMSIS
#include "LPC17xx.h"
#endif

#include <cr_section_macros.h>

#include <stdio.h>
#include <stdint.h>

#include <assert.h>

/* ^^ these are the include guards */

/* Prototypes for the functions */
void init_interrupts(void (*a)());
void approx_wait(uint32_t milliseconds);
void toggleLED(uint8_t control,uint8_t reg);

#endif
