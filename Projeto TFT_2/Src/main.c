
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  ** This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * COPYRIGHT(c) 2018 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f1xx_hal.h"

/* USER CODE BEGIN Includes */
#include "string.h"
#include "stdio.h"
#include "stdbool.h"
#include "font_ascII_256.h"
#include <math.h>
/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/
ADC_HandleTypeDef hadc1;
DMA_HandleTypeDef hdma_adc1;

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/
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

// Variaveis utilizadas para exibicao do display
#define TFTLCD_DELAY 0xFFFF
#define TFTLCD_DELAY8 0x7F
#define MIPI_DCS_REV1   (1<<0)
#define AUTO_READINC    (1<<1)
#define READ_BGR        (1<<2)
#define READ_LOWHIGH    (1<<3)
#define READ_24BITS     (1<<4)
#define XSA_XEA_16BIT   (1<<5)
#define READ_NODUMMY    (1<<6)
#define INVERT_GS       (1<<8)
#define INVERT_SS       (1<<9)
#define MV_AXIS         (1<<10)
#define INVERT_RGB      (1<<11)
#define REV_SCREEN      (1<<12)
#define FLIP_VERT       (1<<13)
#define FLIP_HORIZ      (1<<14)
#define _swap_int16_t(a, b) { int16_t t = a; a = b; b = t; }
uint16_t _lcd_ID, _lcd_rev, _lcd_madctl, _lcd_drivOut, _MC = 0x4E, _MP = 0x4F, _MW = 0x22, _SC = 0x44, _EC = 0x44, _SP = 0x45, _EP = 0x46, _lcd_capable, _lcd_xor;
uint8_t rotation, textsize;
uint16_t aux1, aux2;
uint16_t WIDTH = 240, HEIGHT = 320;
int16_t cursor_x, cursor_y, _width, _height;
uint16_t textcolor, textbgcolor;
int s1 = 1;
bool wrap, _cp437;
static uint8_t done_reset;

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

#define LCD_D0_PORT GPIOB
#define LCD_D0_PIN GPIO_PIN_8
#define LCD_D1_PORT GPIOB
#define LCD_D1_PIN GPIO_PIN_9
#define LCD_D2_PORT GPIOB
#define LCD_D2_PIN GPIO_PIN_15
#define LCD_D3_PORT GPIOB
#define LCD_D3_PIN GPIO_PIN_14
#define LCD_D4_PORT GPIOB
#define LCD_D4_PIN GPIO_PIN_13
#define LCD_D5_PORT GPIOB
#define LCD_D5_PIN GPIO_PIN_12
#define LCD_D6_PORT GPIOB
#define LCD_D6_PIN GPIO_PIN_11
#define LCD_D7_PORT GPIOB
#define LCD_D7_PIN GPIO_PIN_10

#define MINPRESSURE 3000
#define MAXPRESSURE 3800

#define PI 3.14159265

uint32_t ADC_BUF[2];
int touchx_atual = 0;
int touchy_atual = 0;
int touchz_atual = 0;
int flag_adc = 1;
int teste = 0;
int eixo_y_plus = 0;
int eixo_y_minus = 20;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_ADC1_Init(void);

