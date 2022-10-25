#include "pwm.h"

//Pins:
//P1.18: LED1
//P1.23: LED4

//te gebruiken pins
//speaker p26 op application shield --> P2.0 microcontroller
//PWM1.1  speaker
//with duty we can set volume of speaker
//source : http://www.ocfreaks.com/lpc1768-pwm-programming-tutorial/
//source :

//PWM_1 op application shield is laser out --> P21 op microncontroller borjde -->P2.5 op microcontroller


void pwm_init(void) {
	float PCLK = (float) (SystemCoreClock / 4);

	LPC_GPIO1->FIODIR |= (1 << 20); //select p1.18 (LED 1) and write 1 to make it an output

	LPC_PINCON->PINSEL4 |= 1; //select PWM1.1 for speaker
	//LPC_PINCON->PINSEL3 |= (2 <<4);
	//LPC_PINCON->PINSEL3 |= (2 << 14); //select PWM1.4 for p1.23
	LPC_PINCON->PINSEL4 |= (1 <<10); //select PWM1.6 for laser

	//select single edge PWM for speaker
	LPC_PWM1->PCR = 0x0;


	//select double edge PWM for laser
	LPC_PWM1->PCR |= (1 << 6);


	//laser
	LPC_PWM1->MR5 = 5000;
	LPC_PWM1->MR6 = 10000;



	//set PWM clock period to 0,000001s (1Mhz)
	//PR = ((t * PCLK) / MR0) - 1, we'll take MR0 = scale of 100
	LPC_PWM1->PR = PWMPR;

	//reset on PWMMR0: the PWM timer counter will be reset if PWM MR0 matches it
	//LPC_PWM1->MCR = (1 << 1);

	//set PWM rate //schaaaal op 1000
	LPC_PWM1->MR0 = 10000; //default period

	//speaker
	LPC_PWM1->MR1 = 9000; //9ms - default pulse duration



	//Reset the PWM TC ON MR0 match speaker | laser
 	LPC_PWM1->MCR |= (1<<1) /*| (1<<19)*/;

	//set latch enable register to load the new MR0 value
	//niet doen wordt waarde nooit ingesteld  speaker
	LPC_PWM1->LER = (1 << 1) | (1 <<0);
	//laser
	LPC_PWM1->LER |= (1<<6) | (1<<5);

	//enable the PWM channel pin: PWM1, default single edge: speaker | laser
	LPC_PWM1->PCR |= (1 << 9) | (1<<14);

	//Reset PWM TC & PR
	LPC_PWM1->TCR = (1 << 1);


	//enable PWM mode
	LPC_PWM1->TCR = (1 << 3) |  (1 << 0);

	//enable counter
	//LPC_PWM1->TCR |= (1 << 0);
}




void init_Timer0(void)
 // set up timer to have period of 1µs
{
	LPC_TIM0->CTCR = 0x0;
	LPC_TIM0->PR = 25000-1; //setup PR to have period of 1µs

	//reset timer
	LPC_TIM0->TCR = 0x02; //Reset timer

}


void toggleLED(uint8_t control,uint8_t reg)
{
	if (control==1)
	{
			//turn on LED2
			LPC_GPIO1->FIOSET |=(1 << reg);
		} else {
			//turn off LED2
			LPC_GPIO1->FIOCLR|= (1 << reg);
		}
}


void toggleLED1(uint8_t center) {

	toggleLED(center,20);


}

void pwm1_period(uint32_t period)
//set the period for channel PWM 1, period in values µs
{
	LPC_PWM1->MR0 = period;


	//set latch enable register to load the new MR0 value
	LPC_PWM1->LER |= (1 << 0);
}

void pwm1_pulseWidth(uint32_t pulseWidth)
//set the pulseWidth for channel PWM 1, period in values µs
{
	LPC_PWM1->MR1 = pulseWidth;

	//set latch enable register to load the new MR1 value
	LPC_PWM1->LER |= (1 << 1);
}


void pwm6_pulse(uint32_t pulseWidth,uint32_t period,uint32_t start)
{
	//laser
	LPC_PWM1->MR5 = start;
	LPC_PWM1->MR6 = start+pulseWidth;





	//set PWM rate //schaaaal op 1000
	LPC_PWM1->MR0 =  period; //default period


	LPC_PWM1->LER = (1 <<0);
		//laser
		LPC_PWM1->LER |= (1<<6) | (1<<5);
}

void pwm1_duty(float duty) {
	#if printDebugInfo == 1
		printf("PWM1 MR1 = %d\r\n", duty);
	#endif

	LPC_PWM1->MR1 = duty*(LPC_PWM1->MR0);

	//set latch enable register to load the new MR1 value
	LPC_PWM1->LER |= (1 << 1);
}


void delay_ms(uint32_t delay)
//wait for delay given in delay in µs
{
	LPC_TIM0->TCR = 0x02; // reset timer-module
	LPC_TIM0->TCR = 0x01; //enable timer-module

	while(LPC_TIM0->TC < delay); //wait until timer-module has reached the desired value

	//disable the timer module
	LPC_TIM0->TCR = 0x00;

}


void pwm4_duty(uint32_t dutyStart, uint32_t dutyEnd) {
	#if printDebugInfo == 1
		printf("PWM4 MR3 = %u; MR4 = %u\r\n", dutyStart, dutyEnd);
	#endif

	LPC_PWM1->MR3 = dutyStart; //edge high
	LPC_PWM1->MR4 = dutyEnd; //edge low

	//set latch enable register to load the new MR4 value
	LPC_PWM1->LER |= (1 << 3);

	//set latch enable register to load the new MR4 value
	LPC_PWM1->LER |= (1 << 4);
}
