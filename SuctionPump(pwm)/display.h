#ifndef DISPLAY_H_
#define DISPLAY_H_

#include "SPI_0_AVR128DA64.h"
#include "glcd_library.h"
#include "ST7586.h"
//#include "../LCD_lib.h"
#include "ST7586.c"
#include "glcd_library.c"


#include "Fonts/System_45x47.h"
#include "Fonts/System15x16.h"
#include "Fonts/System15x17.h"
#include "Fonts/Arial_Rounded_MT_Bold12x13.h"
#include "Fonts/System15x16_array.h"
#include "Fonts/System12x17.h"

// struct ST7586_reservedArea
// {
// 	uint8_t *drawBuffer;	// Virtual Drawing Area
// 	uint16_t bufferSize;	// Size of the Buffer
// 	uint16_t x1,y1,x2,y2,cx,cy;	// Start and End Coordinates
// 	enum ST7586_errorCodes errorCode;		// In case reserveArea Function fails, error is here stored.
// };


// void print_System12x17(uint16_t x, uint16_t y,const char *character)
// {
// 	int height =  17 , width = 12;
// 	//PrintDrawingArea = ST7586_getReservedAreaExBuffer(x, y, (strlen_P(character) * width) + x, y + height);
// 	uint8_t _temp = ST7586_BufferEnabled;
// 	
// 	
// }

// uint16_t print_System45x47_font_char(uint16_t x, uint16_t y, char character)
// {
// 	uint8_t height = 34, width = 24;
// 	glcd_draw_rectangle_filled(x,y-3,x+width,y+height,ST7586_COLOR_WHITE);
// 	int pos = (5 * width * (character - 48));
// 	USART1_sendInt(character);
// 	int new_pos = 0;
// 	int array_data = 0;
// 	for(int colm_n= 0; colm_n < width; colm_n++)
// 	{
// 		for(int row_n = 0; row_n < 5; row_n++)
// 		{
// 			new_pos = (pos + (colm_n*5) + row_n);
// 			USART1_sendInt(new_pos);
// 			array_data = pgm_read_byte(&(system24x34[new_pos]));
// 			for (int j = 0; j < 8; j++)
// 			{
// 			//	USART1_sendString("---");
// 				if (array_data & (1<<j))
// 				{
// 					ST7586_setPixel(x+colm_n, y+(row_n*8)+j, ST7586_COLOR_BLACK);
// 			//		USART1_sendInt(x+colm_n);
// 			//		USART1_sendInt(y+(row_n*8)+j);
// 				}
// 			}
// 		}
// 	}
// }


// void test(int x, int y)
// {
// 	int data1byte = 0xB7;
// 	for (int j=0; j<8; j++)
// 	{
// 		if (data1byte & (1 << j))
// 		{
// 			glcd_ST7586_setPixel(x, y + j, ST7586_COLOR_BLACK);
// 		}
// 	}
// }

uint16_t print_System12x17_font_char(uint16_t x, uint16_t y, const char character)
{
	int height = 17, width = 25;
	
	int pos = (3 * width * (character - 32));
	char data1byte;
	glcd_draw_rectangle_filled(x,y-3,x+12,y+17,ST7586_COLOR_WHITE);
	for (int i = 0; i < (3*width) + 1; i = i+2)
	{
		for (int k = 0; k < height; )
		{
//			data1byte = pgm_read_byte(&(System12x17[pos + i + (k / 8)]));
			data1byte = pgm_read_byte(&(System17x25[pos + i + (k / 8)]));

			for (int j = 0 ; (j < 8)&&(k < height); j++, k++)
			{
				if (data1byte & (1 << j))
				{
					if (k/8)
					{
						glcd_ST7586_setPixel(x + j + (8), y + (i/3), ST7586_COLOR_BLACK);
					}
					else
					{
						glcd_ST7586_setPixel(x + j, y + (i/3), ST7586_COLOR_BLACK);
					}
				}
				// 				else
				// 				{
				// 					if (k/8)
				// 					{
				// 						glcd_ST7586_setPixel(x + j + (8), y + (i/2), ST7586_COLOR_WHITE);
				// 					}
				// 					else
				// 					{
				// 						glcd_ST7586_setPixel(x + j, y + (i/2), ST7586_COLOR_WHITE);
				// 					}
				// 				}
				
			}
		}
	}
	if(character == 'I')
	{
		width = width/2 + 1;
	}
	return(x + ((width*3)/4) );
}

