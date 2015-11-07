#include "bmp.h"
#include <stdio.h>

//fd  means file descriptor
//buf means buffer to store image data
//ow  means original image width
//oh  means original image height
//Bpp means Bytes per pixel
//to seal struct
void rotateBmp(int nFd, RotateDirect_E eDirection, Bmp_S* psBmp)
{
	int row;
	int col;
	char* p =NULL;
	int nBpp = 0;

	if (NULL == psBmp->m_pcImageData)
	{
		printf("buf == NULL\n");	
		return;
	}

	if (3 == (psBmp->m_sBmpHeader.m_BPP >> 3))
		nBpp = 3;
	else if (4 == (psBmp->m_sBmpHeader.m_BPP >> 3))
		nBpp = 4;
		
	switch(eDirection)
	{
		case ORIGINAL:
		{
			for(row = 0; row < psBmp->m_sBmpHeader.m_nImageHight; row++)
			{
				for(col = 0; col < psBmp->m_sBmpHeader.m_nImageWidth; col++)
				{
					p =(psBmp->m_pcImageData+(row*psBmp->m_sBmpHeader.m_nImageWidth + col)*nBpp);
					write(nFd, p, nBpp);
				}
			}	
		}
		break;
		case CLOCKWISE90:
		{
			for(col = psBmp->m_sBmpHeader.m_nImageWidth-1; col >= 0; col--)
			{
				for(row = 0; row < psBmp->m_sBmpHeader.m_nImageHight; row++)
				{
					p =(psBmp->m_pcImageData+(row*psBmp->m_sBmpHeader.m_nImageWidth + col)*nBpp);
					write(nFd, p, nBpp);
				}
			}
		}
		break;
		case CLOCKWISE180:
		{
			for(row = psBmp->m_sBmpHeader.m_nImageHight-1; row >= 0  ; row--)
			{
				for(col = psBmp->m_sBmpHeader.m_nImageWidth-1 ; col >= 0 ; col--)
				{
					p =(psBmp->m_pcImageData+(row*psBmp->m_sBmpHeader.m_nImageWidth + col)*nBpp);
					write(nFd, p, nBpp);
				}
			}
		}
		break;
		case ANTICLOCKWISE90:
		{
			for(col = 0; col < psBmp->m_sBmpHeader.m_nImageWidth; col++)
			{
				for(row = psBmp->m_sBmpHeader.m_nImageHight-1; row >= 0; row--)
				{
					p =(psBmp->m_pcImageData+(row*psBmp->m_sBmpHeader.m_nImageWidth + col)*nBpp);
					write(nFd , p , nBpp);
				}
			}
		}
		break;
	}

}

#if 0
void rotateBmp(int nFd, char* pcBuf, int nOw, int nOh, RotateDirect_E eDirection, int nBpp)
{
	int row;
	int col;
	char* p =NULL;

	if (NULL == pcBuf)
	{
		printf("buf == NULL\n");	
		return;
	}
		
	switch(eDirection)
	{
		case ORIGINAL:
		{
			for(row = 0; row < nOh; row++)
			{
				for(col = 0; col < nOw; col++)
				{
					p =(pcBuf+(row*nOw + col)*nBpp);
					write(nFd, p, nBpp);
				}
			}	
		}
		break;
		case CLOCKWISE90:
		{
			for(col = nOw-1; col >= 0; col--)
			{
				for(row = 0; row < nOh; row++)
				{
					p =(pcBuf+(row*nOw + col)*nBpp);
					write(nFd, p, nBpp);
				}
			}
		}
		break;
		case CLOCKWISE180:
		{
			for(row = nOh-1; row >= 0  ; row--)
			{
				for(col = nOw-1 ; col >= 0 ; col--)
				{
					p =(pcBuf+(row*nOw + col)*nBpp);
					write(nFd, p, nBpp);
				}
			}
		}
		break;
		case ANTICLOCKWISE90:
		{
			for(col = 0; col < nOw; col++)
			{
				for(row = nOh-1; row >= 0; row--)
				{
					p =(pcBuf+(row*nOw + col)*nBpp);
					write(nFd , p , nBpp);
				}
			}
		}
		break;
	}
}
#endif

void getBmpHeadInfo(int nFd, BmpHeader_S* psBmpHead)
{
	if (-1 == nFd || NULL == psBmpHead)
	{
		printf("-1 == nFd || NULL == psBmpHead\n");	
		return;
	}

	read(nFd , psBmpHead , sizeof(BmpHeader_S));

	printf("Signatue[0]  : %c\n", psBmpHead->m_acSignatue[0]);
	printf("Signatue[1]  : %c\n", psBmpHead->m_acSignatue[1]);
	printf("FileSize     : %d\n", psBmpHead->m_nFileSize);
	printf("Reserv1      : %d\n", psBmpHead->m_Reserv1);
	printf("Reserv2      : %d\n", psBmpHead->m_Reserv2);
	printf("FileOffset   : %d\n", psBmpHead->m_nFileOffset);
	printf("DIBHeaderSize: %d\n", psBmpHead->m_nDIBHeaderSize);
	printf("ImageWidth   : %d\n", psBmpHead->m_nImageWidth);
	printf("ImageHight   : %d\n", psBmpHead->m_nImageHight);
	printf("Planes       : %d\n", psBmpHead->m_Planes);
	printf("BPP          : %d\n", psBmpHead->m_BPP);
	printf("Compression  : %d\n", psBmpHead->m_nCompression);
	printf("ImageSize    : %d\n", psBmpHead->m_nImageSize);
	printf("XPPM         : %d\n", psBmpHead->m_nXPPM);
	printf("YPPM         : %d\n", psBmpHead->m_nYPPM);
	printf("CCT          : %d\n", psBmpHead->m_nCCT);
	printf("ICC          : %d\n", psBmpHead->m_nICC);
}

void getBmpData(int nFd, char* pcDataBuf, int nDataLen)
{
	if (-1 == nFd || NULL == pcDataBuf)
	{
		printf("-1 == nFd || NULL == pcDataBuf\n");	
		return;
	}

	read(nFd, pcDataBuf, nDataLen);
}
