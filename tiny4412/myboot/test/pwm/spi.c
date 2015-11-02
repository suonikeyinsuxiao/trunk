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

void initTimer0()
{
	//enable timer0 irq
	enableSPI(69);

//	//xpwmTOUT0
//	GPD0CON &= ~0xf ; 
//	GPD0CON |= 0x2 ; 

	//prescare 0     100M / 100 ==> 1M
	TCFG0 &= ~0xff ; 
	TCFG0 |= 99 ; 

	//prescare 1
	TCFG1 &= ~0xf ;    //downcouter 1M HZ

	//设置时间 
	TCNTB0 = 1000000 ;   //1M

	//compare
//	TCMPB0 = 1000 ;    //0.1 M 

	TCON &= ~0xf ; 
	TCON |= 0x1 << 3 ; //set autoreload
//	TCON |= 0x1 << 2 ; //invert-on-->0(high level)  invert-off-->1

	//要将TCNTB TCMPB 加载进TCNT  TCMP		
	TCON |= 1 << 1 ; 
	//加载完之后要，清掉
	TCON &= ~(1 << 1) ; 

	//开启中断
	TINT_CSTAT |= 1 ; 

	//开启PWM
	TCON |= 1 ;
}
