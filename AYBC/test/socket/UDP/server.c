#include "netsocket.h"


int main(int argc , char** argv)
{
	int nSockFd = -1;	

	nSockFd = netSocketCreate(NET_SOCKET_UDP);
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
	
	netSocketRecvfrom(nSockFd, acBuf, 1024, acIp, &nPort, &nRealSize,0);

	printf("recv data:%s\n",acBuf);

	netSocketDestroy(&nSockFd);	
}
