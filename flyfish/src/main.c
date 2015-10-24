#include"public.h"
#include"init.h"
#include"fish.h"
#include"image.h"
#include"barrier.h"
#include"gameover.h"
#include<unistd.h>


GAMESTATE_E g_eGameState = GameState_INIT;
GAMEENGINE_S g_sGameEngine;    

#define UPSPEED 10
#define DOWNSPEED 10
#define LEFTSPEED 10
#define RIGHTSPEED 10 

void menuProcess();
void showBackGround(const char* pcFile);
void destroyMenu();

int main()
{
	srand(time(NULL));

    while(1)
	{
		switch(g_eGameState)
		{
			case GameState_INIT:	
			{
				initSDL();	

				if (!initGameEngine(&g_sGameEngine))
				{
					printf("[%s, %d] initGameEngine failed!\n", __func__, __LINE__);	
					destroySDL();
					return 0;
				}

				g_sGameEngine.m_psMainScreen = SDL_SetVideoMode(WINWIDTH, WINHEIGH, BPP, SDL_SWSURFACE);
				SDL_WM_SetCaption("flybird", NULL);

				g_eGameState = GameState_MENU;
			}
			break;
			case GameState_MENU:	
			{
				printf("GameState_MENU");

				if (!loadImage(MENUPATH, &g_sGameEngine.m_psMenu))
				{
					g_eGameState = GameState_ESC;
				}
				else
				{
					SDL_Rect sRec = {
						.x = 0,	
						.y = 0,	
						.w = WINWIDTH,	
						.h = WINHEIGH 
					};

					drawImage(g_sGameEngine.m_psMenu, g_sGameEngine.m_psMainScreen,&sRec);	
					
					menuProcess();
				}
			}
			break;
			case GameState_START:	
			{
				printf("start game\n");	
				int i = 0;
				int nFishIndex = 0;
				SDL_Event event;

				//init success return 1
				if (!initFish(&g_sGameEngine))
					g_eGameState = GameState_ESC;	
			
				if (!initBarrier(&g_sGameEngine, BARRIERNUM))
					g_eGameState = GameState_ESC;	
			
				while(g_eGameState == GameState_START)
				{
					//score OK next game
					showBackGround(BACKGROUNDPATH);
					for (i = 0; i < BARRIERNUM; i = i+2)
					{
						drawBarrier(&g_sGameEngine, i, 20);
					}
					drawFish(&g_sGameEngine, nFishIndex, 0, 0);

					while(SDL_PollEvent(&event))
					{
						switch(event.type)
						{
							case SDL_KEYDOWN:
							{
								if(event.key.keysym.sym==SDLK_UP)
								{
									showBackGround(BACKGROUNDPATH);
									for (i = 0; i < BARRIERNUM; i=i+2)
										drawBarrier(&g_sGameEngine, i, 20);
									drawFish(&g_sGameEngine, nFishIndex, 0, (-UPSPEED));
									//SDL_Delay(10);
								}
								else if(event.key.keysym.sym==SDLK_DOWN)
								{
									showBackGround(BACKGROUNDPATH);
									for (i = 0; i < BARRIERNUM; i=i+2)
										drawBarrier(&g_sGameEngine, i, 20);
									drawFish(&g_sGameEngine, nFishIndex, 0, (DOWNSPEED));
								}
								else if(event.key.keysym.sym==SDLK_LEFT)
								{
									showBackGround(BACKGROUNDPATH);
									for (i = 0; i < BARRIERNUM; i=i+2)
										drawBarrier(&g_sGameEngine, i, 20);
									drawFish(&g_sGameEngine, nFishIndex, (-LEFTSPEED), 0);
								}
								else if(event.key.keysym.sym==SDLK_RIGHT)
								{
									showBackGround(BACKGROUNDPATH);
									for (i = 0; i < BARRIERNUM; i=i+2)
										drawBarrier(&g_sGameEngine, i, 20);
									drawFish(&g_sGameEngine, nFishIndex, RIGHTSPEED, 0);
								}
								else if (event.key.keysym.sym==SDLK_ESCAPE)
								{
									destroyBarrier(&g_sGameEngine);
									g_eGameState = GameState_MENU;
								}
							}
							break;
						}
					}
					SDL_Flip(g_sGameEngine.m_psMainScreen);
					nFishIndex = (nFishIndex+1)%(FISHSHAPE);
					//isHit()
					for (i = 0; i < BARRIERNUM; i += 2)
					{
						if (((g_sGameEngine.m_psaFish[0]->m_nX+FISHWIDTH) > g_sGameEngine.m_psaBarrier[i+1]->m_nX && g_sGameEngine.m_psaFish[0]->m_nY < (g_sGameEngine.m_psaBarrier[i+1]->m_nY + g_sGameEngine.m_psaBarrier[i+1]->m_nH))
								||( ((g_sGameEngine.m_psaFish[0]->m_nX+FISHWIDTH) > g_sGameEngine.m_psaBarrier[i]->m_nX) && ( (g_sGameEngine.m_psaFish[0]->m_nY+FISHHEIGH) > g_sGameEngine.m_psaBarrier[i]->m_nY) )
								/*||( (g_sGameEngine.m_psaFish[0]->m_nX <= (g_sGameEngine.m_psaBarrier[i]->m_nX+BARRIERWIDTH) ) && ( ((g_sGameEngine.m_psaFish[0]->m_nY+FISHHEIGH) >= g_sGameEngine.m_psaBarrier[i]->m_nY) ) )
								||((g_sGameEngine.m_psaFish[0]->m_nX <= (g_sGameEngine.m_psaBarrier[i+1]->m_nX+BARRIERWIDTH)) && (g_sGameEngine.m_psaFish[0]->m_nY <= (g_sGameEngine.m_psaBarrier[i+1]->m_nY + g_sGameEngine.m_psaBarrier[i+1]->m_nH) ) )*/  )
						{
							g_eGameState = GameState_OVER;	
							destroyBarrier(&g_sGameEngine);
							break;
						}
					}
				}
			}
			break;
			case GameState_OVER:
			{
				printf("game over!\n");
				SDL_Rect sDstRect;
				SDL_Event event;

				if (!initGameOver(&g_sGameEngine.m_psGameOver, g_sGameEngine.m_pcGameOverPath))
				{
					g_eGameState = GameState_MENU;
				}
				else//initGameOver success;
				{
					sDstRect.x = WINWIDTH/2 - GAMEOVERWIDTH/2;
					sDstRect.y = WINHEIGH/2 - GAMEOVERHEIGH/2;
					sDstRect.w = GAMEOVERWIDTH;
					sDstRect.h = GAMEOVERHEIGH;

					drawGameOver(g_sGameEngine.m_psGameOver, g_sGameEngine.m_psMainScreen, &sDstRect);
					SDL_Flip(g_sGameEngine.m_psMainScreen);
				}
				while(g_eGameState == GameState_OVER)
				{
					while(SDL_PollEvent(&event))
					{
						switch(event.type)
						{
							case SDL_KEYDOWN:
							{
								if (event.key.keysym.sym==SDLK_ESCAPE)
								{
									destroyGameOver(g_sGameEngine.m_psGameOver);
									g_eGameState = GameState_MENU;
								}
							}
						}
					}
				}
			}
			break;
			case GameState_ESC:	
			{
				printf("quit game\n");
				destroyFish(&g_sGameEngine);
				destroyBarrier(&g_sGameEngine);
				destroyMenu();
				destroySDL();
				return 0;
			}
			break;
		}
	}
	return 0;
}

