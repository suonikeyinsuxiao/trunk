#ifndef __CLK_H
#define __CLK_H


#define CLK_SRC_PERIL0 *(volatile unsigned int*)0x1003c250       
#define CLK_DIV_PERIL0 *(volatile unsigned int*)0x1003c550         
#define CLK_SRC_LEFTBUS *(volatile unsigned int*)0x10034200
#define MPLL_CON0  		*(volatile unsigned int*)0x10040108


#define CLK_DIV_LCD  *(volatile unsigned int*)0X1003c534
#define CLK_SRC_LCD0 *(volatile unsigned int*)0x1003c234
#define CLK_DIV_TOP  *(volatile unsigned int*)0x1003c510 
#define CLK_SRC_TOP0 *(volatile unsigned int*)0x1003c210
#define CLK_SRC_TOP1 *(volatile unsigned int*)0x1003c214

#define CLK_SRC_DMC  *(volatile unsigned int*)0x10040200
#endif 
