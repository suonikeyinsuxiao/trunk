
#include "netsocket.h"

int netSocketCreate(NetSocketType_E eSocketType)
{
	int nSocketFd = -1;	

	if (NET_SOCKET_UDP == eSocketType)
	{
		nSocketFd = socket(PF_INET, SOCK_DGRAM, 0);	
		if (-1 == nSocketFd)
		{
			fprintf(stderr, "[%s,%d]create UDP socket failed:%s\n", __func__, __LINE__, strerror(errno));		
		}
	}
	else if (NET_SOCKET_TCP == eSocketType)
	{
		nSocketFd = socket(PF_INET, SOCK_STREAM, 0);	
		if (-1 == nSocketFd)
		{
			fprintf(stderr, "[%s,%d]create UDP socket failed:%s\n", __func__, __LINE__, strerror(errno));		
		}
		
	}
	else
	{
		printf("not support socket type!\n");		
	}

	return nSocketFd;
}

void netSocketDestroy(int* pnSocketFd)
{
	if ((*pnSocketFd) > 0)	
	{
		close(*pnSocketFd);		
		*pnSocketFd = -1;
	}
}

int netSocketBind(int nSocketFd, int nPort)
{
	struct sockaddr_in addr;	
	int nRet = -1;

	memset(&addr, 0, sizeof(struct sockaddr_in));

	addr.sin_family = AF_INET;
	addr.sin_port = htons(nPort);
	addr.sin_addr.s_addr = htonl(INADDR_ANY);

	nRet = bind(nSocketFd, (struct sockaddr *)&addr, sizeof(struct sockaddr));
	if (-1 == nRet)
	{
		fprintf(stderr, "[%s,%d] bind socket failed:%s\n", __func__, __LINE__, strerror(errno));		
	}

	return nRet;
}

int netSocketListen(int nSocketFd, int nMaxConnectNum)
{
	int nRet = -1;	

	nRet = listen(nSocketFd, nMaxConnectNum);
	if (-1 == nRet)
	{
		fprintf(stderr, "[%s,%d] socket:%d listen failed:%s", __func__, __LINE__, nSocketFd, strerror(errno));		
	}

	return nRet;
}

int netSocketConnect(int nSocketFd, char* pcIp, int nPort)
{
	int nRet = -1;	
	struct sockaddr_in addr;

	memset(&addr, 0, sizeof(struct sockaddr_in));
	addr.sin_family = AF_INET;
	if (NULL == pcIp)
		addr.sin_addr.s_addr = htonl(INADDR_ANY);
	else
		addr.sin_addr.s_addr = inet_addr(pcIp);
	addr.sin_port = htons(nPort);

	nRet = connect(nSocketFd, (struct sockaddr*)&addr, sizeof(struct sockaddr));
	if (-1 == nRet)
	{
		fprintf(stderr, "[%s,%d] socket connect failed:%s\n", __func__, __LINE__, strerror(errno));		
	}

	return nRet;
}

int netSocketAccept(int nSocketFd, struct sockaddr* psPeer, U32 nTimeout_ms)
{
	int nAcceptedFd = -1;	
	int nRet = -1;
	socklen_t nLen = 0;
	fd_set sReadFs;
	struct timeval timeout;
	struct timeval* pTimeout = NULL;
	
	FD_ZERO(&sReadFs);
	FD_SET(nSocketFd, &sReadFs);

	if (0 == nTimeout_ms)
		pTimeout = NULL;
	else
	{
		timeout.tv_sec = nTimeout_ms/1000;
		timeout.tv_usec = (nTimeout_ms%1000)*1000;
		pTimeout = &timeout;
	}
	nRet = select(nSocketFd+1, &sReadFs, NULL, NULL, pTimeout);
	if (nRet < 0)
	{
		fprintf(stderr, "socket select failed:%s\n", strerror(errno));
		return -1;
	}
	else if (0 == nRet)
	{
		printf("socket select timeout\n");	
		return -1;
	}

	if (FD_ISSET(nSocketFd, &sReadFs))	
	{
		nAcceptedFd = accept(nSocketFd, psPeer, &nLen);		
		if (-1 == nAcceptedFd)
		{
			fprintf(stderr, "socket accept failed:%s\n", strerror(errno));		
		}
	}

	return nAcceptedFd;
}

