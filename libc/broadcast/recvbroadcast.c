#include"recvbroadcast.h"

int initSock()
{
	int nSockFd = -1;

	nSockFd = socket(AF_INET, SOCK_DGRAM, 0);
	if (-1 == nSockFd)
	{
		fprintf(stderr, "create broad recv socket failed:%s\n", strerror(errno));	
		return -1;
	}

	int nOptVal = 1;
	if (-1 == setsockopt(nSockFd, SOL_SOCKET, SO_BROADCAST, &nOptVal, sizeof(int)))
	{
		fprintf(stderr, "broad recv setsockopt failed:%s\n", strerror(errno));	
		return -1;
	}

	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(BRAODCASTPORT);
	addr.sin_addr.s_addr = INADDR_ANY; 

	if (-1 == bind(nSockFd, (struct sockaddr*)&addr, sizeof(struct sockaddr)))
	{
		fprintf(stderr, "broad recv bind socket failed%s!\n", strerror(errno));	
		return -1;
	}

	return nSockFd;
}

int recvData(int nSockFd, char* pcDataBuf,int nDataBufLen, struct sockaddr* psSrcAddr)
{
	int nRecvLen = -1;
	int sockLen = 0;

	nRecvLen = recvfrom(nSockFd, pcDataBuf, nDataBufLen, 0, psSrcAddr, &sockLen);	
	if (-1 == nRecvLen)
	{
		fprintf(stderr, "broad recv failed%s!\n", strerror(errno));	
		return -1;
	}
	pcDataBuf[nRecvLen] = '\0';

	return nRecvLen;
}

void closeSock(int nSockFd)
{
	if (-1 != nSockFd)
		close(nSockFd);
}
