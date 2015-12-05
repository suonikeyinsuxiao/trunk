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

	netSocketBind(nSockFd, 0);

	netSocketConnect(nSockFd, argv[1], 12271);

	netSocketSend(nSockFd, (char*)argv[2], strlen(argv[2]), &nRealSize, 0);	

	netSocketDestroy(&nSockFd);	
}
