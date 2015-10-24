#ifndef __COMMON_H
#define __COMMON_H

#include "hardware.h"

typedef int(*PRINT_F)(const char* format, ...);

unsigned int getPROID(void);
unsigned int getPACKAGEID(void);

void delay(void);
void key_delay(void);

void putchar(unsigned char ch);
unsigned char getchar(void);
void puts(const char* pStr);
void printnumber(int num, int base);
int printu(const char* format, ...);

#endif //__COMMON_h
 
 
