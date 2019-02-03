/*
 * lcd.c
 *
 *  Created on: 14 de jan de 2019
 *      Author: Inspiron
 */

#include "lcd.h"
#include "stdbool.h"
#include "stm32f1xx_hal.h"
#include "font_ascII_256.h"
#include "cross_icon.h"

uint16_t _MC = 0x4E;	//	Address Start RAM Set GDDRAM X
uint16_t _MP = 0x4F;	//	Address Start RAM Set GDDRAM Y
uint16_t _SC = 0x44;	//	Horizontal RAM address
uint16_t _EC = 0x44;	//	Horizontal RAM address
uint16_t _SP = 0x45;	//	Vertical RAM address
uint16_t _EP = 0x46;	//	Vertical RAM address

/*
 *  LCD Handlers
 */

//	Start LCD display hardware
void startLCD() {
	WIDTH = 240;
	HEIGHT = 320;
	begin(0x1289);
}

//	Start LCD display hardware
void begin(uint16_t ID) {
	wr_output();
	rs_output();
    reset();

    // More information about LCD can found in https://github.com/erickfunier/STM32_Display_TFT/blob/default/SSD1289_v1.1.pdf
	static const uint16_t SSD1289_regValues[] = {
		0x0000, 0x0001,	//	Display Oscillator. If 0x0001 (Oscillator ON), if 0x0000 (Oscillator OFF) (page 27)
		0x0001, 0x2B3F, //0x2B3F, //	Driver Output Control - 0x2B3F = 0010 1011 0011 1111 (page 27)
		0x0002, 0x0400, //0x0400, //	LCD-Driving-Waveform Control - 0x0400 = 0000 0100 0000 0000 (page 31)
		0x0003, 0xA8A4, //0xA8A4, //	Power Control (1) LCD - 0xA8A4 = 1010 1000 1010 0100 (page 34)
		0x000C, 0x0000, //	Power Control (2) LCD - 0x0000 (page 38)
		0x0007, 0x0233, //0x0233, //	Display Control 0x0233 = 0000 0010 0011 0011 (page 36)
		0x000D, 0x080C, //0x080C, //	Power Control (3) LCD - 0x000C = 0000 0000 0000 1100 (page 39)
		0x000E, 0x2B00, //0x2B00, //	Power Control (4) LCD - 0x2B00 = 0010 1011 0000 0000 (page 39)
		0x001E, 0x00B7, //0x00B7, //	Power Control (5) LCD - 0x00B7 = 0000 0000 1011 0111 (page 46)
		0x0010, 0x0000, //	Sleep Mode - 0x0000. If 0x0000 Sleep OFF, if 0x0001 Sleep ON (page 40)
		0x0011, 0x6070, //0x6070, //	Entry Mode - 0x6070 = 0110 0000 0111 0000 (page 40)
		0x0005, 0x0000, //	Compare Register Red and Green (page 36)
		0x0006, 0x0000, //	Compare Register Blue (page 36)
		0x0016, 0xEF1C, //	Horizontal Porch 0xEF1C = 1110 1111 0001 1100 (page 44)
		0x0017, 0x0003, //	Vertical Porch 0x0003 = 0000 0000 0000 0011 (page 45)
		0x000B, 0x0000, //	Frame Cycle Control (page 36)
		0x000F, 0x0000, //	Gate Scan Position (page 40)
		0x0041, 0x0000, //	Vertical Scroll Control (page 49)
		0x0042, 0x0000, //	Vertical Scroll Control (page 49)
		0x0048, 0x0000, //	First Screen Driving Position (page 49)
		0x0049, 0x013F, //	First Screen Driving Position (page 49)
		0x004A, 0x0000, //	Second Screen Driving Position (page 50)
		0x004B, 0x0000, //	Second Screen Driving Position (page 50)
		0x0044, 0xEF95, //	Horizontal RAM address position (page 49)
		0x0045, 0x0000, //	Vertical RAM address position (page 49)
		0x0046, 0x013F, //	Vertical RAM address position (page 49)
		0x0030, 0x0707, //	Gamma Control (page 48)
		0x0031, 0x0204, //	Gamma Control (page 48)
		0x0032, 0x0204, //	Gamma Control (page 48)
		0x0033, 0x0502, //	Gamma Control (page 48)
		0x0034, 0x0507, //	Gamma Control (page 48)
		0x0035, 0x0204, //	Gamma Control (page 48)
		0x0036, 0x0204, //	Gamma Control (page 48)
		0x0037, 0x0502, //	Gamma Control (page 48)
		0x003A, 0x0302, //	Gamma Control (page 48)
		0x003B, 0x0302, //	Gamma Control (page 48)
		0x0023, 0x0000, //	RAM Write data mask (page 46)
		0x0024, 0x0000, //	RAM Write data mask (page 46)
		0x0025, 0x8000, //	Frame Frequency Control (page 47)
		0x004f, 0x0000, //	RAM address set (page 50)
		0x004e, 0x0000, //	RAM address set (page 50)
	};
	init_table16(SSD1289_regValues, sizeof(SSD1289_regValues));
    invertDisplay(false);
    //setRotation(3);
    fillScreen(BLACK);
}

