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


#if 0
static double channel1Bps2(unsigned char low, unsigned char high)
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

void dosomething(double dIn, unsigned char *pcLow, unsigned char *pcHigh)
{
	//long lMulti = (long)dIn *32768;
	//*pcLow = (unsigned char) (lMulti & (long)0xff);
	//*pcHigh =(unsigned char) ((lMulti & (long)0xff00) >> 8);
	double dMul = 0.0;
	double dData = 0.0;
	unsigned short shData = 0;
	long lMul = 0;

	dMul = (double)(dIn*(double)32768);
	lMul = dIn*(double)32768;
	//printf("dMul=%x,%lf\n", dMul, dMul);
	//printf("lMul=%x,%lf\n", lMul, lMul);
	if (1 == (int)(lMul >> 31))
		    dData = lMul+65536;
	else
		    dData = lMul;
	//printf("dData=%x,%lf\n", dData, dData);
	shData = (unsigned short)dData;
	//printf("sdData=%x,%d\n", shData, shData);
	*pcLow = (unsigned char)(shData&0xff);
	*pcHigh = (unsigned char)((shData>>8)&0xff);
	//printf("Low=%x, High=%x\n", *pcLow, *pcHigh);
}
#endif


// read wave file 
double* wavread(char* pcFileName, WAVEFORMAT_S* psWaveFmt)
{ 
	PL_LOGD(PL_TAG,"enter wavread\n");

	int i = 0;
	FILE* pFile = NULL; 
	char acStr[32] = {0}; 
	char *pcSpeech = NULL; 

	pFile = fopen(pcFileName,"r"); 
	if(!pFile)
	{ 
		PL_LOGE(PL_TAG,"Can not open the wave file: %s.\n",pcFileName); 
		return NULL; 
	} 

	PL_LOGD(PL_TAG,"double=%ld,float=%ld\n",sizeof(double),sizeof(float));
	fseek(pFile, 4, SEEK_SET); 
	fread(&(psWaveFmt->m_nChunkSize), sizeof(char), sizeof(psWaveFmt->m_nChunkSize), pFile); 
	PL_LOGD(PL_TAG,"filesize=%d\n",psWaveFmt->m_nChunkSize);

	PL_LOGD(PL_TAG,"before pos=%ld\n",ftell(pFile));
	fseek(pFile, 8, SEEK_SET); 
	PL_LOGD(PL_TAG,"after pos=%ld\n",ftell(pFile));

	fread(acStr, sizeof(char), 7, pFile); 
	acStr[7] = '\0'; 
	if(strcmp(acStr,"WAVEfmt"))
	{ 
		PL_LOGE(PL_TAG, "The file is not in WAVE format!\n"); 
		return NULL; 
	} 

	for (i = 0; i < 7; i++)
	{
		PL_LOGI(PL_TAG,"acStr[%d]=%x,%c\n",i, acStr[i], acStr[i]);
	}
	
	// read format header 
	fseek(pFile, 16, SEEK_SET); 
	fread((unsigned int*)(&psWaveFmt->m_nSubChunk1Size),sizeof(char),sizeof(psWaveFmt->m_nSubChunk1Size),pFile); 
	PL_LOGI(PL_TAG,"subChunk1Size=%d\n",psWaveFmt->m_nSubChunk1Size);

	fseek(pFile, 20, SEEK_SET); 
	fread(&psWaveFmt->m_stAudioFormat, sizeof(char), sizeof(short), pFile); 
	PL_LOGI(PL_TAG,"audioformat=%d\n",psWaveFmt->m_stAudioFormat);

	fseek(pFile, 22, SEEK_SET); 
	fread(&psWaveFmt->m_stNumChannels, sizeof(char), sizeof(short), pFile); 
	PL_LOGI(PL_TAG,"channels=%d\n",psWaveFmt->m_stNumChannels);

	fseek(pFile, 24, SEEK_SET); 
	fread(&psWaveFmt->m_nSampleRate, sizeof(char), sizeof(int), pFile); 
	PL_LOGI(PL_TAG,"sampleRate=%d\n",psWaveFmt->m_nSampleRate);

	fseek(pFile, 28, SEEK_SET); 
	fread(&psWaveFmt->m_nByteRate, sizeof(char), sizeof(int), pFile); 
	PL_LOGI(PL_TAG,"ByteRate=%d\n",psWaveFmt->m_nByteRate);

	fseek(pFile, 32, SEEK_SET); 
	fread(&psWaveFmt->m_stBlockAlign, sizeof(char), sizeof(short), pFile); 
	PL_LOGI(PL_TAG,"blockAlign=%d\n",psWaveFmt->m_stBlockAlign);

	fseek(pFile, 34, SEEK_SET); 
	fread(&psWaveFmt->m_stBitsPerSample, sizeof(char), sizeof(short), pFile); 
	PL_LOGI(PL_TAG,"bitsPerSample=%d\n",psWaveFmt->m_stBitsPerSample);

	fseek(pFile, 36, SEEK_SET); 
	fread(psWaveFmt->m_aucSubChunk2ID, sizeof(char), 4, pFile); 
	for (i=0; i < 4; i++)
		PL_LOGI(PL_TAG,"chunk2Id=%c\n",psWaveFmt->m_aucSubChunk2ID[i]);

	fseek(pFile, 40, SEEK_SET); 
	fread(&psWaveFmt->m_nSubChunk2Size, sizeof(char), sizeof(int), pFile); 
	PL_LOGI(PL_TAG,"subChunk2size=%d\n",psWaveFmt->m_nSubChunk2Size);

	pcSpeech = (char*)malloc(sizeof(char)*(psWaveFmt->m_nSubChunk2Size)); 
	if(!pcSpeech)
	{ 
		PL_LOGE(PL_TAG, "[%s,%d]Memory alloc failed!\n",__func__, __LINE__); 
		return NULL; 
	}
	char* pcData = pcSpeech;
	fseek(pFile, 20+psWaveFmt->m_nSubChunk1Size+8, SEEK_SET); 
	fread(pcSpeech, 1, psWaveFmt->m_nSubChunk2Size, pFile); 

	//for (i = 0; i < 1024; i=i+2)
	//{
	//	printf("origin data high[%x]=%x, low[%x]=%x\n", i+1+44, pcSpeech[i+1]&0xff, i+44, pcSpeech[i]&0xff);
	//}

	double* pdAudRecord = (double*)malloc(sizeof(double)*(psWaveFmt->m_nSubChunk2Size >> 1));
	double* pdAudTmp = pdAudRecord;
	
	for (i = 0; i < (psWaveFmt->m_nSubChunk2Size >> 1); i++)
	{
		*pdAudTmp = char2double(*pcData, *(pcData+1));
		pdAudTmp++;
		pcData += 2;
	}

	if (NULL != pcSpeech)
	{
		free(pcSpeech);
		pcSpeech = NULL;
	}

	fclose(pFile);

	return pdAudRecord; 
}

void freeWave(double* pdAudRecord)
{
	if (NULL != pdAudRecord)
	{
		free(pdAudRecord);
		pdAudRecord = NULL;
	}
	else
	{
		PL_LOGE(PL_TAG, "pdAudRecord == NULL\n");	
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


