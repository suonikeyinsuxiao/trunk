#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bmp.h"

int main(int argc , char **argv)
{
	if(argc != 3)
		return -1 ; 

	int fd ; 
	int dest_fd ; 
	Bmp_S sBmp;
	char* pcBuffer = NULL;
	//BmpHeader_S header; 

	fd = open(argv[1] , O_RDONLY);
	if(-1 == fd)
	{
		perror("open bmp file fail");
		return -2 ; 
	}

	dest_fd = open( argv[2] , O_RDWR | O_CREAT|O_TRUNC , 0777);
	if(dest_fd < 0 )
	{
		perror("open rgb565 fail");
		return -3 ; 
	}

	getBmpHeadInfo(fd, &sBmp.m_sBmpHeader);

	sBmp.m_pcImageData = malloc(sBmp.m_sBmpHeader.m_nImageSize);
	memset(sBmp.m_pcImageData, 0, sBmp.m_sBmpHeader.m_nImageSize);

	getBmpData(fd, sBmp.m_pcImageData, sBmp.m_sBmpHeader.m_nImageSize);

	close(fd);


#if 0
	//mark original point
	int row , col ; 
	char *p = NULL ; 

	for(row = 0 ; row < 10 ; row++)
	{
		for(col = 0 ; col < 10 ; col++)
		{
			p = sBmp.m_pcImageData + (row * 800 + col)*3 ; 
			*p = 255;
			*(p+1)=255;
			*(p+2)=255;	
		}
	}
#endif

	//1 test
	//rotateBmp(dest_fd, CLOCKWISE180, &sBmp);

	//2 test
	//bmp2Gray(dest_fd, &sBmp, 20);
	
	//3 test
	convertRgb(dest_fd, &sBmp, 24);

	if (NULL != sBmp.m_pcImageData)
	{
		free(sBmp.m_pcImageData);	
		sBmp.m_pcImageData= NULL;
	}
	close(dest_fd);

	return 0 ; 
}
