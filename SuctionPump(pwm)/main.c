/*
 * SuctionPump(pwm).c
 *
 * Created: 04-Feb-22 10:51:01 AM
 * Author : AGVA
 */ 

#define F_CPU							24000000UL

#include <avr/io.h>
#include <util/delay.h>
#include "avr/xmega.h"
#include "UART_1_AVR128DA64.h"
#include "SPI_0_AVR128DA64.h"
#include "Ams5812_i2c.h"
#include "I2C_0_AVR128DA64.h"
#include "ADC_AVR128DA64.h"
#include "ST7586.h"

int  dutyCycle1  = 1500;			// MAX Value	3000
int  dutyCycle2	 = 1000;			// MAX Value	

void TCA0_PWM_init(void);
void PWM_Condition1(void);
void PWM_Condition2(void);


int main(void)
{   _PROTECTED_WRITE (CLKCTRL.OSCHFCTRLA, ((CLKCTRL_FREQSEL_24M_gc)|(CLKCTRL_AUTOTUNE_bm)));			//To increase clock freq
	TCA0_PWM_init();
	Sensor_init();
	USART1_init(9600);
	ADC0_init();
	while (1)
	{   uint8_t i = 200;
		float pressure = 0.0;
		
		while(i)
		{
			pressure = pressure + pressureRead();
			_delay_us(1000);
			i--;
		}
	float	final_pressure = ((pressure/200)*51.7149); //final pressure in mmHg
	USART1_sendFloat(final_pressure,2);
	
	int count = 0;
	count = ADC0_read(channel_1);
	
	dutyCycle1 = 0.3663004*count + 1500;
	TCA0.SINGLE.CMP0 = dutyCycle1;
		
	}
}

void TCA0_PWM_init(void)
{
	//sei();
	/*		FIRST PWM on PA0 PIN		*/
	// Direction set and set output to high
	PORTA.DIR |= (1 << 0);
	PORTA.PIN0CTRL |= (1 << 3);
	
	//PORTA.OUTSET |= (1 << 0);
	TCA0.SINGLE.PER = 3000;     // calculation for 1ms
	TCA0.SINGLE.CMP0 = dutyCycle1;
	TCA0.SINGLE.CNT = 0;

	
	TCA0.SINGLE.CTRLB |= (1 << 4); // compare 0 enable
	TCA0.SINGLE.CTRLB |= (0x3 << 0); // Single slope PWM
	TCA0.SINGLE.CTRLA |= (1<<0); //fclk_per 1 and  Enable pwm
	TCA0.SINGLE.CTRLA |= ((1<<1)|(1<<2)); //fclk_per 8 and  Enable pwm
	
	/*		Second PWM on PA1 PIN		*/
	
	PORTA.DIR |= (1 << 1);						// Direction set and set output to high
	PORTA.PIN1CTRL |= (1 << 3);
	TCA0.SINGLE.CTRLB |= (1 << 5);				// Enable Compare Pa1
	TCA0.SINGLE.CTRLA |= (1 << 1);				// Enable PWM PA1
	TCA0.SINGLE.CMP1 = dutyCycle2;					// Duty cycle

}

