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

#define STEP 85
#define CYCLEBUFFERLEN 4096

typedef enum
{
	e_normal = 0x100, //unit长度为1个字节
	e_double = 0x101, //unit长度为sizeof(double)个字节
	e_unknown
}BUFFERTYPE_E;

/**
 * @brief   cycle buffer context 
 */
typedef struct _cycleBufferCxt_S
{
	unsigned char*	m_pucDataBuf;		//循环buffer地址
	int 			m_nDataBufSize;		//循环buffer总长度
	int 			m_nDataWriteIndex;	//写索引
	int 			m_nDataReadIndex;	//读索引
	int				m_nDataRealLen;		//循环buffer中可获取的数据长度
	pthread_mutex_t m_mutex;
	BUFFERTYPE_E    m_eBufferType;

}CYCLEBUFFERCXT_S;


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
CYCLEBUFFERCXT_S* createCycleBuffer(int nBufferSize, BUFFERTYPE_E eBufType);


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
 * @param[in]    pucData     写入循环buffer的数据首地址		
 * @param[in]    nDataLen	 写入循环buffer的数据unit个数
 *
 * @return   实际写入数据unit个数, error return -1
 */
int writeCycleBuffer(CYCLEBUFFERCXT_S* psCycBufCxt, unsigned char* pucData, int nDataLen);

/**
 * @brief    readCycleBuffer 
 *
 * @param[in]    psCycBufCxt	循环buffer上下文信息
 * @param[out]   pucData		从循环buffer读出的数据
 * @param[in]    nDataLen		需从循环buffer读出的数据unit个数
 *
 * @return   实际读出的数据unit个数, error return -1;如果实际读出的数据unit个数小于nDataLen,需要再次读取剩下的数据
 */
int readCycleBuffer(CYCLEBUFFERCXT_S* psCycBufCxt, unsigned char * pucData, int nDataLen);

/**
 * @brief    isEmptyCycleBuffer	判断循环buffer是否为空 
 *
 * @param[in]    psCycBufCxt
 *
 * @return   1 is empty, 0 is not empty , -1 is error
 */
int isEmptyCycleBuffer(CYCLEBUFFERCXT_S* psCycBufCxt);

/**
 * @brief    isFullCycleBuffer 判断循环buffer是否为满
 *
 * @param[in]    psCycBufCxt
 *
 * @return    1 is full, 0 is not full, -1 is error 
 */
int isFullCycleBuffer(CYCLEBUFFERCXT_S* psCycBufCxt);

/**
 * @brief    getAvailDataLen 获取循环buffer中有效数据unit的个数
 *
 * @param[in]    psCycBufCxt
 *
 * @return   real data length 
 */
int getAvailDataLen(CYCLEBUFFERCXT_S* psCycBufCxt);

#ifdef __cplusplus
}
#endif
#endif // _CYCLEBUFFER_H
