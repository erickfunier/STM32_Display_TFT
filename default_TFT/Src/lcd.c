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

void begin(uint16_t ID) {
	_MW = 0x22;
	_MC = 0x4E;
	_MP = 0x4F;
	_SC = 0x44;
	_EC = 0x44;
	_SP = 0x45;
	_EP = 0x46;

	wr_output();
	rs_output();
    reset();
    _lcd_xor = 0;
	_lcd_capable = 0 | XSA_XEA_16BIT | REV_SCREEN;
	// came from MikroElektronika library http://www.hmsprojects.com/tft_lcd.html
	static const uint16_t SSD1289_regValues[] = {
		0x0000, 0x0001,	//Oscilador do Display. Se 0x0001 (Oscilador ligado), se 0x0000 (Oscilador desligado) (page 27)
		0x0003, 0xA8A4, //Power Control (1) LCD - 0xA8A4 = 1010 1000 1010 0100 (page 34)
		0x000C, 0x0000, //Power Control (2) LCD - 0x0000 (page 38)
		0x000D, 0x080C, //Power Control (3) LCD - 0x000C = 0000 0000 0000 1100 (page 39)    // was 0x800C or 0x080C
		0x000E, 0x2B00, //Power Control (4) LCD - 0x2B00 = 0010 1011 0000 0000 (page 39)
		0x001E, 0x00B7, //Power Control (5) LCD - 0x00B7 = 0000 0000 1011 0111 (page 46)
		0x0001, 0x2B3F, //Driver Output Control - 0x2B3F = 0010 1011 0011 1111 (page 27)  // was 0x2B3F,
		0x0002, 0x0400, //LCD-Driving-Waveform Control - 0x0400 = 0000 0100 0000 0000 (page 31)    // was 0x0600 or 0x0400
		0x0010, 0x0000, //Sleep Mode - 0x0000. Se 0x0000 não entra em Sleep, se 0x0001 entra em modo sleep (page 40)
		0x0011, 0x6070, //Entry Mode - 0x6070 = 0110 0000 0111 0000 (page 40)   // was 0x6070
		0x0005, 0x0000, //Compare Register Red and Green (page 36)
		0x0006, 0x0000, //Compare Register Blue (page 36)
		0x0016, 0xEF1C, //Horizontal Porch 0xEF1C = 1110 1111 0001 1100 (page 44)
		0x0017, 0x0003, //Vertical Porch 0x0003 = 0000 0000 0000 0011 (page 45)
		0x0007, 0x0233, //Display Control 0x0233 = 0000 0010 0011 0011 (page 36) Tela estranha
		0x000B, 0x0000, //Frame Cycle Control (page 36)
		0x000F, 0x0000, //Gate Scan Position (page 40)
		0x0041, 0x0000, //Vertical Scroll Control (page 49)
		0x0042, 0x0000, //Vertical Scroll Control (page 49)
		0x0048, 0x0000, //First Screen Driving Position (page 49)
		0x0049, 0x013F, //First Screen Driving Position (page 49)
		0x004A, 0x0000, //Second Screen Driving Position (page 50)
		0x004B, 0x0000, //Second Screen Driving Position (page 50)
		0x0044, 0xEF95, //Horizontal RAM address position (page 49)
		0x0045, 0x0000, //Vertical RAM address position (page 49)
		0x0046, 0x013F, //Vertical RAM address position (page 49)
		0x0030, 0x0707, //Gamma Control (page 48)
		0x0031, 0x0204, //Gamma Control (page 48)
		0x0032, 0x0204, //Gamma Control (page 48)
		0x0033, 0x0502, //Gamma Control (page 48)
		0x0034, 0x0507, //Gamma Control (page 48)
		0x0035, 0x0204, //Gamma Control (page 48)
		0x0036, 0x0204, //Gamma Control (page 48)
		0x0037, 0x0502, //Gamma Control (page 48)
		0x003A, 0x0302, //Gamma Control (page 48)
		0x003B, 0x0302, //Gamma Control (page 48)
		0x0023, 0x0000, //RAM Write data mask (page 46)
		0x0024, 0x0000, //RAM Write data mask (page 46)
		0x0025, 0x8000, //Frame Frequency Control (page 47)
		0x004f, 0x0000, //RAM address set (page 50)
		0x004e, 0x0000, //RAM address set (page 50)
	};
	init_table16(SSD1289_regValues, sizeof(SSD1289_regValues));
	_lcd_rev = ((_lcd_capable & REV_SCREEN) != 0);
    //setRotation(0);             //PORTRAIT
    invertDisplay(false);
    setRotation(3);
    fillScreen(GREEN);
}

