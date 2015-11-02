#include "wdt.h"
#include "spi.h"
#include "mmu.h"

static PRINT_T  printf = (void *)0x43E11434 ;

int _start(void)
{
	initExceptHandlers();
	enableMMU();
	vectorCopy();

	enableInterruptCPSR();

	enableSPI(75);

	WDTConfig_S sWDT;
	memset(&sWDT, 0, sizeof(WDTConfig_S));

	sWDT.m_nPrescaler = 99;			//100
	sWDT.m_nDivision = 3;			//128
	sWDT.m_nEnableWDT = 1;
	sWDT.m_nEnableInterrupt = 1;
	sWDT.m_nEnableReset =0;
	sWDT.m_nCount = 7812*3;			//3s reset

	initWDT(&sWDT);

	return 0;
}
