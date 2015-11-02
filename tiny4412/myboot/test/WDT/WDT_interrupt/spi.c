#include "spi.h"

void enableInterruptCPSR()
{
	__asm__ __volatile__(
	"mrs r0, cpsr \n"			
	"bic r0, r0, #(1 << 7) \n"
	"msr cpsr, r0 \n"
	);
}

void enableSPI(int nID)
{
	//enable CPU interrupt
	ICCICR_CPU0 = 1;
	//set priority mask for cpu
	ICCPMR_CPU0 |= 0xff;
	//enable Distributor control register
	ICDDCR = 1;
	//enable interrupt ID
	*(volatile unsigned int*)(icdbase+ 0x100 + (nID/32)*4) 
		|= 1 << (nID%32);
	//set interrupt priority
	*(volatile unsigned int*) (icdbase + 0x400 + (nID/4)*4)
		&= ~(0xff << ((nID%4)*8));
	//set interrupt target CPU
	*(volatile unsigned int*) (icdbase + 0x800 + (nID/4)*4)
		&=~(0xff << (nID%4)*8);
	*(volatile unsigned int*) (icdbase + 0x800 + (nID/4)*4)
		|= 0x1 << ((nID%4)*8);
}
