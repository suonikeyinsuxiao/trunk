==================public.h===================================

#define MOVESPEED 5			//barrier move speed
#define PASSSOCRE 10		//get 10 socre passby a barrier

#define WINWIDTH 640
#define WINHEIGH 480

typedef enum
{
    GameState_INIT,
    GameState_MENU,
    GameState_START,
    GameState_ESC
}GAMESTATE_E;

typedef struct _GAMEENGINE
{
	SDL_Surface* m_psMainScreen;//window surface
	SDL_Surface* m_psStartButton;//start game button surface
	SDL_Surface* m_psQuitButton;//quit game button surface
	SDL_Surface* m_psChoice;//choice picture surface
	SDL_Surface* m_psLevel;//第几关图片 surface
	SDL_Surface* m_psBarrierUp;//barrierup surface
	SDL_Surface* m_psBarrierDown;//barrierdown surface
	SDL_Surface* m_psfish[3];//dym fish surface
	unsigned int m_nScore;//分数
	int m_nBarrierOffset[2];//0 is upBarrier offset, 1 is downBarrier offset
	Mix_Music* m_psMusic;
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


GAMESTATE_E g_eGameState = GameState_INIT;
GAMEENGINE_S g_sGameEngine;   //need to  init 

void initSDL();
void uninitSDL();
========================barrier.h=========================
typedef struct _BARRIER
{
	int m_nX;
	int m_nY;
	int m_nW;
	int m_nH;
	int m_nMoveSpeed;
	int m_nDirect;
}BARRIER_S;

//thread process func
void* barrierPthdProcess(void* arg);

/**
 * function:movebarrier()
 *
 * Description:移动barrier
 *
 * parameter:
 *		BARRIER_S* psBar 鱼对象
 *		int nOffset 偏移量
 *		int nDirect  偏移方向
 *
 * return:
 *
 */
void moveFish(BARRIER_S* psBar, int nOffset, int nDirect);

========================fish.h=========================
typedef struct _FISH
{
	int m_nLife;
	int m_nX;
	int m_nY;
	int m_nW;
	int m_nH;
	int m_nYoffset;
	int m_nXoffset;
}FISH_S;

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

========================image.h=========================
/**
 * function:loadImage()
 *
 * Description: load image
 *
 * parameter:
 *		const char* pcFile 图片文件路径
 *
 * return:
 *
 */
SDL_Surface* loadImage(const char* pcFile);

/**
 * function:drawImage()
 *
 * Description:draw picture, format convert ...
 *
 * parameter:
 *		SDL_Surface* psSrc: 源图片Surface
 *
 * return:
 */
void drawImage(SDL_Surface* psSrc);

/**
 * function:showImage()
 *
 * Description:将psSrc表示的图片画在psDst中的psDstRect区域
 *
 * parameter:
 *		psSrc: 源图片Surface
 *		psDst: 画布
 *		psDstRect: 显示区域
 *
 * return:
 *
 */
void showImage(SDL_Surface* psSrc, SDL_Surface* psDst, SDL_Rect* psDstRect);
/**
 * function:initMixer()
 *
 * Description: init Mixer 
 *
 * parameter:
 *
 * return:
 *
 */

========================music.h=========================
void initMixer();

/**
 * function:uninitMixer()
 *
 * Description: uninit Mixer 
 *
 * parameter:
 *
 * return:
 *
 */
void unInitMixer();

/**
 * function:loadMusic()
 *
 * Description:  
 *
 * parameter:
 *
 * return:
 *
 */
void loadMusic();

/**
 * function:playMusic()
 *
 * Description:  
 *
 * parameter:
 *
 * return:
 *
 */
void playMusic();

/**
 * function:pauseMusic()
 *
 * Description:  
 *
 * parameter:
 *
 * return:
 *
 */
void pauseMusic();

/**
 * function:stopMusic()
 *
 * Description:  
 *
 * parameter:
 *
 * return:
 *
 */
void stopMusic();

/**
 * function:destroyMusic()
 *
 * Description:  
 *
 * parameter:
 *
 * return:
 *
 */
void destroyMusic();

/**
 * function:loadWAV()
 *
 * Description:  
 *
 * parameter:
 *
 * return:
 *
 */
void loadWAV();

/**
 * function:playWAV()
 *
 * Description:  
 *
 * parameter:
 *
 * return:
 *
 */
void playWAV();

/**
 * function:pauseWAV()
 *
 * Description:  
 *
 * parameter:
 *
 * return:
 *
 */
void pauseWAV();

/**
 * function:stopWAV()
 *
 * Description:  
 *
 * parameter:
 *
 * return:
 *
 */
void stopWAV();

/**
 * function:destroyWAV()
 *
 * Description:  
 *
 * parameter:
 *
 * return:
 *
 */
void destroyWAV();


========================keyevent.h=========================
/**
 * function:eventProcess()
 *
 * Description:when a event is reported, do something
 *
 * parameter:
 *		SDL_EVENT* psEvent
 * return:
 */
int eventProcess(SDL_EVENT* psEvent);
