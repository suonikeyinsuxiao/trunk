#include "res.h"
//#include "lcd.h"
#include "iic.h"

static PRINT_T printf = (void*)0x43E11434;
#define COUNT 256

int _start(void)
{
	int i = 0;
	unsigned char pcBuf[COUNT] = {};
//	U8 val=0;	
//	//initLcd(FBAddr);
	initI2c();
//	writeI2c(0x70, 0xac, 100);
//	readI2c(0x70, 0xac, &val);
//	printf("val=%d\n",val);

	readI2cBuf(0x70, pcBuf, COUNT);

	for (i = 0; i < COUNT; i++)
	{
		printf("%#x	",pcBuf[i]);	
		if (!(i%8))
			printf("\n");
	}

	return 0;
}
