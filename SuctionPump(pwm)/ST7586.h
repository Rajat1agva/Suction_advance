/*
 * Graustufen-LCD Controller (SMART Response XE Projekt)
 * 
 *
 * ST7586.h
 *
 * Created: 10.09.2018 10:59:17
 *  Author: gfcwfzkm
 */ 


#ifndef ST7586_H_
#define ST7586_H_

#include <avr/io.h>
#include <util/delay.h>
#include <avr/pgmspace.h>
#include <stdlib.h>
//#include "SPI_0_AVR128DA64.h"
// #include "sxe_pinout.h"
// #include "hwSPI.h"

#define ST7586_SERIAL		// 4-Line serial interface
//#define ST7586_PARALLEL	// 6800 Series Interface (unimplemented)

#define ST7586_DISPLAYSIZE_X	400
#define ST7586_DISPLAYSIZE_Y	200

#define ST7586_CSLOW()		(PORTA_OUT &= ~(1 << 7))
#define ST7586_CSHIGH()		(PORTA_OUT |= (1 << 7))
#define ST7586_CMDMODE()	(PORTB.OUT &= ~(1 << 4))
#define ST7586_DATAMODE()	(PORTB.OUT |= (1 << 4))
#define ST7586_SETRST()		(PORTC.OUT &= ~(1 << 5))
#define ST7586_CLRRST()		(PORTC.OUT |= (1 << 5))
#define ST7586_SERIALW(x)	SPI_0_send_char(x)

#define ST7586_COLOR_BLACK		3
#define ST7586_COLOR_DARKGRAY	2
#define ST7586_COLOR_LIGHTGRAY	1
#define ST7586_COLOR_WHITE		0

// All display instructions. A0 = 0 to send the instruction, 1 to send the data/arguments.
enum ST7586_INSTRUCTION_TABLE {
	NOP=0x00,				// No Operation
	RESET=0x01,				// Software Reset
	POWERSAVE=0x10,			// Set Power Save mode (D0=0: Sleep in mode, D0=1: Sleep out mode)
	PARTIALMODE=0x12,		// Set partial mode (D0=0: Partial mode on, D0=1: Partial mode off)
	INVERSEDISPLAY=0x20,	// Set inverse display mode (D0=0: Normal display, D0=1: Inverse display)
	ALLPIXELONOFF=0x22,		// Set all pixel on mode (D0=0: All pixel off mode, D0=1: All pixel on mode)
	DISPLAYONOFF=0x28,		// Set LCD Display (D0=0: Display off, D0=1: Display on)
	SETCOLUMNADDRESS=0x2A,	// Set column address. Starting column address: 0x00=<XS<=0x7F,Ending Column address: XS=<XE<=0x7F
	SETROWADDRESS=0x2B,		// Set row address. Starting row address: 0x00=<YS=<0x9F,Ending row address: YS=<YE=<0x9F
	WRITEDISPLAYDATA=0x2C,	// Write display data to DDRAM
	READDISPLAYDATA=0x2E,	// Read display data from DDRAM (unavailable in serial interface)
	PARTIALDISPLAYAREA=0x30,// Set partial area. Partial disp. addr start: 0x00=<PTS<=0x9F, Partial disp addr end: 0x00=<PTE=<0x9F. Display Area: 64 =< Duty =< 160
	SCROLLAREA=0x33,		// Set scroll area. Top area: TA=0x00~0xA0, Scrolling Area: SA=0x00~0xA0, Bottom Area: BA=0x00~0xA0, TA+SA+BA = 160
	DISPLAYCONTROL=0x36,	// Set scan direction of COM and SEG: MY=0: COM0->COM159, MY=1: COM159->COM0, MX=0: SEG0->SEG383, MX=1: SEG383->SEG0
	STARTLINE=0x37,			// Set display start line. S=0x00~0x9F
	DISPLAYMODE=0x38,		// Set display mode. (D0=0: Gray mode, D0=1: Monochrome mode)
	ENABLEDDRAMINTERFACE=0x3A, // Enable DDRAM interface, followed by a defined argument
	ENABLEDDRAMINTERFACE_FIXARG=0x02,
	DISPLAYDUTY=0xB0,		// Set display duty DT=0x03~0x9F
	FIRSTOUTPUTCOM=0xB1,	// Set first output COM. FC=0x00~0x9F
	FOSCDIVIDER=0xB3,		// Set FOSC dividing ratio
	PARTIALDISPLAY=0xB4,	// Set partial display mode, needs a fixed argument
	PARTIALDISPLAY_FIXARG=0xA0,
	NLINEINVERSION=0xB5,	// Set N-Line inversion
	READMODIFYWRITE=0xB8,	// Read modify write control (D0=0: Enable read modify write, D0=1: Disable read modify write)
	SETVOP=0xC0,			// Set Vop
	VOPINCREASE=0xC1,		// Vop inrease one step
	VOPDECREASE=0xC2,		// Vop decrease one step
	BIASSYSTEM=0xC3,		// Set BIAS system
	BOOSTERLEVEL=0xC4,		// Set booster level
	VOPOFFSET=0xC7,			// Set Vop offset
	ANALOGCONTROL=0xD0,		// Enable analog circuit, requires fixed argument
	ANALOGCONTROL_FIXARG=0x1D,
	AUTOREADCONTROL=0xD7,	// Auto read control. On the Argument: D4=0: Enable auto read, D4=1: Disable auto read
	AUTOREADCONTROL_ARG=0x0F,
	OTPWRRDCONTROL=0xE0,	// OTP WR/RD control
	OTPCONTROLOUT=0xE1,		// OTP control out
	OTPWRITE=0xE2,			// OTP programming procedure
	OTPREAD=0xE3,			// OTP up-load procedure
	OTPSELECTIONCONTROL=0xE4,// OTP selection control. On the argument: D6=0: disable OTP, D6=1: Enable OTP
	OTPSELECTIONCONTROL_ARG=0x19,
	OPTPROGSETTING=0xE5,	// OTP programming setting, needs a fixed argument
	OPTPROGSETTING_FIXARG=0x0F,
	FRAMERATEGRAYMODE=0xF0,	// Frame rate setting in different temperature range (Gray scale mode)
	FRAMERATEMONOMODE=0xF1,	// Frame rate setting in different temperature range (Monochrome mode)
	TEMPERATURERANGE=0xF2,	// Temperature range setting
	TEMPGRADIENTCOMPENSATION=0xF4 // Set temperature gradient compensation coefficient
};

