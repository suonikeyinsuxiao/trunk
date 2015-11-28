#include "bmp.h"
#include <stdio.h>
#include <string.h>

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

	write(nFd , &psBmp->m_sBmpHeader, sizeof(BmpHeader_S));

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
	printf("FileHeaderOffset   : %d\n", psBmpHead->m_nFileHeaderOffset);
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

/*{
	Gray = R*0.299 + G*0.587 + B*0.114
	Gray = (R*1 + G*2 + B*1) >> 2
	Gray = (R*2 + G*5 + B*1) >> 3
	Gray = (R*4 + G*10 + B*2) >> 4
	Gray = (R*9 + G*19 + B*4) >> 5
	Gray = (R*19 + G*37 + B*8) >> 6
	Gray = (R*38 + G*75 + B*15) >> 7
	Gray = (R*76 + G*150 + B*30) >> 8
	Gray = (R*153 + G*300 + B*59) >> 9
	Gray = (R*306 + G*601 + B*117) >> 10
	Gray = (R*612 + G*1202 + B*234) >> 11
	Gray = (R*1224 + G*2405 + B*467) >> 12
	Gray = (R*2449 + G*4809 + B*934) >> 13
	Gray = (R*4898 + G*9618 + B*1868) >> 14
	Gray = (R*9797 + G*19235 + B*3736) >> 15
	Gray = (R*19595 + G*38469 + B*7472) >> 16
	Gray = (R*39190 + G*76939 + B*14943) >> 17
	Gray = (R*78381 + G*153878 + B*29885) >> 18
	Gray = (R*156762 + G*307757 + B*59769) >> 19
	Gray = (R*313524 + G*615514 + B*119538) >> 20
}*/
int getGray(BmpRgb_S* psRgb, int nPrecision, U8* pcGray)
{
	int nRet = 0;
	if (NULL == psRgb)
	{
		printf("NULL == psRgb!!!\n");	
		return -1; 
	}

	if (nPrecision < 0 && nPrecision > 20)
	{
		printf("nPrecision error!\n");	
		return -1;
	}

	switch(nPrecision)
	{
		case 2:
			*pcGray = ((psRgb->m_cRed*1 + psRgb->m_cGreen*2 + psRgb->m_cBlue*1) >> nPrecision);
			break;
		case 3:
			*pcGray = ((psRgb->m_cRed*2 + psRgb->m_cGreen*5 + psRgb->m_cBlue*1) >> nPrecision);
			break;
		case 4:
			*pcGray = ((psRgb->m_cRed*4 + psRgb->m_cGreen*10 + psRgb->m_cBlue*2) >> nPrecision);
			break;
		case 5:
			*pcGray = ((psRgb->m_cRed*9 + psRgb->m_cGreen*19 + psRgb->m_cBlue*4) >> nPrecision);
			break;
		case 6:
			*pcGray = ((psRgb->m_cRed*19 + psRgb->m_cGreen*37 + psRgb->m_cBlue*8) >> nPrecision);
			break;
		case 7:
			*pcGray = ((psRgb->m_cRed*38 + psRgb->m_cGreen*75 + psRgb->m_cBlue*15) >> nPrecision);
			break;
		case 8:
			*pcGray = ((psRgb->m_cRed*76 + psRgb->m_cGreen*150 + psRgb->m_cBlue*30) >> nPrecision);
			break;
		case 9:
			*pcGray = ((psRgb->m_cRed*153 + psRgb->m_cGreen*300 + psRgb->m_cBlue*59) >> nPrecision);
			break;
		case 10:
			*pcGray = ((psRgb->m_cRed*306 + psRgb->m_cGreen*601 + psRgb->m_cBlue*117) >> nPrecision);
			break;
		case 11:
			*pcGray = ((psRgb->m_cRed*612 + psRgb->m_cGreen*1202 + psRgb->m_cBlue*234) >> nPrecision);
			break;
		case 12:
			*pcGray = ((psRgb->m_cRed*1224 + psRgb->m_cGreen*2405 + psRgb->m_cBlue*467) >> nPrecision);
			break;
		case 13:
			*pcGray = ((psRgb->m_cRed*2449 + psRgb->m_cGreen*4809 + psRgb->m_cBlue*934) >> nPrecision);
			break;
		case 14:
			*pcGray = ((psRgb->m_cRed*4898 + psRgb->m_cGreen*9618 + psRgb->m_cBlue*1868) >> nPrecision);
			break;
		case 15:
			*pcGray = ((psRgb->m_cRed*9797 + psRgb->m_cGreen*19235 + psRgb->m_cBlue*3736) >> nPrecision);
			break;
		case 16:
			*pcGray = ((psRgb->m_cRed*19595 + psRgb->m_cGreen*38469 + psRgb->m_cBlue*7472) >> nPrecision);
			break;
		case 17:
			*pcGray = ((psRgb->m_cRed*39190 + psRgb->m_cGreen*76939 + psRgb->m_cBlue*14943) >> nPrecision);
			break;
		case 18:
			*pcGray = ((psRgb->m_cRed*78381 + psRgb->m_cGreen*153878 + psRgb->m_cBlue*29885) >> nPrecision);
			break;
		case 19:
			*pcGray = ((psRgb->m_cRed*156762 + psRgb->m_cGreen*307757 + psRgb->m_cBlue*59769) >> nPrecision);
			break;
		case 20:
			*pcGray = ((psRgb->m_cRed*313524 + psRgb->m_cGreen*615514 + psRgb->m_cBlue*119538) >> nPrecision);
			break;
	}

	return nRet;
}

