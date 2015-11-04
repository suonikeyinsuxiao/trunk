#include "spi.h"

static PRINT_T printf = (void*)0x43E11434;

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


#if 0
struct {
	unsigned int m_nId;
	unsigned int m_nEXId;
	unsigned int m_nTigger;
	unsigned int m_nMask;
	unsigned int m_nBaseAddr;
}extIntConf_S;
//enableExt接口封装使用
#endif

void enableExt(int nId, int nEXId, int nTigger, int nMask)
{

	enableSPI(nId);
	//set GPX3_2 interrupt function
	#define   GPXCON   0x11000C00
	//GPX3CON |= 0xf << 8;
	*(volatile unsigned int*)(GPXCON + ((nEXId/8) << 5))
		&= ~(0xf << ((nEXId%8) << 2));
	*(volatile unsigned int*)(GPXCON + ((nEXId/8) << 5))
		|= (0xf << ((nEXId%8) << 2));

	//set trigger
	*(volatile unsigned int*)(EXT_INT_BASE + ((nEXId/8)<<2))
		&= ~(0xf << ((nEXId%8) << 2));
	*(volatile unsigned int*)(EXT_INT_BASE + ((nEXId/8)<<2))
		|= (nTigger << ((nEXId%8) << 2));
	//set filter
	*(volatile unsigned int*)(EXT_INT_BASE + 0x80 + ((nEXId/4)<<2))
		|= (0xff << ((nEXId%4) << 3));
	//set mask
	*(volatile unsigned int*)(EXT_INT_BASE + 0x100 + ((nEXId/8)<<2))
		&= ~(1 << (nEXId%8));
}

