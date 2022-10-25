#ifndef PWM_H
#define PWM_H

#include "LPC17xx.h"
#include <stdio.h>

#define printDebugInfo 0
#define PWMPR (25-1) // for resolution for 1µs 25PCLK cycles to increment TC by 1

void init_Timer0(void);
void delay_ms(uint32_t delay);
void delay_us(uint32_t delay);

void pwm_init(void);
void pwm1_duty(float duty);
void pwm1_period(uint32_t period);
void pwm1_pulseWidth(uint32_t pulseWidth);
void pwm4_duty(uint32_t dutyStart, uint32_t dutyEnd);
void pwm6_pulse(uint32_t pulseWidth,uint32_t period,uint32_t start);

void toggleLED(uint8_t control,uint8_t reg);
void toggleLED1(uint8_t center);

#endif
