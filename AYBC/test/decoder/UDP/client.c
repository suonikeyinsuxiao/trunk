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
	FILE *fp_open=NULL;
	fp_open=fopen(argv[1],"rb+");

	char* buf = malloc(1024);
	int nSenLen;


	while(1)
	{
		memset(buf,0, 1024);
		if (feof(fp_open))
			break;


		nRealSize = fread(buf, 1, 1024, fp_open);				
		printf("nRealSize=%d\n",nRealSize);

		netSocketSendto(nSockFd, buf, nRealSize, &nSenLen, "127.0.0.1", 12271,0);
	}

	free(buf);
	
}
