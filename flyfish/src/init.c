#include "init.h"
#include<unistd.h>


void initSDL(void)
{
	if(SDL_WasInit(SDL_INIT_EVERYTHING) != 0)    //检查SDL是否已经运行，返回非0为已经运行，提示错误信息，退出程序      
    {
		fprintf(stderr,"SDL is already running !\n");
        return;
    }
					                
    if(SDL_Init(SDL_INIT_EVERYTHING) == -1)    //装载SDL库，返回-1为装载失败，提示错误信息，退出程序
    {
        fprintf(stderr,"Unable to init SDL !\n");
        return;
    }
               
    return;
}

void destroySDL(void)
{
	if(SDL_WasInit(SDL_INIT_EVERYTHING) == 0)    //检查SDL是否已经运行，返回0为没有运行，提示错误信息，退出程序      
    {
		fprintf(stderr,"SDL was not running !\n");
        return;
   	}
								                
   	SDL_Quit();    //退出SDL
												                
  	 return;
}

//init success return 1, failed return 0
int initGameEngine(GAMEENGINE_S* psGameEngine)
{
	char* pcFishPath[FISHSHAPE]	= {FISHUPPATH, FISHMIDPATH, FISHDOWNPATH};
	char* pcBarrierPath[BARRIERPICNUM]	= {BARRIERDOWNPATH, BARRIERUPPATH};
	int i = 0;
	int nRet = 1;

    if (!fileExist(MENUPATH))
	{
		nRet = 0;
		goto ERR;
	}

    if (!fileExist(BACKGROUNDPATH))
	{
		nRet = 0;
		goto ERR;
	}

    if (!fileExist(CHOICEPATH))
	{
		nRet = 0;
		goto ERR;
	}

	for (i = 0; i < FISHSHAPE; i++)
	{
    	if (!fileExist(pcFishPath[i]))
		{
			nRet = 0;
			goto ERR;
		}

		psGameEngine->m_pcaFishPath[i] = pcFishPath[i];
		psGameEngine->m_psaFishSurf[i] = NULL;
		psGameEngine->m_psaFish[i] = NULL;

		//printf("g_sGameEngine->m_pcaFishPath[%d]=%s\n",i, g_sGameEngine.m_pcaFishPath[i]);
	}

	for (i = 0; i < BARRIERPICNUM; i++)
	{
    	if (!fileExist(pcBarrierPath[i]))
		{
			nRet = 0;
			goto ERR;
		}
		
		psGameEngine->m_pcaBarrierPath[i] = pcBarrierPath[i];
		psGameEngine->m_psaBarrierSurf[i] = NULL;
	//	printf("g_sGameEngine.m_pcaBarrierPath[%d]=%s\n", i, g_sGameEngine.m_pcaBarrierPath[i]);
	}

	for (i = 0; i< BARRIERNUM; i++)
	{
		psGameEngine->m_psaBarrier[i] = NULL;
	}

	psGameEngine->m_pcGameOverPath = GAMEOVERPATH;
	if (!fileExist(psGameEngine->m_pcGameOverPath))
	{
		nRet = 0;
		goto ERR;
	}

	psGameEngine->m_psChoice = NULL;
	psGameEngine->m_psMenu = NULL;
	psGameEngine->m_psBackGround = NULL;
	psGameEngine->m_psGameOver = NULL;
	
ERR:
	return nRet;
}

//exist return 1;else return 0
int fileExist(const char* pcFile)
{
	int nRet = 1;
	if (NULL != pcFile)
	{
		if (access(pcFile, 0))
		{
			printf("%s is not exist!\n", pcFile);
			nRet = 0;
		}
	}
	else
	{
		nRet = 0;	
	}
	return nRet;
}
