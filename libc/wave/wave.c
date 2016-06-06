/***********************************************************************
*   Copyright (C) 2016 pilot-lab.inc All rights reserved.
*   
*   @file:       wave.c
*   @brief:      
*   @author:     Pilot labs
*   @maintainer: frank.fu@pilot-lab.com.cn
*   @version:    1.0
*   @date:       2016-03-05
*   
***********************************************************************/
#include "wave.h"
#include "log.h"
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <math.h>

#ifndef PL_TAG
#define PL_TAG "WAVE"
#endif

static double char2double(unsigned char low, unsigned char high)
{
	double dNormaliztion = 0;
	long lDataComplement = 0;
	unsigned long ulDataLow = low;
	unsigned long ulDataHigh = high;
	double dDataReal = (ulDataHigh<<8) + ulDataLow;
	//PL_LOGI(PL_TAG,"xxxx%04x ",(int)dDataReal);
	int nSign = (int)(ulDataHigh >> 7);
	//PL_LOGI(PL_TAG,"%d ", nSign);
	if (nSign == 1)
	{
		lDataComplement = dDataReal - 65536;
	}
	else
	{ 
		lDataComplement = dDataReal;
	}
	//PL_LOGI(PL_TAG,"%d ", lDataComplement);
	dNormaliztion = (double)(lDataComplement/(double)32768);
	//dNormaliztion = (double)(lDataComplement>>15);    
	//PL_LOGI(PL_TAG,"%f ", dNormaliztion);
	return dNormaliztion;
}   

static void double2char(double dIn, unsigned char *pcLow, unsigned char *pcHigh)
{
	double dMul = 0.0; 
	double dData = 0.0;
	unsigned short shData = 0;
	long lMul = 0;

	dMul = (double)(dIn*(double)32768);
	lMul = dIn*(double)32768;
	//printf("dMul=%x,%lf\n", dMul, dMul);
	//printf("lMul=%x,%lf\n", lMul, lMul);
	if (1 == (int)(lMul &0x8000))
		shData = (lMul & 0xffff);
	else
		shData = lMul;
	//printf("sdData=%x,%d\n", shData, shData);
	*pcLow = (unsigned char)(shData&0xff);
	*pcHigh = (unsigned char)((shData>>8)&0xff);
	//printf("Low=%x, High=%x\n", *pcLow, *pcHigh);
}

int openWave(const char* pcFileName, WAVEINFO_S* psWavInfo)
{
	FILE* pFile = NULL; 
	pFile = fopen(pcFileName,"r"); 
	if(!pFile)
	{ 
		PL_LOGE(PL_TAG,"Can not open the wave file: %s.\n",pcFileName); 
		return -1; 
	}
	psWavInfo->m_pFp = pFile;

	return 0;
}

