
#ifndef __INPUT_H__
#define __INPUT_H__
#include<reg51.h>

#define MATRIX_PIN P1
#define SEPARATE_PIN P3

sbit K1 = SEPARATE_PIN^0;
sbit K2 = SEPARATE_PIN^1;
sbit K3 = SEPARATE_PIN^2;
sbit K4 = SEPARATE_PIN^3;
sbit K5 = SEPARATE_PIN^4;
sbit K6 = SEPARATE_PIN^5;
sbit K7 = SEPARATE_PIN^6;
sbit k8 = SEPARATE_PIN^7;

// Get the key code of pressed button
int get_key( unsigned char * record  );
#endif