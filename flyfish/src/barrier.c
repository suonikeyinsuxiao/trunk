
#include "barrier.h"

//#define MINHEIGHT 50
//#define MAXHEIGHT ((WINHEIGH-FISHHEIGH)/2 - MINHEIGHT)

#define HEIGHTSUM 300 
#define INTERVAL 105

int initBarrier(GAMEENGINE_S* psGameEngine, int nBarrierNum)
{
	int i = 0;
    int h = 0;

	for (i = 0; i < nBarrierNum; i=i+2)
	{
		if (NULL == psGameEngine->m_psaBarrierSurf[i%2])
		{
			SDL_Surface* psTmp = IMG_Load(psGameEngine->m_pcaBarrierPath[i%2]);
			psGameEngine->m_psaBarrierSurf[i%2] = SDL_DisplayFormat(psTmp);
			SDL_FreeSurface(psTmp);
		}

		if (NULL == psGameEngine->m_psaBarrierSurf[i%2+1])
		{
			SDL_Surface* psTmp = IMG_Load(psGameEngine->m_pcaBarrierPath[i%2+1]);
			psGameEngine->m_psaBarrierSurf[i%2+1] = SDL_DisplayFormat(psTmp);
			SDL_FreeSurface(psTmp);
		}
	
		if (NULL == psGameEngine->m_psaBarrier[i])
		{
			psGameEngine->m_psaBarrier[i] = malloc(sizeof(BARRIER_S));
			if(NULL == psGameEngine->m_psaBarrier[i])
			{
				printf("[%s,%d]malloc barrier failed!\n", __func__, __LINE__);	
				return 0;
			}
		}

		if (NULL == psGameEngine->m_psaBarrier[i+1])
		{
			psGameEngine->m_psaBarrier[i+1] = malloc(sizeof(BARRIER_S));
			if(NULL == psGameEngine->m_psaBarrier[i+1])
			{
				printf("[%s,%d]malloc barrier failed!\n", __func__, __LINE__);	
				return 0;
			}
		}
		
		h = rand()%(HEIGHTSUM);

		//down
		
			psGameEngine->m_psaBarrier[i]->m_nH = h;//rand()%MAXHEIGHT + MINHEIGHT;	//to be radom
			psGameEngine->m_psaBarrier[i]->m_nX = WINWIDTH-(i/2)*(BARRIERWIDTH+INTERVAL);	
			psGameEngine->m_psaBarrier[i]->m_nY = WINHEIGH - psGameEngine->m_psaBarrier[i]->m_nH;	
			psGameEngine->m_psaBarrier[i]->m_nW = BARRIERWIDTH;	
			psGameEngine->m_psaBarrier[i]->m_nMoveSpeed = 20;	
			psGameEngine->m_psaBarrier[i]->m_nBarrierOffset = WINWIDTH-BARRIERWIDTH;	
		
		//up
		
			psGameEngine->m_psaBarrier[i+1]->m_nH = HEIGHTSUM - h;//rand()%MAXHEIGHT + MINHEIGHT;	
			psGameEngine->m_psaBarrier[i+1]->m_nY = 0;	
			psGameEngine->m_psaBarrier[i+1]->m_nX = WINWIDTH-(i/2)*(BARRIERWIDTH+INTERVAL);	
			psGameEngine->m_psaBarrier[i+1]->m_nW = BARRIERWIDTH;	
			psGameEngine->m_psaBarrier[i+1]->m_nMoveSpeed = 20;	
			psGameEngine->m_psaBarrier[i+1]->m_nBarrierOffset = WINWIDTH-BARRIERWIDTH;	
		
	}

	return 1;
}

