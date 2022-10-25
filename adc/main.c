#include "adc.h"
#include <stdio.h>

/**
 * ADC
 * ---
 *
 *	Has been tested with LPC1768 on an mbed application board. Pins:
 *	AD0.5: physical pin 20 = potentiometer 2
 */

uint16_t adc_value = 0;
uint8_t conversion_done = 0;
uint8_t binary_value = 0;
int dink;

void approx_wait(uint32_t milliseconds);




int main() {
	adc_init();
	adc_start();

	while (1) {
		if (conversion_done == 1) {
			printf("ADC value: %i", adc_value);
			printf(", Voltage: %f V", 3.3 / 4095 * adc_value);
			printf("\r\n");
			printf("binary value: %d\r\n",binary_value);

			//start next conversion
			adc_start();
			convert_AD();
			conversion_done = 0;
		}

		approx_wait(600);
	}
}

void approx_wait(uint32_t milliseconds) {
	for (uint32_t j = 0; j < milliseconds; j++) {
		for (volatile uint32_t i = 0; i < 7320; i++)
			;
	}
}
