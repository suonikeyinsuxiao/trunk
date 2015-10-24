#include"recvbroadcast.h"

int main(int argc, char** argv)
{
	int nSockFd = -1;
	char buffer[1024] = {0};
	int nRecvLen = -1;
	struct sockaddr_in srcAddr;

	nSockFd = initSock();

	while(1)
	{

		nRecvLen = recvData(nSockFd, buffer, 1024, (struct sockaddr*)&srcAddr);
		if (-1 == nRecvLen)
			return -1;

		printf("Recv data:%s\n", buffer);
		memset(buffer, 0, 1024);
	}

	closeSock(nSockFd);

	return 0;
}
