#ifndef __IIC_H
#define __IIC_H
#include "res.h"
//S70
//Xi2cSDA1 	GPD1_2
//Xi2cSCL1	GPD1_3
//XEINT14		GPX1_6

#define GPD1CON  			*(v32*)0x114000c0
#define EXT_INT14CON		*(v32*)0x11400734
#define EXT_INT14_FLTCON0	*(v32*)0x11400868
#define EXT_INT14_FLTCON1	*(v32*)0x1140086C
#define EXT_INT14_MASK		*(v32*)0x11400934
#define EXT_INT14_PEND		*(v32*)0x11400A34

#define I2CCON1 			*(v32*)0x13870000
#define I2CSTAT1 			*(v32*)0x13870004
#define I2CADD1 			*(v32*)0x13870008
#define I2CDS1 				*(v32*)0x1387000C
#define I2CLC1 				*(v32*)0x13870010

void i2cDelay(U32 nCount);

/*
 *desription:	init I2C Bus
 *
 * 
 */
void initI2c();

/*
 *desription:	write a byte from master to slave
 *
 *cSlaveAddr:	slave address
 *
 *cRegAddr:		slave register address
 *
 *cVal:			value to slave
 */
void writeI2c(U8 cSlaveAddr, U8 cRegAddr, U8 cVal);

/*
 *desription:	read a byte from slave 
 *
 *cSlaveAddr:	slave address
 *
 *cRegAddr:		slave register address
 *
 *pcVal:		save read value
 */
void readI2c(U8 cSlaveAddr, U8 cRegAddr, U8* pcVal);

/*
 *desription:	read nBufSize byte from slave 
 *
 *cSlaveAddr:	slave address
 *
 *pcBuf:		save read data	
 *
 *nBufSize:		buffer size	
 */
void readI2cBuf(U8 cSlaveAddr, char* pcBuf, U32 nBufSize);


//S70
#endif