void ST7586_init(char monochrome);

void ST7586_data(uint8_t data[], uint8_t dataSize);

void ST7586_cmd(enum ST7586_INSTRUCTION_TABLE cmd);

// VOP -> Contrast control, basically. Default-Value is 0x0F0
uint16_t ST7586_incVOP();

uint16_t ST7586_decVOP();

// usable VOP Voltage between 7.44V and 18V (0x060 to 0x168)
// Size: 9 bits!
uint16_t ST7586_setVOP(uint16_t vop);

uint16_t ST7586_getVOP();

void ST7586_fill(uint8_t color);

void ST7586_setDispPosition(uint16_t x1, uint8_t y1, uint16_t x2, uint8_t y2);

void ST7586_setPixel(uint16_t x, uint8_t y, uint8_t color);

#ifdef ST7586_PARALLEL
uint8_t ST7586_getPixel(uint16_t x, uint8_t y);
#else
// Unable to read display in serial mode Q~Q
#define ST7586_getPixel(x,y)	asm("NOP")
#endif

#ifdef ST7586_SERIAL
enum ST7586_errorCodes {mallocFail=1,XYoutofDispArea=2,XY1biggerThanXY2=3,bufferFail=4};
struct ST7586_reservedArea
{
	uint8_t *drawBuffer;	// Virtual Drawing Area
	uint16_t bufferSize;	// Size of the Buffer
	uint16_t x1,y1,x2,y2,cx,cy;	// Start and End Coordinates
	enum ST7586_errorCodes errorCode;		// In case reserveArea Function fails, error is here stored.
};

// Temporarly reserve a drawing area on the uC's memory, drawing on it before sending it to the display
struct ST7586_reservedArea ST7586_getReservedAreaMalloc(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);

struct ST7586_reservedArea ST7586_getReservedAreaExBuffer(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint8_t *buffer, uint16_t buffersize);

void ST7586_setPixelReservedArea(struct ST7586_reservedArea *inst, uint16_t x, uint16_t y, uint8_t color);

void ST7586_sendReservedArea(struct ST7586_reservedArea *inst);

void ST7586_destroyReservedArea(struct ST7586_reservedArea *inst);


// Force the currently stored pixel out. Only required when using setPixel()
void ST7586_forcePixelUpdate();

#elif ST7586_PARALLEL
#define ST7586_forcePixelUpdate()	asm("NOP")
#endif

