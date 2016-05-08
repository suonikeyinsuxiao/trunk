#include "messagequeue.h"

#ifndef TAG
#define TAG "main"
#endif


#define LOG(tag, level, fmt, arg...)	\
	printf("[%s][%s:%d]:%s "fmt" \n", tag, __FUNCTION__, __LINE__, level, ##arg);

#define LOGE(tag, fmt, arg...) LOG(tag, "E", fmt, ##arg)
#define LOGD(tag, fmt, arg...) LOG(tag, "D", fmt, ##arg)
#define LOGI(tag, fmt, arg...) LOG(tag, "I", fmt, ##arg)


void* sendThread(void* arg)
{
    //pthread_detach(pthread_self());
    MSGQUEUEHANDLE pMsgQHandle = (MSGQUEUEHANDLE)arg;

    MSG_S msg;
    int nRet = -1;
    int nId = 100;
    int param0 = 1000;

    while(1)
    {
    	msg.m_nMsgId = nId;
    	msg.m_eMsgType = e_INT;
    	msg.m_nMsgLen = sizeof(int);
	    msg.m_pMsgData = &param0;

		nRet = sendMsg(pMsgQHandle, &msg);
        if (-1 == nRet)
        {
	    	LOGE(TAG, "sendThread:post message failed!\n");
	    	sleep(1);
	    	continue;
        }
	LOGE(TAG,"sendThread: msgId=%d, type=%x, data=%d", msg.m_nMsgId, msg.m_eMsgType, param0);
        nId++;
        param0++;
		if (param0 == 2000)
		{
			LOGE(TAG,"sendThread quit\n");
			break;
		}
        //sleep(1);
    }
}

int main(int argc, char** argv)
{

    pthread_t recvpid;
    pthread_t sendpid;
	MSG_S rcvmsg;
	int rcvData;

	MSGQUEUEHANDLE pCxt = createMsgQueue(SIZE);
	if (NULL == pCxt)
	{
		LOGE(TAG, "createMsgQueue failed!\n");	
		return -1;
	}


    pthread_create(&sendpid, NULL, sendThread, pCxt); 

	rcvmsg.m_pMsgData = &rcvData;

	while(1)
	{
		//sleep(1);
		recvMsg(pCxt, &rcvmsg);
		printMsg(&rcvmsg);
	}

	return 0;
}

