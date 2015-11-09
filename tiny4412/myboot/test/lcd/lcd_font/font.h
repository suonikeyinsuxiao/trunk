#ifndef __FONT_H
#define __FONT_H
#include "res.h"

void drawChar(char ch, int nRow, int nCol, U32 nColorBack, U32 nColorFront);
void drawStr(char* pcStr, int nRow, int nCol, U32 nColorBack, U32 nColorFront);

#endif
