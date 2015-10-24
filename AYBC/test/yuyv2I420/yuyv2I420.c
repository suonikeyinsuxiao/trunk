#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

typedef unsigned char U8;
typedef unsigned int  U32;

#define WIDTH  800
#define HEIGHT 600

void I422yuv420(U8 *YUV422, U8 *YUV, U32 width, U32 height);
void yuyv2I422(U8 *yuyv, U8 *YUV422, U32 width, U32 height);

int main(int argc, char** argv)
{
	U8* pcYUYV422 = NULL;
	U8* pcI422 = NULL;
	U8* pcI420 = NULL;
	int nYUYVsize = 0;
	int nI420size = 0;
	int nI422size = 0;
	int nYUYVFd = -1;
	int nI420Fd = -1;
	char* pcYUYVFile="test.yuv";
	char* pcI420File="I420.yuv";
	int start = 0;
	int end = 0;
	int nReadLen = 0;
	int nRetLen = 0;
	
	nYUYVsize = (WIDTH * HEIGHT) << 1 ;//YUYV 4:2:2
	pcYUYV422 = (U8 *)malloc(nYUYVsize*sizeof(char));
	memset(pcYUYV422, 0, nYUYVsize);
	
	nI422size = (WIDTH * HEIGHT) << 1; //YUV 4:2:2
	pcI422 = (U8 *)malloc(nI422size*sizeof(char));
	memset(pcI422, 0, nI422size);
	
	nI420size = WIDTH*HEIGHT * 1.5;//YUV 4:2:0
	pcI420 = (U8 *)malloc(nI420size*sizeof(char));
	memset(pcI420, 0, nI420size);
	
	nYUYVFd = open(pcYUYVFile, O_RDONLY);
	if (-1 == nYUYVFd)
	{
		printf("cannot open input file.");
		return -1;
	}
	
	nI420Fd = open(pcI420File, O_WRONLY|O_TRUNC);
	if (-1 == nI420Fd)
	{
		fprintf(stderr,"open I420.yuv failed:%s\n", strerror(errno));
		return -1;
	}
	
	start = lseek(nYUYVFd,0,SEEK_SET);
	end = lseek(nYUYVFd,0,SEEK_END);
	
	lseek(nYUYVFd,0,SEEK_SET);
	
	while(1)
	{
		nReadLen = read(nYUYVFd, pcYUYV422, WIDTH*HEIGHT*2);
		printf("nReadLen=%d\n",nReadLen);
		
		yuyv2I422(pcYUYV422, pcI422, WIDTH, HEIGHT);
		I422yuv420(pcI422, pcI420, WIDTH, HEIGHT);
		
		write(nI420Fd,pcI420,WIDTH*HEIGHT*3/2);
		
		nRetLen += nReadLen;
		printf("filelen=%d, readtotallen=%d\n",(end-start), nRetLen);
		if (nRetLen == (end-start))
		{
			break;		
		}
		
	}
	
	return 0;	
}

void I422yuv420(U8 *YUV422, U8 *YUV, U32 width, U32 height)
{
	unsigned char *Y, *U, *V;
	unsigned char * Y2, *U2, *V2;
	
	//I422
  	Y = YUV422;
	U = Y + width*height;
	V = U + (width*height>>1);
		
	//420
	Y2 = YUV;
	U2 = Y2 + width*height;
	V2 = U2 +(width*height>>2);

	int i = 0;
	for(i = 0; i < (height/2); ++i)
	{
		memcpy(U2, U, width>>1);
		memcpy(V2, V, width>>1);

		U2 = U2 + (width>>1);
		V2 = V2 + (width>>1);

		U = U + (width);
		V = V + (width);
	}

	memcpy(Y2, Y, width*height);
}

void yuyv2I422(U8 *yuyv, U8 *YUV422, U32 width, U32 height)
{
	U8 *Y = NULL;
	U8 *U = NULL;
	U8 *V = NULL;
	U8 *p = yuyv;
	
	//422
	Y = YUV422;
	U = Y + width*height;
	V = U + (width*height>>1);
	
	int row, col;
	for(row = 0; row < height; ++row)
	{ 
		for(col = 0; col < (width>>1); ++col)
		{    
			Y[col*2]   = p[4*col];    
			U[col]     = p[4*col+1];
			Y[col*2+1] = p[4*col+2];
			V[col]     = p[4*col+3];
		}
		p = p + width*2;

		Y = Y + width;
		U = U + (width>>1);
		V = V + (width>>1);
	}
}