void invertDisplay(bool i) {
    _lcd_rev = ((_lcd_capable & REV_SCREEN) != 0) ^ i;
    _lcd_drivOut &= ~(1 << 13);
	if (_lcd_rev)
		_lcd_drivOut |= (1 << 13);
	writeCmdData(0x01, _lcd_drivOut);
}

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
    if (!(_lcd_capable & MIPI_DCS_REV1) || ((_lcd_ID == 0x1526) && (rotation & 1)))
        setAddrWindow(0, 0, WIDTH - 1, HEIGHT - 1);
}

void fillScreen(uint16_t color) {
	fillRect(0, 0, WIDTH, HEIGHT, color);
}

void setAddrWindow(int16_t x, int16_t y, int16_t x1, int16_t y1) {
    writeCmdData(_MC, x);
    writeCmdData(_MP, y);
    if (!(x == x1 && y == y1)) {  //only need MC,MP for drawPixel
        if (_lcd_capable & XSA_XEA_16BIT) {
            if (rotation & 1)
                y1 = y = (y1 << 8) | y;
            else
                x1 = x = (x1 << 8) | x;
        }
        writeCmdData(_SC, x);
        writeCmdData(_SP, y);
        writeCmdData(_EC, x1);
        writeCmdData(_EP, y1);
    }
}

void cs_idle() {
	HAL_GPIO_WritePin(CS_PORT, CS_PIN, GPIO_PIN_SET);
}

void cs_active() {
	HAL_GPIO_WritePin(CS_PORT, CS_PIN, GPIO_PIN_RESET);
}

void writeCmd (uint16_t cmd) {
	rs_cmd();
	write16(cmd);
	rs_data();
}

void write8(uint8_t x) {
	write_8(x);
	wr_strobe();
	wr_idle();
}

void write16(uint16_t x) {
	uint8_t h = (x)>>8;
	uint8_t l = x;
	write8(h);
	write8(l);
}

void rs_data() {
	HAL_GPIO_WritePin(RS_PORT, RS_PIN, GPIO_PIN_SET);
}

void write_8(uint8_t x) {
	uint16_t aux1, aux2;
	GPIOB->BSRR = 0x0FF0<< 16;
	aux1 = ((x) & 0x03)<<8;
	aux2 = (x) & 0xFC;
	GPIOB->BSRR = ((aux2 | aux1)<<2) & 0x0FF0;
}

void wr_strobe() {
	wr_active();
	wr_idle();
}

void writeCmdData(uint16_t cmd, uint16_t dat) {
    cs_active();
    writeCmd(cmd);
    writeData(dat);
    cs_idle();
}

void init_table16(const uint16_t table[], int16_t size) {
	int16_t i = 0;
	while (size > 0) {
		int16_t cmd = table[i];
		int16_t d = table[i+1];
		writeCmdData(cmd, d);                      //static function
		size -= 2 * sizeof(int16_t);
		i += 2;
	}
}

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

