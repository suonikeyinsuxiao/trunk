#include "res.h"
#include "mmu.h"
#include "spi.h"

static PRINT_T printf = (void*)0x43E11434;

int _start(void)
{
	initExceptHandlers();//开mmu后就不能直接操作物理地址

	enableMMU();

	vectorCopy();

	enableInterruptCPSR();

	initTimer0();


	return 0;
}