void ST7586_memset(uint8_t *array, char c, char arSize);
uint16_t display_vop = 0x00f5;

const uint8_t pixelColorToByte[3][4] PROGMEM =
{
	{0,0x40,0x08,0xE0},
	{0,0x08,0x10,0x1C},
	{0,0x01,0x02,0x03}
};

const uint8_t columnColorToByte[4] PROGMEM = {0, 0x4a, 0x92, 0xff};

#ifdef ST7586_SERIAL
struct dataSerial{
	uint8_t last_col, last_y;
	uint8_t firstTime;
	uint8_t data;
} dataSerial;
#endif

// void ST7586_init(char monochrome)
// {
// 	uint8_t argsArray[4];
// 	dataSerial.firstTime = 1;
//
// 	ST7586_CSHIGH();
// 	ST7586_DATAMODE();
//
// 	ST7586_CLRRST();
// 	_delay_ms(5);
// 	ST7586_SETRST();
// 	_delay_us(20);
// 	ST7586_CLRRST();					// Power-ON -> reset display and wait ~120ms
// 	_delay_ms(200);
//
// 	/* Skip OTP Stuff from the datasheet, not sure if it's always required */
// 	//ST7586_cmd(RESET);
// 	//_delay_ms(120);
// 	ST7586_cmd(0x11);			// Sleep out mode
// 	ST7586_cmd(DISPLAYONOFF);			// Disply OFF
// 	_delay_ms(50);
//
// 	ST7586_cmd(SETVOP);					// Set Vop to 0x0F0
// 	argsArray[0] = (uint8_t)(display_vop & 0x00FF); argsArray[1] = (uint8_t)(display_vop >> 8);
// 	//argsArray[0] = 0x3a;
// 	//argsArray[1] = 0x01;
// 	ST7586_data(argsArray,2);
//
// // 	ST7586_cmd(VOPOFFSET);
// // 	argsArray[0] = 0x00;
// // 	ST7586_data(argsArray,1);
//
// 	ST7586_cmd(BIASSYSTEM);				// BIAS System Ratio 1/14
// 	argsArray[0] = 0x05;
// 	ST7586_data(argsArray,1);
// 	ST7586_cmd(BOOSTERLEVEL);			// Set Booster Level to x8
// 	argsArray[0] = 0x05;
// 	ST7586_data(argsArray,1);
// 	ST7586_cmd(ANALOGCONTROL);			// enable analog circuit
// 	argsArray[0] = 0x1D;
// 	ST7586_data(argsArray,1);
// // 	ST7586_cmd(FOSCDIVIDER);			// Set FOSC Divider to "not divide"
// // 	argsArray[0] = 0;
// // 	ST7586_data(argsArray,1);
//  	ST7586_cmd(NLINEINVERSION);			// Set N-Line Inversion ( 8B, dunno why)
//  	argsArray[0] = 0x8C;
// 	ST7586_data(argsArray,1);
// 	ST7586_cmd(0x39);	// Set Display mode to grayscale or monochrome, when "monochrome" is 1
// 	ST7586_cmd(ENABLEDDRAMINTERFACE);	// Enable DDRAM interface
// 	argsArray[0] = ENABLEDDRAMINTERFACE_FIXARG;
// 	ST7586_data(argsArray,1);
//
//
// 	ST7586_cmd(0x36);   // Scan direction setting
// 	argsArray[0] = 0x00;
// 	ST7586_data(argsArray,1);
//
// 	ST7586_cmd(DISPLAYDUTY);			// Display duty. Set to 9F when duty is 160.
// 	argsArray[0] = 0x9F;
// 	ST7586_data(argsArray,1);
//
// 	ST7586_cmd(INVERSEDISPLAY);			// No display inversion (Normal Display)
//
// // 	ST7586_cmd(DISPLAYCONTROL);			// Normal display scan direction
// // 	argsArray[0] = 0;
// // 	ST7586_data(argsArray,1);
//
// // 	ST7586_cmd(FRAMERATEMONOMODE | monochrome);		// Display Frame Rate in Grayscale/Monochrome Mode (77Hz for all Temperatures)
// // 	argsArray[0] = 0x12;argsArray[2] = 0x12;	// Todo: Proper testing with various temperatures for the right framerates!
// // 	argsArray[3] = 0x12;argsArray[4] = 0x12;
// // 	ST7586_data(argsArray,4);
//
// 	ST7586_cmd(0x2A);
// 	argsArray[0] = 0x00;
// 	argsArray[1] = 0x00;
// 	argsArray[2] = 0x00;
// 	argsArray[3] = 0x7f;
// 	ST7586_data(argsArray,4);
//
// 	ST7586_cmd(0x2B);
// 	argsArray[0] = 0x00;
// 	argsArray[1] = 0x00;
// 	argsArray[2] = 0x00;
// 	argsArray[3] = 0x9f;
// 	ST7586_data(argsArray,4);
//
// 	ST7586_cmd(0x22);                     //All Pixel off mode
// 	ST7586_cmd(DISPLAYONOFF | 1);		// Turn display on
// 	//ST7586_fill(ST7586_COLOR_WHITE);
// }

