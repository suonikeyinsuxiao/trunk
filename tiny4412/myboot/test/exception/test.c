#include "res.h"
#include "mmu.h"

//typedef int(*PRINT_F)(const char* fomat, ...);

static PRINT_T printf = (void*)0x43E11434;

//void vectorCopy();

int _start(void)
{
	initExceptHandlers(EXCEPTHANDLERBASEADDR);//开mmu后就不能直接操作物理地址

	enableMMU();

	vectorCopy();



	__asm__ __volatile__ (

//		"ldr r0, =0x5fff0038 \n"
//		"mov lr,pc \n"
//		"mov pc, r0\n"

		//"sub lr, lr, #4 \n"
		//".word 0x77777777 \n"	//undefined exception
		//"nop \n"
	
		"mov lr, pc \n"	
		".word 0x77777777 \n"	//undefined exception
	);


	return 0;
}


