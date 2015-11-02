#include "mmu.h"
#include "wdt.h"


static PRINT_T printf = (void*)0x43E11434;

//reset exception handler
void doReset(void)
{
	printf("doReset...\n");
}

//undefined exception handler
void doUndefined(void)
{
	printf("doUndefined...\n");
}

//svc exception handler
void doSvc(void)
{
	printf("doSvc...\n");
}

//prefetch abort exception handler
void doPrefetchAbt(void)
{
	printf("doPrefetchAbt...\n");
}

//data abort exception handler
void doDataAbt(void)
{
	printf("doDataAbt...\n");
}

//irq exception handler
void doIrq(void)
{
	WTCLRINT = 1;
	printf("doIrq...\n");
}

//fiq exception handler
void doFiq(void)
{
	printf("doFIQ...\n");
}

//void initExceptHandlers(unsigned long long nPhyBaseAddr)
void initExceptHandlers()
{
	*(unsigned int *)0x51000000 = (unsigned int )doReset ; 
	*(unsigned int *)0x51000004 = (unsigned int )doUndefined; 
	*(unsigned int *)0x51000008 = (unsigned int )doSvc; 
	*(unsigned int *)0x5100000C = (unsigned int )doPrefetchAbt; 
	*(unsigned int *)0x51000010 = (unsigned int )doDataAbt; 
	*(unsigned int *)0x51000014 = (unsigned int )doIrq; 
	*(unsigned int *)0x51000018 = (unsigned int )doFiq; 


	//(*(unsigned int*)nPhyBaseAddr) = 			(unsigned int)doReset;
	//(*(unsigned int*)(nPhyBaseAddr + 4)) = 	(unsigned int)doUndefined;
	//(*(unsigned int*)(nPhyBaseAddr + 8)) = 	(unsigned int)doSvc;
	//(*(unsigned int*)(nPhyBaseAddr + 0xc)) = 	(unsigned int)doPrefetchAbt;
	//(*(unsigned int*)(nPhyBaseAddr + 0x10)) = (unsigned int)doDataAbt;
	//(*(unsigned int*)(nPhyBaseAddr + 0x14)) = (unsigned int)doIrq;
	//(*(unsigned int*)(nPhyBaseAddr + 0x18)) = (unsigned int)doFiq;
}

void vpMap(unsigned int *addr, unsigned va, unsigned pa)
{
	addr[va>>20] = (pa & (~0xfffff)) | 2;
}

void initTTB(unsigned int *addr)
{
	unsigned int pa , va ; 
	//规定倒数第二位必须为1
	//系统 ROM  RAM
	for(va = 0x00000000 ; va < 0x10000000 ; va+=0x100000)
	{
		pa = va; 
		vpMap(addr, va, pa);
	}
	//SFR
	for(va = 0x10000000 ; va < 0x14000000 ; va+= 0x100000)
	{
		pa = va; 
		vpMap(addr, va, pa);
	}
	//DRAM  内存区域
	for(va = 0x40000000 ; va < 0x80000000 ; va+= 0x100000)
	{
		pa = va; 
		vpMap(addr, va, pa);
	}
}

void enableMMU()
{
	//one level section, 4K->1M
	volatile unsigned int val ; 
	unsigned int va = 0xffff0000 ; //中断向量表的虚拟地址
	unsigned int pa = 0x5fff0000 ;//中断向量表的物理地址 

	//1.set translation table base address 
	unsigned int nTTBAddr = TTBADDR ; 
	
	//2.clear translation table(16K)
	memset((char *)nTTBAddr , 0 , 16*1024);

	//3.设置虚拟地址和物理地址的总体映射布局 
	initTTB((unsigned int *)nTTBAddr);

	//4.自定义映射关系
	vpMap((unsigned int *)nTTBAddr , va , pa);
	
	//enable  mmu 
	//这三步都是通过协处理器CP15来设置

	unsigned int nMMUconfig = 0 ; 
	nMMUconfig |= 1 | (1 << 11) |(1 << 13) ; 

	__asm__ __volatile__ (

	//1.给定domain满权限
	"ldr	r0 , =0xffffffff      \n"
	"MCR p15, 0, r0, c3, c0, 0    \n"

	//2.告诉TTB表的位置
	"mov	r0 , %0               \n"
	"MCR p15, 0, r0  , c2, c0, 0  \n"
	
	//Enable MMU
	//3.开MMU
	"MRC p15, 0, r0, c1, c0, 0    \n"
	"orr	r0 , r0 , %1          \n"
	"MCR p15, 0, r0, c1, c0, 0    \n"

	::"r"(nTTBAddr),"r"(nMMUconfig)
	:"r0"
	);
}

void vectorCopy()
{
	extern void _start_vector(void);
	extern void _end_vector(void);

	unsigned int len = 0;

	len = (unsigned int)_end_vector - (unsigned int)_start_vector;
	
	printf("vector len=%d\n", len);

	memcpy((char*)0x5fff0000, (char*)_start_vector, len);
}
