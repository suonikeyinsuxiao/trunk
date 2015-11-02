#include "adc.h"

PRINT_T printf= (void*)0x43E11434;

int _start(void)
{
	int i = 0;
	unsigned int val=0;
	initADC();
	startADC();

	for (i = 0; i < 50; ++i)
	{
		printf("data:%#x\n", readADC());
		startADC();
	}
	

	return 0;
}

void initADC()
{
	printf("initADC...\n");

	ADC_CFG = 0; 	
	//clean
	//ADCCON &= ~(0x1ffff);

	ADCCON &= ~(1 << 16);
	//prescaler enable
	ADCCON |= 1 << 14;
	//prescaler value
	ADCCON &= ~(0xff << 6);
	ADCCON |= 19 << 6;
	//mode
	ADCCON &= ~(1<<2);

	//AIN0
	ADCMUX &= ~0xf;
}

void startADC()
{
	printf("startADC...\n");
	ADCCON |= 1;
}

int readADC()
{
	printf("readADC...\n");
//	while(ADCCON & 0x1);
	while(!(ADCCON & (1 << 15)));

	return (ADCDAT&0xfff);
}