void menuProcess()
{
	int flag = 0;
	SDL_Event event;
	SDL_Rect rectstart,rectquit;
	rectstart.w=BUTTONWIDTH;
	rectstart.h=BUTTONHEIGH;
	rectstart.x=CHOICE_START_X; 
	rectstart.y=CHOICE_START_Y;
	
	rectquit.w=BUTTONWIDTH;
	rectquit.h=BUTTONHEIGH;
	rectquit.x=CHOICE_QUIT_X;
	rectquit.y=CHOICE_QUIT_Y;

	SDL_Rect sMenuRec = {
		.x = 0,	
		.y = 0,	
		.w = WINWIDTH,	
		.h = WINHEIGH 
	};

	if (!loadImage(CHOICEPATH, &g_sGameEngine.m_psChoice))
	{
		destroyMenu();
		g_eGameState = GameState_ESC;		 
	}
	else
	{
		drawImage(g_sGameEngine.m_psChoice, g_sGameEngine.m_psMainScreen,&rectstart);	

		while(g_eGameState == GameState_MENU)
		{
			SDL_Flip(g_sGameEngine.m_psMainScreen);
		    while(SDL_PollEvent(&event))
			{
				switch(event.type)
				{
					case SDL_KEYDOWN:
					{
						if(event.key.keysym.sym==SDLK_UP)
						{
							//start button
							if (flag)
							{
								printf("start button\n");
								flag = 0;
								drawImage(g_sGameEngine.m_psMenu, g_sGameEngine.m_psMainScreen, &sMenuRec);
								drawImage(g_sGameEngine.m_psChoice, g_sGameEngine.m_psMainScreen,&rectstart);
							}
						}
						else if(event.key.keysym.sym==SDLK_DOWN)
						{
							//quit button
							if (!flag)
							{
								printf("quit button\n");
								flag = 1;
								drawImage(g_sGameEngine.m_psMenu,g_sGameEngine.m_psMainScreen,&sMenuRec);
								drawImage(g_sGameEngine.m_psChoice, g_sGameEngine.m_psMainScreen, &rectquit);
							}
						}
						else if(event.key.keysym.sym==SDLK_RETURN)
						{
							if (flag)
							{
								destroyMenu();
								g_eGameState = GameState_ESC;
							}
							else 
							{
								destroyMenu();
								g_eGameState = GameState_START;
							}
						}
					}	
					break;
				}
			}
		}
	}
}

void showBackGround(const char* pcFile)
{
	if (fileExist(pcFile))//file is exist
	{
		if (NULL != g_sGameEngine.m_psBackGround)
			SDL_FreeSurface(g_sGameEngine.m_psBackGround);

		SDL_Surface* psTmp = IMG_Load(pcFile);
		g_sGameEngine.m_psBackGround = SDL_DisplayFormat(psTmp);
		SDL_FreeSurface(psTmp);
		SDL_Rect sRec = {
			.x = 0,	
			.y = 0,	
			.w = WINWIDTH,	
			.h = WINHEIGH 
		};

		SDL_BlitSurface(g_sGameEngine.m_psBackGround, NULL,g_sGameEngine.m_psMainScreen, &sRec);
	}
}

void destroyMenu()
{
	if (NULL != g_sGameEngine.m_psMenu)
	{
		SDL_FreeSurface(g_sGameEngine.m_psMenu);	
		g_sGameEngine.m_psMenu = NULL;
	}
	
	if (NULL != g_sGameEngine.m_psChoice)
	{
		SDL_FreeSurface(g_sGameEngine.m_psChoice);	
		g_sGameEngine.m_psChoice = NULL;
	}
}


