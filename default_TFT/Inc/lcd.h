/*
 * lcd.h
 *
 *  Created on: 14 de jan de 2019
 *      Author: Inspiron
 *
 *      The code was created based on MCU_Friend KBV code
 *      for Arduino Board and following the manual of SSD1289
 *      See manual in:
 *      https://github.com/erickfunier/STM32_Display_TFT/blob/default/SSD1289_v1.1.pdf
 *
 */

#ifndef LCD_H_
#define LCD_H_

#include <stdint.h>
#include <stdbool.h>

// Pinouts referenced by LCD to GPI Port and Pin
#define CS_PORT	GPIOA		// CS_PIN -> PA3
#define CS_PIN	GPIO_PIN_3	// CS_PIN -> PA3
#define RS_PORT GPIOA		// RS_PIN -> PA2
#define RS_PIN GPIO_PIN_2	// RS_PIN -> PA2
#define WR_PORT GPIOA		// WR_PIN -> PA1
#define WR_PIN GPIO_PIN_1	// WR_PIN -> PA1
#define RD_PORT GPIOA		// RD_PIN -> PA0
#define RD_PIN GPIO_PIN_0	// RD_PIN -> PA0
#define RST_PORT GPIOA		// RST_PIN -> PA4
#define RST_PIN GPIO_PIN_4	// RST_PIN -> PA4

// Color definitions for used with LCD
#define BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF

// Commands to LCD control
#define _MW		0x22 // Write Data to GRAM LCD

// Dimension default for display, this dimension not change with display rotation
#define _WIDTH 240;
#define _HEIGHT 320;

bool wrap;	//	Flag to signal if is necessary wrap text

uint8_t rotation;		//	Actual rotation of LCD
uint8_t textsize;		//	Actual textsize for write in LCD
uint8_t done_reset;		//	Flag to signal if the LCD finished reset command
uint16_t _lcd_drivOut; 	//	Variable with LCD Driver Output Control
uint16_t WIDTH, HEIGHT;	// 	Actual Width and Height
uint16_t textcolor;		//	Actual text color to write
uint16_t textbgcolor;	//	Actual Background of text area

int16_t cursor_x;		//	Position of X cursor
int16_t cursor_y;		//	Position of Y cursor

// LCD Handlers
void startLCD();	// Start LCD with size W=240 and H=320
void begin(uint16_t ID);	// Start LCD display hardware
void init_table16(const uint16_t table[], int16_t size);	//	Runs all commands in initializer table of LCD Display SSD1289
void invertDisplay(bool i);	// Inverts display color
void reset();	// Does a reset o LCD Display
void setAddrWindow(int16_t x, int16_t y, int16_t x1, int16_t y1);	// Sets Address of an area in display to paint with a color
void setCursor(int16_t x, int16_t y);	// Sets the cursor for draw a shape or write a text
void write_8(uint8_t x);	// Writes 8 bits in BSRR registers of STM32 to send to LCD
void write8(uint8_t x);		// Function to write 8 bits to LCD
void write16(uint16_t x);	// Function to write 16 bits in two 8 bits commands
void writeCmd (uint16_t cmd);	// Send Command to LCD
void writeCmdData(uint16_t cmd, uint16_t dat);	// Send Command and Data to LCD
void writeData(uint16_t data);	// Send Data to LCD

// LCD Tools
void drawChar(int16_t x, int16_t y, unsigned char c, uint16_t color, uint16_t bg_color, uint8_t size); // Draw char using a defined font
void drawCross(int16_t x, int16_t y, uint16_t color, uint16_t textbgcolor);	// Draw a cross in LCD
void drawHLine(int16_t x, int16_t y, int16_t w, uint16_t color);	// Draw Horizontal line in LCD
void drawPixel(int16_t x, int16_t y, uint16_t color);	// Draw a single pixel in LCD
void drawRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);	// Draw a rectangle in LCD
void drawVLine(int16_t x, int16_t y, int16_t h, uint16_t color);	// Draw Vertical line in LCD
void fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);	// Fill a rectangle in LCD
void fillScreen(uint16_t color);	// Fill screen in LCD
void print(int16_t x, int16_t y, uint16_t color, uint16_t textbgcolor, uint8_t size, char c[]);	// Print text in LCD
void setRotation(uint8_t r);	// Set a rotation to screen
void setTextColor(uint16_t color, uint16_t txtbgcolor);	// Set actual text color for write
void setTextSize(uint8_t size);	// Set actual text size for write
void vertScroll (int16_t top, int16_t scrollines, int16_t offset);	// Does a vertical scroll in LCD
void write(unsigned char c);	// Write a char in LCD screen using actual settings of text

// GPIO Handlers
void ctl_init();		// Initialize all GPIOs used to LCD Control
void cs_active();		// Makes CS PIN in LCD in ACTIVE state (RESET in STM32)
void cs_idle();			// Makes CS PIN in LCD in IDLE state (SET in STM32)
void cs_output();		// Makes CS PIN STM32 in OUTPUT mode
void rd_active();		// Makes RD PIN in LCD in ACTIVE state (RESET in STM32)
void rd_idle();			// Makes RD PIN in LCD in IDLE state (SET in STM32)
void rd_output();		// Makes RD PIN STM32 in OUTPUT mode
void reset_active();	// Makes RESET PIN in LCD in ACTIVE state (RESET in STM32)
void reset_idle();		// Makes RESET PIN in LCD in IDLE state (SET in STM32)
void reset_output();	// Makes RESET PIN STM32 in OUTPUT mode
void rs_analog_input();	// Makes RS PIN STM32 in ANALOG INPUT mode
void rs_cmd();			// Makes RS PIN in LCD in CMD state (RESET in STM32)
void rs_data();			// Makes RS PIN in LCD in DATA state (SET in STM32)
void rs_output();		// Makes RS PIN STM32 in OUTPUT mode
void wr_active();		// Makes WR PIN in LCD in ACTIVE state (RESET in STM32)
void wr_analog_input();	// Makes WR PIN STM32 in ANALOG INPUT mode
void wr_idle();			// Makes WR PIN in LCD in IDLE state (SET in STM32)
void wr_output();		// Makes WR PIN STM32 in OUTPUT mode
void wr_strobe();		// Does a Strobe in WR PIN in LCD (RESET followed by a SET in STM32)

#endif /* LCD_H_ */