void setRotation(uint8_t r) {
	uint16_t GS, SS_v, ORG, REV = _lcd_rev;
	uint8_t val;
    rotation = r & 3;           // just perform the operation ourselves on the protected variables
    if (rotation & 1) {
    	HEIGHT = _WIDTH;
    	WIDTH = _HEIGHT;
    }
    switch (rotation) {
    case 0:                    //PORTRAIT:
        val = 0x48;             //MY=0, MX=1, MV=0, ML=0, BGR=1
        break;
    case 1:                    //LANDSCAPE: 90 degrees
        val = 0x28;             //MY=0, MX=0, MV=1, ML=0, BGR=1
        break;
    case 2:                    //PORTRAIT_REV: 180 degrees
        val = 0x98;             //MY=1, MX=0, MV=0, ML=1, BGR=1
        break;
    case 3:                    //LANDSCAPE_REV: 270 degrees
        val = 0xF8;             //MY=1, MX=1, MV=1, ML=1, BGR=1
        break;
    }
	_MC = 0x4E, _MP = 0x4F, _MW = 0x22, _SC = 0x44, _EC = 0x44, _SP = 0x45, _EP = 0x46;
    if (rotation & 1)
    	val ^= 0xD0;    // exchange Landscape modes
	GS = (val & 0x80) ? (1 << 14) | (1 << 12) : 0;      //called TB (top-bottom)
	SS_v = (val & 0x40) ? (1 << 9) : 0;   //called RL (right-left)
	ORG = (val & 0x20) ? (1 << 3) : 0;  //called AM
	_lcd_drivOut = GS | SS_v | (REV << 13) | 0x013F;      //REV=0, BGR=0, MUX=319
	if (val & 0x08)
		_lcd_drivOut |= 0x0800; //BGR
	writeCmdData(0x01, _lcd_drivOut);   // set Driver Output Control
	writeCmdData(0x11, ORG | 0x6070);   // set GRAM write direction.

    if ((rotation & 1) && ((_lcd_capable & MV_AXIS) == 0)) {
        uint16_t x;
        x = _MC, _MC = _MP, _MP = x;
        x = _SC, _SC = _SP, _SP = x;    //.kbv check 0139
        x = _EC, _EC = _EP, _EP = x;    //.kbv check 0139
    }
    setAddrWindow(0, 0, WIDTH - 1, HEIGHT - 1);
    vertScroll(0, HEIGHT, 0);   //reset scrolling after a rotation
}

void vertScroll (int16_t top, int16_t scrollines, int16_t offset) {
    int16_t vsp;
    if (offset <= -scrollines || offset >= scrollines) offset = 0; //valid scroll
    vsp = top + offset; // vertical start position
	if (offset < 0)
		vsp += scrollines;          //keep in unsigned range
	writeCmdData(0x41, vsp);        //VL#
}

void drawChar(uint16_t WIDTH, uint16_t HEIGHT, int16_t x, int16_t y, unsigned char c, uint16_t color, uint16_t textbgcolor, uint8_t size) {
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

void wr_output() {
	  GPIO_InitTypeDef GPIO_InitStruct;
	  GPIO_InitStruct.Pin = WR_PIN;
	  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	  GPIO_InitStruct.Pull = GPIO_NOPULL;
	  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	  HAL_GPIO_Init(WR_PORT, &GPIO_InitStruct);
}

void rs_output() {
	  GPIO_InitTypeDef GPIO_InitStruct;
	  GPIO_InitStruct.Pin = RS_PIN;
	  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	  GPIO_InitStruct.Pull = GPIO_NOPULL;
	  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	  HAL_GPIO_Init(RS_PORT, &GPIO_InitStruct);
}

void reset() {
    done_reset = 1;
    cs_idle();
    rd_idle();
    wr_idle();
    reset_idle();
    reset_active();
    reset_idle();
}

void rd_idle() {
	HAL_GPIO_WritePin(RD_PORT, RD_PIN, GPIO_PIN_SET);
}

void reset_active() {
	HAL_GPIO_WritePin(RST_PORT, RST_PIN, GPIO_PIN_RESET);
}

void reset_idle() {
	HAL_GPIO_WritePin(RST_PORT, RST_PIN, GPIO_PIN_SET);
}

void setCursor(int16_t x, int16_t y) {
	cursor_x = x;
	cursor_y = y;
}

void setTextColor(uint16_t color, uint16_t txtbgcolor) {
	textcolor = color;
	textbgcolor = txtbgcolor;
}

void setTextSize(uint8_t size) {
	textsize = (size > 0) ? size : 1;
}

void drawRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) {
	drawHLine(x, y, w, color);
	drawHLine(x, y+h-1, w, color);
	drawVLine(x, y, h, color);
	drawVLine(x+w-1, y, h, color);
}