//	Runs all commands in initializer table of LCD Display SSD1289
void init_table16(const uint16_t table[], int16_t size) {
	int16_t i = 0;
	while (size > 0) {
		int16_t cmd = table[i];
		int16_t d = table[i+1];
		writeCmdData(cmd, d);
		size -= 2 * sizeof(int16_t);
		i += 2;
	}
}

//	Inverts screen colors (page 27)
void invertDisplay(bool i) {
    _lcd_drivOut &= ~(1 << 13);
	writeCmdData(0x01, _lcd_drivOut);
}

//	Does a reset o LCD Display
void reset() {
    done_reset = 1;
    cs_idle();
    rd_idle();
    wr_idle();
    reset_idle();
    reset_active();
    reset_idle();
}

//	Sets Address of an area in display to paint with a color
void setAddrWindow(int16_t x, int16_t y, int16_t x1, int16_t y1) {
    writeCmdData(_MC, x);
    writeCmdData(_MP, y);
    if (!(x == x1 && y == y1)) {
		if (rotation & 1)
			y1 = y = (y1 << 8) | y;
		else
			x1 = x = (x1 << 8) | x;
        writeCmdData(_SC, x);
        writeCmdData(_SP, y);
        writeCmdData(_EC, x1);
        writeCmdData(_EP, y1);
    }
}

//	Sets the cursor for draw a shape or write a text
void setCursor(int16_t x, int16_t y) {
	cursor_x = x;
	cursor_y = y;
}

//	Writes 8 bits in BSRR registers of STM32 to send to LCD
void write_8(uint8_t x) {
	uint16_t aux1, aux2;
	GPIOB->BSRR = 0x0FF0<< 16;
	aux1 = ((x) & 0x03)<<8;
	aux2 = (x) & 0xFC;
	GPIOB->BSRR = ((aux2 | aux1)<<2) & 0x0FF0;
}

//	Function to write 8 bits to LCD
void write8(uint8_t x) {
	write_8(x);
	wr_strobe();
	wr_idle();
}

//	Function to write 16 bits in two 8 bits commands
void write16(uint16_t x) {
	uint8_t h = (x)>>8;
	uint8_t l = x;
	write8(h);
	write8(l);
}

//	Send Command to LCD
void writeCmd (uint16_t cmd) {
	rs_cmd();
	write16(cmd);
	rs_data();
}

//	Send Command and Data to LCD
void writeCmdData(uint16_t cmd, uint16_t dat) {
    cs_active();
    writeCmd(cmd);
    writeData(dat);
    cs_idle();
}

//	Send Data to LCD
void writeData(uint16_t data) {
	write16(data);
}

