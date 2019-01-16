/*
 * touch.h
 *
 *  Created on: 14 de jan de 2019
 *      Author: Inspiron
 */

#ifndef TOUCH_H_
#define TOUCH_H_

#define MINPRESSURE 200
#define MAXPRESSURE 1000

int touchx_atual;
int touchy_atual;
int touchz_atual;

void readTouchX();
void readTouchY();
void readTouchZ();
void readTouch();

#endif /* TOUCH_H_ */
