#include "sgi.h"

void enableInterruptCPSR()
{
	__asm__ __volatile__(
	"mrs r0, cpsr \n"			
	"bic r0, r0, #(1 << 7) \n"
	"msr cpsr, r0 \n"
	);
}

void enableSGI(unsigned int nID)
{
	//enable CPU interrupt
	ICCICR_CPU0 = 1;
	//set CPU priority mask
	ICCPMR_CPU0 |= 0xff;
	//enable Distributor 
	ICDDCR = 1;
	//enable interrupt ID
	//The Distributor does not provide registers for INTIDs less than 16 because SGIs are always enabled.	
	//set interrupt priority
	(*(volatile unsigned int*)(icdbase + 0x0400 + ((nID/4)<<2))) &= ~(0xff << ((nID%4)<<4));
	//set interrupt target CPU
	(*(volatile unsigned int*)(icdbase + 0x0800 + ((nID/4)<<2))) &= ~(0xff << ((nID%4)<<4));
	(*(volatile unsigned int*)(icdbase + 0x0800 + ((nID/4)<<2))) |= (0x1 << ((nID%4)<<4));
}

void startSGI(unsigned int nID)
{
	ICDSGIR = (0x1 << 16) | (nID);
}
