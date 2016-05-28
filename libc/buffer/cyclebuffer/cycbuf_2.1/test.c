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
#include <unistd.h>
#include "cyclebuffer.h"

#define AUDIOFILEPATH  "aiscatherine_50.wav"
#define SAVEFILEPATH   "saveaudio.wav"

int nFd = -1;
int nSaveFd = -1;
int nBufSize = 512;
int originFileSize = 0;

void* readCycleBufferThread(void* arg)
{
	CYCLEBUFFERCXT_S* psCycBufCxt = (CYCLEBUFFERCXT_S*)arg;
	unsigned char acBuf[512] = {0};
	int nReadCycleBufSize = 0;
	int nSum = 0;
	int sleepcount = 0;

	nSaveFd = file_create_default(SAVEFILEPATH);
	if (-1 == nSaveFd)
	{
		printf("file_open_rw failed!\n");
		pthread_exit(NULL);
	}

	while(1)
	{
		memset(acBuf, 0, nBufSize);
		//printf("xxxxx\n");

		nReadCycleBufSize = readCycleBuffer(psCycBufCxt, acBuf, nBufSize);	
		if (0 != nReadCycleBufSize)
		{
			printf("read cycle buffer nWriteCycleBufSize=%d\n", nReadCycleBufSize);
			file_write(nSaveFd, acBuf, nReadCycleBufSize);
		}
		else if (0 == nReadCycleBufSize)
		{
			printf("cycle buffer is empty sleep(1)"); 
			sleep(1);
		}

		nSum += nReadCycleBufSize;
		if (nSum == originFileSize)
		{
			printf("child pthread exit!\n");
			break;
		}

		if (0 == (++sleepcount)%10)
		{
			sleepcount = 0;
			usleep(100000);
		}
	}

	printf("exit...\n");

	file_close(nSaveFd);
	printf("xxxxxxxxxxxxxxx...\n");
	//pthread_exit(NULL);
}


int main(int argc, char** argv)
{
	unsigned char* pucBuf = NULL;
	int nBufSize = 512;
	int nReadFileSize = 0;
	int nWriteCycleBufSize = 0;
	pthread_t tid = -1;
	//pthread_t writetid = -1;
	int nSum = 0;

	CYCLEBUFFERCXT_S* psCycBufCxt = createCycleBuffer(CYCLEBUFFERLEN, e_normal);
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

	if (0 != pthread_create(&tid, NULL, readCycleBufferThread, (void*)psCycBufCxt))
	{
		printf("create pthread failed!\n");
		if (NULL != psCycBufCxt)
			destroyCycleBuffer(psCycBufCxt);

		file_close(nFd);
		return -1;
	}

#if 0
	if (0 != pthread_create(&writetid, NULL, writeCycleBufferThread, (void*)psCycBufCxt))
	{
		printf("create write cycle buffer pthread!\n")	
		if (NULL != psCycBufCxt)
			destroyCycleBuffer(psCycBufCxt);

		file_close(nFd);
		return -1;
	}
#endif

	int nlen = 0;
	while(1)
	{
		nReadFileSize = file_read(nFd, pucBuf, nBufSize*2);

		nWriteCycleBufSize = writeCycleBuffer(psCycBufCxt, pucBuf, nReadFileSize);
		if (-1 == nWriteCycleBufSize)
		{
			printf("writeCycleBuffer failed!\n");	
		}
		else if (0 == nWriteCycleBufSize)
		{
			printf("cycle buffer is full, sleep(1)\n");	
			sleep(1);
		}

		//保证每一次pucBuf中的数据完全写入循环Buffer
		while(nWriteCycleBufSize < nReadFileSize)
		{
			nlen = writeCycleBuffer(psCycBufCxt, pucBuf+nWriteCycleBufSize, nReadFileSize-nWriteCycleBufSize);
			nWriteCycleBufSize += nlen;
		}
		
		printf("write cycle buffer nWriteCycleBufSize=%d\n", nWriteCycleBufSize);

		nSum += nReadFileSize;
		if (nSum == originFileSize)
			break;
		//sleep(1);
	}
	printf("read file over!\n");


	pthread_join(tid, NULL);

	printf("11111111111111111\n");

	if (NULL != psCycBufCxt)
		destroyCycleBuffer(psCycBufCxt);
	printf("2222222222222222\n");

	file_close(nFd);
	printf("333333333333\n");

	return 0;
}
