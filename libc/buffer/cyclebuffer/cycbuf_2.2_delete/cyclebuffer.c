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
 * @param[in]    nBufferSize	unit的个数
 * @param[in]    eBufType
 * 				 当eBufType=e_normal时，cycbuffer的实际大小为nBufferSize个字节
 * 				 当eBufType=e_double时，cycbuffer的实际大小为nBufferSize*sizeof(double)个字节
 *
 * @return    
 */
CYCLEBUFFERCXT_S* createCycleBuffer(int nBufferSize, BUFFERTYPE_E eBufType)
{
	CYCLEBUFFERCXT_S* psCycBufCxt = (CYCLEBUFFERCXT_S*)malloc(sizeof(CYCLEBUFFERCXT_S));
	if (NULL == psCycBufCxt)
	{
		PL_LOGE(PL_TAG, "malloc CYCLEBUFFERCXT_S failed!\n");	
		return NULL;
	}

	psCycBufCxt->m_eBufferType = eBufType;
	switch (eBufType)
	{
		case e_normal:
			{
				psCycBufCxt->m_pucDataBuf = (unsigned char*)calloc(nBufferSize, sizeof(unsigned char));
				if (NULL == psCycBufCxt->m_pucDataBuf)
				{
					PL_LOGE(PL_TAG, "calloc buffer(size %dByte) failed!\n", nBufferSize);	
					return NULL;
				}
				psCycBufCxt->m_nDataBufSize	= nBufferSize;
			}
			break;
		case e_double:
			{
				psCycBufCxt->m_pucDataBuf = (unsigned char*)calloc(nBufferSize, sizeof(double));
				if (NULL == psCycBufCxt->m_pucDataBuf)
				{
					PL_LOGE(PL_TAG, "calloc buffer(size %dByte) failed!\n", nBufferSize);	
					return NULL;
				}
				
				psCycBufCxt->m_nDataBufSize	= nBufferSize*sizeof(double);
			}
			break;
		default:
			PL_LOGE(PL_TAG, "unknown cycle buffer type!\n");	
			
	}
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

	if (NULL != psCycBufCxt->m_pucDataBuf)
	{
		free(psCycBufCxt->m_pucDataBuf);
		psCycBufCxt->m_pucDataBuf = NULL;
		psCycBufCxt->m_nDataRealLen = 0;
	}

	pthread_mutex_destroy(&psCycBufCxt->m_mutex);

	free(psCycBufCxt);
	psCycBufCxt = NULL;


	return 0;
}

