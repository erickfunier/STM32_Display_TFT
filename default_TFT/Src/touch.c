/*
 * touch.c
 *
 *  Created on: 14 de jan de 2019
 *      Author: Inspiron
 *
 *  Use this code to set the area of touch
 */

#include "touch.h"
#include "lcd.h"
#include "main.h"
#include "stdbool.h"
#include "stm32f1xx_hal.h"

int touchx_atual = 0;
int touchy_atual = 0;
int touchz_atual = 0;

// Check if touch is valid, return TRUE or FALSE
bool ISPRESSED(void)
{
    int count = 0;
    bool state, oldstate = false;
    readTouchZ();
    while (count < 10) {
        if (flag_adc == 0) {
        	touchz_atual = (4096 - (val_adc1 - val_adc2));
			state = touchz_atual > 200;
			if (state == oldstate) count++;
			else count = 0;
			oldstate = state;
			readTouchZ();
        } else {
        	HAL_Delay(1);
        }
    }
    return oldstate;
}

// Read touch x and y and VALIDATE TOUCH IN AN AREA
void readTouch(){
	char resultx[8];
	char resulty[8];
	int samples = 20;
	int temp = 0;

	readTouchX();
	for (int i = 0; i < samples; i++) {
		if (flag_adc == 0) {
			if (val_adc1 < 1500) temp += val_adc1;
			readTouchX();
		} else {
			i--;
		}
	}
	touchx_atual = temp/samples;

	// show touch x on display
	sprintf(resultx, "%i", touchx_atual);
	fillRect(40, 168, 80, 18, GREEN);
	print(40, 168, BLUE, GREEN, 2, resultx);

	flag_adc = 1;
	temp = 0;

	readTouchY();
	for (int i = 0; i < samples; i++) {
		if (flag_adc == 0) {
			temp += val_adc2;
			readTouchY();
		} else {
			i--;
		}
	}
	touchy_atual = temp/samples;

	// show touch y on display
	sprintf(resulty, "%i", touchy_atual);
	fillRect(154, 168, 80, 18, GREEN);
	print(154, 168, BLUE, GREEN, 2, resulty);

	// AREA TO BUTTON TOUCH
	/*
	 * Define touchx max and min
	 * Define touchy max and min
	 * Below:
	 * 	touchx max = 1240
	 * 	touchx min = 750
	 * 	touchy max = 620
	 * 	touchy min = 250
	 */
	if(ISPRESSED() && touchx_atual > 750 && touchx_atual < 1240 && touchy_atual > 250 && touchy_atual < 620){
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);
	} else {
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET);
	}
}

// Read touch x position on screen
void readTouchX() {
	wr_analog_input();
	wr_active();
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, GPIO_PIN_SET);
	rs_cmd();

	flag_adc = 1;
	startADC1IT();
}

// Read touch y position on screen
void readTouchY() {
	rs_analog_input();
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, GPIO_PIN_RESET);
	rs_cmd();
	wr_idle();
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, GPIO_PIN_RESET);

	flag_adc = 1;
	startADC1IT();
}

// Read touch "z" pressed on screen
void readTouchZ() {
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, GPIO_PIN_SET);
	rs_cmd();
	rs_analog_input();
	wr_active();
	wr_analog_input();

	flag_adc = 1;
	startADC1IT();
}
