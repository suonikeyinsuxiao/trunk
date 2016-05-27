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
	psCycBufCxt->m_nDataRealLen 	= 0;

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
	psCycBufCxt->m_nDataRealLen 	= 0;

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
 * @return   实际写入数据的长度, 0 is full, error return -1
 */
int writeCycleBuffer(CYCLEBUFFERCXT_S* psCycBufCxt, unsigned char* pucData, int nDataLen)
{
	if (psCycBufCxt == NULL || psCycBufCxt->m_pucDataBuf == NULL || pucData == NULL)
	{
		PL_LOGE(PL_TAG, "param error !\n");
		return -1;
	}

	if (isFullCycleBuffer(psCycBufCxt))
		return 0;

	int nFreeLen = 0;
	int nRealWriteLen = 0;

	pthread_mutex_lock(&psCycBufCxt->m_mutex);

	nFreeLen = psCycBufCxt->m_nDataBufSize - psCycBufCxt->m_nDataRealLen;
	nRealWriteLen = nFreeLen > nDataLen ? nDataLen : nFreeLen;

	if (psCycBufCxt->m_nDataWriteIndex >= psCycBufCxt->m_nDataReadIndex)
	{
		int nFirstCopyLen = psCycBufCxt->m_nDataBufSize - psCycBufCxt->m_nDataWriteIndex;	
		if (nRealWriteLen >= nFirstCopyLen)
		{
			memcpy(psCycBufCxt->m_pucDataBuf + psCycBufCxt->m_nDataWriteIndex, pucData, nFirstCopyLen);
			memcpy(psCycBufCxt->m_pucDataBuf, pucData+nFirstCopyLen, nRealWriteLen - nFirstCopyLen);
				
		}
		else
		{
			memcpy(psCycBufCxt->m_pucDataBuf + psCycBufCxt->m_nDataWriteIndex, pucData, nRealWriteLen);
		}
	}
	else
	{
		memcpy(psCycBufCxt->m_pucDataBuf + psCycBufCxt->m_nDataWriteIndex, pucData, nRealWriteLen);
	}

	psCycBufCxt->m_nDataWriteIndex = (nRealWriteLen + psCycBufCxt->m_nDataWriteIndex)%psCycBufCxt->m_nDataBufSize;
	psCycBufCxt->m_nDataRealLen += nRealWriteLen;

	pthread_mutex_unlock(&psCycBufCxt->m_mutex);

	return nRealWriteLen;
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

	if (psCycBufCxt == NULL || psCycBufCxt->m_pucDataBuf == NULL || pucData == NULL)
	{
		PL_LOGE(PL_TAG, "param error !\n");
		return -1;
	}

	if (isEmptyCycleBuffer(psCycBufCxt))
		return 0;

	int nRealReadLen = 0;
	int nTmpLen = 0;

	pthread_mutex_lock(&psCycBufCxt->m_mutex);

	nRealReadLen = nDataLen > psCycBufCxt->m_nDataRealLen ? psCycBufCxt->m_nDataRealLen : nDataLen;
    nTmpLen = psCycBufCxt->m_nDataBufSize - psCycBufCxt->m_nDataReadIndex;

	if (nRealReadLen <= nTmpLen)
	{
		memcpy(pucData, psCycBufCxt->m_pucDataBuf + psCycBufCxt->m_nDataReadIndex, nRealReadLen);
	}
	else
	{
		memcpy(pucData, psCycBufCxt->m_pucDataBuf + psCycBufCxt->m_nDataReadIndex, nTmpLen);
		memcpy(pucData+nTmpLen, psCycBufCxt->m_pucDataBuf, nRealReadLen - nTmpLen);
	}
	psCycBufCxt->m_nDataReadIndex = (psCycBufCxt->m_nDataReadIndex + nRealReadLen)%psCycBufCxt->m_nDataBufSize;
	psCycBufCxt->m_nDataRealLen -= nRealReadLen;

	

	PL_LOGD(PL_TAG, "nDataReadIndex(%d) nDataWriteIndex(%d) nDataLen(%d)\n", nDataReadIndex, nDataWriteIndex, nDataLen);

	pthread_mutex_unlock(&psCycBufCxt->m_mutex);

	return nRealReadLen;
}

/**
 * @brief    isEmptyCycleBuffer	判断循环buffer是否为空 
 *
 * @param[in]    psCycBufCxt
 *
 * @return   1 is empty, 0 is not empty 
 */
int isEmptyCycleBuffer(CYCLEBUFFERCXT_S* psCycBufCxt)
{
	if (psCycBufCxt == NULL || psCycBufCxt->m_pucDataBuf == NULL)
	{
		PL_LOGE(PL_TAG, "param error !\n");
		return -1;
	}

	return (0 == psCycBufCxt->m_nDataRealLen);
}

/**
 * @brief    isFullCycleBuffer 判断循环buffer是否为满
 *
 * @param[in]    psCycBufCxt
 *
 * @return    1 is full, 0 is not full 
 */
int isFullCycleBuffer(CYCLEBUFFERCXT_S* psCycBufCxt)
{
	if (psCycBufCxt == NULL || psCycBufCxt->m_pucDataBuf == NULL)
	{
		PL_LOGE(PL_TAG, "param error !\n");
		return -1;
	}

	return (psCycBufCxt->m_nDataBufSize == psCycBufCxt->m_nDataRealLen);
}

/**
 * @brief    getAvailDataLen 获取循环buffer中有效数据的长度 
 *
 * @param[in]    psCycBufCxt
 *
 * @return   real data length 
 */
int getAvailDataLen(CYCLEBUFFERCXT_S* psCycBufCxt)
{
	if (psCycBufCxt == NULL || psCycBufCxt->m_pucDataBuf == NULL)
	{
		PL_LOGE(PL_TAG, "param error !\n");
		return -1;
	}

	return psCycBufCxt->m_nDataRealLen;
}


