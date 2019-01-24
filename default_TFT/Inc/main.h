/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  ** This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * COPYRIGHT(c) 2019 STMicroelectronics
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H__
#define __MAIN_H__

/* Includes ------------------------------------------------------------------*/

/* USER CODE BEGIN Includes */
#include <stdint.h>

/* USER CODE END Includes */

/* Private define ------------------------------------------------------------*/

#define LED_Pin GPIO_PIN_13 // LED onboard of bluepill
#define LED_GPIO_Port GPIOC // LED onboard of bluepill
#define XTAL_Pin GPIO_PIN_14 // Crystal Osclillator 8 MHz onboard of bluepill
#define XTAL_GPIO_Port GPIOC // Crystal Osclillator 8 MHz onboard of bluepill
#define LCD_RD_Pin GPIO_PIN_0 // RD PIN of LCD mcufriend connected on PA0 of bluepill
#define LCD_RD_GPIO_Port GPIOA // RD PIN of LCD mcufriend connected on PA0 of bluepill
#define LCD_WR_Pin GPIO_PIN_1 // WR PIN of LCD mcufriend connected on PA1 of bluepill
#define LCD_WR_GPIO_Port GPIOA // WR PIN of LCD mcufriend connected on PA1 of bluepill
#define LCD_RS_Pin GPIO_PIN_2 // RS PIN of LCD mcufriend connected on PA2 of bluepill
#define LCD_RS_GPIO_Port GPIOA // RS PIN of LCD mcufriend connected on PA2 of bluepill
#define LCD_CS_Pin GPIO_PIN_3 // CS PIN of LCD mcufriend connected on PA3 of bluepill
#define LCD_CS_GPIO_Port GPIOA // CS PIN of LCD mcufriend connected on PA3 of bluepill
#define LCD_RST_Pin GPIO_PIN_4 // RST PIN of LCD mcufriend connected on PA4 of bluepill
#define LCD_RST_GPIO_Port GPIOA // RST PIN of LCD mcufriend connected on PA4 of bluepill
#define LCD_D0_Pin GPIO_PIN_10 // LCD D0 PIN of LCD mcufriend connected on PB10 of bluepill
#define LCD_D0_GPIO_Port GPIOB // LCD D0 PIN of LCD mcufriend connected on PB10 of bluepill
#define LCD_D1_Pin GPIO_PIN_11 // LCD D1 PIN of LCD mcufriend connected on PB11 of bluepill
#define LCD_D1_GPIO_Port GPIOB // LCD D1 PIN of LCD mcufriend connected on PB11 of bluepill
#define LCD_D2_Pin GPIO_PIN_4 // LCD D2 PIN of LCD mcufriend connected on PB4 of bluepill
#define LCD_D2_GPIO_Port GPIOB // LCD D2 PIN of LCD mcufriend connected on PB4 of bluepill
#define LCD_D3_Pin GPIO_PIN_5 // LCD D3 PIN of LCD mcufriend connected on PB5 of bluepill
#define LCD_D3_GPIO_Port GPIOB // LCD D3 PIN of LCD mcufriend connected on PB5 of bluepill
#define LCD_D4_Pin GPIO_PIN_6 // LCD D4 PIN of LCD mcufriend connected on PB6 of bluepill
#define LCD_D4_GPIO_Port GPIOB // LCD D4 PIN of LCD mcufriend connected on PB6 of bluepill
#define LCD_D5_Pin GPIO_PIN_7 // LCD D5 PIN of LCD mcufriend connected on PB7 of bluepill
#define LCD_D5_GPIO_Port GPIOB // LCD D5 PIN of LCD mcufriend connected on PB7 of bluepill
#define LCD_D6_Pin GPIO_PIN_8 // LCD D6 PIN of LCD mcufriend connected on PB8 of bluepill
#define LCD_D6_GPIO_Port GPIOB // LCD D6 PIN of LCD mcufriend connected on PB8 of bluepill
#define LCD_D7_Pin GPIO_PIN_9 // LCD D7 PIN of LCD mcufriend connected on PB9 of bluepill
#define LCD_D7_GPIO_Port GPIOB // LCD D7 PIN of LCD mcufriend connected on PB9 of bluepill

/* ########################## Assert Selection ############################## */
/**
  * @brief Uncomment the line below to expanse the "assert_param" macro in the 
  *        HAL drivers code
  */
/* #define USE_FULL_ASSERT    1U */

/* USER CODE BEGIN Private defines */


int flag_adc; // flag end of conversion
uint32_t val_adc1; // read result of conv ADC
uint32_t val_adc2; // read result of conv ADC
uint32_t val_adc3; // read result of conv ADC

void wr_analog_input(); // function to set WR PIN (connected to WR PIN of LCD) analog for read touch
void rs_cmd();	// function to set RS PIN to send command to LCD
void rs_analog_input(); // function to set RS PIN (connected to RS PIN of LCD) analog for read touch
void wr_idle();	// function to set WR PIN like idle for LCD
void wr_active();	// function to set WR PIN like active for LCD

void startADC1IT();	// start ADC 1 Interruption for read touch values from LCD

/* USER CODE END Private defines */

#ifdef __cplusplus
 extern "C" {
#endif
void _Error_Handler(char *, int);

#define Error_Handler() _Error_Handler(__FILE__, __LINE__)
#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H__ */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
