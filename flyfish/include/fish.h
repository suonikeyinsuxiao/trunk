#ifndef _FISH_H
#define _FISH_H

#include "public.h"



//thread process func
void* fishPthdProcess(void* arg);

/**
 * function:moveFish()
 *
 * Description:移动鱼
 *
 * parameter:
 *		FISH_S* psFish 鱼对象
 *		int nOffset 偏移量
 *		int nDirect  偏移方向
 *
 * return:
 *
 */
void moveFish(FISH_S* psFish, int nOffset, int nDirect);

int initFish(GAMEENGINE_S* psGameEngine);

void drawFish(GAMEENGINE_S* psGameEngine,int nIndex, int nXoffset, int nYoffset);

void destroyFish(GAMEENGINE_S* psGameEngine);
#endif
