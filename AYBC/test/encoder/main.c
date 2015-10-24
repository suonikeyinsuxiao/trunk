#include "encoder.h"
#include <fcntl.h>

int main(int argc, char** argv)
{
	int nI420Fd = -1;
	int nH264Fd = -1;
	U8* pcEncodedData = NULL;
	U8* pcYuv420Buffer = NULL; 

	ENCODER_S encoder;
	memset(&encoder, 0, sizeof(ENCODER_S));
	encoder.m_nFd = -1;
	encoder.m_nPTS = 0;

	nI420Fd = open("I420.yuv", O_RDONLY);
	if (-1 == nI420Fd)
	{
		fprintf(stderr, "open I420.yuv failed:%s\n", strerror(errno));		
		return -1;
	}

	nH264Fd = open("test.h264", O_CREAT|O_EXCL|O_WRONLY|O_TRUNC, 0777);
	if (-1 == nH264Fd) 
	{
		fprintf(stderr, "open test.h264 failed:%s\n", strerror(errno));		
		goto ERR;
	}
	encoder.m_nFd = nH264Fd; 
	printf("encoder.m_nFd=%d\n", nH264Fd);

	pcYuv420Buffer = (U8*)malloc((WIDTH*HEIGHT*3/2)*sizeof(char));
	if (NULL == pcYuv420Buffer)
	{
		printf("malloc I420 buffer failed!\n");			
		goto ERR;
	}

	//pcEncodedData = malloc(WIDTH*HEIGHT);
	//if (NULL == pcEncodedData)
	//{
	//	printf("malloc encoder buffer failed!\n");			
	//	goto ERR;
	//}

	if (-1 == initEncoder(&encoder, X264_CSP_I420, WIDTH, HEIGHT))
	{
		printf("init encoder failed!\n");
		goto ERR;
	}

	int nReadLen = 0;
	int nRetLen = 0;
	int start = 0;
	int end = 0;
	int nEncoderLen = 0;
	int i = 0;

	end = lseek(nI420Fd, 0, SEEK_END);
	start = lseek(nI420Fd, 0, SEEK_SET);

	while(1)
	{
		memset(pcYuv420Buffer, 0, (WIDTH*HEIGHT*3/2));
		nReadLen = read(nI420Fd,pcYuv420Buffer,(WIDTH*HEIGHT*3/2));	
		nRetLen += nReadLen;

		if (-1 == encodeAFrame(&encoder, pcYuv420Buffer))
			goto ERR;

		encoder.m_nPTS++;

		if (nRetLen == (end-start))
			break;
	}

	flushEncoder(&encoder);
	
	
ERR:
	if (-1 != nI420Fd)
	{
		close(nI420Fd);
		nI420Fd = -1;
	}
	if (-1 != nH264Fd)
	{
		close(nH264Fd);
		nH264Fd = -1;
	}
	if (NULL == pcYuv420Buffer)
	{
		free(pcYuv420Buffer);	
		pcYuv420Buffer = NULL;
	}
	//if (NULL == pcEncodedData)
	//{
	//	free(pcEncodedData);		
	//	pcEncodedData = NULL;
	//}

	unInitEncoder(&encoder);

	return 0;
}
