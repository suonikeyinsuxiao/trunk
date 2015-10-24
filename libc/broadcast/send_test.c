#include"sendbroadcast.h"



int main(int argc, char** argv)
{
	int nSockFd = -1;
	int nSendLen = -1;
	char* pcIP = "255.255.255.255";
	char* pcData = "hello world";
	
	nSockFd = initBroadSock();
	if (-1 == nSockFd)
	{	
		return -1;
	}
	
	nSendLen = sendBroadData(nSockFd, pcIP, pcData, strlen(pcData));
	if (-1 == nSendLen)
		goto ERR;

ERR:

	closeBroadSock(nSockFd);

	return 0;
}