void write(uint8_t c) {
    if(c == '\n') {                        // Newline?
        cursor_x  = 0;                     // Reset x to zero,
        cursor_y += textsize * 8;          // advance y one line
    } else if(c != '\r') {                 // Ignore carriage returns
        if(wrap && ((cursor_x + textsize * 6) > _width)) { // Off right?
            cursor_x  = 0;                 // Reset x to zero,
            cursor_y += textsize * 8;      // advance y one line
        }
        drawChar(WIDTH, HEIGHT, cursor_x, cursor_y, c, textcolor, textbgcolor, textsize);
        cursor_x += textsize * 6;          // Advance x one char
    }
}

void print(int16_t x, int16_t y, uint16_t color, uint16_t txtbgcolor, uint8_t size, char c[]) {
	setCursor(x, y);
	setTextColor(color, txtbgcolor);
	setTextSize(size);
	size_t n = strlen(c);
	for (size_t i = 0; i < n; i++) {
		write(c[i]);
	}
}

void drawHLine(int16_t x, int16_t y, int16_t w, uint16_t color) {
	for(int i = 0; i < w; i++) {
		drawPixel(x+i, y, color);
	}
}

void drawVLine(int16_t x, int16_t y, int16_t h, uint16_t color) {
	for(int i = 0; i < h; i++) {
		drawPixel(x, y+i, color);
	}
}

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

void rd_active() {
	HAL_GPIO_WritePin(RD_PORT, RD_PIN, GPIO_PIN_RESET);
}

void wr_active() {
	HAL_GPIO_WritePin(WR_PORT, WR_PIN, GPIO_PIN_RESET);
}

void wr_idle() {
	HAL_GPIO_WritePin(WR_PORT, WR_PIN, GPIO_PIN_SET);
}

void rs_cmd() {
	HAL_GPIO_WritePin(RS_PORT, RS_PIN, GPIO_PIN_RESET);
}

void rd_output() {
	  GPIO_InitTypeDef GPIO_InitStruct;
	  GPIO_InitStruct.Pin = RD_PIN;
	  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	  GPIO_InitStruct.Pull = GPIO_NOPULL;
	  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	  HAL_GPIO_Init(RD_PORT, &GPIO_InitStruct);
}

void wr_analog_input() {
	  GPIO_InitTypeDef GPIO_InitStruct;
	  GPIO_InitStruct.Pin = WR_PIN;
	  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
	  HAL_GPIO_Init(WR_PORT, &GPIO_InitStruct);
}

void rs_analog_input() {
	  GPIO_InitTypeDef GPIO_InitStruct;
	  GPIO_InitStruct.Pin = RS_PIN;
	  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
	  HAL_GPIO_Init(RS_PORT, &GPIO_InitStruct);
}

void startLCD() {
	WIDTH = 240;
	HEIGHT = 320;
	begin(0x1289);
}

void cs_output() {
	  GPIO_InitTypeDef GPIO_InitStruct;
	  GPIO_InitStruct.Pin = CS_PIN;
	  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	  GPIO_InitStruct.Pull = GPIO_NOPULL;
	  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	  HAL_GPIO_Init(CS_PORT, &GPIO_InitStruct);
}

void reset_output() {
	  GPIO_InitTypeDef GPIO_InitStruct;
	  GPIO_InitStruct.Pin = RST_PIN;
	  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	  GPIO_InitStruct.Pull = GPIO_NOPULL;
	  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	  HAL_GPIO_Init(RST_PORT, &GPIO_InitStruct);
}

void ctl_init() {
	rd_output();
	wr_output();
	rd_output();
	cs_output();
	reset_output();
}

void writeData(uint16_t data) {
	write16(data);
}
