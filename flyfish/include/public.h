#ifndef _PUBLIC_H
#define _PUBLIC_H

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include<SDL/SDL.h>
#include<SDL/SDL_image.h>
#include<SDL/SDL_mixer.h>
#include<SDL/SDL_ttf.h>
//#include<SDL_thread.h>
//#include"fish.h"

#define MOVESPEED 20			//barrier move speed
#define PASSSOCRE 10		//get 10 socre passby a barrier

#define WINWIDTH 1024
#define WINHEIGH 582 
#define BPP 24

#define BUTTONWIDTH 119		//start,quit button width
#define BUTTONHEIGH 75		//start,quit button height
#define CHOICE_START_X 421		//choice icon start position x
#define CHOICE_START_Y 206      //choice icon start position y
#define CHOICE_QUIT_X 421		//choice icon quit position x
#define CHOICE_QUIT_Y 349		//choice icon quit position y

#define FISHLIFE 5								//fish life
#define FISHWIDTH 60							//fish width
#define FISHHEIGH 48							//fish height
#define FISHSHAPE 3								//fish shape
#define FISHSTARTPOINT_X 50 					//fish start point x
#define FISHSTARTPOINT_y (WINHEIGH/2) 			//fish start point y

#define BARRIERPICNUM 2 
#define BARRIERNUM 10 
#define BARRIERWIDTH 52
#define BARRIERHEIGH 500

#define GAMEOVERWIDTH 350 
#define GAMEOVERHEIGH 350

#define MENUPATH "./picture/menu.png"
#define BACKGROUNDPATH "./picture/background.jpg"
//#define STARTBUTTONPATH "./picture/startbutton.png"
//#define QUITBUTTONPATH "./picture/quitbutton.png"
#define CHOICEPATH "./picture/choice.png"
//#define LEVELPATH "./picture/level.png"
#define BARRIERUPPATH "./picture/pipe_up.png"
#define BARRIERDOWNPATH "./picture/pipe_down.png"
#define FISHUPPATH "./picture/fish1.png"
#define FISHMIDPATH "./picture/fish2.png"
#define FISHDOWNPATH "./picture/fish3.png"
#define GAMEOVERPATH "./picture/gameover.png"

typedef struct _FISH
{
	int m_nLife;
	int m_nX;
	int m_nY;
	int m_nW;
	int m_nH;
	int m_nYoffset;
	int m_nXoffset;
	int m_nIndex;
}FISH_S;

typedef struct _BARRIER
{
	int m_nX;
	int m_nY;
	int m_nW;
	int m_nH;
	int m_nMoveSpeed;
	int m_nBarrierOffset;
	//int m_nDirect;
}BARRIER_S;

typedef enum
{
    GameState_INIT,
    GameState_MENU,
    GameState_START,
    GameState_OVER,
    GameState_ESC
}GAMESTATE_E;

typedef struct _GAMEENGINE
{
	SDL_Surface* m_psMainScreen;//window surface
	SDL_Surface* m_psMenu;//menu surface
	SDL_Surface* m_psBackGround;//menu surface
	SDL_Surface* m_psStartButton;//start game button surface
	SDL_Surface* m_psQuitButton;//quit game button surface
	SDL_Surface* m_psChoice;//choice picture surface
	SDL_Surface* m_psLevel;//第几关图片 surface
	Mix_Music*   m_psMusic;
	SDL_Surface* m_psGameOver;
	SDL_Surface* m_psaFishSurf[FISHSHAPE];//dym fish surface
	FISH_S*      m_psaFish[FISHSHAPE];
	SDL_Surface* m_psaBarrierSurf[BARRIERPICNUM];//barrierup surface
	BARRIER_S*   m_psaBarrier[BARRIERNUM];
	char*        m_pcaBarrierPath[BARRIERPICNUM];
	char*        m_pcaFishPath[FISHSHAPE];
	char*        m_pcGameOverPath;
	unsigned int m_nScore;//分数
	int          m_nBarrierOffset[2];//0 is upBarrier offset, 1 is downBarrier offset
}GAMEENGINE_S;


#if 0
typedef struct _SPRITE
{
	int m_nX;
	int m_nY;
	int m_nW;
	int m_nH;
	int m_nYoffset;
	int m_nMoveSpeed;
}SPRITE_S;
#endif

#endif
