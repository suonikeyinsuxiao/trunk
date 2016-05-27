/***********************************************************************
*   Copyright (C) 2016 pilot-lab.inc All rights reserved.
*   
*   @file:       cyclebuffer.c
*   @brief:      cycle buffer 
*   @author:     Pilot labs
*   @maintainer: frank.fu@pilot-lab.com.cn
*   @version:    1.0
*   @date:       2016-05-27
*   
***********************************************************************/
#include "cyclebuffer.h"
#include "log.h"
#include <stdlib.h>
#include <string.h>


#ifndef PL_TAG
#define PL_TAG "CYCLEBUFFER"
#endif

/**
 * @brief    createCycleBuffer 
 *
 * @param[in]    nBufferSize
 *
 * @return   success return CYCLEBUFFERCXT_S*, otherwise NULL
 */
CYCLEBUFFERCXT_S* createCycleBuffer(int nBufferSize)
{
	CYCLEBUFFERCXT_S* psCycBufCxt = (CYCLEBUFFERCXT_S*)malloc(sizeof(CYCLEBUFFERCXT_S));
	if (NULL == psCycBufCxt)
	{
		PL_LOGE(PL_TAG, "malloc CYCLEBUFFERCXT_S failed!\n");	
		return NULL;
	}

	psCycBufCxt->m_pucDataBuf 		= (unsigned char*)calloc(nBufferSize, sizeof(unsigned char));
	if (NULL == psCycBufCxt->m_pucDataBuf)
	{
		PL_LOGE(PL_TAG, "calloc buffer(size %dByte) failed!\n", nBufferSize);	
		return NULL;
	}

	psCycBufCxt->m_nDataBufSize 	= nBufferSize;
	psCycBufCxt->m_nDataWriteIndex 	= 0;
	psCycBufCxt->m_nDataReadIndex 	= 0;

	pthread_mutex_init(&psCycBufCxt->m_mutex, NULL);

	return psCycBufCxt;
}

/**
 * @brief    initCycleBuffer 
 *
 * @param[in]    psCycBufCxt
 * @param[in]    nWriteIndex
 * @param[in]    nReadIndex
 *
 * @return    success return 0; otherwise -1
 */
int initCycleBuffer(CYCLEBUFFERCXT_S* psCycBufCxt, int nWriteIndex, int nReadIndex)
{
	if (psCycBufCxt == NULL || psCycBufCxt->m_pucDataBuf == NULL)
	{
		PL_LOGE(PL_TAG, "initCycleBuffer param error !\n");
		return -1;
	}
	
	memset(psCycBufCxt->m_pucDataBuf, 0, psCycBufCxt->m_nDataBufSize);
	psCycBufCxt->m_nDataWriteIndex 	= 0;
	psCycBufCxt->m_nDataReadIndex 	= 0;

	return 0;
}

/**
 * @brief    destroyCycleBuffer 
 *
 * @param[in]    psCycBufCxt
 *
 * @return    success return 0; otherwise -1
 */
int destroyCycleBuffer(CYCLEBUFFERCXT_S* psCycBufCxt)
{
	if (psCycBufCxt == NULL || psCycBufCxt->m_pucDataBuf == NULL)
	{
		PL_LOGE(PL_TAG, "initCycleBuffer param error !\n");
		return -1;
	}

	free(psCycBufCxt->m_pucDataBuf);
	free(psCycBufCxt);
	pthread_mutex_destroy(&psCycBufCxt->m_mutex);

	return 0;
}

/**
 * @brief    writeCycleBuffer 
 *
 * @param[in]    psCycBufCxt 循环buffer上下文信息
 * @param[in]    pucData     写入循环buffer的数据		
 * @param[in]    nDataLen	 写入循环buffer的数据长度
 *
 * @return   实际写入数据的长度, error return -1
 */
