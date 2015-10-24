#include "netsocket.h"


int main(int argc , char** argv)
{
	int nSockFd = -1;	

	nSockFd = netSocketCreate(NET_SOCKET_TCP);
	if (-1 == nSockFd)
	{
		fprintf(stderr, "create socket failed:%s\n", strerror(errno));
		return -1;
	}

	int nRealSize;
	char acBuf[1024] = {0};
	char acIp[24] = {0};
	int nPort;

	netSocketBind(nSockFd, 12271);

	netSocketListen(nSockFd, 10);

	struct sockaddr_in peerAddr;
	memset(&peerAddr, 0, sizeof(struct sockaddr_in));
	int newSock;

	newSock = netSocketAccept(nSockFd, (struct sockaddr*)&peerAddr, 0);
	
	netSocketRecv(newSock, acBuf, 1024, &nRealSize, 0);
	printf("recv data:%s\n",acBuf);

	netSocketDestroy(&nSockFd);	
}