/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
void begin(uint16_t ID);
void invertDisplay(bool i);
void init_table16(const uint16_t table[], int16_t size);
void fillScreen(uint16_t color);
void testDrawScreen();
void setRotation(uint8_t r);
void vertScroll (int16_t top, int16_t scrollines, int16_t offset);
void writeCmdData(uint16_t cmd, uint16_t dat);
void setAddrWindow(int16_t x, int16_t y, int16_t x1, int16_t y1);
void cs_active();
void cs_idle();
void rs_cmd();
void rs_data();
void wr_active();
void wr_idle();
void wr_strobe();
void rd_idle();
void reset_idle();
void reset_active();
void write8(uint8_t x);
void write16(uint16_t x);
void write_8(uint8_t x);
void writeCmd (uint16_t cmd);
void writeData(uint16_t data);
void reset();
void setCursor(int16_t x, int16_t y);
void setTextColor(uint16_t color, uint16_t txtbgcolor);
void setTextSize(uint8_t s);
void print(char c[]);
void write(uint8_t c);
void drawRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
void drawPixel(int16_t x, int16_t y, uint16_t color);
void drawHLine(int16_t x, int16_t y, int16_t w, uint16_t color);
void drawVLine(int16_t x, int16_t y, int16_t h, uint16_t color);
void readTouch();
void wr_output();
void wr_analog_input();
void readTouchX();
void readTouchY();
void rs_analog_input();
void rs_output();
void drawChar(uint16_t WIDTH, uint16_t HEIGHT, int16_t x, int16_t y, unsigned char c, uint16_t color, uint16_t bg_color, uint8_t size);
void fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);

/* USER CODE END PFP */

/* USER CODE BEGIN 0 */
uint32_t val_adc1; // valor lido no conv ADC
uint32_t val_adc2; // valor lido no conv ADC
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)	{
	if(hadc->Instance == ADC1)	{
		val_adc1 = ADC_BUF[0];
		val_adc2 = ADC_BUF[1];
		flag_adc = 0;
	}
}

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  *
  * @retval None
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration----------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_ADC1_Init();
  /* USER CODE BEGIN 2 */
  begin(0x1289);
  HAL_ADC_Start_DMA(&hadc1,(uint32_t*)ADC_BUF,2);
  HAL_ADC_Start_IT(&hadc1);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {

  /* USER CODE END WHILE */

  /* USER CODE BEGIN 3 */
	  //fillScreen(GREEN);
	   //calibrateTouch();
	  testDrawScreen();
	  readTouch();
	  //drawPixel(40, 10, RED);
	  //drawPixel(80, 20, RED);
	  //drawPixel(209, 299, RED);

  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_PeriphCLKInitTypeDef PeriphClkInit;

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC;
  PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV6;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Configure the Systick interrupt time 
    */
  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

    /**Configure the Systick 
    */
  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

/* ADC1 init function */
static void MX_ADC1_Init(void)
{

  ADC_ChannelConfTypeDef sConfig;

    /**Common config 
    */
  hadc1.Instance = ADC1;
  hadc1.Init.ScanConvMode = ADC_SCAN_ENABLE;
  hadc1.Init.ContinuousConvMode = DISABLE;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.NbrOfConversion = 2;
  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Configure Regular Channel 
    */
  sConfig.Channel = ADC_CHANNEL_1;
  sConfig.Rank = ADC_REGULAR_RANK_1;
  sConfig.SamplingTime = ADC_SAMPLETIME_1CYCLE_5;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Configure Regular Channel 
    */
  sConfig.Channel = ADC_CHANNEL_2;
  sConfig.Rank = ADC_REGULAR_RANK_2;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}

/** 
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void) 
{
  /* DMA controller clock enable */
  __HAL_RCC_DMA1_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA1_Channel1_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Channel1_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel1_IRQn);

}

/** Configure pins as 
        * Analog 
        * Input 
        * Output
        * EVENT_OUT
        * EXTI
     PA1   ------> SharedAnalog_PA1
     PA2   ------> SharedAnalog_PA2
*/
static void MX_GPIO_Init(void)
{

  GPIO_InitTypeDef GPIO_InitStruct;

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13|GPIO_PIN_14, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0|GPIO_PIN_3|GPIO_PIN_4, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_4|GPIO_PIN_5 
                          |GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9, GPIO_PIN_RESET);

  /*Configure GPIO pins : PC13 PC14 */
  GPIO_InitStruct.Pin = GPIO_PIN_13|GPIO_PIN_14;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : PA0 PA3 PA4 */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_3|GPIO_PIN_4;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : PA1 PA2 */
  GPIO_InitStruct.Pin = GPIO_PIN_1|GPIO_PIN_2;
  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : PB10 PB11 PB4 PB5 
                           PB6 PB7 PB8 PB9 */
  GPIO_InitStruct.Pin = GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_4|GPIO_PIN_5 
                          |GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */
