#include "res.h"
#include "lcd.h"
#include "font.h"

//static PRINT_T printf = (void*)0x43E11434;

int _start(void)
{
	int i = 0;
	int j = 0;
	unsigned int col = 0xff;
	int x=100;
	int y=100;
	U32 nBlack = 0;
	U32 nWhite = 0xffffff;

	unsigned int (*p)[800] = (void*)FBAddr;

	initLcd(FBAddr);

	for (i = 0; i < 480; i++)	
	{
		for (j = 0; j < 800; j++)	
		{
			p[i][j]	 = col;			
		}
	}

	drawChar('y',x, y, nWhite, nBlack);
	drawStr("\nhello everyone!\n", x, y, nWhite, nBlack);

	return 0;
}

#if 0
//void drawCh(char ch,int x, int y,  unsigned int (*p)[800]);
//draw a charactor
void drawCh(char ch, int x, int y, unsigned int (*addr)[800])
{
	int i = 0;
	int j = 0;

	char* pc = &asc8x6[ch*8];
	unsigned int *pnTmp = NULL;
	for (i = 0; i < 8; i++)
	{
		for (j = 0; j < 8; j++)	
		{
			pnTmp = &addr[y+j*4][x+i*4];
			if (0x1 & ((*(pc+i)) >> j))
			{
				*pnTmp = 0xffffff;	
				*(pnTmp+1) = 0xffffff;	
				*(pnTmp+2) = 0xffffff;	
				*(pnTmp+3) = 0xffffff;	
			}
			else
			{
				*pnTmp = 0;	
				*(pnTmp+1) = 0;	
				*(pnTmp+2) = 0;	
				*(pnTmp+3) = 0;	
			}
		}
	}
}
#endif

