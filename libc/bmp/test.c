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

	write(dest_fd , &sBmp.m_sBmpHeader, sizeof(BmpHeader_S));

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

	//rotatebmp(dest_fd, buffer, 800, 480, CLOCKWISE90, 3);
	//rotatebmp(dest_fd, buffer, 800, 480, ANTICLOCKWISE90, 3);
	//rotateBmp(dest_fd, sBmp.m_pcImageData, 800, 480, CLOCKWISE180, 3);
	rotateBmp(dest_fd, CLOCKWISE180, &sBmp);
	//rotatebmp(dest_fd, buffer, 800, 480, ORIGINAL, 3);

	if (NULL != sBmp.m_pcImageData)
	{
		free(sBmp.m_pcImageData);	
		sBmp.m_pcImageData= NULL;
	}
	close(dest_fd);

	return 0 ; 
}