// uint16_t print_System12x17_font_char(uint16_t x, uint16_t y, const char character)
// {
// 	int height = 12, width = 17;
// 	
// 	int pos = (2 * width * (character - 32));
// 	char data1byte;
// 	glcd_draw_rectangle_filled(x,y-3,x+12,y+17,ST7586_COLOR_WHITE);
// 	for (int i = 0; i < (2*width) + 1; i = i+2)
// 	{
// 		for (int k = 0; k < height; )
// 		{
//  			data1byte = pgm_read_byte(&(System12x17[pos + i + (k / 8)]));
// 
// 			for (int j = 0 ; (j < 8)&&(k < height); j++, k++)
// 			{
// 				if (data1byte & (1 << j))
// 				{
// 					if (k/8)
// 					{
// 						glcd_ST7586_setPixel(x + j + (8), y + (i/2), ST7586_COLOR_BLACK);
// 					}
// 					else
// 					{
// 						glcd_ST7586_setPixel(x + j, y + (i/2), ST7586_COLOR_BLACK);
// 					}
// 				}
// 				// 				else
// 				// 				{
// 				// 					if (k/8)
// 				// 					{
// 				// 						glcd_ST7586_setPixel(x + j + (8), y + (i/2), ST7586_COLOR_WHITE);
// 				// 					}
// 				// 					else
// 				// 					{
// 				// 						glcd_ST7586_setPixel(x + j, y + (i/2), ST7586_COLOR_WHITE);
// 				// 					}
// 				// 				}
// 				
// 			}
// 		}
// 	}
// 	if(character == 'I')
// 	{
// 		width = width/2 + 1;
// 	}
// 	return(x + ((width*3)/4) );
// }
void print_System12x17_font_str(uint16_t x, uint16_t y, char *str)
{
	int i = 0, x_temp;
	x_temp = x;
	//glcd_draw_rectangle_filled(x, y, (x+((strlen(str)*12)/8)), y+25, ST7586_COLOR_WHITE);
	do
	{
		x_temp = print_System12x17_font_char(x_temp , y, *(str+i));
		i++;
	}while(*(str + i) != '\0');
	//ST7586_forcePixelUpdate();
}


uint16_t print_System12x17_font_char_buf(uint16_t x, uint16_t y, char *character)
{
	int height = 12, width = 17;
    //uint8_t _temp = ST7586_BufferEnabled;
    ST7586_BufferEnabled = 2;
	int pos = (2 * width * ((int)character - 32));
	char data1byte;
	for (int i = 0; i < (2*width) + 1; i = i+2)
	{
		for (int k = 0; k < height; )
		{
			data1byte = pgm_read_byte(&(System12x17[pos + i + (k / 8)]));
			for (int j = 0 ; (j < 8)&&(k < height); j++, k++)
			{
				if (data1byte & (1 << j))
				{
					if (k/8)
					{
						glcd_ST7586_setPixel(x + j + (8), y + (i/2), ST7586_COLOR_BLACK);
					}
					else
					{
						glcd_ST7586_setPixel(x + j, y + (i/2), ST7586_COLOR_BLACK);
					}
				}
				else
				{
				 	if (k/8)
				 	{
				 		glcd_ST7586_setPixel( x + j + (8), y + (i/2), ST7586_COLOR_WHITE);
				 	}
				 	else
				 	{
				 		glcd_ST7586_setPixel(x + j, y + (i/2), ST7586_COLOR_WHITE);
				 	}
				}
				
			}
		}
	}
	if(*character == 'I')
	{
		width = width/2 + 1;
	}
	return(x + ((width*3)/4) );
}