void bmp2Gray(int nFd, Bmp_S* psBmp, int nPrecision)
{
	int row;
	int col;
	char* p = NULL;
	BmpRgb_S sRgb;
	char cGray = 0;
	int nBpp = 0;

	if (NULL == psBmp->m_pcImageData)
	{
		printf("buf == NULL\n");	
		return;
	}

	if (nPrecision < 2 || nPrecision > 20)
	{
		printf("nPrecision error!\n");	
		return;
	}

	write(nFd , &psBmp->m_sBmpHeader, sizeof(BmpHeader_S));

	if (3 == (psBmp->m_sBmpHeader.m_BPP >> 3))
		nBpp = 3;
	else if (4 == (psBmp->m_sBmpHeader.m_BPP >> 3))
		nBpp = 4;
	else if (2 == (psBmp->m_sBmpHeader.m_BPP >> 3))
		nBpp = 2;

	for (row = 0; row < psBmp->m_sBmpHeader.m_nImageHight; ++row)
	{
		for (col = 0; col < psBmp->m_sBmpHeader.m_nImageWidth; ++col)
		{
			p = psBmp->m_pcImageData + ((row*psBmp->m_sBmpHeader.m_nImageWidth+col) * nBpp);
			sRgb.m_cRed = *p;
			sRgb.m_cGreen = *(p+1);
			sRgb.m_cBlue = *(p+2);
			if (-1 == getGray(&sRgb, nPrecision, &cGray))
			{
				printf("getGray error!\n");	
				return;
			}
			*p = cGray;
			*(p+1) = cGray;
			*(p+2) = cGray;
			write(nFd, p, nBpp);
		}
	}
}

//888to565
void convertRgb(int nFd, Bmp_S* psBmp, U16 uBpp)
{
	int row , col ; 
	U16 data ; 
	char *p = NULL ; 
	BmpHeader_S header;

	if (uBpp == psBmp->m_sBmpHeader.m_BPP)
	{
		printf("same rgb format!!!\n");	
		return;
	}

	memcpy(&header, &psBmp->m_sBmpHeader, sizeof(BmpHeader_S));
	header.m_BPP = uBpp ; 
	header.m_nFileSize = (psBmp->m_sBmpHeader.m_nImageWidth) * (psBmp->m_sBmpHeader.m_nImageHight) * (uBpp>>3) + 54 ; 
	header.m_nImageSize = (psBmp->m_sBmpHeader.m_nImageWidth) * (psBmp->m_sBmpHeader.m_nImageHight) * (uBpp>>3);

	//write(nFd , &header , sizeof(BmpHeader_S));

	for(row = 0 ; row < psBmp->m_sBmpHeader.m_nImageHight; row++)
	{
		for(col = 0 ; col < psBmp->m_sBmpHeader.m_nImageWidth; col++)
		{
			switch (uBpp)
			{
				case 8:
					{
						return;	
					}
					break;
				case 16:
					{//888to565
						if (psBmp->m_sBmpHeader.m_BPP == 24)
						{
							p =(psBmp->m_pcImageData + (row * (psBmp->m_sBmpHeader.m_nImageWidth) + col)*(psBmp->m_sBmpHeader.m_BPP>>3));
							data = RGB565((U8)(*(p+2)), (U8)(*(p+1)), (U8)(*(p)));
							write(nFd , &data , (header.m_BPP>>3));
						}
					}
					break;
				case 24:
					{//565to888
						U8 red;
						U8 green;
						U8 blue;

						if (psBmp->m_sBmpHeader.m_BPP == 16)
						{
							p = (psBmp->m_pcImageData + (row * (psBmp->m_sBmpHeader.m_nImageWidth) + col)*(psBmp->m_sBmpHeader.m_BPP>>3));
						    red = *(p+1)>>3;	
							green = ((*(p+1)) & (~(0xf8)) << 3) | ( ((*p) & (~(0x1f))) >> 5);
							blue = (*p) & (0x1f);
							//data = RGB888(red, green, blue);
							write(nFd , &blue , 1);
							write(nFd , &green , 1);
							write(nFd , &red , 1);
						}
					}
					break;
				case 32:
					{
						return;	
					}
					break;
			}
		}
	}
}