int writeCycleBuffer(CYCLEBUFFERCXT_S* psCycBufCxt, unsigned char* pucData, int nDataLen)
{
	if (psCycBufCxt == NULL || psCycBufCxt->m_pucDataBuf == NULL || pucData == NULL)
	{
		PL_LOGE(PL_TAG, "param error !\n");
		return -1;
	}

	int nDataWriteIndex = 0;
	int nDataBufSize = 0;

	pthread_mutex_lock(&psCycBufCxt->m_mutex);

	nDataWriteIndex = psCycBufCxt->m_nDataWriteIndex;
	nDataBufSize = psCycBufCxt->m_nDataBufSize;

	if (nDataLen < (nDataBufSize - nDataWriteIndex))
	{
		memcpy(psCycBufCxt->m_pucDataBuf + nDataWriteIndex, pucData, nDataLen);
		nDataWriteIndex = nDataWriteIndex + nDataLen;
	}
	else
	{
		int nFirstCopyLen = nDataBufSize - nDataWriteIndex;

		if (nFirstCopyLen > 0)
			memcpy(psCycBufCxt->m_pucDataBuf + nDataWriteIndex, pucData, nFirstCopyLen);

		memcpy(psCycBufCxt->m_pucDataBuf, pucData + nFirstCopyLen, nDataLen - nFirstCopyLen);
		nDataWriteIndex = nDataLen - nFirstCopyLen;
	}

	psCycBufCxt->m_nDataWriteIndex = nDataWriteIndex;
	PL_LOGD(PL_TAG, "check nDataLen(%d) nDataWriteIndex(%d)\n"nDataLen, nDataWriteIndex);

	pthread_mutex_unlock(&psCycBufCxt->m_mutex);

	return nDataLen;
}

/**
 * @brief    readCycleBuffer 
 *
 * @param[in]    psCycBufCxt	循环buffer上下文信息
 * @param[out]   pucData		从循环buffer读出的数据
 * @param[in]    nDataLen		需从循环buffer读出的数据的长度
 *
 * @return   实际读出的数据长度, error return -1
 */
int readCycleBuffer(CYCLEBUFFERCXT_S* psCycBufCxt, unsigned char * pucData, int nDataLen)
{

	int nRet = -1;
	int nDataReadIndex = 0;
	int nDataWriteIndex = 0;
	int nDataBufSize = 0;

	if (psCycBufCxt == NULL || psCycBufCxt->m_pucDataBuf == NULL || pucData == NULL)
	{
		PL_LOGE(PL_TAG, "param error !\n");
		return -1;
	}

	pthread_mutex_lock(&psCycBufCxt->m_mutex);
	nDataReadIndex = psCycBufCxt->m_nDataReadIndex;
	nDataWriteIndex = psCycBufCxt->m_nDataWriteIndex;
	nDataBufSize = psCycBufCxt->m_nDataBufSize;

	PL_LOGD(PL_TAG, "nDataReadIndex(%d) nDataWriteIndex(%d) nDataLen(%d)\n", nDataReadIndex, nDataWriteIndex, nDataLen);

	if (nDataReadIndex < nDataWriteIndex)
	{
		if (nDataLen < nDataWriteIndex - nDataReadIndex)
		{
			memcpy(pucData, psCycBufCxt->m_pucDataBuf + nDataReadIndex, nDataLen);
			nDataReadIndex = nDataReadIndex + nDataLen;
			nRet = nDataLen;
		}
		else
		{
			//有截断
			memcpy(pucData, psCycBufCxt->m_pucDataBuf + nDataReadIndex, nDataWriteIndex - nDataReadIndex);
			nRet = nDataWriteIndex - nDataReadIndex;
			nDataReadIndex = nDataWriteIndex;
		}
	}
	else if (nDataReadIndex == nDataWriteIndex)
	{
		nRet = 0;
	}
	else
	{
		if (nDataLen < nDataBufSize - nDataReadIndex)
		{
			memcpy(pucData, psCycBufCxt->m_pucDataBuf + nDataReadIndex, nDataLen);
			nDataReadIndex = nDataReadIndex + nDataLen;
			nRet = nDataLen;
		}
		else
		{
			int nFirstCopyLen = nDataBufSize -  nDataReadIndex;
			memcpy(pucData, psCycBufCxt->m_pucDataBuf + nDataReadIndex, nFirstCopyLen);
			nDataReadIndex = 0;//(nDataReadIndex+nFirstCopyLen)%nDataBufSize
			if (nDataLen < nFirstCopyLen + nDataWriteIndex)
			{
				memcpy(pucData + nFirstCopyLen, psCycBufCxt->m_pucDataBuf, nDataLen - nFirstCopyLen);
				nDataReadIndex = nDataLen - nFirstCopyLen;
				nRet = nDataLen;
			}
			else
			{
				//有截断
				memcpy(pucData + nFirstCopyLen, psCycBufCxt->m_pucDataBuf, nDataWriteIndex);
				nDataReadIndex = nDataWriteIndex;
				nRet = nFirstCopyLen + nDataWriteIndex;
			}
		}
	}

	if (nRet > 0)
	{
		PL_LOGD(PL_TAG, "check  nRet(%d) nDataReadIndex(%d)\n", nRet, nDataReadIndex);
		psCycBufCxt->m_nDataReadIndex = nDataReadIndex;
	}

	pthread_mutex_unlock(&psCycBufCxt->m_mutex);

	return nRet;
}
