#include "font.h"
#include "res.h"
#include "asc8x6.h"

#define WIDTH 	800
#define HEIGHT 	480

static U32 (*pAddr)[WIDTH] = (void*)FBAddr;

void drawChar(char ch, int nRow, int nCol, U32 nColorBack, U32 nColorFront)
{
	int i = 0;
	int j = 0;
	char tmp;

	for (i = 0; i < 8; ++i)
	{
		tmp = asc8x6[ch*8+i];
		for (j = 0; j < 8; ++j)	
		{
			if (0x1 & (tmp >> j))	
				pAddr[nCol+j][nRow+i] = nColorFront; 
			else
				pAddr[nCol+j][nRow+i] = nColorBack;
		}
	}
}

void drawStr(char* pcStr, int nRow, int nCol, U32 nColorBack, U32 nColorFront)
{
	int i = 0;

	while(pcStr[i])
	{
		if (pcStr[i] == '\n')
		{
			nCol += 8;	
			nRow = 0;
//			drawChar(pcStr[i], nRow, nCol, 0xff, 0xff);	
		}
		else
		{
			drawChar(pcStr[i], nRow, nCol, nColorBack, nColorFront);	
			nRow += 8;
		}

		i++;
	}
}