void ST7586_init(char monochrome)
{
	uint8_t argsArray[4];
	dataSerial.firstTime = 1;
	
	ST7586_CSHIGH();
	ST7586_DATAMODE();
	
	ST7586_CLRRST();
	_delay_ms(5);
	ST7586_SETRST();
	_delay_us(20);
	ST7586_CLRRST();					// Power-ON -> reset display and wait ~120ms
	_delay_ms(120);
	
	/* Skip OTP Stuff from the datasheet, not sure if it's always required */
	ST7586_cmd(RESET);
	_delay_ms(120);
	ST7586_cmd(POWERSAVE | 1);			// Sleep out mode
	ST7586_cmd(DISPLAYONOFF);			// Disply OFF
	ST7586_cmd(SETVOP);					// Set Vop to 0x0F0
	argsArray[0] = (uint8_t)(display_vop & 0x00FF); argsArray[1] = (uint8_t)(display_vop >> 8);
	ST7586_data(argsArray,2);
	ST7586_cmd(BIASSYSTEM);				// BIAS System Ratio 1/14
	argsArray[0] = 0x05;
	ST7586_data(argsArray,1);
	ST7586_cmd(BOOSTERLEVEL);			// Set Booster Level to x8
	argsArray[0] = 0x05;
	ST7586_data(argsArray,1);
	ST7586_cmd(ANALOGCONTROL);			// enable analog circuit
	argsArray[0] = 0xd1;
	ST7586_data(argsArray,1);
	ST7586_cmd(FOSCDIVIDER);			// Set FOSC Divider to "not divide"
	argsArray[0] = 0;
	ST7586_data(argsArray,1);
	ST7586_cmd(NLINEINVERSION);			// Set N-Line Inversion ( 8B, dunno why)
	argsArray[0] = 0x8B;
	ST7586_data(argsArray,1);
	ST7586_cmd(DISPLAYMODE | monochrome);	// Set Display mode to grayscale or monochrome, when "monochrome" is 1
	ST7586_cmd(ENABLEDDRAMINTERFACE);	// Enable DDRAM interface
	argsArray[0] = ENABLEDDRAMINTERFACE_FIXARG;
	ST7586_data(argsArray,1);
	ST7586_cmd(DISPLAYCONTROL);			// Normal display scan direction
	argsArray[0] = 0;
	ST7586_data(argsArray,1);
	ST7586_cmd(DISPLAYDUTY);			// Display duty. Set to 9F when duty is 160.
	argsArray[0] = 0x9F;
	ST7586_data(argsArray,1);
	ST7586_cmd(FRAMERATEGRAYMODE | monochrome);		// Display Frame Rate in Grayscale/Monochrome Mode (77Hz for all Temperatures)
	argsArray[0] = 0x12;argsArray[2] = 0x12;	// Todo: Proper testing with various temperatures for the right framerates!
	argsArray[3] = 0x12;argsArray[4] = 0x12;
	ST7586_data(argsArray,4);
	ST7586_cmd(INVERSEDISPLAY);			// No display inversion (Normal Display)
	ST7586_cmd(DISPLAYONOFF | 1);		// Turn display on
	ST7586_fill(ST7586_COLOR_WHITE);
}

void ST7586_data(uint8_t data[], uint8_t dataSize)
{
	#ifdef ST7586_SERIAL
	ST7586_CSLOW();
	for (uint8_t i = 0; i < dataSize; i++)
	{
		ST7586_SERIALW(data[i]);
	}
	ST7586_CSHIGH();
	#elif ST7586_PARALLEL

	#endif
}

void ST7586_cmd(enum ST7586_INSTRUCTION_TABLE cmd)
{
	#ifdef ST7586_SERIAL
	ST7586_CSLOW();
	ST7586_CMDMODE();
	ST7586_SERIALW(cmd);
	ST7586_DATAMODE();
	ST7586_CSHIGH();
	#elif ST7586_PARALLEL
	
	#endif
}

