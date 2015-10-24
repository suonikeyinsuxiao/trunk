#include"gameover.h"

int initGameOver(SDL_Surface** ppsGameOver, const char* pcGameOverPath)
{

    if (!fileExist(pcGameOverPath))//file is exist
		return 0;
	
	SDL_Surface* psTmp = IMG_Load(pcGameOverPath);
	*ppsGameOver = SDL_DisplayFormat(psTmp);
	SDL_FreeSurface(psTmp);

	return 1;
}

void drawGameOver(SDL_Surface* psGameOver, SDL_Surface* psDstSurface, SDL_Rect* psDstRect)
{
	if (NULL == psGameOver || NULL == psDstSurface || NULL == psDstRect)
	{
		printf("NULL == psGameOver || NULL == psDstSurface || NULL == psDstRect\n");	
		return;
	}

	unsigned int key = SDL_MapRGB(psGameOver->format, 0, 0, 0);

	SDL_SetColorKey(psGameOver, SDL_SRCCOLORKEY|SDL_RLEACCEL, key); 

	SDL_BlitSurface(psGameOver, NULL, psDstSurface, psDstRect);

}

void destroyGameOver(SDL_Surface* psGameOver)
{
	if (NULL != psGameOver)
	{
		SDL_FreeSurface(psGameOver);	
		psGameOver = NULL;
	}

}