int getWaveHead(WAVEINFO_S* psWavInfo)
{
	if (NULL == psWavInfo || NULL == psWavInfo->m_pFp)
	{
		PL_LOGE(PL_TAG,"NULL == psWavInfo || NULL == psWavInfo->m_pFp\n"); 
		return -1; 
	}

	int i = 0;
	unsigned int nOffset = 0;

	nOffset = sizeof(psWavInfo->m_sWavHead.m_aucChunkID);
	fseek(psWavInfo->m_pFp, nOffset, SEEK_SET); 
	fread(&(psWavInfo->m_sWavHead.m_nChunkSize), sizeof(char), sizeof(psWavInfo->m_sWavHead.m_nChunkSize), psWavInfo->m_pFp); 
	PL_LOGD(PL_TAG,"filesize=%d\n",psWavInfo->m_sWavHead.m_nChunkSize);

	fread(psWavInfo->m_sWavHead.m_aucFormat, sizeof(char), sizeof(psWavInfo->m_sWavHead.m_aucFormat), psWavInfo->m_pFp); 
	for (i = 0; i < (int)sizeof(psWavInfo->m_sWavHead.m_aucFormat); i++)
	{
		PL_LOGD(PL_TAG,"psWavInfo->m_sWavHead.m_aucFormat[%d]=%c\n", i, psWavInfo->m_sWavHead.m_aucFormat[i]);
	}

	fread(psWavInfo->m_sWavHead.m_aucSubChunk1ID, sizeof(char), sizeof(psWavInfo->m_sWavHead.m_aucSubChunk1ID), psWavInfo->m_pFp); 
	for (i = 0; i < (int)sizeof(psWavInfo->m_sWavHead.m_aucSubChunk1ID); i++)
	{
		PL_LOGD(PL_TAG,"psWavInfo->m_sWavHead.m_aucSubChunk1ID[%d]=%c\n", i, psWavInfo->m_sWavHead.m_aucSubChunk1ID[i]);
	}

	fread(&(psWavInfo->m_sWavHead.m_nSubChunk1Size), sizeof(char), sizeof(psWavInfo->m_sWavHead.m_nSubChunk1Size), psWavInfo->m_pFp); 
	PL_LOGD(PL_TAG,"subChunk1Size=%d\n",psWavInfo->m_sWavHead.m_nSubChunk1Size);

	fread(&(psWavInfo->m_sWavHead.m_stAudioFormat), sizeof(char), sizeof(psWavInfo->m_sWavHead.m_stAudioFormat), psWavInfo->m_pFp); 
	PL_LOGD(PL_TAG,"audioformat=%d\n",psWavInfo->m_sWavHead.m_stAudioFormat);

	fread(&(psWavInfo->m_sWavHead.m_stNumChannels), sizeof(char), sizeof(psWavInfo->m_sWavHead.m_stNumChannels), psWavInfo->m_pFp); 
	PL_LOGD(PL_TAG,"channels=%d\n",psWavInfo->m_sWavHead.m_stNumChannels);

	fread(&(psWavInfo->m_sWavHead.m_nSampleRate), sizeof(char), sizeof(psWavInfo->m_sWavHead.m_nSampleRate), psWavInfo->m_pFp); 
	PL_LOGD(PL_TAG,"sampleRate=%d\n",psWavInfo->m_sWavHead.m_nSampleRate);

	fread(&(psWavInfo->m_sWavHead.m_nByteRate), sizeof(char), sizeof(psWavInfo->m_sWavHead.m_nByteRate), psWavInfo->m_pFp); 
	PL_LOGD(PL_TAG,"ByteRate=%d\n", psWavInfo->m_sWavHead.m_nByteRate);

	fread(&(psWavInfo->m_sWavHead.m_stBlockAlign), sizeof(char), sizeof(psWavInfo->m_sWavHead.m_stBlockAlign), psWavInfo->m_pFp); 
	PL_LOGD(PL_TAG,"blockAlign=%d\n",psWavInfo->m_sWavHead.m_stBlockAlign);

	fread(&(psWavInfo->m_sWavHead.m_stBitsPerSample), sizeof(char), sizeof(psWavInfo->m_sWavHead.m_stBitsPerSample), psWavInfo->m_pFp); 
	PL_LOGD(PL_TAG,"bitsPerSample=%d\n",psWavInfo->m_sWavHead.m_stBitsPerSample);

	fread(psWavInfo->m_sWavHead.m_aucSubChunk2ID, sizeof(char), sizeof(psWavInfo->m_sWavHead.m_aucSubChunk2ID), psWavInfo->m_pFp); 
	for (i=0; i < 4; i++)
		PL_LOGD(PL_TAG,"chunk2Id=%c\n", psWavInfo->m_sWavHead.m_aucSubChunk2ID[i]);
	
	fread(&(psWavInfo->m_sWavHead.m_nSubChunk2Size), sizeof(char), sizeof(psWavInfo->m_sWavHead.m_nSubChunk2Size), psWavInfo->m_pFp); 
	PL_LOGD(PL_TAG,"subChunk2Size=%d\n",psWavInfo->m_sWavHead.m_nSubChunk2Size);

	return 0;
}

int readWave(WAVEINFO_S* psWavInfo)
{
	if (NULL == psWavInfo || NULL == psWavInfo->m_pFp || NULL == psWavInfo->m_pucData)
	{
		PL_LOGE(PL_TAG,"NULL == psWavInfo || NULL == psWavInfo->m_pFp || NULL == psWavInfo->m_pucData\n"); 
		return -1; 
	}

	FILE* pFile = psWavInfo->m_pFp;
	unsigned char *pucSpeech = psWavInfo->m_pucData; 

	fread(pucSpeech, 1, psWavInfo->m_sWavHead.m_nSubChunk2Size, pFile); 

	return 0;
}

void destroyWave(WAVEINFO_S* psWavInfo)
{
	if (NULL != psWavInfo->m_pucData)
	{
		free(psWavInfo->m_pucData);
		psWavInfo->m_pucData = NULL;
	}

	if (NULL != psWavInfo->m_pFp)
	{
		fclose(psWavInfo->m_pFp);	
		psWavInfo->m_pFp = NULL;
	}
}

double* getAudAbs(double* pdAudData, int nDataLen)
{
	if (NULL == pdAudData)
	{
		PL_LOGE(PL_TAG,"getAudAbs input parameter is NULL\n");	
		return NULL;
	}
	PL_LOGI(PL_TAG,"getAudAbs datalen=%d\n", nDataLen);

	int i = 0;	
	
	double* pdAudAbs = (double*)malloc(sizeof(double)*nDataLen);
	if (NULL == pdAudAbs)
	{
		PL_LOGE(PL_TAG,"getAudAbs malloc failed!\n");	
		return NULL;
	}
		
	for (i = 0; i < nDataLen; i++)
	{
		*(pdAudAbs+i) = fabs(*(pdAudData+i));
	}

	return pdAudAbs;
}