/**
 * @brief    writeCycleBuffer 
 *
 * @param[in]    psCycBufCxt 循环buffer上下文信息
 * @param[in]    pucData     写入循环buffer的数据首地址		
 * @param[in]    nDataLen	 写入循环buffer的数据unit个数
 *
 * @return   实际写入数据unit个数, error return -1
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
	//nRealWriteLen = nFreeLen > nDataLen ? nDataLen : nFreeLen;
	switch (psCycBufCxt->m_eBufferType)
	{
		case e_normal:
			{
				nRealWriteLen = nFreeLen > nDataLen ? nDataLen : nFreeLen;
			}
			break;
		case e_double:
			{
				nRealWriteLen = nFreeLen > (nDataLen*sizeof(double)) ? (nDataLen*sizeof(double)) : nFreeLen;
			}
			break;
	}

	if (psCycBufCxt->m_nDataWriteIndex >= psCycBufCxt->m_nDataReadIndex)
	{
		int nFirstCopyLen = psCycBufCxt->m_nDataBufSize - psCycBufCxt->m_nDataWriteIndex;	
		if (nRealWriteLen >= nFirstCopyLen)
		{
#if 0
			switch(psCycBufCxt->m_eBufferType)
			{
				case e_normal:
					{
						memcpy(psCycBufCxt->m_pucDataBuf + psCycBufCxt->m_nDataWriteIndex, pucData, nFirstCopyLen);
						memcpy(psCycBufCxt->m_pucDataBuf, pucData+nFirstCopyLen, nRealWriteLen - nFirstCopyLen);
					}
					break;
				case e_double:
					{
						memcpy(psCycBufCxt->m_pucDataBuf + psCycBufCxt->m_nDataWriteIndex, pucData, nFirstCopyLen*sizeof(double));
						memcpy(psCycBufCxt->m_pucDataBuf, pucData+nFirstCopyLen, nRealWriteLen - nFirstCopyLen*sizeof(double));
					}
					break;
			}
#endif
			memcpy(psCycBufCxt->m_pucDataBuf + psCycBufCxt->m_nDataWriteIndex, pucData, nFirstCopyLen);
			memcpy(psCycBufCxt->m_pucDataBuf, pucData+nFirstCopyLen, nRealWriteLen - nFirstCopyLen);
		}
		else
		{
#if 0
			switch(psCycBufCxt->m_eBufferType)
			{
				case e_normal:
					{
						memcpy(psCycBufCxt->m_pucDataBuf + psCycBufCxt->m_nDataWriteIndex, pucData, nRealWriteLen);
					}
					break;
				case e_double:
					{
						memcpy(psCycBufCxt->m_pucDataBuf + psCycBufCxt->m_nDataWriteIndex, pucData, nRealWriteLen);
					}
					break;
			}
#endif
			memcpy(psCycBufCxt->m_pucDataBuf + psCycBufCxt->m_nDataWriteIndex, pucData, nRealWriteLen);
		}
	}
	else
	{
		memcpy(psCycBufCxt->m_pucDataBuf + psCycBufCxt->m_nDataWriteIndex, pucData, nRealWriteLen);
	}

	switch (psCycBufCxt->m_eBufferType)
	{
		case e_normal:
		case e_double:
			{
				psCycBufCxt->m_nDataWriteIndex = (nRealWriteLen + psCycBufCxt->m_nDataWriteIndex)%psCycBufCxt->m_nDataBufSize;
				psCycBufCxt->m_nDataRealLen += nRealWriteLen;
			}
			break;
		case e_doublew:
			{
				psCycBufCxt->m_nDataWriteIndex = (STEP*sizeof(double) + psCycBufCxt->m_nDataWriteIndex)%psCycBufCxt->m_nDataBufSize;
				psCycBufCxt->m_nDataRealLen += nRealWriteLen;
			}
			break;
	}

	if (psCycBufCxt->m_eBufferType == e_double)
		nRealWriteLen = nRealWriteLen/sizeof(double);//返回写入cyclebuffer的最小单位个数

	pthread_mutex_unlock(&psCycBufCxt->m_mutex);

	return nRealWriteLen;
}

/**
 * @brief    readCycleBuffer 
 *
 * @param[in]    psCycBufCxt	循环buffer上下文信息
 * @param[out]   pucData		从循环buffer读出的数据
 * @param[in]    nDataLen		需从循环buffer读出的数据unit个数
 *
 * @return   实际读出的数据unit个数, error return -1;如果实际读出的数据unit个数小于nDataLen,需要再次读取剩下的数据
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

//	nRealReadLen = nDataLen > psCycBufCxt->m_nDataRealLen ? psCycBufCxt->m_nDataRealLen : nDataLen;
	switch (psCycBufCxt->m_eBufferType)
	{
		case e_normal:
			{
				nRealReadLen = nDataLen > psCycBufCxt->m_nDataRealLen ? psCycBufCxt->m_nDataRealLen : nDataLen;
			}
			break;
		case e_double:
			{
				nRealReadLen = (nDataLen*sizeof(double)) > psCycBufCxt->m_nDataRealLen ? psCycBufCxt->m_nDataRealLen : (nDataLen*sizeof(double));
			}
			break;
	}

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
	switch (psCycBufCxt->m_eBufferType)
	{
		case e_normal:
			{
				psCycBufCxt->m_nDataReadIndex = (psCycBufCxt->m_nDataReadIndex + nRealReadLen)%psCycBufCxt->m_nDataBufSize;
				psCycBufCxt->m_nDataRealLen -= nRealReadLen;
			}
			break;
		case e_double:
			{
				psCycBufCxt->m_nDataReadIndex = (psCycBufCxt->m_nDataReadIndex + STEP*sizeof(double))%psCycBufCxt->m_nDataBufSize;
				psCycBufCxt->m_nDataRealLen -= STEP*sizeof(double);
			}
			break;
	}

	
	if (psCycBufCxt->m_eBufferType == e_double)
		nRealReadLen = nRealReadLen/sizeof(double);//返回读出cyclebuffer的最小单位个数

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
 * @brief    getAvailDataLen 获取循环buffer中有效数据unit的个数
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

	int nDataLen = 0;

	switch(psCycBufCxt->m_eBufferType)
	{
		case(e_normal):	
		{
			nDataLen = psCycBufCxt->m_nDataRealLen;
		}
		break;
		case(e_double):
		{
			nDataLen = psCycBufCxt->m_nDataRealLen/sizeof(double);
		}
		break;
	}

	return nDataLen; 
}


