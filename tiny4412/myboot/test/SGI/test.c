#include "res.h"
#include "mmu.h"
#include "sgi.h"

static PRINT_T printf = (void*)0x43E11434;

int _start(void)
{
	enableMMU();

	vectorCopy();

	enableInterruptCPSR();

	enableSGI(5);

	startSGI(5);
	return 0;
}