/*
 * 	LCD Tools
 */
// Draw char using a defined font
/*
 * Parameters:
 *	int16_t x = x position on LCD
 *	int16_t y = y position on LCD
 *	unsigned char c = char to draw
 *	uint16_t color = color of text
 *	uint16_t textbgcolor = background color of char
 *	uint8_t size = font size
 */
void drawChar(int16_t x, int16_t y, unsigned char c, uint16_t color, uint16_t textbgcolor, uint8_t size) {
	for(int8_t i=0; i<8; i++) {
		for(int8_t j=0; j<5; j++) {
			if(font[c][j][7-i] == 1) {
				if (size == 1)
					drawPixel(x+j, y+i, color);
				else
					fillRect(x+j*size, y+i*size, size, size, color);
			} else {
				if (size == 1)
					drawPixel(x+j, y+i, textbgcolor);
				else
					fillRect(x+j*size, y+i*size, size, size, textbgcolor);
			}
		}
	}
}

// Draw a cross in LCD
/*
 * Parameters:
 * 	int16_t x = x position on LCD
 * 	int16_t y = y position on LCD
 * 	uint16_t color = color of cross
 * 	uint16_t textbgcolor = background color of cross
 */
void drawCross(int16_t x, int16_t y, uint16_t color, uint16_t textbgcolor) {
	for(int8_t i=0; i<15; i++) {
		for(int8_t j=0; j<15; j++) {
			if(cross_icon[j][14-i] == 1) {
				drawPixel(x+j, y+i, color);
			} else {
				drawPixel(x+j, y+i, textbgcolor);
			}
		}
	}
}

// Draw Horizontal line in LCD
/*
 * Parameters:
 * 	int16_t x = x position on LCD
 * 	int16_t y = y position on LCD
 * 	int16_t w = width of horizontal line
 * 	uint16_t color = color of horizontal line
 */
void drawHLine(int16_t x, int16_t y, int16_t w, uint16_t color) {
	for(int i = 0; i < w; i++) {
		drawPixel(x+i, y, color);
	}
}

// Draw a single pixel in LCD
/*
 * Parameters:
 * 	int16_t x = x position on LCD
 * 	int16_t y = y position on LCD
 * 	uint16_t color = color of pixel
 */
void drawPixel(int16_t x, int16_t y, uint16_t color)
{
    // MCUFRIEND just plots at edge if you try to write outside of the box:
    if (rotation & 1) {
    	if (x < 0 || y < 0 || x >= HEIGHT || y >= WIDTH)
    	        return;
    	setAddrWindow(y, x, y, x);
    } else {
    	if (x < 0 || y < 0 || x >= WIDTH || y >= HEIGHT)
    	        return;
    	setAddrWindow(x, y, x, y);
    }
    writeCmdData(_MW, color);
}

// Draw a rectangle in LCD
/*
 * Parameters:
 * 	int16_t x = x position on LCD
 * 	int16_t y = y position on LCD
 * 	int16_t w = width of rectangle
 * 	int16_t h = height of rectangle
 * 	uint16_t color = color of rectangle border
 */
void drawRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) {
	drawHLine(x, y, w, color);
	drawHLine(x, y+h-1, w, color);
	drawVLine(x, y, h, color);
	drawVLine(x+w-1, y, h, color);
}

// Draw Vertical line in LCD
/*
 * Parameters:
 * 	int16_t x = x position on LCD
 * 	int16_t y = y position on LCD
 * 	int16_t h = height of vertical line
 */
void drawVLine(int16_t x, int16_t y, int16_t h, uint16_t color) {
	for(int i = 0; i < h; i++) {
		drawPixel(x, y+i, color);
	}
}

// Fill a rectangle in LCD
/*
 * Parameters:
 * 	int16_t x = x position on LCD
 * 	int16_t y = y position on LCD
 * 	int16_t w = width of rectangle
 * 	int16_t h = height of rectangle
 * 	uint16_t color = color of rectangle
 */
void fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) {
	int16_t end;
	if (w < 0) {
		w = -w;
		x -= w;
	}                           //+ve w
	end = x + w;
	if (x < 0)
		x = 0;
	if (end > WIDTH)
		end = WIDTH;
	w = end - x;
	if (h < 0) {
		h = -h;
		y -= h;
	}                           //+ve h
	end = y + h;
	if (y < 0)
		y = 0;
	if (end > HEIGHT)
		end = HEIGHT;
	h = end - y;
	setAddrWindow(x, y, x + w - 1, y + h - 1);
	cs_active();
	writeCmd(_MW);
	if (h > w) {
		end = h;
		h = w;
		w = end;
	}
	uint8_t hi = color >> 8, lo = color & 0xFF;
	while (h-- > 0) {
		end = w;
		do {
			write8(hi);
			write8(lo);
		} while (--end != 0);
    }
    cs_idle();
}

// Fill screen in LCD
/*
 * Parameters:
 * 	uint16_t color = color of screen
 */
void fillScreen(uint16_t color) {
	fillRect(0, 0, WIDTH, HEIGHT, color);
}

// Print text in LCD
/*
 * Parameters:
 * 	int16_t x = x position on LCD
 * 	int16_t y = y position on LCD
 * 	uint16_t color = color of text
 * 	uint16_t textbgcolor = background color of text
 * 	uint8_t size = font size
 * 	char c[] = char vector to print
 */
void print(int16_t x, int16_t y, uint16_t color, uint16_t textbgcolor, uint8_t size, char c[]) {
	setCursor(x, y);
	setTextColor(color, textbgcolor);
	setTextSize(size);
	size_t n = strlen(c);
	for (size_t i = 0; i < n; i++) {
		write(c[i]);
	}
}

// Set a rotation to screen
/*
 * Parameters:
 * 	uint8_t r = (0 = Portrait, 1 = LANDSCAPE (90 degrees), 2 = PORTRAIT_REVERSE (180 degrees), 3 = LANDSCAPE_REVERSE(270 degrees))
 */
void setRotation(uint8_t r) {
	uint16_t GS, SS_v, ORG;
	uint8_t val;
    rotation = r & 3;           // just perform the operation ourselves on the protected variables
    switch (rotation) {
    case 0:                    //PORTRAIT:
        val = 0x48;             //MY=0, MX=1, MV=0, ML=0, BGR=1
        break;
    case 1:                    //LANDSCAPE: 90 degrees
        val = 0x28;             //MY=0, MX=0, MV=1, ML=0, BGR=1
    	HEIGHT = _WIDTH;
    	WIDTH = _HEIGHT;
    	val ^= 0xD0;    // exchange Landscape modes
        break;
    case 2:                    //PORTRAIT_REV: 180 degrees
        val = 0x98;             //MY=1, MX=0, MV=0, ML=1, BGR=1
        break;
    case 3:                    //LANDSCAPE_REV: 270 degrees
        val = 0xF8;             //MY=1, MX=1, MV=1, ML=1, BGR=1
    	HEIGHT = _WIDTH;
    	WIDTH = _HEIGHT;
    	val ^= 0xD0;    // exchange Landscape modes
        break;
    }

	GS = (val & 0x80) ? (1 << 14) | (1 << 12) : 0;      //called TB (top-bottom)
	SS_v = (val & 0x40) ? (1 << 9) : 0;   //called RL (right-left)
	ORG = (val & 0x20) ? (1 << 3) : 0;  //called AM
	_lcd_drivOut = GS | SS_v | 0x213F;      //BGR=0, MUX=319
	if (val & 0x08)
		_lcd_drivOut |= 0x0800; //BGR
	writeCmdData(0x01, _lcd_drivOut);   // set Driver Output Control
	writeCmdData(0x11, ORG | 0x6070);   // set GRAM write direction.

    if (rotation & 1) {
    	_MC = 0x4F;	//	Address Start RAM Set GDDRAM Y
    	_MP = 0x4E;	//	Address Start RAM Set GDDRAM X
    	_SC = 0x45;	//	Vertical RAM address
    	_EC = 0x46;	//	Vertical RAM address
    	_SP = 0x44;	//	Horizontal RAM address
    	_EP = 0x44;	//	Horizontal RAM address
    } else {
    	_MC = 0x4E;	//	Address Start RAM Set GDDRAM X
    	_MP = 0x4F;	//	Address Start RAM Set GDDRAM Y
    	_SC = 0x44;	//	Horizontal RAM address
    	_EC = 0x44;	//	Horizontal RAM address
    	_SP = 0x45;	//	Vertical RAM address
    	_EP = 0x46;	//	Vertical RAM address
    }
    setAddrWindow(0, 0, WIDTH - 1, HEIGHT - 1);
    vertScroll(0, HEIGHT, 0);   //reset scrolling after a rotation
}

