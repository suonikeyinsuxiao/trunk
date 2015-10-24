#ifndef __GAME_OVER_H
#define __GAME_OVER_H

#include"public.h"
#include"init.h"

int initGameOver(SDL_Surface** psGameOver, const char* pcGameOverPath);

void drawGameOver(SDL_Surface* psGameOver, SDL_Surface* psDstSurface, SDL_Rect* psDstRect);

void destroyGameOver(SDL_Surface* psGameOver);

#endif