int netSetsockopt(int nSocketFd, int nLevel, int nOptName, const void* pOptVal, socklen_t nOptLen)
{
	int nRet = -1;	

	nRet = setsockopt(nSocketFd, nLevel, nOptName, pOptVal, nOptLen);
	if (-1 == nRet)
	{
		fprintf(stderr, "[%s,%d]setsockopt failed:%s\n", __func__, __LINE__, strerror(errno));
	}

	return nRet;
}

int netGetsockopt(int nSocketFd, int nLevel, int nOptName, void* pOptVal, socklen_t* pnOptLen)
{
	int nRet = -1;	

	nRet = getsockopt(nSocketFd, nLevel, nOptName, pOptVal, pnOptLen);
	if (-1 == nRet)
	{
		fprintf(stderr, "[%s,%d]getsockopt failed:%s\n", __func__, __LINE__, strerror(errno));
	}

	return nRet;
}

int netSocketRecv(int nSocketFd, char* pcBuf, int nBufSize, int* pnRecvLen, U32 nTimeout)
{
	if (NULL == pcBuf)	
	{
		printf("netSocketRecv NULL == pcBuf\n");		
		return -1;
	}

	int nRet = -1;
	fd_set sReadFs;
	struct timeval timeout;
	struct timeval* pTimeout = NULL;

	FD_ZERO(&sReadFs);
	FD_SET(nSocketFd, &sReadFs);

	if (0 == nTimeout)
		pTimeout = NULL;
	else
	{
		timeout.tv_sec = nTimeout/1000;
		timeout.tv_usec = (nTimeout%1000)*1000;
		pTimeout = &timeout;
	}

	nRet = select(nSocketFd+1, &sReadFs, NULL, NULL, pTimeout);
	if (nRet < 0)
	{
		fprintf(stderr, "netsocketrecv socket select failed:%s\n", strerror(errno));
		return -1;
	}
	else if (0 == nRet)
	{
		printf("netsocketrecv socket select timeout\n");	
		return -1;
	}

	if (FD_ISSET(nSocketFd, &sReadFs))	
	{
		nRet = recv(nSocketFd, pcBuf, nBufSize, 0);
		if (nRet < 0)
		{
			fprintf(stderr, "netSocketRecv socket accept failed:%s\n", strerror(errno));		
			return -1;
		}
		else if (0 == nRet)
		{
			printf("the peer has performed a orderly shutdown:%s\n", strerror(errno));	
			return -1;
		}
		
		*pnRecvLen = nRet;
	}

	return 0;
}

int netSocketRecvfrom(int nSocketFd, char* pcBuf, int nBufSize, char* pcIp, int* pnPort, int* pnRecvLen, U32 nTimeout_ms)
{
	if (NULL == pcBuf || pcIp == NULL || pnPort == NULL)
	{
		printf("netSocketRecvfrom pcIp == NULL || pnPort == NULL \n");
		return -1;
	}

	int nRet = -1;		
	socklen_t nSockLen = sizeof(struct sockaddr);
	fd_set sReadFs;
	struct timeval timeout;
	struct timeval* pTimeout = NULL;
	struct sockaddr_in peerAddr;

	memset(&peerAddr, 0, sizeof(struct sockaddr_in));

	FD_ZERO(&sReadFs);
	FD_SET(nSocketFd, &sReadFs);

	if (0 == nTimeout_ms)
		pTimeout = NULL;
	else
	{
		timeout.tv_sec = nTimeout_ms/1000;
		timeout.tv_usec = (nTimeout_ms%1000)*1000;
		pTimeout = &timeout;
	}

	nRet = select(nSocketFd+1, &sReadFs, NULL, NULL, pTimeout);
	if (nRet < 0)
	{
		fprintf(stderr, "netsocketrecvfrom socket select failed:%s\n", strerror(errno));
		return -1;
	}
	else if (0 == nRet)
	{
		printf("netsocketrecvfrom socket select timeout\n");	
		return -1;
	}

	if (FD_ISSET(nSocketFd, &sReadFs))	
	{
		fcntl(nSocketFd, F_SETFL, O_NONBLOCK);
		*pnRecvLen = recvfrom(nSocketFd, pcBuf, nBufSize, 0, (struct sockaddr*)&peerAddr, &nSockLen);
		if ((*pnRecvLen) < 0)
		{
			fprintf(stderr, "netSocketRecvfrom socket accept failed:%s\n", strerror(errno));		
			return -1;
		}
		else if (0 == (*pnRecvLen))
		{
			printf("netSocketRecvfrom the peer has performed a orderly shutdown:%s\n", strerror(errno));	
			return -1;
		}

//		printf("peer ip:%s\n",inet_ntoa(peerAddr.sin_addr));
//		printf("peer port:%d\n",ntohs(peerAddr.sin_port));
		strcpy(pcIp, inet_ntoa(peerAddr.sin_addr));
		*pnPort = ntohs(peerAddr.sin_port);
	}

	return 0;
}

