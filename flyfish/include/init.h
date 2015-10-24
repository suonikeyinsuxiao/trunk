#ifndef _INIT_H
#define _INIT_H

#include "public.h"

void initSDL(void);
void destroySDL(void);
int initGameEngine(GAMEENGINE_S* psGameEngine);		//init success return 1, failed return 0
int fileExist(const char* pcFile);


#endif //_INIT_H
