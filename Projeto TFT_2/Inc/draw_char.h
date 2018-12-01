/*
 * draw_char.h
 *
 *  Created on: 27 de nov de 2018
 *      Author: Inspiron
 */

#ifndef DRAW_CHAR_H_
#define DRAW_CHAR_H_

#include "main.h"

void drawChar(uint16_t WIDTH, uint16_t HEIGHT, int16_t x, int16_t y, unsigned char c, uint16_t color, uint16_t textbgcolor, uint8_t size) {
	if((x >= WIDTH)            || // Clip right
	   (y >= HEIGHT)           || // Clip bottom
	   ((x + 6 * size - 1) < 0) || // Clip left
	   ((y + 8 * size - 1) < 0))   // Clip top
		return;
	switch(c) {
	case '!':
		for(int8_t i=0; i<5; i++) {
			for(int8_t j=0; j<8; j++) {
				if(char_EX[j][i] == 1) {
					if (size == 1)
						drawPixel(x+i, y+j, color);
					else
						fillRect(x+i*size, y+j*size, size, size, color);
				}
			}
		}
		break;

	case ':':
		for(int8_t i=0; i<5; i++) {
			for(int8_t j=0; j<8; j++) {
				if(char_TWO[j][i] == 1) {
					if (size == 1)
						drawPixel(x+i, y+j, color);
					else
						fillRect(x+i*size, y+j*size, size, size, color);
				}
			}
		}
		break;

	case '-':
		for(int8_t i=0; i<5; i++) {
			for(int8_t j=0; j<8; j++) {
				if(char_MINUS[j][i] == 1) {
					if (size == 1)
						drawPixel(x+i, y+j, color);
					else
						fillRect(x+i*size, y+j*size, size, size, color);
				}
			}
		}
		break;

	case '0':
		for(int8_t i=0; i<8; i++) {
			for(int8_t j=0; j<5; j++) {
				if(char_0[j][7-i] == 1) {
					if (size == 1)
						drawPixel(x+j, y+i, color);
					else
						fillRect(x+j*size, y+i*size, size, size, color);
				}
			}
		}
		break;

	case '1':
		for(int8_t i=0; i<8; i++) {
			for(int8_t j=0; j<5; j++) {
				if(char_1[j][7-i] == 1) {
					if (size == 1)
						drawPixel(x+j, y+i, color);
					else
						fillRect(x+j*size, y+i*size, size, size, color);
				}
			}
		}
		break;

	case '2':
		for(int8_t i=0; i<8; i++) {
			for(int8_t j=0; j<5; j++) {
				if(char_2[j][7-i] == 1) {
					if (size == 1)
						drawPixel(x+j, y+i, color);
					else
						fillRect(x+j*size, y+i*size, size, size, color);
				}
			}
		}
		break;

	case '3':
		for(int8_t i=0; i<8; i++) {
			for(int8_t j=0; j<5; j++) {
				if(char_3[j][7-i] == 1) {
					if (size == 1)
						drawPixel(x+j, y+i, color);
					else
						fillRect(x+j*size, y+i*size, size, size, color);
				}
			}
		}
		break;

	case '4':
		for(int8_t i=0; i<8; i++) {
			for(int8_t j=0; j<5; j++) {
				if(char_4[j][7-i] == 1) {
					if (size == 1)
						drawPixel(x+j, y+i, color);
					else
						fillRect(x+j*size, y+i*size, size, size, color);
				}
			}
		}
		break;

	case '5':
		for(int8_t i=0; i<8; i++) {
			for(int8_t j=0; j<5; j++) {
				if(char_5[j][7-i] == 1) {
					if (size == 1)
						drawPixel(x+j, y+i, color);
					else
						fillRect(x+j*size, y+i*size, size, size, color);
				}
			}
		}
		break;

	case '6':
		for(int8_t i=0; i<5; i++) {
			for(int8_t j=0; j<8; j++) {
				if(char_6[j][i] == 1) {
					if (size == 1)
						drawPixel(x+i, y+j, color);
					else
						fillRect(x+i*size, y+j*size, size, size, color);
				}
			}
		}
		break;

	case '7':
		for(int8_t i=0; i<5; i++) {
			for(int8_t j=0; j<8; j++) {
				if(char_7[j][i] == 1) {
					if (size == 1)
						drawPixel(x+i, y+j, color);
					else
						fillRect(x+i*size, y+j*size, size, size, color);
				}
			}
		}
		break;

	case '8':
		for(int8_t i=0; i<5; i++) {
			for(int8_t j=0; j<8; j++) {
				if(char_8[j][i] == 1) {
					if (size == 1)
						drawPixel(x+i, y+j, color);
					else
						fillRect(x+i*size, y+j*size, size, size, color);
				}
			}
		}
		break;

	case '9':
		for(int8_t i=0; i<5; i++) {
			for(int8_t j=0; j<8; j++) {
				if(char_9[j][i] == 1) {
					if (size == 1)
						drawPixel(x+i, y+j, color);
					else
						fillRect(x+i*size, y+j*size, size, size, color);
				}
			}
		}
		break;

	case 'A':
		for(int8_t i=0; i<5; i++) {
			for(int8_t j=0; j<8; j++) {
				if(char_A[j][i] == 1) {
					if (size == 1)
						drawPixel(x+i, y+j, color);
					else
						fillRect(x+i*size, y+j*size, size, size, color);
				}
			}
		}
		break;

	case 'B':
		for(int8_t i=0; i<5; i++) {
			for(int8_t j=0; j<8; j++) {
				if(char_B[j][i] == 1) {
					if (size == 1)
						drawPixel(x+i, y+j, color);
					else
						fillRect(x+i*size, y+j*size, size, size, color);
				}
			}
		}
		break;

	case 'C':
		for(int8_t i=0; i<5; i++) {
			for(int8_t j=0; j<8; j++) {
				if(char_C[j][i] == 1) {
					if (size == 1)
						drawPixel(x+i, y+j, color);
					else
						fillRect(x+i*size, y+j*size, size, size, color);
				}
			}
		}
		break;

	case 'E':
		for(int8_t i=0; i<5; i++) {
			for(int8_t j=0; j<8; j++) {
				if(char_E[j][i] == 1) {
					if (size == 1)
						drawPixel(x+i, y+j, color);
					else
						fillRect(x+i*size, y+j*size, size, size, color);
				}
			}
		}
		break;

	case 'F':
		for(int8_t i=0; i<5; i++) {
			for(int8_t j=0; j<8; j++) {
				if(char_F[j][i] == 1) {
					if (size == 1)
						drawPixel(x+i, y+j, color);
					else
						fillRect(x+i*size, y+j*size, size, size, color);
				}
			}
		}
		break;

	case 'H':
		for(int8_t i=0; i<5; i++) {
			for(int8_t j=0; j<8; j++) {
				if(char_H[j][i] == 1) {
					if (size == 1)
						drawPixel(x+i, y+j, color);
					else
						fillRect(x+i*size, y+j*size, size, size, color);
				}
			}
		}
		break;

	case 'L':
		for(int8_t i=0; i<5; i++) {
			for(int8_t j=0; j<8; j++) {
				if(char_L[j][i] == 1) {
					if (size == 1)
						drawPixel(x+i, y+j, color);
					else
						fillRect(x+i*size, y+j*size, size, size, color);
				}
			}
		}
		break;

	case 'O':
		for(int8_t i=0; i<5; i++) {
			for(int8_t j=0; j<8; j++) {
				if(char_O[j][i] == 1) {
					if (size == 1)
						drawPixel(x+i, y+j, color);
					else
						fillRect(x+i*size, y+j*size, size, size, color);
				}
			}
		}
		break;

	case 'P':
		for(int8_t i=0; i<5; i++) {
			for(int8_t j=0; j<8; j++) {
				if(char_P[j][i] == 1) {
					if (size == 1)
						drawPixel(x+i, y+j, color);
					else
						fillRect(x+i*size, y+j*size, size, size, color);
				}
			}
		}
		break;

	case 'S':
		for(int8_t i=0; i<5; i++) {
			for(int8_t j=0; j<8; j++) {
				if(char_S[j][i] == 1) {
					if (size == 1)
						drawPixel(x+i, y+j, color);
					else
						fillRect(x+i*size, y+j*size, size, size, color);
				}
			}
		}
		break;

	case 'T':
		for(int8_t i=0; i<5; i++) {
			for(int8_t j=0; j<8; j++) {
				if(char_T[j][i] == 1) {
					if (size == 1)
						drawPixel(x+i, y+j, color);
					else
						fillRect(x+i*size, y+j*size, size, size, color);
				}
			}
		}
		break;

	case 'U':
		for(int8_t i=0; i<5; i++) {
			for(int8_t j=0; j<8; j++) {
				if(char_U[j][i] == 1) {
					if (size == 1)
						drawPixel(x+i, y+j, color);
					else
						fillRect(x+i*size, y+j*size, size, size, color);
				}
			}
		}
		break;
	case 'X':
		for(int8_t i=0; i<5; i++) {
			for(int8_t j=0; j<8; j++) {
				if(char_X[j][i] == 1) {
					if (size == 1)
						drawPixel(x+i, y+j, color);
					else
						fillRect(x+i*size, y+j*size, size, size, color);
				}
			}
		}
		break;

	case 'Y':
		for(int8_t i=0; i<5; i++) {
			for(int8_t j=0; j<8; j++) {
				if(char_Y[j][i] == 1) {
					if (size == 1)
						drawPixel(x+i, y+j, color);
					else
						fillRect(x+i*size, y+j*size, size, size, color);
				}
			}
		}
		break;

	case 'a':
		for(int8_t i=0; i<8; i++) {
			for(int8_t j=0; j<5; j++) {
				if(char_a[j][7-i] == 1) {
					if (size == 1)
						drawPixel(x+j, y+i, color);
					else
						fillRect(x+j*size, y+i*size, size, size, color);
				}
			}
		}
		break;

	case 'b':
		for(int8_t i=0; i<8; i++) {
			for(int8_t j=0; j<5; j++) {
				if(char_b[j][7-i] == 1) {
					if (size == 1)
						drawPixel(x+j, y+i, color);
					else
						fillRect(x+j*size, y+i*size, size, size, color);
				}
			}
		}
		break;

	case 'c':
		for(int8_t i=0; i<8; i++) {
			for(int8_t j=0; j<5; j++) {
				if(char_c[j][7-i] == 1) {
					if (size == 1)
						drawPixel(x+j, y+i, color);
					else
						fillRect(x+j*size, y+i*size, size, size, color);
				}
			}
		}
		break;

	case 'e':
		for(int8_t i=0; i<8; i++) {
			for(int8_t j=0; j<5; j++) {
				if(char_e[j][7-i] == 1) {
					if (size == 1)
						drawPixel(x+j, y+i, color);
					else
						fillRect(x+j*size, y+i*size, size, size, color);
				}
			}
		}
		break;

	case 'f':
		for(int8_t i=0; i<8; i++) {
			for(int8_t j=0; j<5; j++) {
				if(char_f[j][7-i] == 1) {
					if (size == 1)
						drawPixel(x+j, y+i, color);
					else
						fillRect(x+j*size, y+i*size, size, size, color);
				}
			}
		}
		break;

	case 'o':
		for(int8_t i=0; i<8; i++) {
			for(int8_t j=0; j<5; j++) {
				if(char_o[j][7-i] == 1) {
					if (size == 1)
						drawPixel(x+j, y+i, color);
					else
						fillRect(x+j*size, y+i*size, size, size, color);
				}
			}
		}
		break;

	case 't':
		for(int8_t i=0; i<8; i++) {
			for(int8_t j=0; j<5; j++) {
				if(char_t[j][7-i] == 1) {
					if (size == 1)
						drawPixel(x+j, y+i, color);
					else
						fillRect(x+j*size, y+i*size, size, size, color);
				}
			}
		}
		break;
	default:
		for(int8_t i=0; i<5; i++) {
				for(int8_t j=0; j<8; j++) {
					fillRect(x+i*size, y+j*size, size, size, textbgcolor);
				}
			}
		break;
	}
}




#endif /* DRAW_CHAR_H_ */
