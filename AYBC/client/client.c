#include "common.h"
#include "encoder.h"
#include "videoCapture.h"
#include "netsocket.h"
#include "message.h"

void processMsg(MessageType_E msg);
void* sendThread(void* arg);

static int nSendThreadExit = -1;//1表示线程退出了,0表示线程在运行
static int nCameralStartFlag = 0;//0表示stop,1表示start
static int nExit = 0;//1表示主线程退出
static ENCODER_S encoder;
static pthread_t sendTid;	

int main(int argc, char** argv)
{
	int nRet = 0;

//	U8* pcEncodedData = NULL;
	
	if(-1 == initCamera())	
	{
		printf("initCamera failed!\n");			
		goto ERR;
	}
	
	if (-1 == initEncoder(&encoder, X264_CSP_I420, WIDTH, HEIGHT))
	{
		printf("init encoder failed!\n");
		goto ERR;
	}

	int nSockFd = -1;
	nSockFd = netSocketCreate(NET_SOCKET_TCP);
	if (-1 == nSockFd)
	{
		goto ERR;
	}
	encoder.m_nTCPSock = nSockFd;

	if (-1 == netSocketBind(encoder.m_nTCPSock, CLIENT_PORT))
	{
		goto ERR;
	}

	if (-1 == netSocketConnect(encoder.m_nTCPSock, NULL, SERVER_PORT))
	{
		goto ERR;
	}

	AYBCMsg_S sRecvMsgBuf;
	char acBuffer[AYBC_MSG_MAX_LEN] = {0};
	int nRecvLen = 0;

	while(1)
	{
		memset(&sRecvMsgBuf, 0, sizeof(AYBCMsg_S));
		if (0 == netSocketRecv(encoder.m_nTCPSock, acBuffer, AYBC_MSG_MAX_LEN, &nRecvLen, 0))			
		{
			decomposeMsg(acBuffer, &sRecvMsgBuf);	

			printf("TCP msgtype=%d,server ip:%s-port:%d",sRecvMsgBuf.m_eMsgType, sRecvMsgBuf.m_sRemoteAddr.m_acIp, sRecvMsgBuf.m_sRemoteAddr.m_port);
			memcpy(encoder.m_sServerUdp.m_acIp,sRecvMsgBuf.m_sRemoteAddr.m_acIp, strlen(sRecvMsgBuf.m_sRemoteAddr.m_acIp));
			encoder.m_sServerUdp.m_port = sRecvMsgBuf.m_sRemoteAddr.m_port;
			processMsg(sRecvMsgBuf.m_eMsgType);
		}

		if (nExit == 1)
		{
			break;		
		}
	}

	pthread_join(sendTid);

	if (-1 == stopCamera())
	{
		printf("stopCamera error!\n");		
		goto ERR;
	}

ERR:

	unInitCamera();

	unInitEncoder(&encoder);

	netSocketDestroy(&encoder.m_nUDPSock);
	netSocketDestroy(&encoder.m_nTCPSock);

	return 0;	
}

void processMsg(MessageType_E msg)
{
	switch(msg)
	{
		case MSG_START:
		{
			printf("start video...\n");
			if (0 == nCameralStartFlag)
			{
				if (-1 == startCamera())
					printf("startCamera failed!\n");			
				nCameralStartFlag = 1;
			}

			if (-1 == nSendThreadExit)
				pthread_create(&sendTid, NULL, sendThread, NULL);
		}
		break;
		case MSG_STOP:
		{
			printf("stop video...\n");
			if (-1 == stopCamera())
			{
				printf("stopCamera error!\n");		
			}

			nCameralStartFlag = 0;
		}
		break;
		case MSG_EXIT:
		{
			printf("exit video...\n");
			nExit = 1;
		}
		break;
		case MSG_BEATHEART:
		{
		}
		break;
		default:
		{
			printf("unknown message type...\n");
		}
	}
}

void* sendThread(void* arg)
{
	U8* pcYuyvBuffer = NULL; 
	U8* pcI422Buffer = NULL; 
	U8* pcYuv420Buffer = NULL; 
	int testFrame = 1;
	int nUdpSock = -1;	

	nUdpSock = netSocketCreate(NET_SOCKET_UDP);
	if (-1 == nUdpSock)
		return NULL;

	encoder.m_nUDPSock = nUdpSock;

	pcYuyvBuffer = (U8*)malloc(((WIDTH*HEIGHT)<<1)*sizeof(char));
	if (NULL == pcYuyvBuffer)
	{
		printf("malloc YUYV buffer failed!\n");			
		goto THREAD_ERR;
	}

	pcI422Buffer = (U8*)malloc(((WIDTH*HEIGHT)<<1)*sizeof(char));
	if (NULL == pcI422Buffer)
	{
		printf("malloc I422 buffer failed!\n");			
		goto THREAD_ERR;
	}

	pcYuv420Buffer = (U8*)malloc((WIDTH*HEIGHT*3/2)*sizeof(char));
	if (NULL == pcYuv420Buffer)
	{
		printf("malloc I420 buffer failed!\n");			
		goto THREAD_ERR;
	}

	nSendThreadExit = 0;//running...

	while(1)
	{
	//	if (500 == testFrame)
	//		break;

		memset(pcYuyvBuffer, 0, WIDTH*HEIGHT*2);
		memset(pcYuv420Buffer, 0, WIDTH*HEIGHT*3/2);
		memset(pcI422Buffer, 0, WIDTH*HEIGHT*2);
		//printf("pcYuv420Buffer:%10s\n", pcYuv420Buffer);
		if (-1 == getOneFrame(pcYuyvBuffer))
		{
			printf("getOneFrame error!\n");			
			goto THREAD_ERR;
		}
		//先将422转为420
		yuyv2I422(pcYuyvBuffer, pcI422Buffer, WIDTH, HEIGHT);
		I422yuv420(pcI422Buffer, pcYuv420Buffer, WIDTH, HEIGHT);
		//printf("pcYuv420Buffer:%s\n", pcYuv420Buffer);
		//encode
		if (-1 == encodeAFrame(&encoder, pcYuv420Buffer))
		{
			printf("encodeAFrame failed!\n");
			goto THREAD_ERR;
		}

        encoder.m_nPTS = testFrame; 

		testFrame++;
		if (nExit == 1)
			goto THREAD_ERR;
	}

	flushEncoder(&encoder);//待定

THREAD_ERR:

	nSendThreadExit = -1;

	netSocketDestroy(&encoder.m_nUDPSock);

	if (NULL != pcYuyvBuffer)
	{
		free(pcYuyvBuffer);			
		pcYuyvBuffer = NULL;
	}

	if (NULL != pcYuv420Buffer)
	{
		free(pcYuv420Buffer);			
		pcYuv420Buffer = NULL;
	}
	
	if (NULL != pcI422Buffer)
	{
		free(pcI422Buffer);		
		pcI422Buffer = NULL;
	}

	pthread_exit(NULL);
}
