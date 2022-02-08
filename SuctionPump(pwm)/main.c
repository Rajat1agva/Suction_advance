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
#include "bool_numerics.h"
#include "icons_and_numerics.h"

int  dutyCycle1  = 1500;			// MAX Value	3000
int  dutyCycle2	 = 1000;			// MAX Value	
float	final_pressure = 0; 
uint8_t pre_def[3] = {192, 24, 3}, inv_def[3] = {27,195,216}; 
void TCA0_PWM_init(void);
void PWM_Condition1(void);
void PWM_Condition2(void);

void ST7586_send_data(uint8_t data[], uint8_t dataSize);



int main(void)
{   _PROTECTED_WRITE (CLKCTRL.OSCHFCTRLA, ((CLKCTRL_FREQSEL_24M_gc)|(CLKCTRL_AUTOTUNE_bm)));			//To increase clock freq
	PORTB_DIR |= (1<<4);  // data/cmd
	PORTC_DIR |= (1<<5); //Reset pin
	TCA0_PWM_init();
	Sensor_init();
	USART1_init(9600);
	ADC0_init();
	SPI_0_init();
	ST7586_init(1);
	_print_icon(80,10,32,battery_SOC4_15x32,15,32);
	_print_icon(230,40,48,mmhg_icon_24x48,24,48);
	//ST7586_fill(ST7586_COLOR_BLACK);
	while (1)
	{   uint8_t i = 200;
		float pressure = 0.0;
		
		while(i)
		{
			pressure = pressure + pressureRead();
			_delay_us(1000);
			i--;
		}
	final_pressure = ((pressure/200)*51.7149); //final pressure in mmHg
	USART1_sendFloat(final_pressure,2);
	
	int count = 0;
	count = ADC0_read(channel_1);
	USART1_sendInt(count);
	dutyCycle1 = 0.3663004*count + 1500;
	TCA0.SINGLE.CMP0 = dutyCycle1;
	show_mmgh_3digit(150,80);	
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

void __set_pixel (int x1, int y1, int x2, bool color, uint8_t _data[], int array_length)		// with x2,y2 will not be limited as it is not creating any hindrence
{
	uint8_t tempArgs[4];
	//uint8_t _data[3] = {0xC0, 0xF2, 0xDC};
	ST7586_cmd(SETCOLUMNADDRESS);	// Set Column Address
	// Stupid display addressing. Why requiring two bytes, when the max value is 127 for columns / 160 for rows???
	tempArgs[0] = 0;
	tempArgs[1] = x1/3;
	tempArgs[2] = 0;
	tempArgs[3] = x2/3;
	ST7586_send_data(tempArgs, 4);
	ST7586_cmd(SETROWADDRESS);		// Set Row Address
	tempArgs[0] = 0;
	tempArgs[1] = y1;
	tempArgs[2] = 0;
	tempArgs[3] = 159;
	ST7586_send_data(tempArgs, 4);
	ST7586_cmd(WRITEDISPLAYDATA);
	ST7586_send_data(_data, array_length);
	//	USART1_sendString("set pixel");
}

void ST7586_send_data(uint8_t data[], uint8_t dataSize)
{
	
	ST7586_CSLOW();
	for (uint8_t i = 0; i < dataSize; i++)
	{
		ST7586_SERIALW(data[i]);
	}
	ST7586_CSHIGH();
}
void _print_icon(int x1, int y1, int x2, uint8_t *font, uint8_t height , uint8_t width)		//function to print array of uint_8 of any no. of elements, by giving x1,y1,x2,y2
{																							//width should be a multiple of 8 and the icon should also be generated with the width as multiple of 8(GLCD fount creator) but this length could be limited my reducing the x2 as the multiple of 3
	int new_pos = 0, buff_length = (width*3);
	uint8_t array_data = 0;

	for(int _length = 0; _length < height; _length++)
	{
		uint8_t _buff[buff_length];
		for (int i = 0; i < buff_length; i++)			// to make the buff initialize with 0
		{
			_buff[i] = 0x00;
		}
		uint8_t i = 0, k = 0;
		for(int _width = 0; _width < (width/8); _width++)
		{
			array_data = pgm_read_byte(&(font[new_pos]));
			new_pos++;
			for (int j = 7; j >= 0; j--)
			{
				if (array_data & (1<<j))
				{
					_buff[i] = _buff[i] | pre_def[k];
				}
				if(++k > 2)
				{
					k = 0;
					i++;
				}
			}
		}
		__set_pixel(x1, (_length+y1), x2, 1, &_buff[0], buff_length);
		i=0;
	}
}

void show_mmgh_3digit (int x, int y)
{
	int buff =(int)final_pressure;
	uint8_t digit = 0, digit_a = 0, digit_b = 0, digit_c = 0;
	if (buff > 999)
	
	{
		USART1_sendString("pressure digit overflow!!");
		final_pressure = 0;
	}
	digit_c = buff%10;
	buff = buff/ 10;
	digit_b = buff%10;
	digit_a = buff/10;
	buff = 0;
	print_bool(x, y, digit_a);
	print_bool(x+27, y, digit_b);
	print_bool(x+54, y, digit_c);
	
}
void print_bool(int x, int y, int n)
{
	uint8_t height = 36, width = 24;
	int new_pos = 0;
	bool array_data = false;
	uint8_t i = 0, k = 0;

	for(int _length = 0; _length < height; _length++)
	{
		// 			USART1_sendString("length");
		// 			USART1_sendInt(_length);
		uint8_t _buff[8] = {0};
		for(int _width = 0; _width < width; _width++)
		{
			// 				USART1_sendString("width");
			// 				USART1_sendInt(_width);
			// 				USART1_sendString("pos");
			// 				USART1_sendInt(new_pos);
			
			array_data = pgm_read_byte(&(system24x36[n][new_pos]));
			new_pos++;
			// 				USART1_sendString("data");
			// 				USART1_sendInt(array_data);
			if (array_data)
			{
				_buff[i] = _buff[i] | pre_def[k];
				// 					USART1_sendString("buff");
				// 					USART1_sendInt(_buff[i]);
			}
			if(++k > 2)
			{
				k = 0;
				i++;
			}

		}
		
		_set_pixel(x, (_length+y) , 1, &_buff[0], 8);
		i=0;
	}

}
void _set_pixel (int x, int y, bool color, uint8_t _data[], int array_length)
{
	uint8_t tempArgs[4];
	//uint8_t _data[3] = {0xC0, 0xF2, 0xDC};
	ST7586_cmd(SETCOLUMNADDRESS);	// Set Column Address
	// Stupid display addressing. Why requiring two bytes, when the max value is 127 for columns / 160 for rows???
	tempArgs[0] = 0;
	tempArgs[1] = x/3;
	tempArgs[2] = 0;
	tempArgs[3] = 128;
	ST7586_send_data(tempArgs, 4);
	ST7586_cmd(SETROWADDRESS);		// Set Row Address
	tempArgs[0] = 0;
	tempArgs[1] = y;
	tempArgs[2] = 0;
	tempArgs[3] = 159;
	ST7586_send_data(tempArgs, 4);
	ST7586_cmd(WRITEDISPLAYDATA);
	ST7586_send_data(_data, array_length);
	//	USART1_sendString("set pixel");
}