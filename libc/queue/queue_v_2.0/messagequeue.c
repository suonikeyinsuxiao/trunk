/***********************************************************************
*   Copyright (C) 2016 pilot-lab.inc All rights reserved.
*   
*   @file:       messagequeue.c
*   @brief:      
*   @author:     Pilot labs
*   @maintainer: frank.fu@pilot-lab.com.cn
*   @version:    1.0
*   @date:       2016-05-09
*   
***********************************************************************/
#include "messagequeue.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

#ifndef TAG
#define TAG "MSGQUEUE"
#endif


#define LOG(tag, level, fmt, arg...)	\
	printf("[%s][%s:%d]:%s "fmt" \n", tag, __FUNCTION__, __LINE__, level, ##arg);

#define LOGE(tag, fmt, arg...) LOG(tag, "E", fmt, ##arg)
#define LOGD(tag, fmt, arg...) LOG(tag, "D", fmt, ##arg)
#define LOGI(tag, fmt, arg...) LOG(tag, "I", fmt, ##arg)



typedef struct _MsgNode_S
{
	int 				m_nFree;
	MSG_S				m_sMsg;
	struct _MsgNode_S* 	m_psNext;

}MSGNODE_S;

typedef struct _MsgQueueContext_S
{
	int				m_nCount;
	int				m_nMaxMsgNum;
	MSGNODE_S* 		m_psHead;
	MSGNODE_S* 		m_psNodes;
	pthread_mutex_t m_mutex;
	sem_t			m_sem;

}MSGQUEUECONTEXT_S;

/**
 * @brief    createMsgQueue 
 *
 * @param[in]    nMaxMsgNum
 *
 * @return   success return message handle, failed return NULL 
 */
MSGQUEUEHANDLE createMsgQueue(int nMaxMsgNum)
{
	MSGQUEUECONTEXT_S* psMsgQueCxt = NULL;
	int i = 0;

	psMsgQueCxt = (MSGQUEUECONTEXT_S*)malloc(sizeof(MSGQUEUECONTEXT_S));
	if (NULL == psMsgQueCxt)
	{
		LOGE(TAG, "malloc message queue context failed");	
		return NULL;
	}
	memset(psMsgQueCxt, 0, sizeof(MSGQUEUECONTEXT_S));

	psMsgQueCxt->m_psNodes = (MSGNODE_S*)malloc(nMaxMsgNum * sizeof(MSGNODE_S));
	if (NULL == psMsgQueCxt->m_psNodes)
	{
		LOGE(TAG, "malloc message nodes failed");	
		free(psMsgQueCxt);
		return NULL;
	}
	memset(psMsgQueCxt->m_psNodes, 0, nMaxMsgNum * sizeof(MSGNODE_S));

	for (i = 0; i < nMaxMsgNum; i++)
	{
		psMsgQueCxt->m_psNodes[i].m_nFree = 1;
		psMsgQueCxt->m_psNodes[i].m_sMsg.m_eMsgType = e_UNKNOWN;
	}

	psMsgQueCxt->m_nMaxMsgNum = nMaxMsgNum;
	pthread_mutex_init(&psMsgQueCxt->m_mutex, NULL);

	sem_init(&psMsgQueCxt->m_sem, 0, 0);

	return psMsgQueCxt;
}

/**
 * @brief    destoryMsgQueue 
 *
 * @param[in]    pMSGQUEUEHANDLE
 */
void destoryMsgQueue(MSGQUEUEHANDLE psMsgQueCxt)
{
	if (NULL == psMsgQueCxt)
	{
		LOGE(TAG, "NULL == psMsgQueCxt");	
		return;
	}

	MSGQUEUECONTEXT_S* psMQCxt = (MSGQUEUECONTEXT_S*)psMsgQueCxt;
	MSGNODE_S* psNode = NULL;

	if (NULL != psMQCxt->m_psNodes)
	{
		while(psMQCxt->m_psHead)
		{
			psNode = psMQCxt->m_psHead;
			psMQCxt->m_psHead = psNode->m_psNext;
			if (!psNode->m_nFree)
			{
				free(psNode->m_sMsg.m_pMsgData);	
				psNode->m_sMsg.m_pMsgData = NULL;
			}
		
		}
		free(psMQCxt->m_psNodes);	
		psMQCxt->m_psNodes =NULL;
	}

	pthread_mutex_destroy(&psMQCxt->m_mutex);
	sem_destroy(&psMQCxt->m_sem);

	free(psMQCxt);
	psMQCxt = NULL;

	return;
}

//insert a message into the messagequeue
/**
 * @brief    sendMsg, insert a message into the messagequeue
 * 				notice:	psMsg->m_pMsgData the caller need to allocate storage space
 *
 * @param[in]    pMSGQUEUEHANDLE
 * @param[in]    psMsg
 *
 * @return   success return 0, failed return -1 
 */
int sendMsg(MSGQUEUEHANDLE psMsgQueCxt, MSG_S* psMsg)
{
	if (NULL == psMsgQueCxt)
	{
		LOGE(TAG, "NULL == psMsgQueCxt");	
		return -1;
	}

	int i = 0;
	MSGNODE_S* psNode = NULL;
	MSGNODE_S* psTmp = NULL;

	
	MSGQUEUECONTEXT_S* psMQCxt = (MSGQUEUECONTEXT_S*)psMsgQueCxt;

	pthread_mutex_lock(&psMQCxt->m_mutex);

	if (psMQCxt->m_nCount >= psMQCxt->m_nMaxMsgNum)
	{
		LOGD(TAG, "message count exceed, current message count=%d, max message count =%d", psMQCxt->m_nCount, psMQCxt->m_nMaxMsgNum);	
		pthread_mutex_unlock(&psMQCxt->m_mutex);
		return -1;
	}
	
	psTmp = psMQCxt->m_psNodes;
	for (i = 0; i < psMQCxt->m_nMaxMsgNum; i++, psTmp++)
	{
		if (psTmp->m_nFree)	
		{
			psNode = psTmp;//get free message node  
			break;
		}
	}

	psNode->m_nFree = 0;
	psNode->m_sMsg.m_nMsgId = psMsg->m_nMsgId;
	psNode->m_sMsg.m_nMsgLen = psMsg->m_nMsgLen;
	psNode->m_sMsg.m_eMsgType = psMsg->m_eMsgType;

	switch(psMsg->m_eMsgType)
	{
		case e_INT:
			{
				psNode->m_sMsg.m_pMsgData = (int*)malloc(psMsg->m_nMsgLen);
				memcpy(psNode->m_sMsg.m_pMsgData, psMsg->m_pMsgData, psMsg->m_nMsgLen);
			}
			break;
		case e_LONG:
			{
				psNode->m_sMsg.m_pMsgData = (long*)malloc(psMsg->m_nMsgLen);
				memcpy(psNode->m_sMsg.m_pMsgData, psMsg->m_pMsgData, psMsg->m_nMsgLen);
			
			}
			break;
		case e_FLOAT:
			{
				psNode->m_sMsg.m_pMsgData = (float*)malloc(psMsg->m_nMsgLen);
				memcpy(psNode->m_sMsg.m_pMsgData, psMsg->m_pMsgData, psMsg->m_nMsgLen);
			
			}
			break;
		case e_DOUBLE:
			{
				psNode->m_sMsg.m_pMsgData = (double*)malloc(psMsg->m_nMsgLen);
				memcpy(psNode->m_sMsg.m_pMsgData, psMsg->m_pMsgData, psMsg->m_nMsgLen);
			
			}
			break;
		case e_STRING:
			{
				psNode->m_sMsg.m_pMsgData = (unsigned char*)malloc(psMsg->m_nMsgLen);
				memcpy(psNode->m_sMsg.m_pMsgData, psMsg->m_pMsgData, psMsg->m_nMsgLen);
			
			}
			break;
		default:
			{
			LOGI(TAG, "unknown message type!!!");
			psNode->m_sMsg.m_pMsgData = (unsigned char*)malloc(psMsg->m_nMsgLen);
			memcpy(psNode->m_sMsg.m_pMsgData, psMsg->m_pMsgData, psMsg->m_nMsgLen);
			}
			break;
	}

	psNode->m_psNext = NULL;

	psTmp = psMQCxt->m_psHead;
	if (NULL == psTmp)
		psMQCxt->m_psHead = psNode;
	else
	{
		while(NULL != psTmp->m_psNext)
			psTmp = psTmp->m_psNext;
		psTmp->m_psNext = psNode;	
	}

	psMQCxt->m_nCount++;

	pthread_mutex_unlock(&psMQCxt->m_mutex);

	sem_post(&psMQCxt->m_sem);

	return 0;
}

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
int waitSemTimeout(sem_t* pSem, long lTime_ms)
{
	int nRet = -1;

    struct timeval  tv;
    struct timespec ts;

    if(lTime_ms == -1)
    {
        nRet = sem_wait(pSem);
    }
    else
    {
        gettimeofday(&tv, NULL);
        ts.tv_sec  = tv.tv_sec;
        ts.tv_nsec = tv.tv_usec*1000 + lTime_ms*1000000;
        ts.tv_sec += ts.tv_nsec/(1000*1000*1000);
        ts.tv_nsec = ts.tv_nsec % (1000*1000*1000);
        
        nRet = sem_timedwait(pSem, &ts);
    }

    return nRet;
}

/**
 * @brief    recvMsg, get a message from message queue
 *				notice:	psMsg->m_pMsgData the caller need to allocate storage space
 * @param[in]    pMSGQUEUEHANDLE
 * @param[in]    psMsg
 *
 * @return    success return 0, failed return -1
 */
int recvMsg(MSGQUEUEHANDLE psMsgQueCxt, MSG_S* psMsg)
{
	if (NULL == psMsgQueCxt)
	{
		LOGE(TAG, "NULL == psMsgQueCxt");	
		return -1;
	}

	MSGQUEUECONTEXT_S* psMQCxt = (MSGQUEUECONTEXT_S*)psMsgQueCxt;
	MSGNODE_S* psNode = NULL;

	if (waitSemTimeout(&psMQCxt->m_sem, -1) < 0)
	{
		return -1;	
	}

	pthread_mutex_lock(&psMQCxt->m_mutex);

	if (psMQCxt->m_nCount <= 0)
	{
		LOGE(TAG, "message queue is empty!!!");	
		pthread_mutex_unlock(&psMQCxt->m_mutex);
		return -1;
	}

	psNode = psMQCxt->m_psHead;
	psMQCxt->m_psHead = psNode->m_psNext;

	psNode->m_nFree = 1;
	psMsg->m_nMsgId = psNode->m_sMsg.m_nMsgId;
	psMsg->m_nMsgLen = psNode->m_sMsg.m_nMsgLen;
	psMsg->m_eMsgType = psNode->m_sMsg.m_eMsgType;

	memcpy(psMsg->m_pMsgData, psNode->m_sMsg.m_pMsgData, psNode->m_sMsg.m_nMsgLen);
	free(psNode->m_sMsg.m_pMsgData);
	psNode->m_sMsg.m_pMsgData = NULL;
#if 0
	switch(psMQCxt->m_eMsgType)
	{
		case e_INT:
			{
				memcpy(psMsg->m_pMsgData, psNode->m_sMsg.m_pMsgData, psNode->m_sMsg.m_nMsgLen);
				free(psNode->m_sMsg.m_pMsgData);
				psNode->m_sMsg.m_pMsgData = NULL;
			}
			break;
		case e_LONG:
			{
				psNode->m_sMsg.m_pMsgData = (long*)malloc(psMsg->m_nMsgLen);
				memcpy(psNode->m_sMsg.m_pMsgData, psMsg->m_pMsgData, psMsg->m_nMsgLen);
			
			}
			break;
		case e_FLOAT:
			{
				psNode->m_sMsg.m_pMsgData = (float*)malloc(psMsg->m_nMsgLen);
				memcpy(psNode->m_sMsg.m_pMsgData, psMsg->m_pMsgData, psMsg->m_nMsgLen);
			
			}
			break;
		case e_DOUBLE:
			{
				psNode->m_sMsg.m_pMsgData = (double*)malloc(psMsg->m_nMsgLen);
				memcpy(psNode->m_sMsg.m_pMsgData, psMsg->m_pMsgData, psMsg->m_nMsgLen);
			
			}
			break;
		case e_STRING:
			{
				psNode->m_sMsg.m_pMsgData = (unsigned char*)malloc(psMsg->m_nMsgLen);
				memcpy(psNode->m_sMsg.m_pMsgData, psMsg->m_pMsgData, psMsg->m_nMsgLen);
			
			}
			break;
		default:
			LOGI(TAG, "unknown message type!!!");
			return -1;
	}
#endif

	psMQCxt->m_nCount--;

	pthread_mutex_unlock(&psMQCxt->m_mutex);

	return 0;
}

/**
 * @brief    clearMsgQueue 
 *
 * @param[in]    pMSGQUEUEHANDLE
 *
 * @return    success return 0, failed return -1
 */
int clearMsgQueue(MSGQUEUEHANDLE psMsgQueCxt)
{
	if (NULL == psMsgQueCxt)
	{
		LOGE(TAG, "NULL == psMsgQueCxt");	
		return -1;
	}

	MSGQUEUECONTEXT_S* psMQCxt = (MSGQUEUECONTEXT_S*)psMsgQueCxt;
	MSGNODE_S* psNode = NULL;
	int i;

	pthread_mutex_lock(&psMQCxt->m_mutex);

	do
	{
		if (sem_getvalue(&psMQCxt->m_sem, &i) != 0)//sem free	
		{
			if (NULL != psMQCxt->m_psNodes)
			{
				while(psMQCxt->m_psHead)
				{
					psNode = psMQCxt->m_psHead;
					psMQCxt->m_psHead = psNode->m_psNext;
					if (!psNode->m_nFree)
					{
						free(psNode->m_sMsg.m_pMsgData);	
						psNode->m_sMsg.m_pMsgData = NULL;
					}
				
				}

				psMQCxt->m_psHead = NULL;
				psMQCxt->m_nCount = 0;
				for (i = 0; i < psMQCxt->m_nMaxMsgNum; i++)
					psMQCxt->m_psNodes[i].m_nFree = 1;
			}	
			break;
		}

		sem_trywait(&psMQCxt->m_sem);
	}while(1);

	pthread_mutex_unlock(&psMQCxt->m_mutex);

	return 0;
}

void printMsg(MSG_S* psMsg)
{
	LOGD(TAG, "msg id=%d", psMsg->m_nMsgId);
	LOGD(TAG, "msg type=%x", psMsg->m_eMsgType);
	LOGD(TAG, "msg len=%d", psMsg->m_nMsgLen);

	switch(psMsg->m_eMsgType)
	{
		case e_INT:
			LOGD(TAG, "msg data=%d",  *((int*)(psMsg->m_pMsgData)));
			break;
		case e_LONG:
			LOGD(TAG, "msg data=%ld", *((long*)(psMsg->m_pMsgData)));
			break;
		case e_FLOAT:
			LOGD(TAG, "msg data=%f",  *((float*)(psMsg->m_pMsgData)));
			break;
		case e_DOUBLE:
			LOGD(TAG, "msg data=%lf", *((double*)(psMsg->m_pMsgData)));
			break;
		case e_STRING:
			LOGD(TAG, "msg data=%s",  (char*)psMsg->m_pMsgData);
			break;

	}
	LOGD(TAG,"\n");
}


