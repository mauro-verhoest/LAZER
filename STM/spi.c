#include "spi.h"

/*
#include "LPC17xx.h"
#include <stdio.h>
*/

//https://sites.google.com/site/johnkneenmicrocontrollers/spi/ssp-spi-_lpc1768

void	SpiInit( ) {
//LPC_SC->PCONP |= 1 <<21;	//enable POWER to SSP0 (redundant following reset)
LPC_SC->PCONP |= 1 << 10;
LPC_SC->PCLKSEL0 |= 1<<20;	//pclk = cclk = 12 Mhz
LPC_SSP0->CPSR |= 8;	//internal divider


//LPC_PINCON->PINSEL0 |= 0x80000000;	//Pin P0.15 allocated to function 3 SCLK
//LPC_PINCON->PINSEL1 |= 0x02<<0;	//Pin P0.16 allocated to function 3 NSS/SSEL
//LPC_PINCON->PINSEL1 |= 0x02<<2;	//Pin P0.17 allocated to function 3 MISO
//LPC_PINCON->PINSEL1 |= 0x02<<4;	//Pin P0.18 allocated to function 3 MOSI

// set pins for lcd
LPC_PINCON->PINSEL0 |= (2<<14); //sck1 P0.7
//LPC_PINCON->PINSEL0 |= (2<<16);//MISO1 P0.8 we don't use miso for lcd
LPC_PINCON->PINSEL0 |= (2<<18);//MOSI P0.9

LPC_SSP1->CR0 |= 3<<6;	//clock phase
LPC_SSP1->CR0 |= 7<<0;	// 8 bits
LPC_SSP1->CR1 |= 1<<1;	//enable SSP
LPC_GPIO0->FIODIR |= (1<<18);	//make output P0.18 chipselect
LPC_GPIO0->FIOSET|= (1<<18);
}
void	spi_tfr(char mo) {
LPC_GPIO0->FIOCLR = 1<<18;	//select slave
LPC_SSP1->DR = mo;
while ((LPC_SSP1->SR & (1<<4)));
LPC_GPIO0->FIOSET = 1<<18;	//release slave

}

//int	main ( ) {
//	SystemInit();
//char mi;
//while(1) {
//mi = spi_tfr(0x69);
//}
//}
/*
void spi_init(void) {
	//1. On reset, power is enabled for SPI
	//2. PCLK for SPI CCLK/4


	// Pins: The SPI pins are configured using both PINSEL0 (Table 80) and PINSEL1
	//(Table 81), as well as the PINMODE (Section 8.4) register. PINSEL0[31:30] is used to
	//configure the SPI CLK pin. PINSEL1[1:0], PINSEL1[3:2] and PINSEL1[5:4] are used
	//to configure the pins SSEL, MISO and MOSI, respectively.
	LPC_PINCON->PINSEL0 |= (2<<14); //sck1 P0.7
	LPC_PINCON->PINSEL0 |= (2<<16);//MISO1 P0.8
	LPC_PINCON->PINSEL0 |= (2<<18);//MOSI P0.9


	//4. Set DLL (Divisor Latch LSB) & DLM (Divisor Latch MSB)
	//PCLK = 24 000 000 Hz:
	//LPC_UART0->DLL = 0x68;
	//LPC_UART0->DLM = 0;

	//PCLK = 25 000 000 Hz:
	LPC_UART0->DLL = 0x6C;
	LPC_UART0->DLM = 0;


	//5. Set FDR (Fractional Divider Register) with MULVAL = 2 [4:7] and DIVADDVAL = 1 [0:3]
	//0010 0001
	LPC_UART0->FDR = 0x21;

	//3. Disable DLAB again
	LPC_UART0->LCR &= ~(1 << 7);

	//6. Enable FIFOs
	LPC_UART0->FCR |= 1;

	//7. Select TXD0 and RXD0 as pin function --> PINSEL0: RXD0 [7:6] and TXD0 [5:4]
	//0101 0000
	LPC_PINCON->PINSEL0 |= 0x50;

	//8. Configure pin 3 and 2 as neither pull-up nor pull-down: pin 3 [7:6] and pin 2 [5:4]
	//1010 0000
	LPC_PINCON->PINMODE0 |= 0xA0;

	//9. Enable 8-bit character length --> LCR [1:0]
	//0000 0011
	LPC_UART0->LCR |= 3;
}

void uart_putc(char c) {
	//wait until transmitter is empty in Line Status Register
	//0010 0000
	while ((LPC_UART0->LSR & 0x20) == 0)
		;

	//Transmit Holding Register: next character to be transmitted is written here
	LPC_UART0->THR = c;
}

char uart_getc(void) {
	//wait if Receiver Data Ready is not empty in Line Status Register
	//0000 0001
	while ((LPC_UART0->LSR & 1) == 0)
		;

	//Receiver Buffer Register: contains the next received character to be read.
	return LPC_UART0->RBR;
}
*/
