/***********************************************************************
*   Copyright (C) 2016 pilot-lab.inc All rights reserved.
*   
*   @file:       test.c
*   @brief:      
*   @author:     Pilot labs
*   @maintainer: frank.fu@pilot-lab.com.cn
*   @version:    1.0
*   @date:       2016-05-27
*   
***********************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include "cyclebuffer.h"

#define CYCLEBUFFERLEN 4096
#define AUDIOFILEPATH  "aiscatherine_50.wav"
#define SAVEFILEPATH   "saveaudio.wav"

int nFd = -1;
int nSaveFd = -1;
int nBufSize = 512;
int originFileSize = 0;

void* doThread(void* arg)
{
	CYCLEBUFFERCXT_S* psCycBufCxt = (CYCLEBUFFERCXT_S*)arg;
	unsigned char acBuf[512] = {0};
	int nReadCycleBufSize = 0;
	int nSum = 0;

	nSaveFd = file_create_default(SAVEFILEPATH);
	if (-1 == nSaveFd)
	{
		printf("file_open_rw failed!\n");
		pthread_exit(NULL);
	}

	while(1)
	{
		memset(acBuf, 0, nBufSize);
		printf("xxxxx\n");
		if (((psCycBufCxt->m_nDataWriteIndex - psCycBufCxt->m_nDataReadIndex) > nBufSize) || ((psCycBufCxt->m_nDataBufSize-psCycBufCxt->m_nDataReadIndex + psCycBufCxt->m_nDataWriteIndex) > nBufSize))
		{
			nReadCycleBufSize = readCycleBuffer(psCycBufCxt, acBuf, nBufSize);	
			printf("read cycle buffer nWriteCycleBufSize=%d\n", nReadCycleBufSize);
			file_write(nSaveFd, acBuf, nReadCycleBufSize);
			nSum += nReadCycleBufSize;
			if (nSum == originFileSize)
			{
				printf("child pthread exit!\n");
				break;
			}
		
		}
		sleep(1);
#if 0
		nReadCycleBufSize = readCycleBuffer(psCycBufCxt, acBuf, nBufSize);	
		if (-1 == nReadCycleBufSize)
		{
			printf("readCycleBuffer\n");
		}
		else if(nReadCycleBufSize < nBufSize)
		{
			file_write(nSaveFd, acBuf, nReadCycleBufSize)
		}
		else
			file_write(nSaveFd, acBuf, nReadCycleBufSize)

		nSum += nReadCycleBufSize;
		if (nSum == originFileSize)
			break;
#endif
	}

	file_close(nSaveFd);
}


int main(int argc, char** argv)
{
	unsigned char* pucBuf = NULL;
	int nBufSize = 512;
	int nReadFileSize = 0;
	int nWriteCycleBufSize = 0;
	pthread_t tid = -1;
	int nSum = 0;

	CYCLEBUFFERCXT_S* psCycBufCxt = createCycleBuffer(CYCLEBUFFERLEN);
	if (NULL == psCycBufCxt)
	{
		printf("createCycleBuffer failed!\n");	
		return -1;
	}

	if (0 != initCycleBuffer(psCycBufCxt, 0, 0))
	{
		printf("initCycleBuffer failed!\n");	
		return -1;
	}

	nFd = file_open_rw(AUDIOFILEPATH);
	if (-1 == nFd)
	{
		printf("file_open_rw failed!\n");
		if (NULL != psCycBufCxt)
			destroyCycleBuffer(psCycBufCxt);
		return -1;
	}

	originFileSize = file_getSize(nFd);
	printf("%s size is %d\n", AUDIOFILEPATH, originFileSize);

	pucBuf = (unsigned char*)malloc(sizeof(unsigned char)*nBufSize);
	if (NULL == pucBuf)
	{
		printf("file_open_rw failed!\n");
		if (NULL != psCycBufCxt)
			destroyCycleBuffer(psCycBufCxt);

		file_close(nFd);
		return -1;
	}

	if (0 != pthread_create(&tid, NULL, doThread, (void*)psCycBufCxt))
	{
		printf("create pthread failed!\n");
		if (NULL != psCycBufCxt)
			destroyCycleBuffer(psCycBufCxt);

		file_close(nFd);
		return -1;
	}


	while(1)
	{
		nReadFileSize = file_read(nFd, pucBuf, nBufSize);

		nWriteCycleBufSize = writeCycleBuffer(psCycBufCxt, pucBuf, nReadFileSize);
		if (-1 == nWriteCycleBufSize)
		{
			printf("writeCycleBuffer failed!\n");	
		}
		printf("write cycle buffer nWriteCycleBufSize=%d\n", nWriteCycleBufSize);

		nSum += nReadFileSize;
		if (nSum == originFileSize)
			break;
		sleep(1);
	}
	printf("read file over!\n");


	pthread_join(tid, NULL);

	if (NULL != psCycBufCxt)
		destroyCycleBuffer(psCycBufCxt);

	file_close(nFd);

	return 0;
}
