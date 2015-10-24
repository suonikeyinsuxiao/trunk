#include "common.h"
#include "encoder.h"
#include "videoCapture.h"

#define H264FILE "test.h264"

int main(int argc, char** argv)
{
	int nRet = 0;
	U8* pcYuyvBuffer = NULL; 
	U8* pcI422Buffer = NULL; 
	U8* pcYuv420Buffer = NULL; 
//	U8* pcEncodedData = NULL;
	ENCODER_S encoder;
	
	if(-1 == initCamera())	
	{
		printf("initCamera failed!\n");			
		goto ERR;
	}
	
	if (-1 == startCamera())
	{
		printf("startCamera failed!\n");			
		goto ERR;
	}

	pcYuyvBuffer = (U8*)malloc(((WIDTH*HEIGHT)<<1)*sizeof(char));
	if (NULL == pcYuyvBuffer)
	{
		printf("malloc YUYV buffer failed!\n");			
		goto ERR;
	}

	pcI422Buffer = (U8*)malloc(((WIDTH*HEIGHT)<<1)*sizeof(char));
	if (NULL == pcI422Buffer)
	{
		printf("malloc I422 buffer failed!\n");			
		goto ERR;
	}

	pcYuv420Buffer = (U8*)malloc((WIDTH*HEIGHT*3/2)*sizeof(char));
	if (NULL == pcYuv420Buffer)
	{
		printf("malloc I420 buffer failed!\n");			
		goto ERR;
	}


	if (-1 == initEncoder(&encoder, X264_CSP_I420, WIDTH, HEIGHT))
	{
		printf("init encoder failed!\n");
		goto ERR;
	}

	int nEncodeLen = -1;
	int nH264Fd = -1;

	nH264Fd = open(H264FILE, O_CREAT | O_EXCL | O_RDWR | O_TRUNC, 0777);
	if (-1 == nH264Fd)
	{
		fprintf(stderr, "open %s failed:%s", H264FILE, strerror(errno));		
		goto ERR;
	}
	encoder.m_nFd = nH264Fd;

	int testFrame = 0;

	while(1)
	{
		if (500 == testFrame)
			break;

		memset(pcYuyvBuffer, 0, WIDTH*HEIGHT*2);
		memset(pcYuv420Buffer, 0, WIDTH*HEIGHT*3/2);
		memset(pcI422Buffer, 0, WIDTH*HEIGHT*2);
		//printf("pcYuv420Buffer:%10s\n", pcYuv420Buffer);
		if (-1 == getOneFrame(pcYuyvBuffer))
		{
			printf("getOneFrame error!\n");			
			goto ERR;
		}
		//先将422转为420
		yuyv2I422(pcYuyvBuffer, pcI422Buffer, WIDTH, HEIGHT);
		I422yuv420(pcI422Buffer, pcYuv420Buffer, WIDTH, HEIGHT);
		//printf("pcYuv420Buffer:%s\n", pcYuv420Buffer);
		//encode
		if (-1 == encodeAFrame(&encoder, pcYuv420Buffer))
		{
			printf("encodeAFrame failed!\n");
			goto ERR;
		}

        encoder.m_nPTS = testFrame; 

		testFrame++;
	}

	flushEncoder(&encoder);

	if (-1 == stopCamera())
	{
		printf("stopCamera error!\n");		
		goto ERR;
	}

	close(encoder.m_nFd);
	encoder.m_nFd = -1;
	
ERR:

	unInitCamera();

	if (NULL != pcYuyvBuffer)
	{
		free(pcYuyvBuffer);			
		pcYuyvBuffer = NULL;
	}

	if (NULL != pcYuv420Buffer)
	{
		free(pcYuv420Buffer);			
		pcYuv420Buffer = NULL;
	}
	
	if (NULL != pcI422Buffer)
	{
		free(pcI422Buffer);		
		pcI422Buffer = NULL;
	}

	unInitEncoder(&encoder);

	return 0;	
}
