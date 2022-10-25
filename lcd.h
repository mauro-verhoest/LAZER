/*
 * lcd.h
 *
 *  Created on: 18 okt. 2022
 *      Author: Family_laptop
 */

#ifndef LCD_H_
#define LCD_H_

#ifdef __USE_CMSIS
#include "LPC17xx.h"
#endif

#include <cr_section_macros.h>
#include <stdio.h>
#include "spi.h"
#include "pwm.h"
#include <string.h>
#include "Font_array.h"

unsigned char buffer[512];

void lcdInit(void);
void lcdReset(void);
void writeCmd(unsigned char cmd);
void writeData(unsigned char data);

void pixel(int a,int b,int color,uint8_t drawMode);
void placeCursor(char page,char col);
void copyToLcd(void);
void clearLcd(void);
void createChar(unsigned char c);
void dispChar(int lijn,int kolom,unsigned char c);
void dispString(int lijn,int kolom,char * str);


#endif /* LCD_H_ */
