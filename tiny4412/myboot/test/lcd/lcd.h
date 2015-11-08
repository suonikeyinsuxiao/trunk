#ifndef __LCD_H
#define __LCD_H
#include "clk.h"

#define	GPF0CON *(volatile unsigned int*)0x11400180		//			0x22222222
#define	GPF1CON *(volatile unsigned int*)0x114001A0		//(0X20)	0x22222222
#define	GPF2CON *(volatile unsigned int*)0x114001C0		//(0X20)	0x22222222
#define	GPF3CON *(volatile unsigned int*)0x114001E0		//(0X20)	0x22222222

#define	LCDCBLK_CFG 	*(volatile unsigned int*)0x10010210
#define	VIDCON0			*(volatile unsigned int*)0x11c00000
#define	VIDCON1			*(volatile unsigned int*)0x11c00004
#define	VIDTCON0		*(volatile unsigned int*)0x11c00010
#define	VIDTCON1		*(volatile unsigned int*)0x11c00014		
#define	VIDTCON2		*(volatile unsigned int*)0x11c00018
#define	WINCON0			*(volatile unsigned int*)0x11c00020		
#define	VIDOSD0A		*(volatile unsigned int*)0x11c00040
#define	VIDOSD0B		*(volatile unsigned int*)0x11c00044
#define	VIDW00ADD0B0	*(volatile unsigned int*)0x11C000A0
#define	VIDW00ADD1B0	*(volatile unsigned int*)0x11C000D0
#define	VIDW00ADD2		*(volatile unsigned int*)0x11C00100
#define	SHADOWCON		*(volatile unsigned int*)0x11C00034


void initLcd(unsigned int nAddr);

#endif
