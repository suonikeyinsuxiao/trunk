#ifndef __WDT_H 
#define __WDT_H

typedef int (*PRINT_T)(const char* fmt, ...);

#define WDTBASEADDR 0x10060000
#define WTCON    *(volatile unsigned int*)(WDTBASEADDR+0x0000) ///W atchdog timer control register         0x0000_8021
#define WTDAT    *(volatile unsigned int*)(WDTBASEADDR+0x0004) ///W atchdog timer data register            0x0000_8000
#define WTCNT    *(volatile unsigned int*)(WDTBASEADDR+0x0008) ///W atchdog timer count register           0x0000_8000
#define WTCLRINT *(volatile unsigned int*)(WDTBASEADDR+0x000C) ///W atchdog timer interrupt clear register  Undefined

#define    AUTOMATIC_WDT_RESET_DISABLE  *(volatile unsigned int *)(0x10020408)
#define    MASK_WDT_RESET_REQUEST 		*(volatile unsigned int *)(0x1002040C)

typedef struct _WDT
{
	unsigned int m_nPrescaler;
	unsigned int m_nDivision;
	unsigned int m_nEnableWDT;
	unsigned int m_nEnableInterrupt;
	unsigned int m_nEnableReset;
	unsigned int m_nCount;
}WDTConfig_S;

void initWDT(WDTConfig_S* psWDT);
void feedDog(unsigned int count);

#endif
