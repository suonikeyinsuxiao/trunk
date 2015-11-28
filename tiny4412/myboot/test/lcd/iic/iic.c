#include "iic.h"

void i2cDelay(U32 nCount)
{
	while(nCount--);
}

void initI2c()
{
	//config i2c gpio
	GPD1CON &= ~(0xff00);
	GPD1CON |= (0x22 << 8);
	
	//ACK enable;prescaler;interrupt enable
	I2CCON1 &= ~(0xf);
	I2CCON1 |= (1 << 7)|(1 << 6)|(1 << 5)|1;
	// M TX;serial output enable
	I2CSTAT1 |= (3 << 6)|(1 << 4);
}

void writeI2c(U8 cSlaveAddr, U8 cRegAddr, U8 cVal)
{
	U8 data[2]={cRegAddr, cVal};
	int nCount = 0;
	//Master TX
	//I2CSTAT1 |= 3 << 6;//num 29 has configed
	//write slaveaddress to I2CDS1
	I2CDS1 = cSlaveAddr;
	//write 0xf0(M/T START) to I2CSTAT1
	I2CSTAT1 = 0xf0;

	//write data
	while(1)
	{
		if (I2CCON1 & (1<<4))	
		{
			if (2 == nCount)
				break;
			//write new data to I2CDS1		
			I2CDS1 = data[nCount];
			//clear penging bit to resume
			I2CCON1 &= ~(1<<4);
			nCount++;
		}
	}

	//STOP
	I2CSTAT1 = 0xd0;
	//clear penging bit to resume
	I2CCON1 &= ~(1<<4);
	//wait until the stop condition tack effect
	i2cDelay(10000);
}

void readI2c(U8 cSlaveAddr, U8 cRegAddr, U8* pcVal)
{
	U32 nCount=0;
	U8 data[2]={cRegAddr};
	//write slaveaddress to I2CDS1
	I2CDS1 = cSlaveAddr;
	//write 0xf0(M/T START) to I2CSTAT1
	I2CSTAT1 = 0xf0;

	while(1)
	{
		if (I2CCON1 & (1<<4))	
		{
			if (1 == nCount)
				break;
			//write new data to I2CDS1		
			I2CDS1 = data[nCount];
			//clear penging bit to resume
			I2CCON1 &= ~(1<<4);
			nCount++;
		}
	}

	//STOP
	I2CSTAT1 = 0xd0;
	//clear penging bit to resume
	I2CCON1 &= ~(1<<4);
	//wait until the stop condition tack effect
	i2cDelay(10000);

	nCount=0;
	I2CDS1 = cSlaveAddr;
	//write 0xb0(M/R START) to I2CSTAT1
	I2CSTAT1 = 0xb0;

	while(1)
	{
		if (I2CCON1 & (1<<4))	
		{
			if (2 == nCount)
				break;

			//read new data
			data[nCount] = I2CDS1&0XFF;
			//*pcVal = I2CDS1;	
			//clear penging bit to resume
			I2CCON1 &= ~(1<<4);
			nCount++;
		}
	}

	//STOP
	I2CSTAT1 = 0x90;
	//clear penging bit to resume
	I2CCON1 &= ~(1<<4);
	//wait until the stop condition tack effect
	i2cDelay(10000);

	*pcVal = data[1];
}

void readI2cBuf(U8 cSlaveAddr, char* pcBuf, U32 nBufSize)
{
	int nCount = 0;
	//write slaveaddress to I2CDS1
	I2CDS1 = cSlaveAddr;
	//write 0xb0(M/R START) to I2CSTAT1
	I2CSTAT1 = 0xb0;

	while(1)
	{
		if (I2CCON1 & (1<<4))	
		{
			if (nCount == nBufSize)
				break;
			//read new data
			pcBuf[nCount] = I2CDS1&0XFF;
			nCount++;
			//clear penging bit to resume
			I2CCON1 &= ~(1<<4);
		}
	}

	//STOP
	I2CSTAT1 = 0x90;
	//clear penging bit to resume
	I2CCON1 &= ~(1<<4);
	//wait until the stop condition tack effect
	i2cDelay(10000);
}