void print_System12x17_font_str_buff(uint16_t x, uint16_t y, char *str)
{
	PrintDrawingArea = ST7586_getReservedAreaMalloc(x, y, (strlen_P(str) * 12) + x, y + 16);
	uint8_t _temp = ST7586_BufferEnabled;
	ST7586_BufferEnabled = 2;
	
	//char c = pgm_read_byte(character);
	while (*str != '\0')
	{
		print_System12x17_font_char_buf(x, 10, *(str++)) ;
		x += 12;
	}
	
	ST7586_sendReservedArea(&PrintDrawingArea);
	ST7586_destroyReservedArea(&PrintDrawingArea);
	ST7586_forcePixelUpdate();
	ST7586_BufferEnabled = _temp;
}


/************************************************************************/
/* Creating pixels in buffer.                                                                     */
/************************************************************************/
void set_pixel_buff(char **buffer,int x, int y, char color)  // should be less than x :25*8 = 200 and y: 20
{
	if (color)
	{
		buffer[x/8][y] |= (1 << (x % 8));
	}
	else
	{
		buffer[x/8][y] &= ~(1 << (x % 8));
	}
}

int set_char_buff(char **buffer, int x, int y, char character)
{
	int height = 12, width = 17;
	
	int pos = (2 * width * (character - 32));
	char data1byte;
	for (int i = 0; i < (2*width) + 1; i = i+2)
	{
		for (int k = 0; k < height; )
		{
			data1byte = pgm_read_byte(&(System12x17[pos + i + (k / 8)]));
			for (int j = 0 ; (j < 8)&&(k < height); j++, k++)
			{
				if (data1byte & (1 << j))
				{
					if (k/8)
					{
						set_pixel_buff(buffer ,x + j + (8), y + (i/2) +3, ST7586_COLOR_BLACK);
					}
					else
					{
						set_pixel_buff(buffer ,x + j, y + (i/2) + 3, ST7586_COLOR_BLACK);
					}
				}
//  				else
//  				{
//  					if (k/8)
//  					{
//  						set_pixel_buff(buffer ,x + j + (8), y + (i/2) + 3, ST7586_COLOR_WHITE);
//  					}
//  					else
//  					{
//  						set_pixel_buff(buffer ,x + j, y + (i/2) + 3, ST7586_COLOR_WHITE);
//  					}
// 				 }
			}
		}
	}
	if(character == 'I')
	{
		width = width/2 + 1;
	}
	return(x + ((width*3)/4));
}

//char ptr[20][25];
void creating_buffer_to_print(int x, int y, char *str)
{
	//USART1_sendString("Creating buffer..");
	y = y-3;
	char **ptr = (char **)calloc(40 , sizeof(char));     // Max rows for 20pixels vertical
	for (int i = 0; i < 20; i++)
	{
		ptr[i] = (char *)calloc(40 , sizeof(char));     // Max for 25*8pixels horizontal
	}
	if (ptr == NULL)
	{
		USART1_sendString("Error in creating buffer.");
		return;
	}
	//USART1_sendString("Buffer created.");
// 	for (int i = 0; i < 20; i++)
// 	{
// 		for(int j = 0; j < 25; j++)
// 		{
// 			ptr[j][i] = 0x00;
// 		}
// 	}
	
	int size_of_string = strlen(str);
	//USART1_sendString("length of string");
	//USART1_sendInt(size_of_string);
	
	int temp_x = 0, temp_y = 0;
	int i=0;
	while(*(str + i) != '\0')
	{
		temp_x = set_char_buff(ptr ,temp_x , temp_y, *(str+i));
		i++;
	}
	
	//USART1_sendString("Drawing complete. \nPrinting..");
	//size_of_string--;
	//glcd_draw_line(x, y, x + (size_of_string*12), y, ST7586_COLOR_BLACK);
	size_of_string = ((size_of_string*8) +4)/8;
	int temp = size_of_string+((size_of_string)/2);
	for (int i = 0; i < 20 ; i++)
	{
		for (int j = 0; (j < 40) && (j < temp); j++)
		{
			for (int k = 0; k < 8; k++)
			{
				if (ptr[j][i] & (1 << k))
				{
					glcd_ST7586_setPixel(x + (j*8) + k, y + i, ST7586_COLOR_WHITE);
					//ST7586_forcePixelUpdate();
				}
				else
				{
					glcd_ST7586_setPixel(x + (j*8) + k, y + i, ST7586_COLOR_BLACK);
				}
			}
		}
	}
	//ST7586_forcePixelUpdate();
	//USART1_sendString("Destroying buffer..");
	for (int i = 0; i < 20; i++)
	{
		free(ptr[i]);
	}
	free(ptr);
	//USART1_sendString("Buffer destroyed.");
}

