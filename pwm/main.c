#include "pwm.h"

/**
 * PWM
 * -------------
 *
 *	- LED 1: fade in and out in small steps with single edge PWM
 *	- LED 4: fade in and out in big steps with double edge PWM
 */

void approx_wait(uint32_t milliseconds);
void play_Tune(uint32_t freq,uint32_t length,uint32_t delay);
void play_TuneIndex(uint32_t index);
void make_LaserPulses(uint32_t period,uint32_t pulseWidth,uint32_t n_Pulses);


uint8_t center=0;

//bron star wars code de  Heer.  Ing. M Verhoest

//to make star wars tunes
 const static uint32_t  arrayLength = 34;
 //Tunes freq in Hz
 uint32_t freqArray [] = {440,440,440,349,523,440,349,523,440,659,659,659,698,523,415,349,523,415,659,440,440,659,830,784,740,698,740,455,622,587,554,523,466,423};
 //lengthArray in ms
 uint32_t lengthArray[] =  {500,500,500,350,150,500,350,150,500,500,500,500,350,150,500,350,150,650,500,300,150,400,200,200,125,125,250,250,400,200,200,125,123,250};
 //delayArray in ms
 uint32_t delayArray[] = {100,100,100,100,100,100,100,250,100,100,100,100,100,100,100,100,250,100,100,100,100,100,100,100,100,100,350,100,100,100,100,100,100,350};


 //playtune with speaker
 //freq in Hz,length ms,delay ms,maxVolume 0.0 to 1.0
 void play_Tune(uint32_t freq,uint32_t length,uint32_t delay)
 //bron https://www.geeksforgeeks.org/chrono-in-c/
 {

	 LPC_PWM1->TCR = (1<<0) | (1<<3); //enable counters and PWM Mode
	 LPC_PWM1->PCR 	|= (1 << 9);

     //out of bounds checking
     if((freq >0) && (delay>=0))
     {
    	 //LPC_PWM1->PCR |= (1 << 9);
     float f = (float) (1.0/(freq));
     f *= 1000000;


     pwm1_period((uint32_t) f);
     pwm1_duty(0.9); //volume
     //pwm1_pulseWidth(length*1000);
     delay_ms(length);
     //approx_wait(length);
     //pwm1_pulseWidth(1);
     LPC_PWM1->PCR &= ~(1 << 9); //turn off PWM-output
     //pwm1_duty(0); //volume off
     delay_ms(delay);
     //approx_wait(delay);
     }
 }

 //play tune according to given index
 void play_TuneIndex(uint32_t index)
 {
     //cout << "in playtuneindex met index" << index << endl;
     //out of bounds checking
     if((index>=1) &&(index<=arrayLength))
     {
         play_Tune(freqArray[index-1],lengthArray[index-1],delayArray[index-1]);
     }

 }

 //playtune with speaker
 //freq in Hz,length ms,delay ms,maxVolume 0.0 to 1.0
 void make_LaserPulses(uint32_t period,uint32_t pulseWidth,uint32_t n_Pulses)
 //bron https://www.geeksforgeeks.org/chrono-in-c/
 {

	 LPC_PWM1->TCR = (1<<0) | (1<<3); //enable counters and PWM Mode
	 LPC_PWM1->PCR 	|= (1 << 14);
	 pwm6_pulse(pulseWidth,period,1000);



	 for (uint32_t i = 0; i <= n_Pulses; i++) {
	     		delay_ms(period);
	 	    		}




     LPC_PWM1->PCR &= ~(1 << 14); //turn off PWM-output


 }




int main(void) {
	pwm_init();
init_Timer0();
	 play_TuneIndex(2);
	 play_TuneIndex(4);
	 for (uint32_t i = 0; i <= arrayLength; i++) {
    		play_TuneIndex(i);
	   		center = !center;
	    					toggleLED1(center);
	    		}
	 make_LaserPulses(1000000,900000,69);
	 return 0;
}

//
//    for (uint32_t i = 0; i <= arrayLength; i++) {
//    		play_TuneIndex(i);
//    		center = !center;
//    					toggleLED1(center);
//    		}
//	pwm1_duty(90);
//			approx_wait(1000);
//
//
//    pwm1_period(860000);
//    approx_wait(1000);
//    play_TuneIndex(2);




//	while (1)
//
//		//single edge PWM on LED1
//		//fade in:
//		for (uint32_t i = 0; i <= 1000000; i = i + 1000) {
//			pwm1_period(i);
//			//pwm4_duty(i*10, i*10+3);
//			center = !center;
//			toggleLED1(center);
//			approx_wait(1000);
//
//		}
//
//		//fade out:
//		for (int i = 20000; i >= 0; i--) {
//			pwm1_period(i);
//			approx_wait(10);
//			center = !center;
//			toggleLED1(center);
//		}
//
//
//		//pwm1_duty(100);
//
//		#if printDebugInfo == 1
//			printf("\r\n");
//			approx_wait(3000);
//		#endif
//
//		//double edge PWM on LED4
//		pwm4_duty(30, 40);
//		approx_wait(1000);
//
//		pwm4_duty(5, 95);
//		approx_wait(1000);
//
//		pwm4_duty(30, 40);
//		approx_wait(1000);
//
//		#if printDebugInfo == 1
//			printf("\r\n");
//			approx_wait(3000);
//		#endif
//	}
//
//	return 0;
//}

void approx_wait(uint32_t milliseconds)
{
  for (uint32_t j = 0; j < milliseconds; j++)
  {
    for (volatile uint32_t i = 0; i < 7320; i++)
      ;
  }
}
