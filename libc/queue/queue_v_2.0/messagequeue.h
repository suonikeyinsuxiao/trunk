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

MSGQUEUEHANDLE createMsgQueue(int nMaxMsgNum);
void destoryMsgQueue(MSGQUEUEHANDLE pMSGQUEUEHANDLE);
int sendMsg(MSGQUEUEHANDLE pMSGQUEUEHANDLE, MSG_S* psMsg);
int waitSemTimeout(sem_t* pSem, long lTime_ms);
int recvMsg(MSGQUEUEHANDLE pMSGQUEUEHANDLE, MSG_S* psMsg);
int clearMsgQueue(MSGQUEUEHANDLE pMSGQUEUEHANDLE);
void printMsg(MSG_S* psMsg);
