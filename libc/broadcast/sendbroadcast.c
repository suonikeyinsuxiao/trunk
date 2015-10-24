#include"sendbroadcast.h"


/*
 *1.受限广播：他不被路由转发，数据包仅出现在本地网络中,255.255.255.255
 *2.直接广播：网络广播会被路由，且数据包发送到指定网络段中,192.168.1.255
 */

int initBroadSock()
{
	int sockFd;

	sockFd = socket(AF_INET, SOCK_DGRAM, 0);
	if (-1 == sockFd)
	{
		fprintf(stderr, "create socket failed:%s\n", strerror(errno));	
		return -1;
	}

	int nOptVal = 1;
	if (-1 == setsockopt(sockFd, SOL_SOCKET, SO_BROADCAST, &nOptVal, sizeof(int)))	
	{
		fprintf(stderr, "setsockopt SO_BROADCAST failed:%s\n", strerror(errno));	
		return -1;
	}

	return sockFd;
}

int sendBroadData(int nSockFd, char* pcDstIP, char* pcData, int nDataLen)
{
	if (NULL == pcData)
	{
		printf("NULL == pcData\n");	
		return -1;
	}

	if (NULL == pcDstIP)
	{
		printf("NULL == pcBuffer\n");	
		return -1;
	}

	int nSendLen = 0;
	struct sockaddr_in dstAddr;
	memset(&dstAddr, 0, sizeof(struct sockaddr_in));

#if 0
	char buffer[nDataLen+1] = {0};
	char* pcBuffer = malloc(sizeof(char) * nDataLen);
	if (NULL == pcBuffer)
	{
		printf(stderr, "malloc buffer failed!\n");	
		return -1;
	}

	memcpy(buffer, pcData, nDataLen);
	buffer[nDataLen] = '\0';
#endif

	dstAddr.sin_family = AF_INET;
	dstAddr.sin_port = htons(BROADCASTPORT); 
	dstAddr.sin_addr.s_addr = inet_addr(pcDstIP);

	//nSendLen = sendto(nSockFd, pcBuffer, nDataLen, 0, (struct sockaddr*)&dstAddr, sizeof(struct sockaddr));
	nSendLen = sendto(nSockFd, pcData, nDataLen, 0, (struct sockaddr*)&dstAddr, sizeof(struct sockaddr));
	if (-1 == nSendLen)
	{
		fprintf(stderr,"send data failed:%s\n", strerror(errno));	
		return -1;
	}

#if 0
	if (NULL != pcBuffer)
	{
		free(pcBuffer);
	}
#endif

	return nSendLen;
}

void closeBroadSock(int nSockFd)
{
	if (-1 != nSockFd)
		close(nSockFd);
}
