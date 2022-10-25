#include "GPIO.h"

void approx_wait(uint32_t milliseconds) {
	//keep the processor busy approximately for a given time
	for (uint32_t j = 0; j < milliseconds; j++) {
		for (volatile uint32_t i = 0; i < 7320; i++)
			;
	}
}

uint8_t center = 0;
uint8_t trigger = 0;

void (*nextStruct1)();
void (*nextStruct2)();
void (*nextStruct3)();

void EINT3_IRQHandler() {

	if (((LPC_GPIOINT->IO0IntStatR >> 16) & 1) == 1) {
		center = !center; //toggle
		LPC_GPIOINT->IO0IntClr = (1 << 16);
		nextStruct2();
	}
	else if(((LPC_GPIOINT->IO0IntStatR >> 4) & 1) == 1){	// Mauser FIX
		trigger = !trigger;
		LPC_GPIOINT->IO0IntClr = (1 << 4);
		nextStruct1();
	}
	else if(((LPC_GPIOINT->IO0IntStatF >> 5) & 1) == 1){	// Mauser FIX
		LPC_GPIOINT->IO0IntClr = (1 << 5);
		nextStruct3();
	}

}

void init_GPIO(void(*a)(), void(*b)(), void(*c)()) {

	//if a FIOPIN register is read, its bit(s) masked with 1 in the FIOMASK register will be set to 0
	//regardless of the physical pin state
	LPC_GPIO0->FIOMASK = 0xFFFFFFFF;
	LPC_GPIO0->FIOMASK &= ~(1<<16);
	LPC_GPIO0->FIOMASK &= ~(1<<4);

	LPC_GPIO0->FIODIR &= ~(1<<4);
	LPC_PINCON->PINMODE0 |=(3<<8);
	LPC_GPIOINT->IO0IntEnR |= (1<<4); //enable GPIO interrupt for p0.30 rising edge

	LPC_GPIO0->FIOMASK &= ~(1<<5);
	LPC_GPIO0->FIODIR &= ~(1<<5);
	LPC_GPIOINT->IO0IntEnF |= (1<<5);

    //CENTER
	LPC_GPIO0->FIODIR &= ~(1 << 16); 		//select p0.16 (JOYSTICK CENTER) and write 0 to make it an input
	LPC_PINCON->PINMODE1 |= 3; 				//enable pull-down p0.16 (needed for mbed application board)

	LPC_GPIOINT->IO0IntEnR |= (1 << 16); 	//enable GPIO interrupt for p0.16 rising edge

	//NVIC->ISER[0] |= (1 << 21); ---> interrupt hoofschakelaar  kennen
	NVIC_EnableIRQ(EINT3_IRQn); 			//enable External Interrupt 0 Interrupt---> KENNEN

	nextStruct1 = a;
	nextStruct2 = b;
	nextStruct3 = c;
}

/*
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
*/


