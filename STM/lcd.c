/*
===============================================================================
 Name        : lcd.c
 Author      : M. Verhoest
 Version     :
 Copyright   : $(copyright)
 Description : main definition
===============================================================================
*/
#include "lcd.h"

// TODO: insert other include files here

// TODO: insert other definitions and declarations here
//MOSI p5 P0.9 v
//nRESET P6 P0.8
//sck P7 v
//A0 P8  P0.6
//nCS P11 P0.18
void lcdInit(void)
{
	//Define output port directions
	LPC_GPIO0->FIODIR1 |= (1<<8);  //nRESET
	LPC_GPIO0->FIODIR0 |= (1<<6); //A0
	//LPC_GPIO0->FIODIR2 |= (1<<2); //nCS
	LPC_GPIO0->FIOSET =  (1<<6) | (1<<8);  //Initialise control lines high



	//lcd init
	SpiInit();

	//timer init
	init_Timer0();
}



void setRegister(uint8_t control,uint8_t reg)
{
	if (control==1)
	{
			//turn on LED2
			LPC_GPIO0->FIOSET |=(1 << reg);
		} else {
			//turn off LED2
			LPC_GPIO0->FIOCLR|= (1 << reg);
		}
}

void setReset(uint8_t control)
{
	setRegister(control,8);
}

void setAZero(uint8_t control)
{
	setRegister(control,6);
}

void setNCS(uint8_t control)
{
	setRegister(control,18);
}

void writeCmd(unsigned char cmd)
{

	///setNCS(0);
	setAZero(0);
	spi_tfr(cmd);
	//setNCS(1);
	setAZero(1);
}


void writeData(unsigned char data)
{
	//setAZero(1);
	//setNCS(0);
	spi_tfr(data);
	//setNCS(1);
}

void lcdReset(void)
{
	//setAZero(0);
	//setNCS(1);
	//display reset
	setReset(0);
	delay_us(50);
	setReset(1);
	delay_ms(5);
	//end reset

	//start init
	writeCmd(0xAE); //display off
	writeCmd(0xA2); //bias voltage

	writeCmd(0xA0);
	writeCmd(0xC8); //column normal

	writeCmd(0x22); //voltage resistor ratio
	writeCmd(0x2F); //power on
	writeCmd(0x40); //start line =0
	writeCmd(0xAF); //display ON

	writeCmd(0x81); //set contrast
	writeCmd(0x17); //set contrast

	writeCmd(0xA6); //display normal


	//clear and update LCD
	memset(buffer,0x00,512); //clear display buffer
	//copy to lcd
	copyToLcd();
	//locate 0 0
	//set_font
	clearLcd();


	placeCursor(0,0);

}

void pixel(int a,int b,int color,uint8_t drawMode)
//set one pixel in the buffer
{
	//check if not out of bounds of screen
	if(a>128 || b>32 || a < 0 || b< 0) return;

	//drawMode 1 normal
	if(drawMode == 1)
	{
		if(color ==0)
		{
			buffer[a+(b/8*128)] &= ~(1<<(b%8)); //erase pixel
		}
		else
		{
			buffer[a+((b/8)*128)] |= (1<<(b%8)); //set pixel
		}

	}
	else //drawMode 0 XOR
	{
		if(color ==1)
		{
			buffer[a+((b/8)*128)] ^= (1<<b%8); //xor pixel
		}
	}
}


void placeCursor(char pagina,char kolom)
{
	//out of bounds errors
	if( pagina > 3)
	{
		pagina =0;
	}
	if(kolom>127)
	{
		kolom = 0;
	}
	writeCmd(0x00+(kolom & 0x0F));
	writeCmd(0x10+(kolom/16));
	writeCmd(0xB0+pagina); //SET pagina adress to zero
	writeCmd(0xE0); //auto increment the lcd
}


void copyToLcd(void)
//copy buffer to lcd
{
	int k=0;

	//write page one 
	writeCmd(0x00); //set the column low nibble to 0
	writeCmd(0x10); //set the column high nibble to 0
	writeCmd(0xB0); //set the page address to 0
	//setAZero(1);

	for(k=0;k<128;k++)
	{
		writeData(buffer[k]);
	}

	//write page two 
	writeCmd(0x00); //set the colum low nibble to 0
	writeCmd(0x10); //set the colum high nibble to 0
	writeCmd(0xB1); //set page adress to one 
	//setAZero(1);
	for ( k = 256; k < 384; k++)
	{
		writeData(buffer[k]);
	}
	
	//write page three
	writeCmd(0x00); //set the colum low nibble to zero 
	writeCmd(0x10); //set the colim high nibble to zero 
	writeCmd(0xB3); //set page adress three 

	//setNCS(0);

	for (k  = 0; k < 512; k++)
	{
		writeData(buffer[k]);
	}
	


}

void dispLine(char c)
{
	int k;
	for(k=0;k<128;k++)
	{
		writeData(c);
	}
}


void clearLcd(void)
{
	int k;
	for(k=0;k<4;k++)
	{
		placeCursor(k,0);
		dispLine(0x00);

		 //TESTING
		//dispLine(0x00); //PRODUCTION
	}

	placeCursor(0,0);
}


void createChar(unsigned char c)
{
	int x,y;
	y = (c-(' '))*8;
	for(x=0;x<8;x++)
	//8 pixels width per char
	{
		spi_tfr(Font_Array_dink[y]);
		y++;
	}
}

void dispChar(int lijn,int kolom,unsigned char c)
{
	placeCursor(lijn,8*kolom);
	createChar(c);
}

void dispString(int lijn,int kolom,char * str)
{
	placeCursor(lijn,8*kolom);
	while(*str)
		createChar(*str++);
}