uint16_t ST7586_incVOP()
{
	ST7586_cmd(VOPINCREASE);
	return (++display_vop);
}

uint16_t ST7586_decVOP()
{
	ST7586_cmd(VOPDECREASE);
	return (--display_vop);
}

// usable VOP Voltage between 7.44V and 18V (0x0060 to 0x0168)
// Size: 9 bits!
uint16_t ST7586_setVOP(uint16_t vop)
{
	uint8_t argsArray[2];
	display_vop = 0x1FF & vop;	// Only 9 bits are allowed!
	ST7586_cmd(SETVOP);					// Set Vop to 0x0F0
	argsArray[0] = (uint8_t)(display_vop & 0x00FF); argsArray[1] = (uint8_t)(display_vop >> 8);
	ST7586_data(argsArray,2);
	return (display_vop);
}

uint16_t ST7586_getVOP()
{
	return (display_vop);
}

void ST7586_fill(uint8_t color)
{
	uint8_t fillArray[ST7586_DISPLAYSIZE_X/3];
	
	for (uint8_t i = 0; i < (ST7586_DISPLAYSIZE_X / 3); i++)
	{
		fillArray[i] = pgm_read_byte(&(columnColorToByte[color]));
	}
	
	ST7586_setDispPosition(0,0,ST7586_DISPLAYSIZE_X-1,ST7586_DISPLAYSIZE_Y-1);
	ST7586_cmd(WRITEDISPLAYDATA);
	for (uint8_t y = 0; y < ST7586_DISPLAYSIZE_Y; y++)
	{
		ST7586_data(fillArray,ST7586_DISPLAYSIZE_X/3);
	}
}

void ST7586_setDispPosition(uint16_t x1, uint8_t y1, uint16_t x2, uint8_t y2)
{
	uint8_t tempArgs[4];
	// Check if it's outside the display sizes, and if x1/y1 is smaller than x2/y2
	if ((x1 > ST7586_DISPLAYSIZE_X-1) || (x2 > ST7586_DISPLAYSIZE_X-1) || (y1 > ST7586_DISPLAYSIZE_Y) || (y2 > ST7586_DISPLAYSIZE_Y))
	return;
	if ((x1>x2) || (y1>y2))
	return;
	
	ST7586_cmd(SETCOLUMNADDRESS);	// Set Column Address
	// Stupid display addressing. Why requiring two bytes, when the max value is 127 for columns / 160 for rows???
	tempArgs[0] = 0;
	tempArgs[1] = x1/3;
	tempArgs[2] = 0;
	tempArgs[3] = x2/3;
	ST7586_data(tempArgs, 4);
	ST7586_cmd(SETROWADDRESS);		// Set Row Address
	tempArgs[0] = 0;
	tempArgs[1] = y1;
	tempArgs[2] = 0;
	tempArgs[3] = y2;
	ST7586_data(tempArgs, 4);
}

void ST7586_setPixel(uint16_t x, uint8_t y, uint8_t color)
{
	#ifdef ST7586_SERIAL
	// Fancy precise pixel drawing for graphic libraries
	if (dataSerial.firstTime)
	{
		dataSerial.last_col = (uint8_t)(x/3);
		dataSerial.last_y = y;
		dataSerial.data = pgm_read_byte(&(pixelColorToByte[x%3][color]));
		dataSerial.firstTime = 0;
	}
	else
	{
		if ( (dataSerial.last_col != (x/3)) || (dataSerial.last_y != y) )
		{
			ST7586_setDispPosition(dataSerial.last_col*3, dataSerial.last_y, dataSerial.last_col*3, dataSerial.last_y);
			ST7586_cmd(WRITEDISPLAYDATA);
			ST7586_data(&dataSerial.data, 1);
			dataSerial.data = pgm_read_byte(&(pixelColorToByte[x%3][color]));
			dataSerial.last_col = (uint8_t)(x/3);
			dataSerial.last_y = y;
		}
		else
		{
			dataSerial.data |= pgm_read_byte(&(pixelColorToByte[x%3][color]));
		}
	}
	#elif ST7586_PARALLEL
	
	#endif
}