// Set actual text color for write
/*
 * Parameters:
 * 	uint16_t color = color of text
 * 	uint16_t txtbgcolor = background color of text
 */
void setTextColor(uint16_t color, uint16_t txtbgcolor) {
	textcolor = color;
	textbgcolor = txtbgcolor;
}

// Set actual text size for write
/*
 * Parameters:
 * 	uint8_t size = font size
 */
void setTextSize(uint8_t size) {
	textsize = (size > 0) ? size : 1;
}

// Does a vertical scroll in LCD
/*
 * Parameters:
 * 	int16_t top = vertical start pixel position
 * 	int16_t scrollines = number of lines to scroll verticaly
 * 	int16_t offset = offset of vertical star pixel position
 */
void vertScroll (int16_t top, int16_t scrollines, int16_t offset) {
    int16_t vsp;
    if (offset <= -scrollines || offset >= scrollines) offset = 0; //valid scroll
    vsp = top + offset; // vertical start position
	if (offset < 0)
		vsp += scrollines;
	writeCmdData(0x41, vsp);
}

// Write a char in LCD screen using actual settings of text
/*
 * Parameters:
 * 	unsigned char c = char to write
 */
void write(unsigned char c) {
    if(c == '\n') {                        // Newline?
        cursor_x  = 0;                     // Reset x to zero,
        cursor_y += textsize * 8;          // advance y one line
    } else if(c != '\r') {                 // Ignore carriage returns
        if(wrap && ((cursor_x + textsize * 6) > WIDTH)) { // Off right?
            cursor_x  = 0;                 // Reset x to zero,
            cursor_y += textsize * 8;      // advance y one line
        }
        drawChar(cursor_x, cursor_y, c, textcolor, textbgcolor, textsize);
        cursor_x += textsize * 6;          // Advance x one char
    }
}

/*
 * 	GPIO Handlers
 */
//	Initialize all GPIOs used to LCD Control
void ctl_init() {
	rd_output();
	wr_output();
	rd_output();
	cs_output();
	reset_output();
}

//	Makes CS PIN in LCD in ACTIVE state (RESET in STM32)
void cs_active() {
	HAL_GPIO_WritePin(CS_PORT, CS_PIN, GPIO_PIN_RESET);
}

//	Makes CS PIN in LCD in IDLE state (SET in STM32)
void cs_idle() {
	HAL_GPIO_WritePin(CS_PORT, CS_PIN, GPIO_PIN_SET);
}

//	Makes CS PIN STM32 in OUTPUT mode
void cs_output() {
	  GPIO_InitTypeDef GPIO_InitStruct;
	  GPIO_InitStruct.Pin = CS_PIN;
	  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	  GPIO_InitStruct.Pull = GPIO_NOPULL;
	  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	  HAL_GPIO_Init(CS_PORT, &GPIO_InitStruct);
}

//	Makes RD PIN in LCD in ACTIVE state (RESET in STM32)
void rd_active() {
	HAL_GPIO_WritePin(RD_PORT, RD_PIN, GPIO_PIN_RESET);
}