void drawBarrier(GAMEENGINE_S* psGameEngine, int nIndex, int nMoveSpeed)
{
	if (NULL == psGameEngine)	
	{
		printf("[%s,%d]NULL == psGameEngine\n", __func__, __LINE__);
		return ;
	}

	SDL_Rect sSrcUpRect;
	SDL_Rect sSrcDownRect;
	SDL_Rect sDstUpRect;
	SDL_Rect sDstDownRect;

	int h = rand()%(HEIGHTSUM);

	if (psGameEngine->m_psaBarrier[nIndex]->m_nMoveSpeed != nMoveSpeed)
	{
		psGameEngine->m_psaBarrier[nIndex]->m_nMoveSpeed = nMoveSpeed;
		psGameEngine->m_psaBarrier[nIndex+1]->m_nMoveSpeed = nMoveSpeed;
	}

	psGameEngine->m_psaBarrier[nIndex]->m_nX -= psGameEngine->m_psaBarrier[nIndex]->m_nMoveSpeed;
	psGameEngine->m_psaBarrier[nIndex+1]->m_nX -= psGameEngine->m_psaBarrier[nIndex+1]->m_nMoveSpeed;
	if (1 > psGameEngine->m_psaBarrier[nIndex]->m_nX)
	{
			//up
			psGameEngine->m_psaBarrier[nIndex+1]->m_nH = HEIGHTSUM - h;	
			psGameEngine->m_psaBarrier[nIndex+1]->m_nY = 0;	
			psGameEngine->m_psaBarrier[nIndex+1]->m_nX = WINWIDTH-(BARRIERWIDTH+INTERVAL);	
			psGameEngine->m_psaBarrier[nIndex+1]->m_nW = BARRIERWIDTH;	

			//down	
			psGameEngine->m_psaBarrier[nIndex]->m_nH = h;	//to be radom
			psGameEngine->m_psaBarrier[nIndex]->m_nX = WINWIDTH-(BARRIERWIDTH+INTERVAL);	
			psGameEngine->m_psaBarrier[nIndex]->m_nY = WINHEIGH - psGameEngine->m_psaBarrier[nIndex]->m_nH;	
			psGameEngine->m_psaBarrier[nIndex]->m_nW = BARRIERWIDTH;	
	}
	
			//up	
			sSrcUpRect.h = psGameEngine->m_psaBarrier[nIndex+1]->m_nH;
			sSrcUpRect.y = BARRIERHEIGH - sSrcUpRect.h;
			sSrcUpRect.x = 0;
			sSrcUpRect.w = psGameEngine->m_psaBarrier[nIndex+1]->m_nW;
		
			//down
			sSrcDownRect.x = 0;
			sSrcDownRect.y = 0;
			sSrcDownRect.h = psGameEngine->m_psaBarrier[nIndex]->m_nH;
			sSrcDownRect.w = psGameEngine->m_psaBarrier[nIndex]->m_nW;
		

		sDstUpRect.x = psGameEngine->m_psaBarrier[nIndex+1]->m_nX;
		sDstUpRect.y = psGameEngine->m_psaBarrier[nIndex+1]->m_nY;
		sDstUpRect.w = 0;
		sDstUpRect.h = 0;

		sDstDownRect.x = psGameEngine->m_psaBarrier[nIndex]->m_nX;
		sDstDownRect.y = psGameEngine->m_psaBarrier[nIndex]->m_nY;
		sDstDownRect.w = 0;
		sDstDownRect.h = 0;

		SDL_BlitSurface(psGameEngine->m_psaBarrierSurf[nIndex%2+1], &sSrcUpRect, psGameEngine->m_psMainScreen, &sDstUpRect);
		SDL_BlitSurface(psGameEngine->m_psaBarrierSurf[nIndex%2], &sSrcDownRect, psGameEngine->m_psMainScreen, &sDstDownRect);
}

void destroyBarrier(GAMEENGINE_S* psGameEngine)
{
	if (NULL == psGameEngine)	
	{
		printf("[%s,%d]NULL == psGameEngine\n", __func__, __LINE__);
		return ;
	}

	int i = 0;
	for (i = 0; i < BARRIERNUM; i++)
	{
		if (NULL != psGameEngine->m_psaBarrier[i])	
		{
			//printf("free\n");
			free(psGameEngine->m_psaBarrier[i]);
			psGameEngine->m_psaBarrier[i] = NULL;
		}
		if (NULL != psGameEngine->m_psaBarrierSurf[i%2])
		{
			SDL_FreeSurface(psGameEngine->m_psaBarrierSurf[i%2]);
			psGameEngine->m_psaBarrierSurf[i%2] = NULL;
		}
	}
}