int netSocketSend(int nSocketFd, char *pcBuf, int nBufSize, int *pnSendLen, U32 nTimeout_ms)
{
	if (NULL == pcBuf)	
	{
		printf("netSocketSend NULL == pcBuf\n");		
		return -1;
	}

	int nRet = -1;
	fd_set sWriteFs;
	struct timeval timeout;
	struct timeval*  pTimeout=NULL;

	FD_ZERO(&sWriteFs);
	FD_SET(nSocketFd, &sWriteFs);

	if (0 == nTimeout_ms)
		pTimeout = NULL;
	else
	{
		timeout.tv_sec = nTimeout_ms/1000;
		timeout.tv_usec = (nTimeout_ms%1000)*1000;
		pTimeout = &timeout;
	}
	nRet = select(nSocketFd+1, NULL, &sWriteFs, NULL, pTimeout);
	if (nRet < 0)
	{
		fprintf(stderr, "netsocketsend socket select failed:%s\n", strerror(errno));
		return -1;
	}
	else if (0 == nRet)
	{
		printf("netsocketsend socket select timeout\n");	
		return -1;
	}

	if (FD_ISSET(nSocketFd, &sWriteFs))	
	{
		*pnSendLen = send(nSocketFd, pcBuf, nBufSize, 0);
		if ((*pnSendLen) < 0)
		{
			fprintf(stderr, "netSocketRecv socket accept failed:%s\n", strerror(errno));		
			return -1;
		}
	}

	return 0;
}

int netSocketSendto(int nSocketFd, char *pcBuf, int nBufSize,int* pnSendLen, char* pcIp, int nPort, U32 nTimeout_ms)
{
	if (NULL == pcBuf)	
	{
		printf("netSocketSendto NULL == pcBuf\n");		
		return -1;
	}

	int nRet = -1;
	fd_set sWriteFs;
	struct timeval timeout;
	struct timeval* pTimeout=NULL;
	struct sockaddr_in peerAddr;

	timeout.tv_sec = nTimeout_ms/1000;
	timeout.tv_usec = (nTimeout_ms%1000)*1000;

	memset(&peerAddr, 0, sizeof(struct sockaddr_in));
	peerAddr.sin_family = AF_INET;
	if (NULL == pcIp)
		peerAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	else
		peerAddr.sin_addr.s_addr = inet_addr(pcIp);
	peerAddr.sin_port = htons(nPort);

	FD_ZERO(&sWriteFs);
	FD_SET(nSocketFd, &sWriteFs);

	if (0 == nTimeout_ms)
		pTimeout = NULL;
	else
	{
		timeout.tv_sec = nTimeout_ms/1000;
		timeout.tv_usec = (nTimeout_ms%1000)*1000;
		pTimeout = &timeout;
	}
	nRet = select(nSocketFd+1, NULL, &sWriteFs, NULL, pTimeout);
	if (nRet < 0)
	{
		fprintf(stderr, "netsocketsendto socket select failed:%s\n", strerror(errno));
		return -1;
	}
	else if (0 == nRet)
	{
		printf("netsocketsendto socket select timeout\n");	
		return -1;
	}

	if (FD_ISSET(nSocketFd, &sWriteFs))	
	{
		*pnSendLen = sendto(nSocketFd, pcBuf, nBufSize, 0, (struct sockaddr*)&peerAddr, sizeof(struct sockaddr));
		if ((*pnSendLen) < 0)
		{
			fprintf(stderr, "netsocketsento socket sendto:%s\n", strerror(errno));		
			return -1;
		}
	}

	return 0;
}
