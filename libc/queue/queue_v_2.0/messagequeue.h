/***********************************************************************
*   Copyright (C) 2016 pilot-lab.inc All rights reserved.
*   
*   @file:       messagequeue.h
*   @brief:      
*   @author:     Pilot labs
*   @maintainer: frank.fu@pilot-lab.com.cn
*   @version:    1.0
*   @date:       2016-05-09
*   
***********************************************************************/
#ifndef _MESSAGEQUEUE_H
#define _MESSAGEQUEUE_H
#ifdef __cplusplus
extern "C"
{
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>

#define SIZE 5

typedef void* MSGQUEUEHANDLE;
typedef void* PMESSAGE;

typedef enum
{
	e_INT = 0x1001,
	e_LONG, 
	e_FLOAT, 
	e_DOUBLE,
	e_STRING,
	e_UNKNOWN
}MSGTYPE_E;

typedef struct _Msg_S
{
    int 		m_nMsgId;
	int 		m_nMsgLen;
	PMESSAGE 	m_pMsgData;	
	MSGTYPE_E	m_eMsgType;

}MSG_S;



/**
 * @brief    createMsgQueue 
 *
 * @param[in]    nMaxMsgNum
 *
 * @return   success return message handle, failed return NULL 
 */
MSGQUEUEHANDLE createMsgQueue(int nMaxMsgNum);

/**
 * @brief    destoryMsgQueue 
 *
 * @param[in]    pMSGQUEUEHANDLE
 */
void destoryMsgQueue(MSGQUEUEHANDLE pMsgQueueHandle);

/**
 * @brief    sendMsg, insert a message into the messagequeue
 * 				notice:	psMsg->m_pMsgData the caller need to allocate storage space
 *
 * @param[in]    pMSGQUEUEHANDLE
 * @param[in]    psMsg
 *
 * @return   success return 0, failed return -1 
 */
int sendMsg(MSGQUEUEHANDLE pMsgQueueHandle, MSG_S* psMsg);

/**
 * @brief    waitSemTimeout 
 * 				lTime_ms=-1,If the semamphore currently has the value zero, 
 * 					then the call blocks until either it becomes possible to perform the decrement.
 * 			If the timeout has already expired by the time of the call, and the semaphore could not be locked immediately, then waitSemTimeOut fails with a timeout error
 *
 * @param[in]    pSem
 * @param[in]    lTime_ms
 *
 * @return   return 0 on success; on error, the value of the semaphore is left unchanged, -1 is returned, and errno is set to indicate the error 
 */
int waitSemTimeout(sem_t* pSem, long lTime_ms);

/**
 * @brief    recvMsg, get a message from message queue
 *				notice:	psMsg->m_pMsgData the caller need to allocate storage space
 * @param[in]    pMSGQUEUEHANDLE
 * @param[in]    psMsg
 *
 * @return    success return 0, failed return -1
 */
int recvMsg(MSGQUEUEHANDLE pMsgQueueHandle, MSG_S* psMsg);

/**
 * @brief    clearMsgQueue 
 *
 * @param[in]    pMSGQUEUEHANDLE
 *
 * @return    success return 0, failed return -1
 */
int clearMsgQueue(MSGQUEUEHANDLE pMsgQueueHandle);

/**
 * @brief    printMsg 
 *
 * @param[in]    psMsg
 */
void printMsg(MSG_S* psMsg);

#ifdef __cplusplus
}
#endif
#endif // _MESSAGEQUEUE_H