//	Makes RD PIN in LCD in IDLE state (SET in STM32)
void rd_idle() {
	HAL_GPIO_WritePin(RD_PORT, RD_PIN, GPIO_PIN_SET);
}

//	Makes RD PIN STM32 in OUTPUT mode
void rd_output() {
	  GPIO_InitTypeDef GPIO_InitStruct;
	  GPIO_InitStruct.Pin = RD_PIN;
	  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	  GPIO_InitStruct.Pull = GPIO_NOPULL;
	  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	  HAL_GPIO_Init(RD_PORT, &GPIO_InitStruct);
}

//	Makes RESET PIN in LCD in ACTIVE state (RESET in STM32)
void reset_active() {
	HAL_GPIO_WritePin(RST_PORT, RST_PIN, GPIO_PIN_RESET);
}

//	Makes RESET PIN in LCD in IDLE state (SET in STM32)
void reset_idle() {
	HAL_GPIO_WritePin(RST_PORT, RST_PIN, GPIO_PIN_SET);
}

//	Makes RESET PIN STM32 in OUTPUT mode
void reset_output() {
	  GPIO_InitTypeDef GPIO_InitStruct;
	  GPIO_InitStruct.Pin = RST_PIN;
	  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	  GPIO_InitStruct.Pull = GPIO_NOPULL;
	  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	  HAL_GPIO_Init(RST_PORT, &GPIO_InitStruct);
}

//	Makes RS PIN STM32 in ANALOG INPUT mode
void rs_analog_input() {
	  GPIO_InitTypeDef GPIO_InitStruct;
	  GPIO_InitStruct.Pin = RS_PIN;
	  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
	  HAL_GPIO_Init(RS_PORT, &GPIO_InitStruct);
}

//	Makes RS PIN in LCD in CMD state (RESET in STM32)
void rs_cmd() {
	HAL_GPIO_WritePin(RS_PORT, RS_PIN, GPIO_PIN_RESET);
}

//	Makes RS PIN in LCD in DATA state (SET in STM32)
void rs_data() {
	HAL_GPIO_WritePin(RS_PORT, RS_PIN, GPIO_PIN_SET);
}

//	Makes RS PIN STM32 in OUTPUT mode
void rs_output() {
	  GPIO_InitTypeDef GPIO_InitStruct;
	  GPIO_InitStruct.Pin = RS_PIN;
	  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	  GPIO_InitStruct.Pull = GPIO_NOPULL;
	  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	  HAL_GPIO_Init(RS_PORT, &GPIO_InitStruct);
}

//	Makes WR PIN in LCD in ACTIVE state (RESET in STM32)
void wr_active() {
	HAL_GPIO_WritePin(WR_PORT, WR_PIN, GPIO_PIN_RESET);
}

//	Makes WR PIN STM32 in ANALOG INPUT mode
void wr_analog_input() {
	  GPIO_InitTypeDef GPIO_InitStruct;
	  GPIO_InitStruct.Pin = WR_PIN;
	  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
	  HAL_GPIO_Init(WR_PORT, &GPIO_InitStruct);
}

//	Makes WR PIN in LCD in IDLE state (SET in STM32)
void wr_idle() {
	HAL_GPIO_WritePin(WR_PORT, WR_PIN, GPIO_PIN_SET);
}

//	Makes WR PIN STM32 in OUTPUT mode
void wr_output() {
	  GPIO_InitTypeDef GPIO_InitStruct;
	  GPIO_InitStruct.Pin = WR_PIN;
	  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	  GPIO_InitStruct.Pull = GPIO_NOPULL;
	  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	  HAL_GPIO_Init(WR_PORT, &GPIO_InitStruct);
}

//	Does a Strobe in WR PIN in LCD (RESET followed by a SET in STM32)
void wr_strobe() {
	wr_active();
	wr_idle();
}
