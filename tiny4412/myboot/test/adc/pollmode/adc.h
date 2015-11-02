#ifndef __ADC_H
#define __ADC_h

#define adcBase 0x126c0000
#define ADCCON    *(volatile unsigned int *)(adcBase+0x0000) 
#define ADCDLY    *(volatile unsigned int *)(adcBase+0x0008) 
#define ADCDAT    *(volatile unsigned int *)(adcBase+0x000C) 
#define CLRINTADC *(volatile unsigned int *)(adcBase+0x0018) 
#define ADCMUX    *(volatile unsigned int *)(adcBase+0x001C) 
#define ADC_CFG   *(volatile unsigned int *)(0x10010000+0x0118)

typedef int(*PRINT_T)(const char* fmt, ...);

void initADC();
void delay(void);
void startADC();
int readADC();
#endif	//__ADC_H
