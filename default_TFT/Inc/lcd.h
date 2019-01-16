/*
 * lcd.h
 *
 *  Created on: 14 de jan de 2019
 *      Author: Inspiron
 */

#ifndef LCD_H_
#define LCD_H_

#include <stdint.h>
#include <stdbool.h>

// Declaracao dos Pinos e GPIOs utilizadas
#define CS_PORT	GPIOA
#define CS_PIN	GPIO_PIN_3
#define RS_PORT GPIOA
#define RS_PIN GPIO_PIN_2
#define WR_PORT GPIOA
#define WR_PIN GPIO_PIN_1
#define RD_PORT GPIOA
#define RD_PIN GPIO_PIN_0
#define RST_PORT GPIOA
#define RST_PIN GPIO_PIN_4

#define MIPI_DCS_REV1   (1<<0)
#define XSA_XEA_16BIT   (1<<5)
#define REV_SCREEN      (1<<12)
#define MV_AXIS         (1<<10)

// Definicao de cores para o Display
#define BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF

#define _WIDTH 240;
#define _HEIGHT 320;

bool wrap;

uint8_t rotation;
uint8_t textsize;
uint8_t done_reset;

uint16_t _lcd_ID;
uint16_t _lcd_xor;
uint16_t _lcd_capable;
uint16_t _lcd_rev;
uint16_t _lcd_drivOut;
uint16_t WIDTH, HEIGHT;
uint16_t _MW;
uint16_t _MC;
uint16_t _MP;
uint16_t _SC;
uint16_t _EC;
uint16_t _SP;
uint16_t _EP;
uint16_t textcolor;
uint16_t textbgcolor;

int16_t cursor_x;
int16_t cursor_y;
int16_t _width;
int16_t _height;

void startLCD();
void begin(uint16_t ID);
void invertDisplay(bool i);
void fillScreen(uint16_t color);
void fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
void setAddrWindow(int16_t x, int16_t y, int16_t x1, int16_t y1);
void cs_active();
void writeCmd (uint16_t cmd);
void write8(uint8_t x);
void cs_idle();
void write16(uint16_t x);
void rs_data();
void write_8(uint8_t x);
void wr_strobe();
void writeCmdData(uint16_t cmd, uint16_t dat);
void init_table16(const uint16_t table[], int16_t size);
void drawPixel(int16_t x, int16_t y, uint16_t color);
void setRotation(uint8_t r);
void vertScroll (int16_t top, int16_t scrollines, int16_t offset);
void drawChar(uint16_t WIDTH, uint16_t HEIGHT, int16_t x, int16_t y, unsigned char c, uint16_t color, uint16_t bg_color, uint8_t size);
void wr_output();
void rs_output();
void reset();
void setCursor(int16_t x, int16_t y);
void rd_idle();
void reset_idle();
void reset_active();
void setTextColor(uint16_t color, uint16_t txtbgcolor);
void setTextSize(uint8_t size);
void drawRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
void write(uint8_t c);
void print(int16_t x, int16_t y, uint16_t color, uint16_t txtbgcolor, uint8_t size, char c[]);
void drawHLine(int16_t x, int16_t y, int16_t w, uint16_t color);
void drawVLine(int16_t x, int16_t y, int16_t h, uint16_t color);
void drawCross(int16_t x, int16_t y, uint16_t color, uint16_t textbgcolor);
void rd_active();
void wr_active();
void wr_idle();
void rs_cmd();
void rd_output();
void wr_analog_input();
void rs_analog_input();
void cs_output();
void reset_output();
void ctl_init();
void writeData(uint16_t data);


#endif /* LCD_H_ */
