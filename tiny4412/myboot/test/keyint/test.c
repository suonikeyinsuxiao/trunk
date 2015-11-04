#include "res.h"
#include "mmu.h"
#include "spi.h"

static PRINT_T printf = (void*)0x43E11434;

int _start(void)
{
	enableMMU();
	vectorCopy();//拷贝中断向量表

	enableInterruptCPSR();

	enableExt(64, 26,4, 0);

	return 0;
}