/************************program for buffer printing for st7586****************************/
// void glcd_buf_print12x17(uint16_t x, uint16_t y, char *character)
// {
// 	int height = 12, width = 17;
// 	
// 	PrintDrawingArea = ST7586_getReservedAreaMalloc(x,y,(strlen(character) * width) + x, y + height);
// 	uint8_t _temp = ST7586_BufferEnabled;
// 	ST7586_BufferEnabled = 2;
// 	
// 	while(*character)
// 	{
// 		print_System12x17_font_char(x, y, *character);
// 		if (*character++)
// 		{
// 			x += width;
// 		}
// 	}
// 	
// 	ST7586_sendReservedArea(&PrintDrawingArea);
// 	ST7586_destroyReservedArea(&PrintDrawingArea);
// 	ST7586_forcePixelUpdate();
// 	ST7586_BufferEnabled = _temp;
// }
// /************************program not working ^ ****************************/
// 
// 
// void glcd_buf_print_progmem12x17(uint16_t x, uint16_t y,const char *character)
// {
// 	int height = 12, width = 17;
// 	PrintDrawingArea = ST7586_getReservedAreaMalloc(x, y, (strlen_P(character) * width) + x, y + height);
// 	uint8_t _temp = ST7586_BufferEnabled;
// 	ST7586_BufferEnabled = 2;
// 	
// 	char c = pgm_read_byte(character);
// 	while ( (c = pgm_read_byte(character++)) )
// 	{
// 		print_System12x17_font_char(x, y, c);
// 		x += width;
// 	}
// 	
// 	ST7586_sendReservedArea(&PrintDrawingArea);
// 	ST7586_destroyReservedArea(&PrintDrawingArea);
// 	ST7586_forcePixelUpdate();
// 	ST7586_BufferEnabled = _temp;
// }



