#ifdef ST7586_SERIAL
struct ST7586_reservedArea ST7586_getReservedAreaMalloc(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
	struct ST7586_reservedArea drawArea;
	drawArea.errorCode = 0;
	
	if ((x1 > ST7586_DISPLAYSIZE_X-1) || (x2 > ST7586_DISPLAYSIZE_X-1) || (y1 > ST7586_DISPLAYSIZE_Y) || (y2 > ST7586_DISPLAYSIZE_Y))
	{
		drawArea.errorCode = XYoutofDispArea;
	}
	else if ((x1>x2) || (y1>y2))
	{
		drawArea.errorCode = XY1biggerThanXY2;
	}
	else
	{
		drawArea.cx = (x2 - x1) + 1;
		drawArea.cy = (y2 - y1) + 1;
		uint16_t calBufSize = ((drawArea.cx * drawArea.cy) >> 2) + 1;
		drawArea.drawBuffer = (uint8_t *)malloc(calBufSize);
		
		if (drawArea.drawBuffer==NULL)
		{
			drawArea.errorCode = mallocFail;
		}
		else
		{
			drawArea.bufferSize = calBufSize;
			for (uint16_t i = 0; i < calBufSize; i++)
			{
				drawArea.drawBuffer[i] = 0;
			}
			drawArea.x1 = x1;
			drawArea.x2 = x2;
			drawArea.y1	= y1;
			drawArea.y2	= y2;
		}
	}
	return drawArea;
}

struct ST7586_reservedArea ST7586_getReservedAreaExBuffer(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint8_t *buffer, uint16_t buffersize)
{
	struct ST7586_reservedArea drawArea;
	drawArea.errorCode = 0;
	
	if ((x1 > ST7586_DISPLAYSIZE_X-1) || (x2 > ST7586_DISPLAYSIZE_X-1) || (y1 > ST7586_DISPLAYSIZE_Y) || (y2 > ST7586_DISPLAYSIZE_Y))
	{
		drawArea.errorCode = XYoutofDispArea;
	}
	else if ((x1>x2) || (y1>y2))
	{
		drawArea.errorCode = XY1biggerThanXY2;
	}
	else
	{
		uint16_t calBufSize = ((x2-x1+1)/3) * (y2-y1+1);
		
		if (buffersize<calBufSize)
		{
			drawArea.errorCode = bufferFail;
		}
		else
		{
			drawArea.drawBuffer = buffer;
			drawArea.bufferSize = buffersize;
			drawArea.x1 = x1;
			drawArea.x2 = x2;
			drawArea.y1	= y1;
			drawArea.y2	= y2;
		}
	}
	return drawArea;
}

void ST7586_setPixelReservedArea(struct ST7586_reservedArea *inst, uint16_t x, uint16_t y, uint8_t color)
{
	if (inst->errorCode)
	{
		return;
	}
	else if ((x < inst->x1) || (x > inst->x2) || (y < inst->y1) || (y > inst->y2))
	{
		inst->errorCode = XYoutofDispArea;
		return;
	}
	
	x -= inst->x1;
	y -= inst->y1;
	uint16_t coords = ((x + (inst->cx * y)) >> 2);
	uint16_t shift = (((inst->cx * y) + x) % 4) << 1;
	inst->drawBuffer[coords] |= (color << shift);
};

void ST7586_sendReservedArea(struct ST7586_reservedArea *inst)
{
	if (inst->errorCode)
	{
		return;
	}
	
	uint16_t position;
	uint16_t shift, data;
	for (uint16_t y = 0; y < inst->cy; y++)
	{
		for (uint16_t x = 0; x < inst->cx; x++)
		{
			position = ((x + (inst->cx * y)) >> 2);
			shift = (((((inst->cx) * y) + x) % 4) << 1);
			data = (inst->drawBuffer[position] >> (uint8_t)shift) & 0x03;
			ST7586_setPixel(x + inst->x1, (uint8_t)(y + inst->y1), data);
		}
		ST7586_forcePixelUpdate();
	}
}

void ST7586_destroyReservedArea(struct ST7586_reservedArea *inst)
{
	free(inst->drawBuffer);
	inst->bufferSize = 0;
}

void ST7586_forcePixelUpdate()
{
	ST7586_setDispPosition(dataSerial.last_col*3, dataSerial.last_y, dataSerial.last_col*3+1, dataSerial.last_y);
	ST7586_cmd(WRITEDISPLAYDATA);
	ST7586_data(&dataSerial.data, 1);
	dataSerial.firstTime = 1;
}
#endif

void ST7586_memset(uint8_t *array, char c, char arSize)
{
	for (uint8_t i = 0; i < arSize; i++)
	{
		array[i] = c;
	}
}

#endif /* ST7586_H_ */