#include "res.h"
//#include "mmu.h"
//#include "sgi.h"
#include "lcd.h"
#define FBAddr 0x60000000
unsigned int  nFBaddr = 0x60000000;

//static PRINT_T printf = (void*)0x43E11434;

int _start(void)
{
	int i = 0;
	int j = 0;
	unsigned int col = 0xff;

	//unsigned int (*p)[800] = (void*)nFBaddr;
	//initLcd(nFBaddr);

	unsigned int (*p)[800] = (void*)FBAddr;
	initLcd(FBAddr);

#if 0
	while(1)
	{
		for (i = 0; i < 480; i++)	
		{
			for (j = 0; j < 800; j++)	
			{
				p[i][j]	 = col;			
			}
		}

		col = col << 8;
		if (col == 0xff000000)
			col = 0xff;

	}
#endif
		
	return 0;
}

