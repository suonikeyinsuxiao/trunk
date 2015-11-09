#ifndef __BMP_H
#define __BMP_H

#include <fcntl.h>
#include <unistd.h>

typedef  unsigned char  U8 ; 
typedef  unsigned short U16 ; 
typedef  unsigned int   U32 ; 

#define  RGB888(r,g,b)    			((r<<16)|(g<<8)|b)
#define  RGBGRAY(r,g,b)				((r+g+b)/3)				// a simple gray arithmetic
#define  RGB565(r,g,b)    		((((r)>>3)<<11)|(((g)>>2)<<5)|((b)>>3))

#pragma  pack(1)
typedef struct _bmpHeader
{
	//bmp header
	U8  m_acSignatue[2] ;   // B  M
	U32 m_nFileSize ;     //文件大小
	U16 m_Reserv1 ; 
	U16 m_Reserv2 ; 
	U32 m_nFileHeaderOffset ;   //文件头偏移量
	
	//DIm_B header
	U32 m_nDIBHeaderSize ; //DIB头大小
	U32 m_nImageWidth   ;  //文件宽度
	U32 m_nImageHight   ;  //文件高度
	U16 m_Planes       ; 
	U16 m_BPP          ;  //每个相素点的位数
	U32 m_nCompression  ; 
	U32 m_nImageSize    ;  //图片大小
	U32 m_nXPPM ; 
	U32 m_nYPPM ; 
	U32 m_nCCT ; 
	U32 m_nICC ;  
}BmpHeader_S;

typedef struct _bmp
{
	BmpHeader_S  m_sBmpHeader;
	char* 		 m_pcImageData;
}Bmp_S;

typedef struct _bmpRgb
{
	U8	m_cRed;
	U8	m_cGreen;
	U8	m_cBlue;
}BmpRgb_S;

#pragma  pack()

typedef enum
{
	ORIGINAL,
	CLOCKWISE90,		//顺时针转90
	CLOCKWISE180,		//顺时针转180
	ANTICLOCKWISE90		//逆时针转90
}RotateDirect_E;

/*
 *description : rotate bmp file
 *
 *nFd:destination bmp file
 *
 *pcBuf: buffer to store image data
 *
 *nOw:original image width
 *
 *nOh: original image height
 *
 *eDirection: rotary direction 
 *
 * nBpp: Bytes per pixel
 */
void rotateBmp(int nFd, RotateDirect_E eDirection, Bmp_S* psBmp);
//void rotateBmp(int nFd, char* pcBuf, int nOw, int nOh, RotateDirect_E eDirection, int nBpp);

/*
 *description :get head information of bmp file
 *
 *nFd means source bmp file
 *
 *psBmpHead pointer to BmpHeader_S
 */
void getBmpHeadInfo(int nFd, BmpHeader_S* psBmpHead);

/*
 *description :get image data of bmp file
 *
 *nFd means source bmp file
 *
 *pcDataBuf buffer to store image data 
 *
 *nDataLen buffer length
 */
void getBmpData(int nFd, char* pcDataBuf, int nDataLen);

/*
 *description: get RGB Gray by precision
 *
 *psRgb: RGB
 *
 *nPrecision: precision
 *
 *pcGray: gray value
 *
 * return: -1 error, 0 success
 * */
int getGray(BmpRgb_S* psRgb, int nPrecision, U8* pcGray);

/*
 *desription:RGB convert to Gray 
 *
 *nFd:dest file descriptor
 *
 *psBmp: Bmp_S

 *nPrecision: precision (2~20)
 * 
 */
void bmp2Gray(int nFd, Bmp_S* psBmp,int nPrecision);

/*
 *desription: convert original rgb to uBpp RGB
 *
 *nFd: dest bmp file
 *
 *psBmp: souce bmp file information
 *
 * uBpp: converted RGB format
 */
void convertRgb(int nFd, Bmp_S* psBmp, U16 uBpp);
#endif
