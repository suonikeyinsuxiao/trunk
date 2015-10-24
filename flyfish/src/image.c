#include"image.h"


int loadImage(const char* pcFilePath, SDL_Surface** ppsSurface)
{
    if (!fileExist(pcFilePath))//file is exist
		return 0;

	SDL_Surface* psTmp = IMG_Load(pcFilePath);
	*ppsSurface = SDL_DisplayFormat(psTmp);
	SDL_FreeSurface(psTmp);

	return 1;
}

void drawImage(SDL_Surface* psSrc, SDL_Surface* psDst, SDL_Rect* psDstRect)
{
    if (NULL == psSrc || NULL ==  psDst || NULL == psDstRect)
	{
		printf("%s,%d, input parameters error!\n", __func__, __LINE__);	
		return;
	}
	SDL_BlitSurface(psSrc, NULL, psDst, psDstRect);

	return;
}
