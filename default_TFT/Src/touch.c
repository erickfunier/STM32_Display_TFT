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

/*

void calibrateTouch(){
	char resultx[50];
	char resulty[50];
	int samples = 100;
	int temp = 0;
	int f_touch = 0;
	int failcount = 0;
	int count = 0;
	bool OK = false;

	int x1, x2, x3;
	int y1, y2, y3;
	int touchx1 = 0, touchx2 = 0, touchx3 = 0;
	int touchy1 = 0, touchy2 = 0, touchy3 = 0;
	int alphaX, betaX, deltaX;
	int alphaY, betaY, deltaY;
	int X, Y;

	setTextColor(BLUE, GREEN);
	setCursor(60, 120);
	setTextSize(2);
	print("Pressione na cruz!");

	drawCross(20, 20, BLUE, GREEN);

	while (!OK) {
		while (!ISPRESSED()) {};
		fillRect(60, 120, 220, 16, GREEN);
		setCursor(60, 120);
		print("Mantenha pressionado!");
		count = 0;
		do {
			if (samples-count-1 < 10) fillRect(65, 140, 25, 16, GREEN);
			setCursor(60, 140);
			sprintf(resultx, "%i", samples-count-1);
			print(resultx);
			readTouch();
			if (touchz_atual > 200) {
				touchx1 += touchx_atual;
				touchy1 += touchy_atual;
				count++;
			} else {
				failcount++;
			}
		} while ((count < samples) && (failcount < 10000));

		if(count >= samples) {
			OK = true;
		} else {
			touchx1 = 0;
			touchy1 = 0;
			count = 0;
		}
		if (failcount >= 10000) {
			fillRect(90, 120, 200, 16, GREEN);
			setCursor(90, 120);
			print("FAIL!");
		}
	};

	touchx1 = touchx1/samples;
	touchy1 = touchy1/samples;

	fillRect(20, 20, 20, 20, GREEN); //Apaga a cruz
	fillRect(60, 140, 25, 16, GREEN); //Apaga o timer
	drawCross(100, 200, BLUE, GREEN); //Desenha nova cruz
	fillRect(60, 120, 280, 16, GREEN); //Apaga texto
	setCursor(60, 180);
	print("Pressione na cruz!");
	while (ISPRESSED()) {}; //Aguarda retirar o toque da cruz anterior
	OK = false;

	while (!OK) {
		while (!ISPRESSED()) {};
		fillRect(60, 180, 220, 16, GREEN);
		setCursor(60, 180);
		print("Mantenha pressionado!");
		count = 0;
		do {
			if (samples-count-1 < 10) fillRect(65, 200, 25, 16, GREEN);
			setCursor(60, 200);
			sprintf(resultx, "%i", samples-count-1);
			print(resultx);
			readTouch();
			if (touchz_atual > 200) {
				touchx2 += touchx_atual;
				touchy2 += touchy_atual;
				count++;
			} else {
				failcount++;
			}
		} while ((count < samples) && (failcount < 10000));

		if(count >= samples) {
			OK = true;
		} else {
			touchx2 = 0;
			touchy2 = 0;
			count = 0;
		}
		if (failcount >= 10000) {
			fillRect(90, 120, 200, 16, GREEN);
			setCursor(90, 120);
			print("FAIL!");
		}
	}

	touchx2 = touchx2/samples;
	touchy2 = touchy2/samples;

	fillRect(200, 100, 20, 20, GREEN); //Apaga a cruz
	fillRect(60, 200, 25, 16, GREEN); //Apaga o timer
	drawCross(200, 20, BLUE, GREEN); //Desenha nova cruz
	fillRect(60, 180, 280, 16, GREEN); //Apaga texto
	setCursor(60, 100);
	print("Pressione na cruz!");
	while (ISPRESSED()) {}; //Aguarda retirar o toque da cruz anterior
	OK = false;

	while (!OK) {
		while (!ISPRESSED()) {};
		fillRect(60, 100, 220, 16, GREEN);
		setCursor(60, 100);
		print("Mantenha pressionado!");
		count = 0;
		do {
			if (samples-count-1 < 10) fillRect(65, 120, 25, 16, GREEN);
			setCursor(60, 120);
			sprintf(resultx, "%i", samples-count-1);
			print(resultx);
			readTouch();
			if (touchz_atual > 200) {
				touchx3 += touchx_atual;
				touchy3 += touchy_atual;
				count++;
			} else {
				failcount++;
			}
		} while ((count < samples) && (failcount < 10000));

		if(count >= samples) {
			OK = true;
		} else {
			touchx3 = 0;
			touchy3 = 0;
			count = 0;
		}
		if (failcount >= 10000) {
			fillRect(90, 100, 200, 16, GREEN);
			setCursor(90, 100);
			print("FAIL!");
		}
	}

	touchx3 = touchx3/samples;
	touchy3 = touchy3/samples;

	int ident[3][3];

	x1 = 20 + 15/2; // Ponto X1 + Largura do ponto
	y1 = 20 + 15/2; // Ponto Y1 + Largura do ponto
	x2 = 200 + 15/2; // Ponto X2 + Largura do ponto
	y2 = 100 + 15/2; // Ponto Y2 + Largura do ponto
	x3 = 200 + 15/2; // Ponto X3 + Largura do ponto
	y3 = 20 + 15/2; // Ponto Y3 + Largura do ponto

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

	fillScreen(GREEN);

	sprintf(resultx, "%i", touchx1);

	setCursor(10, 40);
	setTextSize(2);
	print(resultx);

	sprintf(resultx, "%i", touchy1);

	setCursor(110, 40);
	setTextSize(2);
	print(resultx);

	sprintf(resultx, "%i", touchx2);

	setCursor(10, 80);
	setTextSize(2);
	print(resultx);

	sprintf(resulty, "%i", touchy2);

	setCursor(110, 80);
	setTextSize(2);
	print(resulty);

	sprintf(resulty, "%i", touchx3);

	setCursor(10, 120);
	setTextSize(2);
	print(resulty);

	sprintf(resulty, "%i", touchy3);

	setCursor(110, 120);
	setTextSize(2);
	print(resulty);

	alphaX = (ident[0][0]*x1) + (ident[0][1]*x2) + (x3);
	betaX = (ident[1][0]*x1) + (ident[1][1]*x2) + (x3);
	deltaX = (ident[2][0]*x1) + (ident[2][1]*x2) + (x3);

	alphaY = (ident[0][0]*y1) + (ident[0][1]*y2) + (y3);
	betaY = (ident[1][0]*y1) + (ident[1][1]*y2) + (y3);
	deltaY = (ident[2][0]*y1) + (ident[2][1]*y2) + (y3);

	X = (alphaX * touchx1) + (betaX * touchy1) + deltaX;
	Y = (alphaY * touchy1) + (betaY * touchy1) + deltaY;

	sprintf(resulty, "%i", X);

	setCursor(10, 160);
	setTextSize(2);
	print(resulty);

	sprintf(resulty, "%i", Y);

	setCursor(110, 160);
	setTextSize(2);
	print(resulty);

	run = true;

	/*int ident[3][3];

	//fillRect(20, 20, 11, 11, RED);
	drawCross(20, 20, BLUE, GREEN);

	x1 = 20 + 11/2; // Ponto X1 + Largura do ponto
	y1 = 20 + 11/2; // Ponto Y1 + Largura do ponto

	while (f_touch == 0) {
		for (int i = 0; i < samples; i++) {
			readTouchX();
			if (flag_adc == 0) {
				temp += val_adc1;
			} else {
				i--;
			}
		}
		touchx1 = temp/samples;
		if (ISPRESSED()) f_touch = 1;
	}

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
			} else {
				i--;
			}
		}
		temp = temp/samples;
		if (ISPRESSED()) f_touch = 1;
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
			} else {
				i--;
			}
		}
		temp = temp/samples;
		if (ISPRESSED()) f_touch = 1;
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
*/
