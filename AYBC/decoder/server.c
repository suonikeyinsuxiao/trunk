#include <stdio.h>
#include <string.h>
#include <fcntl.h>


#include "netsocket.h"
#include "decoder.h"

typedef int(*readBufCB_F)(void *opaque, uint8_t *buf, int buf_size);

void showDuration(int nDuration, char* pcDuration);

static int nSockFd = -1;
static int nRecvLen;
char acip[24] = {0};
int port = 0;

//Callback
int read_buffer(void *opaque, uint8_t *buf, int buf_size)
{
	printf("recv...nSockFd=%d................\n",nSockFd);
	if (-1 == netSocketRecvfrom(nSockFd, buf, buf_size,acip, &port, &nRecvLen, 10000))	
	{
		printf("read_buffer return\n");			
		return -1;
	}
	
	printf("ip=%s,port=%d,nRecvLen=%d\n",acip, port, nRecvLen);
	return nRecvLen;
}

int main(int argc, char** argv)
{
	int nRet = -1;
	decoderHandle_S decoder;

	memset(&decoder, 0 , sizeof(decoderHandle_S));

	char *pcFrameBuf = (char*)malloc((WIDTH*HEIGHT*3/2)*sizeof(char));

	int fd = -1;
	fd = open("xxx_mem.yuv",O_CREAT|O_EXCL|O_WRONLY|O_TRUNC,0777);
	if (-1 == fd)
	{
		fprintf(stderr, "open xxx_mem.yuv failed:%s\n",strerror(errno));
		return -1;
	}

	decoder.m_nFd = fd;

	nSockFd = netSocketCreate(NET_SOCKET_UDP);
	if (-1 == nSockFd)
	{
		fprintf(stderr, "create socket failed:%s\n", strerror(errno));
		return -1;
	}

	netSocketBind(nSockFd,12271);
	
	printf("register callback %p\n", read_buffer);
	if (-1 == registerCallBack(read_buffer))
	{
		return -1;
	}

	if (-1 == initDecoder(&decoder))
	{
		return -1;
	}


	int test = 0;
	
	//读取一帧压缩的视频数据
	while(0 == decodeFrame(&decoder, pcFrameBuf, WIDTH*HEIGHT))
	{

		if (decoder.m_nDataFlag)
		{
			printf("wrire len=%d\n\n",write(fd, pcFrameBuf, (WIDTH*HEIGHT*3/2)));
			test++;
			//printf("write len....\n");
		}
		else
		{
			printf("no data write...\n");
		}
		if(test==1)
			break;
		decoder.m_nDataFlag = 0;
	}

	unInitDecoder(&decoder);
	free(pcFrameBuf);
	pcFrameBuf = NULL;
		
	return 0;
}

void showDuration(int nDuration, char* pcDuration)
{
	//duration是以微秒为单位  
	//转换成hh:mm:ss形式  
	if (nDuration > 0 && pcDuration != NULL)
	{
		int ns, thh, tmm, tss;  
		ns 	= nDuration/1000000;  
		thh = ns / 3600;  
		tmm = (ns % 3600) / 60;  
		tss = (ns % 60);  
		sprintf(pcDuration,"%02d:%02d:%02d\n",thh,tmm,tss); 
	}
}
