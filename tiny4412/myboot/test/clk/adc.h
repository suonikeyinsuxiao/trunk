#ifndef __ADC_H
#define __ADC_h

#define adcBase 0x126c0000
#define ADCCON    *(volatile unsigned int *)(adcBase+0x0000) 
#define ADCDLY    *(volatile unsigned int *)(adcBase+0x0008) 
#define ADCDAT    *(volatile unsigned int *)(adcBase+0x000C) 
#define CLRINTADC *(volatile unsigned int *)(adcBase+0x0018) 
#define ADCMUX    *(volatile unsigned int *)(adcBase+0x001C) 
#define ADC_CFG   *(volatile unsigned int *)(0x10010000+0x0118)

#define intCmbBase 0x10440000
#define	IESR0     *(volatile unsigned int *)(intCmbBase+0x0000)	// Interrupt enable set register for group 0 to 3      0x00000000
#define	IECR0     *(volatile unsigned int *)(intCmbBase+0x0004)	// Interrupt enable clear register for group 0 to 3    0x00000000
#define	ISTR0     *(volatile unsigned int *)(intCmbBase+0x0008)	// Interrupt status register for group 0 to 3           Undefined
#define	IMSR0     *(volatile unsigned int *)(intCmbBase+0x000C)	// Interrupt masked status register for group 0 to 3    Undefined
#define	IESR1     *(volatile unsigned int *)(intCmbBase+0x0010)	// Interrupt enable set register for group 4 to 7      0x00000000
#define	IECR1     *(volatile unsigned int *)(intCmbBase+0x0014)	// Interrupt enable clear register for group 4 to 7    0x00000000
#define	ISTR1     *(volatile unsigned int *)(intCmbBase+0x0018)	// Interrupt status register for group 4 to 7           Undefined
#define	IMSR1     *(volatile unsigned int *)(intCmbBase+0x001C)	// Interrupt masked status register for group 4 to 7    Undefined
#define	IESR2     *(volatile unsigned int *)(intCmbBase+0x0020)	// Interrupt enable set register for group 8 to 11     0x00000000
#define	IECR2     *(volatile unsigned int *)(intCmbBase+0x0024)	// Interrupt enable clear register for group 8 to 11   0x00000000
#define	ISTR2     *(volatile unsigned int *)(intCmbBase+0x0028)	// Interrupt status register for group 8 to 11          Undefined
#define	IMSR2     *(volatile unsigned int *)(intCmbBase+0x002C)	// Interrupt masked status register for group 8 to 11   Undefined
#define	IESR3     *(volatile unsigned int *)(intCmbBase+0x0030)	// Interrupt enable set register for group 12 to 15    0x00000000
#define	IECR3     *(volatile unsigned int *)(intCmbBase+0x0034)	// Interrupt enable clear register for group 12 to 15  0x00000000
#define	ISTR3     *(volatile unsigned int *)(intCmbBase+0x0038)	// Interrupt masked status register for group 12 to 15  Undefined
#define	IMSR3     *(volatile unsigned int *)(intCmbBase+0x003C)	// Interrupt status register for group 16 to 17         Undefined
#define	IESR4     *(volatile unsigned int *)(intCmbBase+0x0040)	// Interrupt enable set register for group 16 to 17    0x00000000
#define	IECR4     *(volatile unsigned int *)(intCmbBase+0x0044)	// Interrupt enable clear register for group 16 to 17  0x00000000
#define	ISTR4     *(volatile unsigned int *)(intCmbBase+0x0048)	// Interrupt masked status register for group 16 to 17  Undefined
#define	IMSR4     *(volatile unsigned int *)(intCmbBase+0x004C)	// Interrupt status register for group 16 to 17         Undefined
#define	CIPSR0    *(volatile unsigned int *)(intCmbBase+0x0100)	// Combined interrupt pending status0                   Undefined
	 

void initADC();
void delay(void);
void startADC();
int readADC();
void configADCInt();
#endif	//__ADC_H
