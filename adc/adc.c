#include "adc.h"

void ADC_IRQHandler() {
	//[3:0]: reserved bits
	//[15:4]: this field contains a binary fraction representing the voltage on the AD0[5] pin --> 12 bits
	//[16:31]: not important
	//0b00000000 00000000 11111111 11110000
	adc_value = (LPC_ADC->ADDR5 & 0xFFF0) >> 4; //read from adc channel 5: mask [15:4] and right shift with 4 bits
	conversion_done = 1;
}

void adc_init() {
	//power adc: set the PCADC bit (reset: by default disabled)
	LPC_SC->PCONP |= (1 << 12);

	//enable adc: set the PDN bit
	LPC_ADC->ADCR |= (1 << 21);

	//ADC clk = PCLK / scale factor = PCLK / (CLKDIV + 1)
	//PCLK = 24 MHz (or 25MHz), CLKDIV = 1 --> ADC clk = 24 / (1 + 1) = 12MHz < 13MHz
	LPC_ADC->ADCR |= (1 << 8);

	//configure pin AD0.5 [31:30]
	LPC_PINCON->PINSEL3 |= (3 << 30);

	//select AD0.5
	LPC_ADC->ADCR |= (1 << 5);

	//only the individual ADC channels generate interrupts
	LPC_ADC->ADINTEN &= ~(1 << 8);

	//interrupt enabled on channel 5
	LPC_ADC->ADINTEN |= (1 << 5);

	//enable the interrupt vector
	NVIC_EnableIRQ(ADC_IRQn);
}


void convert_AD()
//returns value if between 800-1024
//else return a zero
{

	if((3200<adc_value) & (adc_value<=4096))
	{
		//printf("1\n");
		binary_value = 1;
	}
	else
	{
		//printf("0\n");
		binary_value = 0;
	}

}

void adc_start() {
	//start conversion now
	LPC_ADC->ADCR |= (1 << 24);
}
