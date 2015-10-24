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

	netSocketSendto(nSockFd, (char*)argv[2], strlen(argv[2]), &nRealSize, argv[1], 12271, 0);	

	netSocketDestroy(&nSockFd);	
}