//
//
//uint8_t font_height, font_width;
//char *custom_font;
//const char custom_char[] PROGMEM= {0x00,0x00,0xF8,0x00,0xDC,0x01,0x8E,0x03,0x8E,0x03,0x8E,0x03,0x8E,0x03,0x8E,0x03,0x8E,0x03,0xFE,0x03,0x8E,0x03,0x8E,0x03,0x8E,0x03,0x8E,0x03,0x8E,0x03,0x00,0x00,0x00,0x00};
//
//
//// void set_font(uint8_t height, uint8_t width, char* select_font)
//// {
//// 	font_height = height;
//// 	font_width = width;
//// 	//custom_font = pgm_read_byte(&select_font);
//// }
//
//uint16_t print_System15x16_font_char(uint16_t x, uint16_t y, const char character)
//{
//int height = 15, width = 16;
//
//int pos = (2 * width * (character - 32));
//char data1byte;
//for (int i = 0; i < (2*width) + 1; i = i+2)
//{
//for (int k = 0; k < height; )
//{
//data1byte = pgm_read_byte(&(System15x16[pos + i + (k / 8)]));
//for (int j = 0 ; (j < 8)&&(k < height); j++, k++)
//{
//if (data1byte & (1 << j))
//{
//if (k/8)
//{
//glcd_ST7586_setPixel(x + j + (8), y + (i/2), ST7586_COLOR_BLACK);
//}
//else
//{
//glcd_ST7586_setPixel(x + j, y + (i/2), ST7586_COLOR_BLACK);
//}
//}
//
//}
//}
//}
//return(x + (width / 2));
//}
//
//void print_System15x16_font_str(uint16_t x, uint16_t y, char *str)
//{
//int i = 0, x_temp = 0;
//x_temp = x;
//do
//{
//x_temp = print_System15x16_font_char(x_temp, y, *(str+i));
//i++;
//}while(*(str + i) != '\0');
//}
//
//
//
//void print_Arial_Rounded_font_char(uint16_t x, uint16_t y, const char character)
//{
//int height = 12, width = 13;
//
//int pos = (2 * width * (character - 32));
//char data1byte;
//for (int i = 0; i < (2*width) + 1; i = i+2)
//{
//for (int k = 0; k < height; )
//{
//data1byte = pgm_read_byte(&(Arial_Rounded_MT_Bold12x13[pos + i + (k / 8)]));
//for (int j = 0 ; (j < 8)&&(k < height); j++, k++)
//{
//if (data1byte & (1 << j))
//{
//if (k/8)
//{
//glcd_ST7586_setPixel(x + j + (8), y + (i/2), ST7586_COLOR_BLACK);
//}
//else
//{
//glcd_ST7586_setPixel(x + j, y + (i/2), ST7586_COLOR_BLACK);
//}
//}
//
//}
//}
//}
//}
//
//
//
//uint16_t print_System15x16_array_font_char(uint16_t x, uint16_t y, const char character)
//{
//int height = 15, width = 16;
//if ((character == 'I'))
//{
//width = 8;
//}
//int pos = (character - 32);
//char data1byte;
//
//if (width < 10)
//{
//width = 8;
//for (int i = 0; i < (height); i++)
//{
//data1byte = ~pgm_read_byte(&(System15x17_array[pos][i]));
//for (int j = 0 ; (j < width); j++)
//{
//if (data1byte & (1 << j))
//{
//glcd_ST7586_setPixel(x + j, y + i, ST7586_COLOR_BLACK);
//}
//}
//}
//}
//else
//{
//for (int i = 0; i < (2 * width) + 1; i = i+2)
//{
//for (int k = 0; k < height; )
//{
//data1byte = pgm_read_byte(&(System15x17_array[pos][i + (k / 8)]));
//for (int j = 0 ; (j < 8)&&(k < height); j++, k++)
//{
//if (data1byte & (1 << j))
//{
//if (k/8)
//{
//glcd_ST7586_setPixel(x + j + (8), y + (i/2), ST7586_COLOR_BLACK);
//}
//else
//{
//glcd_ST7586_setPixel(x + j, y + (i/2), ST7586_COLOR_BLACK);
//}
//}
//
//}
//}
//}
//}
//
//return (x + (width) - 1);
//}
//
//void print_System15x16_array_font_str(uint16_t x, uint16_t y, char *str)
//{
//int i = 0, x_temp;
//x_temp = x;
//do
//{
//x_temp = print_System15x16_array_font_char(x_temp -3, y, *(str+i));
//i++;
//}while(*(str + i) != '\0');
//}
//


/************************************************************************/
/* Custom Battery icon.                                                                     */
/************************************************************************/
uint16_t battery_case[8] = { 0b01111111111111, 0b01000000000001, 0b01000000000001, 0b11000000000001, 0b11000000000001, 0b01000000000001, 0b01000000000001, 0b01111111111111};
uint16_t battery[8] = { 0b01111111111111, 0b01000000000001, 0b01000000000001, 0b11000000000001, 0b11000000000001, 0b01000000000001, 0b01000000000001, 0b01111111111111};

void battery_icon(void)
{
	// Drawing ICON.
	int i = 0, j = 0;
	for (char cy = 0; cy < 7; cy++)
	{
		for (uint16_t cx = 299; cx < 312; cx++)
		{
			GLCD_SETXY(cx, cy, ST7586_COLOR_WHITE);
		}
	}
	for (j = 0; j < 8; j++)
	{
		for(i = 0; i < 14; i++)
		{
			if (battery[j] & (1 << (13- i)))
			{
				glcd_ST7586_setPixel(298 + i, 0 + j, ST7586_COLOR_BLACK);
			}
		}
	}
	glcd_draw_line(299, 7, 315, 7, ST7586_COLOR_BLACK);
}

uint8_t BatteryShowLine  = 0b00000010;
void show_battery_status(uint8_t Showline)
{
	int i = 0, j = 0;
	
	for (i = 2; i < 6; i++)
	{
		battery[i] = battery_case[i];
	}
	
	for (i = 0;i < Showline; i++)
	{
		for (j = 2; j < 6; j++)
		{
			battery[j] = battery[j] | (0b00000110 << ((3 * i) + 1));
		}
	}
	battery_icon();
}
#endif