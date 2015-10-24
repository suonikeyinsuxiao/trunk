#include"videoCapture.h"

int main(void )
{
	int nRet = 0;
	//char yuyv[WIDTH*HEIGHT*2];
	char* pcYuvBuffer = NULL; 

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

	pcYuvBuffer = (char*)malloc((WIDTH*HEIGHT*2)*sizeof(char));
	if (NULL == pcYuvBuffer)
	{
		printf("malloc YUYV buffer failed!\n");			
		goto ERR;
	}

	int nDstFd = open("mytest.yuv", O_CREAT|O_EXCL|O_RDWR|O_TRUNC, 0777);
	if (nDstFd == -1)
	{
		fprintf(stderr, "open mytest.yuv failed:%s\n", strerror(errno));		
	}

	int nFrameNum = 0;

	while(1)
	{
		if (20 == nFrameNum)
			break;

		memset(pcYuvBuffer, 0, WIDTH*HEIGHT*2);

		//if (-1 == getOneFrame(yuyv))
		if (-1 == getOneFrame(pcYuvBuffer))
		{
			printf("getOneFrame error!\n");			
			goto ERR;
		}

		//if (-1 == write(nDstFd, yuyv, WIDTH*HEIGHT*2))
		if (-1 == write(nDstFd, pcYuvBuffer, WIDTH*HEIGHT*2))
		{
			fprintf(stderr, "write error:%s\n", strerror(errno));		
			goto ERR;
		}

		nFrameNum++;
	}

	
	if (-1 == stopCamera())
	{
		printf("stopCamera error!\n");			
	}


ERR:
	if (NULL != pcYuvBuffer)
	{
		free(pcYuvBuffer);			
		pcYuvBuffer = NULL;
	}
			
	unInitCamera();
	
	close(nDstFd);
	nDstFd = -1;
		
	return 0;
}
