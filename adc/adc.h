#ifndef ADC_H
#define ADC_H


#include "LPC17xx.h"
#include <stdio.h>

extern uint16_t adc_value;
extern uint8_t conversion_done;
extern uint8_t binary_value;


void adc_init(void);
void adc_start(void);
void convert_AD(void);

#endif