void begin(uint16_t ID) {
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

void fillScreen(uint16_t color) {
	fillRect(0, 0, WIDTH, HEIGHT, color);
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

void testDrawScreen() {
	// Titulo
	setCursor(16, 16);
	setTextColor(BLUE, GREEN);
	setTextSize(4);
	print("UfAbC TeStE!!");

	// Botao 1
	setCursor(16, 70);
	setTextColor(BLUE, GREEN);
	setTextSize(4);
	print("BoTaO 1");
	drawRect(61, 10, 45, 185, BLUE);
	drawRect(62, 11, 43, 183, BLUE);

	setCursor(16, 148);
	setTextColor(BLUE, GREEN);
	setTextSize(2);
	print("X:");

	setCursor(130, 148);
	setTextColor(BLUE, GREEN);
	setTextSize(2);
	print("Y:");

	setCursor(200, 148);
	setTextColor(BLUE, GREEN);
	setTextSize(2);
	print("Z:");

	//vertScroll(0, 320, teste);

	if (teste >= 320) {
		teste = 0;
	} else {
		teste += 1;
	}

	/*if (touchx_atual > 150 && touchx_atual < 500 && touchy_atual > 780 && touchy_atual < 1450) {
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);
	} else {
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET);
	}*/


	fillRect(320-teste, 100, 4, 30, RED);
	fillRect(320-teste+4, 100, 4, 30, GREEN);

	/*if (eixo_y_plus < 20) {
		fillRect(100+teste-2, 100+eixo_y_plus, 2, 2, RED);
		fillRect(100+teste-6, 100+eixo_y_plus-1, 2, 2, GREEN);
		eixo_y_plus++;

	} else {
		fillRect(100+teste-2, 100+eixo_y_minus, 2, 2, RED);
		fillRect(100+teste-6, 100+eixo_y_minus+1, 2, 2, GREEN);
		eixo_y_minus--;

		if (eixo_y_minus == 0) {
			eixo_y_plus = 0;
			eixo_y_minus = 20;
		}
	}*/
}

void calibrateTouch(){
	char resultx[50];
	char resulty[50];
	int samples = 500;
	int temp = 0;
	int f_touch = 0;

	setTextColor(BLUE, GREEN);

	int x1, x2, x3 = 0;
	int y1, y2, y3 = 0;
	int touchx1, touchx2, touchx3 = 0;
	int touchy1, touchy2, touchy3 = 0;

	int ident[3][3];

	fillRect(20, 20, 11, 11, RED);

	x1 = 20 + 11/2; // Ponto X1 + Largura do ponto
	y1 = 20 + 11/2; // Ponto Y1 + Largura do ponto

	while (f_touch == 0) {
		for (int i = 0; i < samples; i++) {
			readTouchX();
			if (flag_adc == 0) {
				temp += val_adc1;
			}
		}
		temp = temp/samples;
		if (temp > 10) f_touch = 1;
	}

	for (int i = 0; i < 20; i++) {
		readTouchX();
		if (flag_adc == 0) {
			temp += val_adc1;
		}
	}
	touchx1 = temp/samples;

	sprintf(resultx, "%i", touchx1);

	fillRect(40, 148, 80, 18, GREEN);

	setCursor(40, 148);
	setTextSize(2);
	print(resultx);

	temp = 0;
	for (int i = 0; i < samples; i++) {
		readTouchY();
		if (flag_adc == 0) {
			temp += val_adc2;
		}
	}
	touchy1 = temp/samples;

	sprintf(resulty, "%i", touchy1);

	fillRect(154, 148, 80, 18, GREEN);

	setCursor(154, 148);
	setTextSize(2);
	print(resulty);

	fillRect(240, 120, 11, 11, RED);

	x2 = 240 + 11/2; // Ponto X2 + Largura do ponto
	y2 = 120 + 11/2; // Ponto Y2 + Largura do ponto

	f_touch = 0;
	temp = 0;
	val_adc2 = 0;
	while (f_touch == 0) {
		for (int i = 0; i < samples; i++) {
			readTouchY();
			if (flag_adc == 0) {
				temp += val_adc2;
			}
		}
		temp = temp/samples;
		if (temp < 200 && temp > 50) f_touch = 1;
	}

	for (int i = 0; i < 20; i++) {
		readTouchX();
		if (flag_adc == 0) {
			temp += val_adc1;
		}
	}
	touchx2 = temp/samples;

	sprintf(resultx, "%i", touchx2);

	fillRect(40, 148, 80, 18, GREEN);

	setCursor(40, 148);
	setTextSize(2);
	print(resultx);

	temp = 0;
	for (int i = 0; i < samples; i++) {
		readTouchY();
		if (flag_adc == 0) {
			temp += val_adc2;
		}
	}
	touchy2 = temp/samples;

	sprintf(resulty, "%i", touchy2);

	fillRect(154, 148, 80, 18, GREEN);

	setCursor(154, 148);
	setTextSize(2);
	print(resulty);

	fillRect(20, 200, 11, 11, RED);

	x3 = 20 + 11/2; // Ponto X3 + Largura do ponto
	y3 = 200 + 11/2; // Ponto Y3 + Largura do ponto

	f_touch = 0;
	temp = 0;
	val_adc1 = 0;
	while (f_touch == 0) {
		for (int i = 0; i < samples; i++) {
			readTouchX();
			if (flag_adc == 0) {
				temp += val_adc1;
			}
		}
		temp = temp/samples;
		if (temp > 10) f_touch = 1;
	}

	for (int i = 0; i < samples; i++) {
		readTouchX();
		if (flag_adc == 0) {
			temp += val_adc1;
		}
	}
	touchx3 = temp/samples;

	sprintf(resultx, "%i", touchx3);

	fillRect(40, 148, 80, 18, GREEN);

	setCursor(40, 148);
	setTextSize(2);
	print(resultx);

	temp = 0;
	for (int i = 0; i < samples; i++) {
		readTouchY();
		if (flag_adc == 0) {
			temp += val_adc2;
		}
	}
	touchy3 = temp/samples;

	sprintf(resulty, "%i", touchy3);

	fillRect(154, 148, 80, 18, GREEN);

	setCursor(154, 148);
	setTextSize(2);
	print(resulty);

	int A[3][3] =
	{
		{touchx1,touchy1,1}  ,
		{touchx2,touchy2,1}  ,
		{touchx3,touchy3,1}
	};

	for(int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			if (i == j) {
				ident[i][j] = 1;
			}
			else {
				ident[i][j] = 0;
			}
		}
	}

	for (int j = 0; j < 3; j++) {
		for (int k = 0; k < 3; k++) {
			A[j][k] = A[j][k]/A[j][j];
			ident[j][k] = ident[j][k]/A[j][j];
		}

		for (int i = 0; i < 3; i++) {
			if (i != j) {
				for (int k = 0; k < 3; k++) {
					A[j][k] = A[i][k] - (A[i][j] * A[j][k]);
					ident[i][k] = ident[i][k] - (A[i][j] * ident[j][k]);
				}
			}
		}
	}

	sprintf(resultx, "%i", touchx1);

	setCursor(30, 40);
	setTextSize(2);
	print(resultx);

	sprintf(resultx, "%i", touchy1);

	setCursor(60, 40);
	setTextSize(2);
	print(resultx);

	sprintf(resultx, "%i", touchx2);

	setCursor(90, 40);
	setTextSize(2);
	print(resultx);

	sprintf(resulty, "%i", touchy2);

	setCursor(30, 80);
	setTextSize(2);
	print(resulty);

	sprintf(resulty, "%i", touchx3);

	setCursor(80, 80);
	setTextSize(2);
	print(resulty);

	sprintf(resulty, "%i", touchy3);

	setCursor(120, 80);
	setTextSize(2);
	print(resulty);

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

void print(char c[]) {
	size_t n = strlen(c);
	for (size_t i = 0; i < n; i++) {
		write(c[i]);
	}
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

void drawRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) {
	drawHLine(x, y, w, color);
	drawHLine(x, y+h-1, w, color);
	drawVLine(x, y, h, color);
	drawVLine(x+w-1, y, h, color);
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

void vertScroll (int16_t top, int16_t scrollines, int16_t offset) {
    int16_t vsp;
    if (offset <= -scrollines || offset >= scrollines) offset = 0; //valid scroll
    vsp = top + offset; // vertical start position
	if (offset < 0)
		vsp += scrollines;          //keep in unsigned range
	writeCmdData(0x41, vsp);        //VL#
}

void writeCmdData(uint16_t cmd, uint16_t dat) {
    cs_active();
    writeCmd(cmd);
    writeData(dat);
    cs_idle();
}

void drawChar(uint16_t WIDTH, uint16_t HEIGHT, int16_t x, int16_t y, unsigned char c, uint16_t color, uint16_t textbgcolor, uint8_t size) {
	for(int8_t i=0; i<8; i++) {
		for(int8_t j=0; j<5; j++) {
			if(font[c][j][7-i] == 1) {
				if (size == 1)
					drawPixel(x+j, y+i, color);
				else
					fillRect(x+j*size, y+i*size, size, size, color);
			}
		}
	}
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

void rd_active() {
	HAL_GPIO_WritePin(RD_PORT, RD_PIN, GPIO_PIN_RESET);
}

void rd_idle() {
	HAL_GPIO_WritePin(RD_PORT, RD_PIN, GPIO_PIN_SET);
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

void rs_data() {
	HAL_GPIO_WritePin(RS_PORT, RS_PIN, GPIO_PIN_SET);
}

void cs_active() {
	HAL_GPIO_WritePin(CS_PORT, CS_PIN, GPIO_PIN_RESET);
}

void cs_idle() {
	HAL_GPIO_WritePin(CS_PORT, CS_PIN, GPIO_PIN_SET);
}

void reset_active() {
	HAL_GPIO_WritePin(RST_PORT, RST_PIN, GPIO_PIN_RESET);
}

void reset_idle() {
	HAL_GPIO_WritePin(RST_PORT, RST_PIN, GPIO_PIN_SET);
}

void wr_strobe() {
	wr_active();
	wr_idle();
}

void write16(uint16_t x) {
	uint8_t h = (x)>>8;
	uint8_t l = x;
	write8(h);
	write8(l);
}

void write8(uint8_t x) {
	write_8(x);
	wr_strobe();
	wr_idle();
}

void write_8(uint8_t x) {
	GPIOB->BSRR = 0x0FF0<< 16;
	aux1 = ((x) & 0x03)<<8;
	aux2 = (x) & 0xFC;
	GPIOB->BSRR = ((aux2 | aux1)<<2) & 0x0FF0;
}

void readTouch(){
	char resultx[50];
	char resulty[50];
	int samples = 2;
	int temp = 0;
	int temp1 = 0;

	for (int i = 0; i < samples; i++) {
		readTouchX();
		if (flag_adc == 0) {
			temp += val_adc1;
		} else {
			i--;
		}
	}
	touchx_atual = temp/samples;

	sprintf(resultx, "%i", touchx_atual);

	fillRect(40, 148, 80, 18, GREEN);

	setCursor(40, 148);
	setTextSize(2);
	print(resultx);

	temp = 0;
	for (int i = 0; i < samples; i++) {
		readTouchY();
		if (flag_adc == 0) {
			temp += val_adc2;
		} else {
			i--;
		}
	}
	touchy_atual = temp/samples;

	sprintf(resulty, "%i", touchy_atual);

	fillRect(154, 148, 80, 18, GREEN);

	setCursor(154, 148);
	setTextSize(2);
	print(resulty);

	temp = 0;
	for (int i = 0; i < samples; i++) {
		readTouchZ();
		if (flag_adc == 0) {
			temp += val_adc1;
			temp1 += val_adc2;
		}
	}
	touchx_atual = temp/samples;
	touchy_atual = temp1/samples;

	touchz_atual = touchx_atual - touchy_atual;

	sprintf(resultx, "%i", touchz_atual);
	//sprintf(resulty, "%i", touchy_atual);

	fillRect(210, 148, 100, 18, GREEN);

	setCursor(210, 148);
	setTextSize(2);
	print(resultx);

	if (touchz_atual > MINPRESSURE && touchz_atual < MAXPRESSURE) {
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);
	} else {
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET);
	}
}

void readTouchX() {
	wr_analog_input(); //_yp
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, GPIO_PIN_SET);
	rs_cmd(); //_ym LOW

	flag_adc = 1;
	HAL_ADC_Start_IT(&hadc1);
	wr_output();
}

void readTouchY() {
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, GPIO_PIN_RESET);
	rs_cmd();
	rs_analog_input(); //_yp

	wr_idle(); //_yp HIGH
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, GPIO_PIN_RESET);

	flag_adc = 1;
	HAL_ADC_Start_IT(&hadc1);
	rs_output();
}

