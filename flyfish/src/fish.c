
#include"fish.h"
#include"image.h"


/*
 *init success return 1, else return 0
 * */
int initFish(GAMEENGINE_S* psGameEngine)
{
	if (NULL == psGameEngine)
	{
		printf("[%s,%d]init fish failed!\n", __func__, __LINE__);	
		return 0;
	}
	
    int i = 0;
	for (i = 0; i < FISHSHAPE; i++)
	{
		if (NULL == psGameEngine->m_psaFishSurf[i])
		{
			SDL_Surface* psTmp = IMG_Load(psGameEngine->m_pcaFishPath[i]);
			psGameEngine->m_psaFishSurf[i] = SDL_DisplayFormat(psTmp);
			SDL_FreeSurface(psTmp);
//			printf("psGameEngine->m_psaFishSurf[%d]=%p,psGameEngine->m_pcaFishPath[i]=%s\n",i,  psGameEngine->m_psaFishSurf[i], psGameEngine->m_pcaFishPath[i]);
		}

		if (NULL == psGameEngine->m_psaFish[i])
		{
			psGameEngine->m_psaFish[i] = malloc(sizeof(FISH_S));
			if(NULL == psGameEngine->m_psaFish[i])
			{
				printf("[%s,%d]malloc fish failed!\n", __func__, __LINE__);	
				return 0;
			}
		}

    	psGameEngine->m_psaFish[i]->m_nIndex = i;
    	psGameEngine->m_psaFish[i]->m_nLife = FISHLIFE;
    	psGameEngine->m_psaFish[i]->m_nX = FISHSTARTPOINT_X;
    	psGameEngine->m_psaFish[i]->m_nY = FISHSTARTPOINT_y;
    	psGameEngine->m_psaFish[i]->m_nW = FISHWIDTH;
    	psGameEngine->m_psaFish[i]->m_nH = FISHHEIGH;
    	psGameEngine->m_psaFish[i]->m_nYoffset = 0;
    	psGameEngine->m_psaFish[i]->m_nXoffset = 0;

	}
	
	return 1;
}

void drawFish(GAMEENGINE_S* psGameEngine, int nIndex, int nXoffset, int nYoffset)
{
	if (NULL == psGameEngine)	
	{
		printf("[%s,%d]NULL == psGameEngine\n", __func__, __LINE__);	
		return ;
	}
	
	int i = 0;
	for (i = 0; i < FISHSHAPE; i++)
	{
		psGameEngine->m_psaFish[i]->m_nXoffset = nXoffset;
		psGameEngine->m_psaFish[i]->m_nYoffset = nYoffset;
		psGameEngine->m_psaFish[i]->m_nX += nXoffset;
		psGameEngine->m_psaFish[i]->m_nY += nYoffset;
	}
	

	SDL_Rect sRect = {
		.x = psGameEngine->m_psaFish[nIndex]->m_nX,	
		.y = psGameEngine->m_psaFish[nIndex]->m_nY,	
		.w = psGameEngine->m_psaFish[nIndex]->m_nW,	
		.h = psGameEngine->m_psaFish[nIndex]->m_nH 
	};
//	printf("draw fish0 psGameEngine->m_psaFishSurf[nIndex]=%p\n",psGameEngine->m_psaFishSurf[nIndex]);
	unsigned int key = SDL_MapRGB(psGameEngine->m_psaFishSurf[nIndex]->format, 0, 0, 0);

	//printf("key:%d\n", key);
	SDL_SetColorKey(psGameEngine->m_psaFishSurf[nIndex], SDL_SRCCOLORKEY|SDL_RLEACCEL, key); 
	//printf("ret:%d\n", ret);

	SDL_BlitSurface(psGameEngine->m_psaFishSurf[nIndex], NULL, psGameEngine->m_psMainScreen, &sRect);
//	SDL_Flip(psGameEngine->m_psMainScreen);
}

void destroyFish(GAMEENGINE_S* psGameEngine)
{
	if (NULL == psGameEngine)	
	{
		printf("[%s,%d]NULL ==psGameEngine\n", __func__, __LINE__);	
		return ;
	}

	int i = 0;
	for (i = 0; i < FISHSHAPE; i++)
	{
		if (NULL != psGameEngine->m_psaFish[i] && NULL != psGameEngine->m_psaFishSurf[i])	
		{
			//printf("free\n");
			free(psGameEngine->m_psaFish[i]);
			psGameEngine->m_psaFish[i] = NULL;
			SDL_FreeSurface(psGameEngine->m_psaFishSurf[i]);
			psGameEngine->m_psaFishSurf[i] = NULL;

		}
	}
}
