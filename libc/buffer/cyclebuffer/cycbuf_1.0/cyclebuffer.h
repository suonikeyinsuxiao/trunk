/***********************************************************************
*   Copyright (C) 2016 pilot-lab.inc All rights reserved.
*   
*   @file:       cyclebuffer.h
*   @brief:      
*   @author:     Pilot labs
*   @maintainer: frank.fu@pilot-lab.com.cn
*   @version:    1.0
*   @date:       2016-05-27
*   
***********************************************************************/
#ifndef _CYCLEBUFFER_H
#define _CYCLEBUFFER_H
#ifdef __cplusplus
extern "C"
{
#endif

#include <pthread.h>

/**
 * @brief   cycle buffer context 
 */
typedef struct _cycleBufferCxt_S
{
	unsigned char*	m_pucDataBuf;		//循环buffer地址
	int 			m_nDataBufSize;		//循环buffer总长度
	int 			m_nDataWriteIndex;	//写索引
	int 			m_nDataReadIndex;	//读索引
	pthread_mutex_t m_mutex;
}CYCLEBUFFERCXT_S;

/**
 * @brief    createCycleBuffer 
 *
 * @param[in]    nBufferSize
 *
 * @return   success return CYCLEBUFFERCXT_S*, otherwise NULL
 */
CYCLEBUFFERCXT_S* createCycleBuffer(int nBufferSize);


/**
 * @brief    initCycleBuffer 
 *
 * @param[in]    psCycBufCxt
 * @param[in]    nWriteIndex
 * @param[in]    nReadIndex
 *
 * @return    success return 0; otherwise -1
 */
int initCycleBuffer(CYCLEBUFFERCXT_S* psCycBufCxt, int nWriteIndex, int nReadIndex);

/**
 * @brief    destroyCycleBuffer 
 *
 * @param[in]    psCycBufCxt
 *
 * @return    success return 0; otherwise -1
 */
int destroyCycleBuffer(CYCLEBUFFERCXT_S* psCycBufCxt);

/**
 * @brief    writeCycleBuffer 
 *
 * @param[in]    psCycBufCxt 循环buffer上下文信息
 * @param[in]    pucData     写入循环buffer的数据		
 * @param[in]    nDataLen	 写入循环buffer的数据长度
 *
 * @return   实际写入数据的长度, error return -1
 */
int writeCycleBuffer(CYCLEBUFFERCXT_S* psCycBufCxt, unsigned char* pucData, int nDataLen);

/**
 * @brief    readCycleBuffer 
 *
 * @param[in]    psCycBufCxt	循环buffer上下文信息
 * @param[out]   pucData		从循环buffer读出的数据
 * @param[in]    nDataLen		需从循环buffer读出的数据的长度
 *
 * @return   实际读出的数据长度, error return -1;如果实际读出的数据小于nDataLen,需要再次读取剩下的数据
 */
int readCycleBuffer(CYCLEBUFFERCXT_S* psCycBufCxt, unsigned char * pucData, int nDataLen);

#ifdef __cplusplus
}
#endif
#endif // _CYCLEBUFFER_H