void readTouchZ() {
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, GPIO_PIN_SET);
	rs_cmd();
	rs_analog_input();
	wr_active();
	wr_analog_input();

	flag_adc = 1;
	HAL_ADC_Start_IT(&hadc1);
	rs_output();
	wr_output();
}

void writeCmd (uint16_t cmd) {
	rs_cmd();
	write16(cmd);
	rs_data();
}

void writeData(uint16_t data) {
	write16(data);
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

void setWriteDir() {
  GPIO_InitTypeDef GPIO_InitStruct;

  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3
						  |GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3
	                          |GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7, GPIO_PIN_RESET);
}

void rd_output() {
	  GPIO_InitTypeDef GPIO_InitStruct;
	  GPIO_InitStruct.Pin = RD_PIN;
	  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	  GPIO_InitStruct.Pull = GPIO_NOPULL;
	  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	  HAL_GPIO_Init(RD_PORT, &GPIO_InitStruct);
}

void wr_analog_input() {
	  GPIO_InitTypeDef GPIO_InitStruct;
	  GPIO_InitStruct.Pin = WR_PIN;
	  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
	  HAL_GPIO_Init(WR_PORT, &GPIO_InitStruct);
}

void wr_output() {
	  GPIO_InitTypeDef GPIO_InitStruct;
	  GPIO_InitStruct.Pin = WR_PIN;
	  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	  GPIO_InitStruct.Pull = GPIO_NOPULL;
	  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	  HAL_GPIO_Init(WR_PORT, &GPIO_InitStruct);
}

void rs_analog_input() {
	  GPIO_InitTypeDef GPIO_InitStruct;
	  GPIO_InitStruct.Pin = RS_PIN;
	  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
	  HAL_GPIO_Init(RS_PORT, &GPIO_InitStruct);
}

void rs_output() {
	  GPIO_InitTypeDef GPIO_InitStruct;
	  GPIO_InitStruct.Pin = RS_PIN;
	  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	  GPIO_InitStruct.Pull = GPIO_NOPULL;
	  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	  HAL_GPIO_Init(RS_PORT, &GPIO_InitStruct);
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

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  file: The file name as string.
  * @param  line: The line in file as a number.
  * @retval None
  */
void _Error_Handler(char *file, int line)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  while(1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
