#ifndef _BARRIER_H

#define _BARRIER_H

#include "public.h"



//thread process func
//void* barrierPthdProcess(void* arg);

/**
 * function:movebarrier()
 *
 * Description:移动barrier
 *
 * parameter:
 *		BARRIER_S* psBar barrier对象
 *		int nOffset 偏移量
 *		int nDirect  偏移方向
 *
 * return:
 *
 */
//void moveBarrier(BARRIER_S* psBar, int nOffset, int nDirect);

int initBarrier(GAMEENGINE_S* psGameEngine,int nBarrierNum);

void drawBarrier(GAMEENGINE_S* psGameEngine, int nIndex, int nMoveSpeed);

void destroyBarrier(GAMEENGINE_S* psGameEngine);
#endif
