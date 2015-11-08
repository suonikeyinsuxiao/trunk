#include "res.h"
#include "mmu.h"
#include "spi.h"
#include "clk.h"
//#include "pwm.h"

static PRINT_T printf = (void*)0x43E11434;

int _start(void)
{

	printf("CLK_DIV_LCD=%#x\n",CLK_DIV_LCD);
	printf("CLK_SRC_LCD0=%#x\n",CLK_SRC_LCD0);
	printf(" CLK_DIV_TOP =%#x  \n",CLK_DIV_TOP );
	printf(" CLK_SRC_TOP0 =%#x  \n",CLK_SRC_TOP0);
	printf("CLK_SRC_DMC=%#x\n",CLK_SRC_DMC);
	printf(" CLK_SRC_TOP1 =%#x  \n",CLK_SRC_TOP1);




//	initExceptHandlers();//开mmu后就不能直接操作物理地址
//
//	enableMMU();
//
//	vectorCopy();
//
//	enableInterruptCPSR();
//	enableSPI(42);
//	configADCInt();
//
//	initADC();
//	startADC();

	return 0;
}


