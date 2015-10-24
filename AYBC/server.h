#ifndef __SERVER_H
#define __SERVER_H

#include "message.h"
#include "queue.h"

#define SERVERPORT 12345 //server port

typedef enum
{
	SERVER_UNCONNECTED,
	SERVER_CONNECTED
}SERVERSTATUS_E;

typedef struct _ClientHandle
{
	int m_nClientSockFd;	//accept() return sockfd
	int m_nClientIndex;		//index of client connected to server 
	
	pthread_t m_clientPthread_tid;	//recieve video data pthread
	int m_nClientPthread_exit;		//client thread exit flag

	AYBCQueue_S* m_psQHandler;		//data buffer queue handler

}AYBCClientHandle_S;

typedef struct _ServerHandle
{
	int m_nServerSockFd;				//server socket	
	U16 m_port;							//server port

	pthread_t m_acceptPthread_tid;		//accept thread id
	int m_nacceptPthread_exit;			//accept thread exit flag

	int m_clientCount;					//max number of client allowed to connect server
	AYBCClientHandle_S* m_clientHandle; //client handle
	//void* pdecoder					//decoder handler

}AYBCServerHandle_S;

//等待客户端连接线程
void acceptPthread(void *args);

//与客户端通讯线程
void clientPthread(void *args);

void initServer(AYBCServerHandle_S* psServHandle);

void destroyServer(AYBCServerHandle_S* psServHandle);

#endif	//__SERVER_H
