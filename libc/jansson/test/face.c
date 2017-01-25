/***********************************************************************
*   Copyright (C) 2016 pilot-lab.inc All rights reserved.
*   
*   @file:       face.c
*   @brief:      
*   @author:     Pilot labs
*   @maintainer: frank.fu@pilot-lab.com.cn
*   @version:    1.0
*   @date:       2016-12-27
*   
***********************************************************************/
#include "face.h"

#include "OkaoAPI.h"
#include "CommonDef.h"
#include "OkaoDtAPI.h"
#include "OkaoPtAPI.h"
#include "OkaoFrAPI.h"
#include <jansson.h>
#include <string.h>
#include "netcamera.h"
//#include "capture.h"
#include "video.h"
#include "sql.h"

struct STREAM_HD;
extern struct _SYW_video_S s_VideoHandle;

typedef enum
{
	e_cmd_takephoto_request = 0,
	e_cmd_takephoto_response,
	e_cmd_register_request, 
	e_cmd_register_response,
	e_cmd_chgname_request,
	e_cmd_chgname_response,
	e_cmd_album_name_query_request,
	e_cmd_album_name_query_response,
	e_cmd_album_query_request,
	e_cmd_album_query_response,
	e_cmd_rm_request,
	e_cmd_rm_response
}CMD_KEY_E;

typedef enum
{
	e_blue = 0,
	e_green,
	e_red,
}BGR_E;

/*m_nAlbumUserId==-1表示空*/
typedef struct _person_s
{
	int                 m_nAlbumUserId;
	int                 m_anAlbumUserDataId[DATAID_MAX];
	unsigned char      	m_acName[NAMELEN];
	int                 m_nNameLen;
}PERSON_S;

typedef struct _handles_s
{
	HDETECTION 		m_hDT;			/* Face Detection Handle */
	HDTRESULT 		m_hDtResult;	/* Face Detection Result Handle */
	HPOINTER 		m_hPT;			/* Facial Parts Detection Handle */
	HPTRESULT 		m_hPtResult;	/* Facial Parts Detection Result Handle */
	HALBUM 			m_hAL;			/* Album Handle */
	HFEATURE 		m_hFD;			/* Feature Handle */

	SQL_HANDLE_S*	m_psSQLHandle;	/* sql handle */

	int				m_nCmdId;		/* json cmd id */
	unsigned char*  m_pcYuyv;		/* 保存当前需要注册用的yuv信息 */
	FACEINFO		m_sDtInfo;		/* 保存注册时检测到的人脸信息 */
}HANDLES;

HANDLES g_faceHandles;

#define FACE_LOG(fmt, arg...)  \
	        printf("[%s:%d] "fmt"\n", __FUNCTION__, __LINE__, ##arg)

#define PATH_RM_FACE_ALBUM_JPG 			("rm /mnt/UDISK/sdmmc/usr/face/album/jpg/")
#define PATH_RM_FACE_ALBUM_YUV 			("rm /mnt/UDISK/facerec/album/yuv/")
#define PATH_CP_FACE_PREVIEW_JPG		("cp /mnt/UDISK/sdmmc/usr/face/preview/jpg/preview.jpg")
#define PATH_CP_FACE_PREVIEW_YUV		("cp /mnt/UDISK/facerec/preview/yuv/preview.yuv")
#define PATH_FACE_PREVIEW_JPG 			("/mnt/UDISK/sdmmc/usr/face/preview/jpg/")
#define PATH_FACE_ALBUM_JPG				("/mnt/UDISK/sdmmc/usr/face/album/jpg/")
#define PATH_FACE_ALBUM_YUV				("/mnt/UDISK/facerec/album/yuv/")
#define PATH_FACE_PREVIEW_YUV			("/mnt/UDISK/facerec/preview/yuv/")
#define PATH_ALBUM_DB					("/mnt/UDISK/facerec/album.dat")
#define PATH_MKDIR_FACE_ALBUM_JPG		("mkdir -p /mnt/UDISK/sdmmc/usr/face/album/jpg/")	//手机端显示的照片路径
#define PATH_MKDIR_FACE_PREVIEW_JPG		("mkdir -p /mnt/UDISK/sdmmc/usr/face/preview/jpg/")	//手机端拍照预览的照片路径
#define PATH_MKDIR_FACE_ALBUM_YUV		("mkdir -p /mnt/UDISK/facerec/album/yuv/")
#define PATH_MKDIR_FACE_PREVIEW_YUV		("mkdir -p /mnt/UDISK/facerec/preview/yuv/")

#define MOOD_PATH               		("/pilot-app/pilot-sdcard/mood/")
#define CMD_APLAY           			("/pilot-app/pilot-bin/aplay")
#define CAMERA_CLICK_WAV				("camera_click.wav")

HALBUM CreateOrLoadAlbum(UINT32 unBufSize);
static BOOL SetDtParameters(HDETECTION hDT);
static BOOL RegisterData(HFEATURE hFD, HALBUM hAL, INT32 nUid, INT32 nDid);
static BOOL ExtractFeature(HANDLES *pHandles, RAWIMAGE *pImage, INT32 nWidth, INT32 nHeight);
static BOOL BackupAlbum(HALBUM hAL);

int initFaceAlbum(HANDLES* pHandles)
{
	int i = 0;
	int j = 0;
	unsigned int nDBSize = 0;
	int nRet = 0;
    BOOL bRegist;       /* Registration confirmation flag */
	char* pcSQL = NULL;
	SQL_HANDLE_S* pSqlHandle = NULL;

	system(PATH_MKDIR_FACE_ALBUM_YUV);
	system(PATH_MKDIR_FACE_ALBUM_JPG);
	system(PATH_MKDIR_FACE_PREVIEW_YUV);
	system(PATH_MKDIR_FACE_PREVIEW_JPG);

	nDBSize = get_file_size(PATH_ALBUM_DB);
	if (-1 == nDBSize)
		nDBSize = 0;
	FACE_LOG("album size=%d\n", nDBSize);

	pHandles->m_psSQLHandle = (SQL_HANDLE_S*)initSqlHandle();
	pcSQL = pHandles->m_psSQLHandle->m_sSqlCmd.m_pcSql;
	pSqlHandle = pHandles->m_psSQLHandle;

	if (0 == access(PATH_DATABASE, F_OK))
	{
		if (SQLITE_OK != sql_open_db_rw(pSqlHandle, PATH_DATABASE))	
		{
			FACE_LOG("open database failed");
			destroySqlHandle(pSqlHandle);
			return -1;
		}
		FACE_LOG("open database success!");	
	}
	else
	{
		if (SQLITE_OK != sql_create_db(pSqlHandle, PATH_DATABASE))
		{
			FACE_LOG("create database failed");
			destroySqlHandle(pSqlHandle);
			return -1;
		}
		FACE_LOG("create database success!");	

#if 1
		//创建表
		sprintf(pcSQL, "CREATE TABLE ALBUM(USERID INT NOT NULL,DATAID INT NOT NULL,NAME TEXT NOT NULL,AGE INT,GENDER TEXT);");
		sql_create_table(pSqlHandle, pcSQL);
#endif
		
#if 0
		//插入记录
		sprintf(pcSQL, "INSERT INTO ALBUM (USERID,DATAID,NAME) VALUES (0, 0, 'frank');");
		sql_insert_record(pSqlHandle, pcSQL);
#endif
	}

	initFaceLibs(pHandles, nDBSize);

	pHandles->m_pcYuyv = NULL;

	return 1;
}

void drawRectangle(RAWIMAGE* pYUV, RAWIMAGE* pRGB, FACEINFO info, int nWidth, int nHeight, BGR_E eColor)
{
	if ((info.ptLeftTop.x == 0) && (info.ptLeftTop.y == 0) && (info.ptRightBottom.x == 0) && (info.ptRightBottom.y == 0) )
	{
		FACE_LOG("invalid parameters!!!");
		return;
	}

	int x, y;

	OKAO_ImgConvertYUV422toBGR(pYUV, nWidth, nHeight, pRGB);

	//top
	for (x = info.ptLeftTop.x, y = info.ptLeftTop.y; x < info.ptRightTop.x; ++x)
	{
		switch (eColor)
		{
			case e_blue:
			{
				*(pRGB + (y*nWidth + x)*3)      = 255;
				*(pRGB + (y*nWidth + x)*3 + 1)  = 0;
				*(pRGB + (y*nWidth + x)*3 + 2)  = 0;
				break;
			}
			case e_green:
			{
				*(pRGB + (y*nWidth + x)*3)      = 0;
				*(pRGB + (y*nWidth + x)*3 + 1)  = 255;
				*(pRGB + (y*nWidth + x)*3 + 2)  = 0;
				break;
			}
			case e_red:
			{
				*(pRGB + (y*nWidth + x)*3)      = 0;
				*(pRGB + (y*nWidth + x)*3 + 1)  = 0;
				*(pRGB + (y*nWidth + x)*3 + 2)  = 255;
				break;
			}
		}
	}
	//bottom
	for (x = info.ptLeftBottom.x, y = info.ptLeftBottom.y; x < info.ptRightBottom.x; ++x)
	{
		switch (eColor)
		{
			case e_blue:
			{
				*(pRGB + (y*nWidth + x)*3)      = 255;
				*(pRGB + (y*nWidth + x)*3 + 1)  = 0;
				*(pRGB + (y*nWidth + x)*3 + 2)  = 0;
				break;
			}
			case e_green:
			{
				*(pRGB + (y*nWidth + x)*3)      = 0;
				*(pRGB + (y*nWidth + x)*3 + 1)  = 255;
				*(pRGB + (y*nWidth + x)*3 + 2)  = 0;
				break;
			}
			case e_red:
			{
				*(pRGB + (y*nWidth + x)*3)      = 0;
				*(pRGB + (y*nWidth + x)*3 + 1)  = 0;
				*(pRGB + (y*nWidth + x)*3 + 2)  = 255;
				break;
			}
		}
	}
	//left
	for (x = info.ptLeftTop.x, y = info.ptLeftTop.y; y < info.ptLeftBottom.y; ++y)
	{	
		switch (eColor)
		{
			case e_blue:
			{
				*(pRGB + (y*nWidth + x)*3)      = 255;
				*(pRGB + (y*nWidth + x)*3 + 1)  = 0;
				*(pRGB + (y*nWidth + x)*3 + 2)  = 0;
				break;
			}
			case e_green:
			{
				*(pRGB + (y*nWidth + x)*3)      = 0;
				*(pRGB + (y*nWidth + x)*3 + 1)  = 255;
				*(pRGB + (y*nWidth + x)*3 + 2)  = 0;
				break;
			}
			case e_red:
			{
				*(pRGB + (y*nWidth + x)*3)      = 0;
				*(pRGB + (y*nWidth + x)*3 + 1)  = 0;
				*(pRGB + (y*nWidth + x)*3 + 2)  = 255;
				break;
			}
		}
		
	}
	//right
	for (x = info.ptRightTop.x, y = info.ptRightTop.y; y < info.ptRightBottom.y; ++y)
	{
		switch (eColor)
		{
			case e_blue:
			{
				*(pRGB + (y*nWidth + x)*3)      = 255;
				*(pRGB + (y*nWidth + x)*3 + 1)  = 0;
				*(pRGB + (y*nWidth + x)*3 + 2)  = 0;
				break;
			}
			case e_green:
			{
				*(pRGB + (y*nWidth + x)*3)      = 0;
				*(pRGB + (y*nWidth + x)*3 + 1)  = 255;
				*(pRGB + (y*nWidth + x)*3 + 2)  = 0;
				break;
			}
			case e_red:
			{
				*(pRGB + (y*nWidth + x)*3)      = 0;
				*(pRGB + (y*nWidth + x)*3 + 1)  = 0;
				*(pRGB + (y*nWidth + x)*3 + 2)  = 255;
				break;
			}
		}
	}

	OKAO_ImgConvertBGRtoYUV422(pRGB, nWidth, nHeight, pYUV);
}

void playTakePhotoClick()
{
	char acMoodStr[256] = {0};
	//int nLen = 0;
	//nLen = strlen(MOOD_PATH);
	//strncpy(acMoodStr, MOOD_PATH, strlen(MOOD_PATH));
	//acMoodStr[nLen] = '\0';
	//strcat(acMoodStr, "camera_click.wav");
	//FACE_LOG("mood file:%s\n", acMoodStr);
	sprintf(acMoodStr, "%s %s%s", CMD_APLAY, MOOD_PATH, CAMERA_CLICK_WAV);
	FACE_LOG("click:%s\n", acMoodStr);
	system(acMoodStr);
}

#if 1
//packed yuv422转RGB24
static int yuyv2rgb24(unsigned char *yuyv, unsigned char *rgb, int width, int height)
{
	int i, in, rgb_index = 0;
	char y0, u0, y1, v1;
	int r, g, b;
	int out = 0, x, y;

	for(in = 0; in < width * height * 2; in += 4)
	{
		y0 = yuyv[in+0];
		u0 = yuyv[in+1];
		y1 = yuyv[in+2];
		v1 = yuyv[in+3];

		for (i = 0; i < 2; i++)
		{
			if (i)
				y = y1;
			else
				y = y0;
			r = y + (140 * (v1-128))/100;  //r
			g = y - (34 * (u0-128))/100 - (71 * (v1-128))/100; //g
			b = y + (177 * (u0-128))/100; //b
			if(r > 255)   r = 255;
			if(g > 255)   g = 255;
			if(b > 255)   b = 255;
			if(r < 0)     r = 0;
			if(g < 0)     g = 0;
			if(b < 0)     b = 0;

			//y = height - rgb_index/width -1;//上下倒置
			y = rgb_index/width;
			x = rgb_index%width;
			//rgb[(y*width+x)*3+0] = b;
			//rgb[(y*width+x)*3+1] = g;
			//rgb[(y*width+x)*3+2] = r;
			rgb[(y*width+x)*3+0] = r;
			rgb[(y*width+x)*3+1] = g;
			rgb[(y*width+x)*3+2] = b;
			rgb_index++;
		}
	}
	return 0;
}
#endif

/*
 * Parse text into a JSON object. If text is valid JSON, returns a
 * json_t structure, otherwise prints and error and returns null.
 */
json_t *load_json(const char *text) 
{
	json_t *root;
	json_error_t error;

	root = json_loads(text, 0, &error);

	if (root) {
		return root;
	} else {
		fprintf(stderr, "json error on line %d: %s\n", error.line, error.text);
		return (json_t *)0;
	}
}

#if 0
//packed格式yuyv转成planner格式I422
static void yuyv2I422(unsigned char *yuyv, unsigned char *YUV422, int width, int height)
{
	unsigned char *Y = NULL;
	unsigned char *U = NULL;
	unsigned char *V = NULL;
	unsigned char *p = yuyv;
	
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
#endif

int saveYuvFile(unsigned char* pcYuv, int nYuvLen, unsigned char* pcPath)
{
	FILE* psYuvFile = NULL;	
	size_t nWriteLen = 0;

	if ( NULL == (psYuvFile = fopen(pcPath, "wb")))
	{
		FACE_LOG("can't open %s\n", pcPath);
		return 0;
	}

	nWriteLen = fwrite(pcYuv, 1, nYuvLen, psYuvFile);
	if (nWriteLen != nYuvLen)
	{
		FACE_LOG("write %s failed\n", pcPath);	
		fclose(psYuvFile);
		return 0;
	}

	fclose(psYuvFile);

	return 1;
}

static int getMaxFaceInfo(FACEINFO sSrcInfo, FACEINFO* psOutInfo)
{
	if ( sSrcInfo.ptLeftTop.x < 0 
			|| sSrcInfo.ptLeftTop.y < 0
			|| sSrcInfo.ptRightTop.x < 0
			|| sSrcInfo.ptRightTop.y < 0
			|| sSrcInfo.ptLeftBottom.x < 0
			|| sSrcInfo.ptLeftBottom.y < 0
			|| sSrcInfo.ptRightBottom.x < 0
			|| sSrcInfo.ptRightBottom.y < 0
			)
	{
		return 0;
	}

	int nW 		= sSrcInfo.ptRightTop.x - sSrcInfo.ptLeftTop.x;
	int nH 		= sSrcInfo.ptLeftBottom.y - sSrcInfo.ptLeftTop.y;
	int nWMax 	= psOutInfo->ptRightTop.x - psOutInfo->ptLeftTop.x;
	int nHMax	= psOutInfo->ptLeftBottom.y - psOutInfo->ptLeftTop.y;

	if ((nWMax*nHMax) < (nW*nH))
	{
		psOutInfo->nID 				= sSrcInfo.nID;
		psOutInfo->ptLeftTop.x 		= sSrcInfo.ptLeftTop.x;
		psOutInfo->ptLeftTop.y 		= sSrcInfo.ptLeftTop.y;
		psOutInfo->ptRightTop.x 	= sSrcInfo.ptRightTop.x;
		psOutInfo->ptRightTop.y 	= sSrcInfo.ptRightTop.y;
		psOutInfo->ptLeftBottom.x 	= sSrcInfo.ptLeftBottom.x;
		psOutInfo->ptLeftBottom.y 	= sSrcInfo.ptLeftBottom.y;
		psOutInfo->ptRightBottom.x 	= sSrcInfo.ptRightBottom.x;
		psOutInfo->ptRightBottom.y 	= sSrcInfo.ptRightBottom.y;
		psOutInfo->nPose 			= sSrcInfo.nPose;
		psOutInfo->nConfidence 		= sSrcInfo.nConfidence;

	}
	return 1;
}

static void yuyv2I422(unsigned char *yuyv, unsigned char *YUV422, unsigned int width, unsigned int height)
{
	unsigned char *Y = NULL;
	unsigned char *U = NULL;
	unsigned char *V = NULL;
	unsigned char *p = yuyv;

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

static void YUV422p2RGB24(unsigned char *yuv422[3], unsigned char *rgb24, int width, int height)
{
	int R,G,B,Y,U,V;
	int x,y;
	int nWidth = width>>1; //色度信号宽度

	for (x = 0; x < width; x++)
	{
		for (y = 0; y < height; y++)
		{
			Y = *(yuv422[0] + y*width + x);
			U = *(yuv422[1] + y*nWidth + (x>>1));
			V = *(yuv422[2] + y*nWidth + (x>>1));

			R = Y + 1.402*(V-128);
			G = Y - 0.34414*(U-128) - 0.71414*(V-128);
			B = Y + 1.772*(U-128);
			//R = Y + 1.732*(V-128);
			//G = Y - 0.698*(U-128) - 0.703*(V-128);
			//B = Y + 1.3707*(U-128);

			//防止越界                                                                
			if (R>255)R=255;
			if (R<0)R=0;
			if (G>255)G=255;
			if (G<0)G=0;
			if (B>255)B=255;
			if (B<0)B=0;

			//*(rgb24 + (y*width + x)*3)      = B;
			//*(rgb24 + (y*width + x)*3 + 1)  = G;
			//*(rgb24 + (y*width + x)*3 + 2)  = R;
			*(rgb24 + (y*width + x)*3)      = R;
			*(rgb24 + (y*width + x)*3 + 1)  = G;
			*(rgb24 + (y*width + x)*3 + 2)  = B;
		}
	}
}

int takeFacePhoto(void* pVideoHandle, HANDLES* pHandles)
{
	struct _SYW_video_S* psVideoHandle = (struct _SYW_video_S*)pVideoHandle;
	int nRet = 0;
	char* pcYUV = NULL;
	char* pcYUVTmp = NULL;
	char* pcRGB = NULL;
	char acPath[512] = {0};
	char* pcI422 = NULL;
	char* acpYuv422p[3] = {NULL};
	int nW = psVideoHandle->m_sCapHandle.m_nWidth;
	int nH = psVideoHandle->m_sCapHandle.m_nHeight;
	FACEINFO info;
	FACEINFO maxInfo;
	int nIndex = 0;
	int nFaceCount = 0;

	/* Sets Face Detection parameters */
	if (SetDtParameters(pHandles->m_hDT) != TRUE) 
	{
		return 0;
	}

	if (psVideoHandle->m_sCapHandle.m_nFmt == V4L2_PIX_FMT_YUYV)
	{
		//if (NULL == pHandles->m_pcYuyv)
		//{
		//	pHandles->m_pcYuyv = (char*)malloc(nW*nH*2);
		//	pcYUV = pHandles->m_pcYuyv;
		//}
		//else
		//	pcYUV = pHandles->m_pcYuyv;

		pcYUV = (char*)malloc(nW*nH*2);
		FACE_LOG("share yuv  buffer = %p\n", psVideoHandle->m_pcYuyvBuffer);
		memcpy(pcYUV, psVideoHandle->m_pcYuyvBuffer, nW*nH*2);
		//yuyv2I422(pcYUV, pcI422, nW, nH);
	}

	//pcRGB = (char*)malloc(nW*nH*3);

	do {
            /******************/
            /* Face detection */
            /******************/
            /* Executes Face Detection */
            nRet = OKAO_Detection(pHandles->m_hDT, pcYUV, nW, nH, ACCURACY_NORMAL, pHandles->m_hDtResult);
            if ( nRet != OKAO_NORMAL ) {
                FACE_LOG( "OKAO_Detection() Error : %d\n", nRet );
                break;
            }
            /* Gets the number of detected face */
            nRet = OKAO_GetDtFaceCount(pHandles->m_hDtResult, &nFaceCount);
            if ( nRet != OKAO_NORMAL ) {
                FACE_LOG( "OKAO_GetDtFaceCount() Error : %d\n", nRet );
                break;
            }
            FACE_LOG( "OKAO_GetDtFaceCount() OK : nFaceCount = %d\n", nFaceCount );
            for ( nIndex = 0; nIndex < nFaceCount; nIndex++ ) {   /*** Face Loop ***/
                /* Gets the detection result for each face */
                nRet = OKAO_GetDtFaceInfo(pHandles->m_hDtResult, nIndex, &info);
                if ( nRet != OKAO_NORMAL ) {
                   	FACE_LOG( "OKAO_GetDtFaceInfo(%d) Error : %d\n", nIndex, nRet );
                    break;
                }

				getMaxFaceInfo(info, &maxInfo);
                FACE_LOG( "   <NO.%d>  (Confidence=%d, [%d,%d][%d,%d][%d,%d][%d,%d], ID=%d)\n",
                                    nIndex, info.nConfidence,
                                    info.ptLeftTop.x, info.ptLeftTop.y,
                                    info.ptRightTop.x, info.ptRightTop.y,
                                    info.ptLeftBottom.x, info.ptLeftBottom.y,
                                    info.ptRightBottom.x, info.ptRightBottom.y,
                                    info.nID);
			}

			if (nFaceCount == 0)
				break;

			FACE_LOG("3333333333333\n");
			//video_lock();
			nRet = OKAO_PT_SetPositionFromHandle(pHandles->m_hPT, pHandles->m_hDtResult, 0);
    		if ( nRet != OKAO_NORMAL ) {
    		    /* Error handling */
    		    FACE_LOG("OKAO_PT_SetPositionFromHandle() Error : %d\n", nRet );
				nRet = 0;
				free(pcYUV);
    		    return nRet;
    		}

    		nRet = OKAO_PT_DetectPoint(pHandles->m_hPT, pcYUV, nW, nH, pHandles->m_hPtResult);
    		if ( nRet != OKAO_NORMAL ) {
    		    /* Error handling */
    		    FACE_LOG( "OKAO_PT_DetectPoint() Error : %d\n", nRet );
				nRet = 0;
				free(pcYUV);
    		    return nRet;
    		}

    		nRet = OKAO_FR_ExtractFeatureFromPtHdl(pHandles->m_hFD, pcYUV, nW, nH, pHandles->m_hPtResult);
    		if ( nRet != OKAO_NORMAL ) {
    		    /* Error handling */
        		FACE_LOG( "OKAO_FR_ExtractFeatureFromPtHdl() Error : %d\n", nRet );
				nRet = 0;
				free(pcYUV);
    		    return nRet;
			}
			//video_unlock();
	} while(0);

	FACE_LOG("22222222222\n");
	pcRGB = (char*)malloc(nW*nH*3);
	if (NULL == pcRGB)
	{
		FACE_LOG("malloc rgb failed!");
		free(pcYUV);	
		return 0;
	}

	if (nFaceCount == 0)
	{
		nRet = 0;
		yuyv2rgb24(pcYUV, pcRGB, nW, nH);
	}
	else
	{
		pcI422 = (char*)malloc(nW*nH*2);
		acpYuv422p[0] = pcI422;
		acpYuv422p[1] = pcI422 + nW*nH;
		acpYuv422p[2] = acpYuv422p[1] + ((nW*nH) >> 1);

#if 0
		pcYUVTmp = (char*)malloc(nW*nH*2);
		if (NULL == pcYUVTmp)
		memcpy(pcYUVTmp, pcYUV, nW*nH*2);

		drawRectangle(pcYUVTmp, pcRGB, maxInfo, nW, nH, e_red);
		yuyv2I422(pcYUVTmp, pcI422, nW, nH);
		YUV422p2RGB24(acpYuv422p, pcRGB, nW, nH);
#endif

		drawRectangle(pcYUV, pcRGB, maxInfo, nW, nH, e_red);
		yuyv2I422(pcYUV, pcI422, nW, nH);
		YUV422p2RGB24(acpYuv422p, pcRGB, nW, nH);
		
		nRet = 1;
	}

	FACE_LOG("11111111111111\n");
	//拍照声效
	playTakePhotoClick();

	//写jpg
	strncpy(acPath, PATH_FACE_PREVIEW_JPG, strlen(PATH_FACE_PREVIEW_JPG));
	strncat(acPath, "preview.jpg", strlen("preview.jpg"));
	FACE_LOG("preview jpg path:%s\n", acPath);
	write_JPEG_file(acPath, 90, nW, nH, pcRGB);

	//yuyv2rgb24(pcYUV, pcRGB, nW, nH);
#if 0
	//写yuv
	memset(acPath, 0, 512);
	strncpy(acPath, PATH_FACE_PREVIEW_YUV, strlen(PATH_FACE_PREVIEW_YUV));
	strncat(acPath, "preview.yuv", strlen("preview.yuv"));
	FACE_LOG("preview yuv path:%s\n", acPath);
	saveYuvFile(pcYUV, (nW*nH*2), acPath);
#endif


	if (NULL != pcYUV)
	{
		free(pcYUV);
		pcYUV = NULL;
	}

	if (NULL != pcYUVTmp)
	{
		free(pcYUVTmp);
		pcYUVTmp = NULL;
	}

	if (NULL != pcI422)
	{
		free(pcI422);
		pcI422 = NULL;
	}

	if (NULL != pcRGB)
	{
		free(pcRGB);
		pcRGB = NULL;
	}

	nRet = 1;
	return nRet;
}

int decodeTakePhotoRequest(unsigned char*  pcStr, HANDLES* pHandles)
{
	if (NULL == pcStr || NULL == pHandles)
	{
		FACE_LOG("param error!");
		return 0;
	}

	//FACE_LOG("ENTER!!!");
	int nRet = 0;
	json_t* psRoot 			= NULL;
	json_t* psCmd_id 		= NULL;

	psRoot = load_json((const char*)pcStr);
	if (psRoot)
	{
		psCmd_id = json_object_get(psRoot, CMDID_KEY);
		if (NULL == psCmd_id)
		{
			FACE_LOG("no cmd_id!");
			nRet = 0;
			goto ERR;
		}
		pHandles->m_nCmdId = (int)json_integer_value(psCmd_id);
		FACE_LOG("cmd_id=%d", pHandles->m_nCmdId);

		//执行实际拍照动作
		//这里要保存两张,preview.jpg和preview.yuv
		nTakePhoto = 1;
		nRet = takeFacePhoto(&s_VideoHandle, pHandles);
		nTakePhoto = 0;
	}
	else
	{
		FACE_LOG("load json failed!\n");
		nRet = 0;
		goto ERR;
	}

	//nRet = 1;

ERR:

	if (NULL != psCmd_id)
		json_decref(psCmd_id);
	if (NULL != psRoot)
		json_decref(psRoot);

	FACE_LOG("END!!!");

	return nRet;
}

int encodeTakePhotoResponse(void* pcs, HANDLES* pHandles)
{
	if (NULL == pHandles)
	{
		FACE_LOG("param error!");
		return 0;
	}

	int nRet = 0;
	json_t *psRoot 			= NULL;
	json_t *psCmd			= NULL;
	json_t* psCmd_id 		= NULL;
	struct STREAM_HD msg;
	unsigned char* pcTmp	= NULL;
	unsigned char* pcStr 	= NULL;
	int nLen = 0;

	psRoot = json_object();
	if (NULL == psRoot)
	{
		FACE_LOG("create json failed!\n");
		nRet = 0;
		goto ERR;
	}

	psCmd = json_string(CMD_TAKEPHOTO_RESPONSE);
	if (NULL == psCmd)
	{
		FACE_LOG("create %s failed!\n", CMD_TAKEPHOTO_RESPONSE);
		nRet = 0;
		goto ERR;
	}

	psCmd_id = json_integer(pHandles->m_nCmdId);
	if (NULL == psCmd_id)
	{
		FACE_LOG("create %s failed!\n", CMD_TAKEPHOTO_RESPONSE);
		nRet = 0;
		goto ERR;
	}

	json_object_set_new(psRoot, CMD_KEY, psCmd);
	json_object_set_new(psRoot, CMDID_KEY, psCmd_id);

	pcStr = json_dumps(psRoot, JSON_INDENT(4));
	FACE_LOG("string:%s\n", pcStr);
	nLen = strlen(pcStr);
	//FACE_LOG("response len=%d\n", nLen);

	pcTmp = malloc(nLen+4);
	memset(pcTmp,'\0',nLen+4);

	msg.tag = 0xB1;
	msg.lens = nLen;

	memcpy(pcTmp, &msg, sizeof(struct STREAM_HD));
	memcpy(pcTmp+4, pcStr, nLen);

	ssudp_send(pcs,pcTmp,nLen+4,SS_WAIT);

	nRet = 1;

ERR:

	if (NULL != psCmd_id)
		json_decref(psCmd_id);
	if (NULL != psCmd)
		json_decref(psCmd);
	if (NULL != psRoot)
		json_decref(psRoot);

	free(pcTmp);

	return nRet;
}

int decodeRegisterRequest(unsigned char* pcStr, HANDLES* pHandles, int* pnAlbumUserId, int* pnAlbumDataId)
{
	if (NULL == pcStr || NULL == pHandles)
	{
		FACE_LOG("param error!");
		return 0;
	}

	int nRet = 0;
	json_t* psRoot 			= NULL;
	json_t* psCmd_id 		= NULL;
	json_t* psMetadata 		= NULL;
	json_t* psAlbumUserId	= NULL;
	json_t*	psName 			= NULL;
	json_t*	psStatus		= NULL;
	int nStatus				= 0; 
	int nCount				= 0;
	char* pcSQL				= NULL;

	psRoot = load_json((const char*)pcStr);
	if (psRoot)
	{
		psCmd_id = json_object_get(psRoot, CMDID_KEY);
		if (NULL == psCmd_id)
		{
			FACE_LOG("no cmd_id!");
			nRet = 0;
			goto ERR;
		}
		pHandles->m_nCmdId = (int)json_integer_value(psCmd_id);
		//FACE_LOG("cmd_id=%d", pHandles->m_nCmdId);

		psMetadata = json_object_get(psRoot, METADATA_KEY);
		if (NULL == psMetadata)
		{
			FACE_LOG("no %s!", METADATA_KEY);
			nRet = 0;
			goto ERR;
		}

		psAlbumUserId = json_object_get(psMetadata, METADATA_ALBUM_USERID);
		if (NULL == psMetadata)
		{
			FACE_LOG("no %s!", METADATA_ALBUM_USERID);	
			nRet = 0;
			goto ERR;
		}
		*pnAlbumUserId = (int)json_integer_value(psAlbumUserId);
		//FACE_LOG("album_userid=%d", *pnAlbumUserId);

		psName = json_object_get(psMetadata, METADATA_NAME);
		if (NULL == psMetadata)
		{
			FACE_LOG("no %s!", METADATA_NAME);
			nRet = 0;
			goto ERR;
		}

		psStatus = json_object_get(psMetadata, METADATA_STATUS);
		if (NULL == psMetadata)
		{
			FACE_LOG("no %s!", METADATA_NAME);
			nRet = 0;
			goto ERR;
		}
		nStatus = json_integer_value(psStatus);
		//FACE_LOG("status=%d\n", nStatus);

		if (nStatus)
		{
			//判断当前userid下哪个dataid是空,然后将preview.jpg, preview.yuv拷贝到指定/usrid/dataid.jpg, /usrid/dataid.yuv.
			int i = 0;
			int nUserID = *pnAlbumUserId;
			unsigned char acCmd[512] = {0};
			pcSQL = pHandles->m_psSQLHandle->m_sSqlCmd.m_pcSql;

			sprintf(pcSQL, "SELECT * FROM ALBUM WHERE USERID=%d AND NAME='%s';", nUserID-1, json_string_value(psName));
			sql_query_record(pHandles->m_psSQLHandle, pcSQL);

			if (-1 != pHandles->m_psSQLHandle->m_sPerson.m_nAlbumUserId)
			{
				//nUserID = pHandles->m_psSQLHandle->m_sPerson.m_nAlbumUserId;

				FACE_LOG("userid=%d, name=%s",pHandles->m_psSQLHandle->m_sPerson.m_nAlbumUserId, pHandles->m_psSQLHandle->m_sPerson.m_acName);
				for (i = 0; i < DATAID_MAX; ++i)
				{
					//FACE_LOG("dataid[%d]=%d",i,pHandles->m_psSQLHandle->m_sPerson.m_nAlbumUserDataId[i]);
					memset(acCmd, 0, 512);

					if (0 == pHandles->m_psSQLHandle->m_sPerson.m_nAlbumUserDataId[i])//dataid空闲
					{
						FACE_LOG("dataid[%d]=%d",i,pHandles->m_psSQLHandle->m_sPerson.m_nAlbumUserDataId[i]);
						//mkdir album jpg
						sprintf(acCmd, "%s%d", PATH_MKDIR_FACE_ALBUM_JPG, nUserID);
						//FACE_LOG("jpg:%s\n", acCmd);
						system(acCmd);
					
						//save jpg
						memset(acCmd, 0, 512);
						*pnAlbumDataId = i+1;
						//FACE_LOG("dataid=%d\n", *pnAlbumDataId);
						//sprintf(acCmd, "%s %s%d/%d_%d.jpg -f", PATH_CP_FACE_PREVIEW_JPG, PATH_FACE_ALBUM_JPG, nUserID, nUserID,*pnAlbumDataId);
						sprintf(acCmd, "%s %s%d/%d%d.jpg -f", PATH_CP_FACE_PREVIEW_JPG, PATH_FACE_ALBUM_JPG, nUserID, nUserID,*pnAlbumDataId);
						FACE_LOG("jpg:%s\n", acCmd);
						system(acCmd);

						//mkdir album yuv
						memset(acCmd, 0, 512);
						sprintf(acCmd, "%s%d", PATH_MKDIR_FACE_ALBUM_YUV, nUserID);
						//FACE_LOG("yuv:%s\n", acCmd);
						system(acCmd);

						//save yuv
						memset(acCmd, 0, 512);
						//sprintf(acCmd, "%s %s%d/%d_%d.yuv -f", PATH_CP_FACE_PREVIEW_YUV, PATH_FACE_ALBUM_YUV, nUserID, nUserID,*pnAlbumDataId);
						sprintf(acCmd, "%s %s%d/%d%d.yuv -f", PATH_CP_FACE_PREVIEW_YUV, PATH_FACE_ALBUM_YUV, nUserID, nUserID,*pnAlbumDataId);
						FACE_LOG("yuv:%s\n", acCmd);
						system(acCmd);

#if 0
						//注册人脸信息到数据库
						do
						{
							FACE_LOG("dynamic register userid=%d, dataid=%d\n", nUserID, *pnAlbumDataId);
							if (!RegisterData(pHandles->m_hFD, pHandles->m_hAL, nUserID, *pnAlbumDataId))
							{
								FACE_LOG("register data failed!\n");
								nRet = 0;
							}

							//更新写入数据库文件
							BackupAlbum(pHandles->m_hAL);
						}while(0);
#endif
						
						sprintf(pcSQL, "INSERT INTO ALBUM (USERID,DATAID,NAME) VALUES (%d, %d, '%s');", nUserID-1, i, json_string_value(psName));
						sql_insert_record(pHandles->m_psSQLHandle, pcSQL);

						break;
					}
				}
			}
			else
			{
				FACE_LOG("userid=%d, is not exist", nUserID);

				sprintf(pcSQL, "INSERT INTO ALBUM (USERID,DATAID,NAME) VALUES (%d, 0, '%s');",nUserID-1, json_string_value(psName));
				sql_insert_record(pHandles->m_psSQLHandle, pcSQL);

#if 0
				//注册人脸信息到数据库
				do
				{
					FACE_LOG("dynamic register userid=%d, dataid=%d\n", nUserID, *pnAlbumDataId);
					if (!RegisterData(pHandles->m_hFD, pHandles->m_hAL, nUserID, *pnAlbumDataId))
					{
						FACE_LOG("register data failed!\n");
						nRet = 0;
					}

					//更新写入数据库文件
					BackupAlbum(pHandles->m_hAL);
				}while(0);
#endif
			}

			nRet = 1;
			if (i == DATAID_MAX)
			{
				FACE_LOG("full!");	
				nRet = 0;
			}
		}
		else
			nRet = 0;
	}
	else
	{
		FACE_LOG("load json failed!\n");
		return 0;
	}

ERR:

	if (NULL != psMetadata)
		json_decref(psMetadata);
	if (NULL != psCmd_id)
		json_decref(psCmd_id);
	if (NULL != psRoot)
		json_decref(psRoot);

	return nRet;
}

int encodeRegisterResponse(void* pcs, HANDLES* pHandles, int nAlbumUserId, int nAlbumDataId, int nStatus)
{
	if (NULL == pHandles)
	{
		FACE_LOG("param error!");
		return 0;
	}

	int nRet				= 0;
	json_t *psRoot 			= NULL;
	json_t *psCmd			= NULL;
	json_t* psCmd_id 		= NULL;
	json_t* psMetadata 		= NULL;
	struct STREAM_HD msg;
	unsigned char* pcTmp 	= NULL;
	unsigned char* pcStr 	= NULL;
	int nLen = 0;
	char* pcSQL				= NULL;

	psRoot = json_object();
	if (NULL == psRoot)
	{
		FACE_LOG("create json failed!\n");
		nRet = 0;
		goto ERR;
	}

	psCmd = json_string(CMD_REGISTER_RESPONSE);
	if (NULL == psCmd)
	{
		FACE_LOG("create %s failed!\n", CMD_REGISTER_RESPONSE);
		nRet = 0;
		goto ERR;
	}

	psMetadata = json_object();
	json_object_set_new(psMetadata, METADATA_ALBUM_USERID, json_integer(nAlbumUserId));
	json_object_set_new(psMetadata, METADATA_ALBUM_DATAID, json_integer(nAlbumDataId));

	pcSQL = pHandles->m_psSQLHandle->m_sSqlCmd.m_pcSql;
	sprintf(pcSQL, "SELECT NAME FROM ALBUM WHERE USERID=%d;", nAlbumUserId-1);
	sql_query_record(pHandles->m_psSQLHandle, pcSQL);

	json_object_set_new(psMetadata, METADATA_NAME, json_string((const char*)&(pHandles->m_psSQLHandle->m_sPerson.m_acName[0])));
	json_object_set_new(psMetadata, METADATA_STATUS, json_integer(nStatus));
	//FACE_LOG("name:%s\n",pHandles->m_psSQLHandle->m_sPerson.m_acName);	

	psCmd_id = json_integer(pHandles->m_nCmdId);
	if (NULL == psCmd_id)
	{
		FACE_LOG("create %s failed!\n", CMD_CHGNAME_RESPONSE);
		nRet = 0;
		goto ERR;
	}

	json_object_set_new(psRoot, CMD_KEY, psCmd);
	json_object_set_new(psRoot, CMDID_KEY, psCmd_id);
	json_object_set_new(psRoot, METADATA_KEY, psMetadata);

	pcStr = json_dumps(psRoot, JSON_INDENT(4));
	FACE_LOG("string:%s\n", pcStr);
	nLen = strlen(pcStr);
	//FACE_LOG("response len=%d\n", nLen);

	pcTmp = malloc(nLen+4);
	memset(pcTmp,'\0',nLen+4);

	msg.tag = 0xB1;
	msg.lens = nLen;

	memcpy(pcTmp, &msg, sizeof(struct STREAM_HD));
	memcpy(pcTmp+4, pcStr, nLen);

	ssudp_send(pcs,pcTmp,nLen+4,SS_WAIT);

	nRet = 1;

ERR:

	if (NULL != psMetadata)
		json_decref(psMetadata);
	if (NULL != psCmd_id)
		json_decref(psCmd_id);
	if (NULL != psCmd)
		json_decref(psCmd);
	if (NULL != psRoot)
		json_decref(psRoot);

	free(pcTmp);

	return nRet;

	return 1;
}

int decodeChgnameRequest(unsigned char* pcStr, HANDLES* pHandles, int* pnAlbumUserId)
{
	if (NULL == pcStr || NULL == pHandles)
	{
		FACE_LOG("param error!");
		return 0;
	}

	json_t* psRoot 			= NULL;
	json_t* psCmd_id 		= NULL;
	json_t* psMetadata 		= NULL;
	json_t*	psAlbumUserId 	= NULL;
	json_t*	psName 			= NULL;
	int 	nRet 			= 0;
	int 	nNameRealLen 	= 0;
	char*	pcSQL			= NULL;
	int 	nUserID			= -1;

	psRoot = load_json((const char*)pcStr);
	if (psRoot)
	{
		psCmd_id = json_object_get(psRoot, CMDID_KEY);
		if (NULL == psCmd_id)
		{
			FACE_LOG("no cmd_id!");	
			nRet = 0;
			goto ERR;
		}
		pHandles->m_nCmdId = (int)json_integer_value(psCmd_id);
		//FACE_LOG("cmd_id=%d", pHandles->m_nCmdId);

		psMetadata = json_object_get(psRoot, METADATA_KEY);
		if (NULL == psMetadata)
		{
			FACE_LOG("no %s!", METADATA_KEY);
			nRet = 0;
			goto ERR;
		}

		psAlbumUserId = json_object_get(psMetadata, METADATA_ALBUM_USERID);
		if (NULL == psMetadata)
		{
			FACE_LOG("no %s!", METADATA_ALBUM_USERID);	
			nRet = 0;
			goto ERR;
		}
		*pnAlbumUserId = (int)json_integer_value(psAlbumUserId);
		nUserID = *pnAlbumUserId - 1;
		//FACE_LOG("album_userid=%d", nUserID);

		psName = json_object_get(psMetadata, METADATA_NAME);
		if (NULL == psMetadata)
		{
			FACE_LOG("no %s!", METADATA_NAME);
			nRet = 0;
			goto ERR;
		}
		nNameRealLen = strlen(json_string_value(psName));
		//FACE_LOG("namelen=%d", nNameRealLen);

		pcSQL = pHandles->m_psSQLHandle->m_sSqlCmd.m_pcSql;
		sprintf(pcSQL, "UPDATE ALBUM SET NAME='%s' WHERE USERID=%d",json_string_value(psName), nUserID);
		sql_update_record(pHandles->m_psSQLHandle, pcSQL);

		strncpy((char*)&pHandles->m_psSQLHandle->m_sPerson.m_acName[0], json_string_value(psName), NAMELEN);

		nRet = 1;
	}
	else
	{
		FACE_LOG("load json failed!\n");
		return 0;
	}

ERR:
	if (NULL != psName)
		json_decref(psName);
	if (NULL != psAlbumUserId)
		json_decref(psAlbumUserId);
	if (NULL != psMetadata)
		json_decref(psMetadata);
	if (NULL != psCmd_id)
		json_decref(psCmd_id);
	if (NULL != psRoot)
		json_decref(psRoot);
		

	return nRet;
}
/*
 *{
 *    "cmd":"chgname_response",
 *	  "cmd_id":123,
 *	  "metadata":{
 *	      "album_userid":0,
 *	      "name":"wangwu"
 *	      "status"：1		--->1,表示修改成功；0,表示修改失败;
 *	  }
 *}
 * */
int encodeChgnameResponse(void* pcs, int nAlbumUserId, HANDLES* pHandles)
{
	if (NULL == pHandles)
	{
		FACE_LOG("param error!");
		return 0;
	}

	int nRet				= 0;
	json_t *psRoot 			= NULL;
	json_t *psCmd			= NULL;
	json_t* psCmd_id 		= NULL;
	json_t* psMetadata 		= NULL;
	struct STREAM_HD msg;
	unsigned char* pcTmp 			= NULL;
	unsigned char* pcStr 			= NULL;
	int nLen = 0;

	psRoot = json_object();
	if (NULL == psRoot)
	{
		FACE_LOG("create json failed!\n");
		nRet = 0;
		goto ERR;
	}

	psCmd = json_string(CMD_CHGNAME_RESPONSE);
	if (NULL == psCmd)
	{
		FACE_LOG("create %s failed!\n", CMD_CHGNAME_RESPONSE);
		nRet = 0;
		goto ERR;
	}

	psMetadata = json_object();
	json_object_set_new(psMetadata, METADATA_ALBUM_USERID, json_integer(nAlbumUserId));
	json_object_set_new(psMetadata, METADATA_NAME, json_string((const char*)&pHandles->m_psSQLHandle->m_sPerson.m_acName[0]));
	json_object_set_new(psMetadata, METADATA_STATUS, json_integer(1));
	//FACE_LOG("name:%s\n",pHandles->m_psSQLHandle->m_sPerson.m_acName);

	psCmd_id = json_integer(pHandles->m_nCmdId);
	if (NULL == psCmd_id)
	{
		FACE_LOG("create %s failed!\n", CMD_CHGNAME_RESPONSE);
		nRet = 0;
		goto ERR;
	}

	json_object_set_new(psRoot, CMD_KEY, psCmd);
	json_object_set_new(psRoot, CMDID_KEY, psCmd_id);
	json_object_set_new(psRoot, METADATA_KEY, psMetadata);

	pcStr = json_dumps(psRoot, JSON_INDENT(4));
	FACE_LOG("string:%s\n", pcStr);
	nLen = strlen(pcStr);
	//FACE_LOG("response len=%d\n", nLen);

	pcTmp = malloc(nLen+4);
	memset(pcTmp,'\0',nLen+4);

	msg.tag = 0xB1;
	msg.lens = nLen;

	memcpy(pcTmp, &msg, sizeof(struct STREAM_HD));
	memcpy(pcTmp+4, pcStr, nLen);

	ssudp_send(pcs,pcTmp,nLen+4,SS_WAIT);

	nRet = 1;

ERR:

	if (NULL != psMetadata)
		json_decref(psMetadata);
	if (NULL != psCmd_id)
		json_decref(psCmd_id);
	if (NULL != psCmd)
		json_decref(psCmd);
	if (NULL != psRoot)
		json_decref(psRoot);

	free(pcTmp);

	return nRet;
}

int decodeAlbumNameQuery(unsigned char* pcStr, HANDLES* pHandles)
{
	if (NULL == pcStr || NULL == pHandles)
	{
		FACE_LOG("param error!");
		return 0;
	}

	json_t* psRoot 			= NULL;
	json_t* psCmd_id 		= NULL;
	int 	nRet 			= 0;

	psRoot = load_json((const char*)pcStr);
	if (psRoot)
	{
		psCmd_id = json_object_get(psRoot, CMDID_KEY);
		if (NULL == psCmd_id)
		{
			FACE_LOG("no cmd_id!");
			nRet = 0;
			goto ERR;
		}
		pHandles->m_nCmdId = (int)json_integer_value(psCmd_id);
		//FACE_LOG("cmd_id=%d", pHandles->m_nCmdId);
	}
	else
	{
		FACE_LOG("load json failed!\n");
		return 0;
	}

	nRet = 1;

ERR:

	if (NULL != psCmd_id)
		json_decref(psCmd_id);
	if (NULL != psRoot)
		json_decref(psRoot);

	return nRet;
}

/*
{
		"cmd":"album_name_query_response",
		"cmd_id":123,
		"metadata":[
		{"album_userid":0,"name":"zhangsan"},
		{"album_userid":1,"name":"lisi"},
		{"album_userid":2,"name":"wangwu"}		--->status:1,表示删除成功；0,表示删除失败;
		{"album_userid":3,"name":"zhaoliu"},
		{"album_userid":4,"name":"xiongda"}
	]
}*/
int encodeAlbumNameQueryResponse(void* pcs, HANDLES* pHandles)
{
	if (NULL == pHandles)
	{
		FACE_LOG("param error!");
		return 0;
	}

	unsigned char* pcStr			= NULL;
	int nRet						= 0;
	json_t *psRoot 					= NULL;
	json_t *psCmd					= NULL;
	json_t* psCmd_id 				= NULL;
	json_t* psMetadata 				= NULL;
	json_t* psArrayMetadata 		= NULL;
	json_t* psArrayMetadataObj0		= NULL;
	json_t* psArrayMetadataObj[USERID_MAX]		= {NULL};
	struct STREAM_HD msg;
	unsigned char* pcTmp 			= NULL;
	int nLen 						= 0;
	int i 							= 0;
	char* pcSQL 					= NULL;
	int nUserNum 					= 0;
	int nUserID 					= -1;

	psRoot = json_object();
	if (NULL == psRoot)
	{
		FACE_LOG("create json failed!\n");
		nRet = 0;
		goto ERR;
	}

	psCmd = json_string(CMD_ALBUM_NAME_QUERY_RESPONSE);
	if (NULL == psCmd)
	{
		FACE_LOG("create %s failed!\n", CMD_CHGNAME_RESPONSE);
		nRet = 0;
		goto ERR;
	}
	json_object_set_new(psRoot, CMD_KEY, psCmd);

	psCmd_id = json_integer(pHandles->m_nCmdId);
	if (NULL == psCmd_id)
	{
		FACE_LOG("create %s failed!\n", CMD_CHGNAME_RESPONSE);
		nRet = 0;
		goto ERR;
	}

	json_object_set_new(psRoot, CMDID_KEY, psCmd_id);

	psArrayMetadata = json_array();

	pcSQL = pHandles->m_psSQLHandle->m_sSqlCmd.m_pcSql;
	sprintf(pcSQL, "SELECT DISTINCT USERID FROM ALBUM ORDER BY USERID;");
	sql_get_user_num(pHandles->m_psSQLHandle, pcSQL, &nUserNum);
	FACE_LOG("user num=%d", nUserNum);

	do
	{
		int anUsersId[nUserNum];
		for (i = 0; i < nUserNum; ++i)
			anUsersId[i] = -1;

		sprintf(pcSQL, "SELECT DISTINCT USERID FROM ALBUM ORDER BY USERID;");
		sql_get_users_id(pHandles->m_psSQLHandle,pcSQL,anUsersId,nUserNum);

		for (i = 0; i < nUserNum; ++i)
		{
			FACE_LOG("anUsersId[%d]=%d", i, anUsersId[i]);

			psArrayMetadataObj[i] = json_object();
			json_object_set_new(psArrayMetadataObj[i], METADATA_ALBUM_USERID, json_integer(anUsersId[i]+1));

			sprintf(pcSQL, "SELECT NAME FROM ALBUM WHERE USERID=%d;", anUsersId[i]);
			sql_query_record(pHandles->m_psSQLHandle, pcSQL);

			json_object_set_new(psArrayMetadataObj[i], METADATA_NAME, json_string(&(pHandles->m_psSQLHandle->m_sPerson.m_acName[0])));
				
			json_array_append(psArrayMetadata, psArrayMetadataObj[i]);
			FACE_LOG("UserId=%d, Name=%s", anUsersId[i], pHandles->m_psSQLHandle->m_sPerson.m_acName);
		}
	}while(0);

	json_object_set_new(psRoot, METADATA_KEY, psArrayMetadata);

	pcStr = json_dumps(psRoot, JSON_INDENT(4));
	FACE_LOG("string:%s\n", pcStr);
	nLen = strlen(pcStr);
	//printf("response len=%d\n", nLen);

	pcTmp = malloc(nLen+4);
	memset(pcTmp,'\0',nLen+4);

	msg.tag = 0xB1;
	msg.lens = nLen;

	memcpy(pcTmp, &msg, sizeof(struct STREAM_HD));
	memcpy(pcTmp+4, pcStr, nLen);

	ssudp_send(pcs,pcTmp,nLen+4,SS_WAIT);
	nRet = 1;

ERR:

	if (NULL != psArrayMetadata)
		json_decref(psArrayMetadata);
	if (NULL != psMetadata)
		json_decref(psMetadata);
	if (NULL != psCmd_id)
		json_decref(psCmd_id);
	if (NULL != psCmd)
		json_decref(psCmd);
	if (NULL != psRoot)
		json_decref(psRoot);

	free(pcTmp);
	return nRet;
}

int decodeAlbumQuery(unsigned char* pcStr, HANDLES* pHandles)
{
	int nRet = 0;
	return 1;
}

int encodeAlbumQueryResponse(unsigned char* pcStr, HANDLES* pHandles)
{
	int nRet = 0;
	return 1;
}

int rmJPG(int nAlbumUserId, int nAlbumDataId, HANDLES* pHandles)
{
	int nRet = 0;
	unsigned char acPath[128] = PATH_RM_FACE_ALBUM_JPG; 
	int nLen = strlen(PATH_RM_FACE_ALBUM_JPG);
	unsigned char* pcTmp = acPath+nLen;

	//sprintf(pcTmp, "%d/%d_%d.jpg -f", nAlbumUserId, nAlbumUserId, nAlbumDataId);
	sprintf(pcTmp, "%d/%d%d.jpg -f", nAlbumUserId, nAlbumUserId, nAlbumDataId);
	FACE_LOG("string:%s\n", acPath);
	system(acPath);

	nRet = 1;
	return nRet;
}

int rmYUV(int nAlbumUserId, int nAlbumDataId, HANDLES* pHandles)
{
	int nRet = 0;
	unsigned char acPath[128] = PATH_RM_FACE_ALBUM_YUV; 
	int nLen = strlen(PATH_RM_FACE_ALBUM_YUV);
	unsigned char* pcTmp = acPath+nLen;

	//sprintf(pcTmp, "%d/%d_%d.yuv -f", nAlbumUserId, nAlbumUserId, nAlbumDataId);
	sprintf(pcTmp, "%d/%d%d.yuv -f", nAlbumUserId, nAlbumUserId, nAlbumDataId);
	FACE_LOG("string:%s\n", acPath);
	system(acPath);

	nRet = 1;
	return nRet;
}

int rmAlbumInfo(int nAlbumUserId, int nAlbumDataId, HANDLES* pHandles)
{
	int nRet = 0;

	return nRet;
}

static int checkUserDataId(int nAlbumUserId, HANDLES* pHandles)
{
	int nIndex = 0;
	int i = 0;
	SQL_HANDLE_S* psSqlHandle = (SQL_HANDLE_S*)pHandles->m_psSQLHandle;

	for (i = 0; i < DATAID_MAX; ++i)
	{
		if (1 == psSqlHandle->m_sPerson.m_nAlbumUserDataId[i])
			break;
	}

	if ( i == DATAID_MAX)
		return 1;
	else 
		return 0;
}

/*
{
	"cmd":"rm_request",
	"cmd_id":123,
	"metadata":[
	{"album_userid":0,"album_dataid":0,"name":"zhangsan"},
	{"album_userid":0,"album_dataid":1,"name":"zhangsan"},
	{"album_userid":0,"album_dataid":2,"name":"zhangsan"}
	]
}
*/
int decodeRmRequest(unsigned char* pcStr, HANDLES* pHandles)
{
	if (NULL == pcStr || NULL == pHandles)
	{
		FACE_LOG("param error!");
		return 0;
	}

	json_t* psRoot 			= NULL;
	json_t* psCmd_id 		= NULL;
	json_t* psMetadata 		= NULL;
	int 	nRet 			= 0;
	int nMetadataSize		= 0;
	size_t index			= 0;
	json_t* psValue			= NULL;
	char* pcSQL				= NULL;


	psRoot = load_json((const char*)pcStr);
	if (psRoot)
	{
		psCmd_id = json_object_get(psRoot, CMDID_KEY);
		if (NULL == psCmd_id)
		{
			FACE_LOG("no cmd_id!");
			nRet = 0;
			goto ERR;
		}
		//FACE_LOG("cmd_id=%d", (int)json_integer_value(psCmd_id));

		psMetadata = json_object_get(psRoot, METADATA_KEY);
		if (NULL == psMetadata)
		{
			FACE_LOG("no %s!", METADATA_KEY);
			nRet = 0;
			goto ERR;
		}

		nMetadataSize = json_array_size(psMetadata);
		//FACE_LOG("nMetadataSize =%d\n", nMetadataSize);

		pcSQL = pHandles->m_psSQLHandle->m_sSqlCmd.m_pcSql;
		json_array_foreach(psMetadata, index, psValue)
		{
			//FACE_LOG("index=%d\n", index);
			int nAlbumUserId = 0;
			int nAlbumDataId = 0;
			nAlbumUserId = json_integer_value(json_object_get(psValue, METADATA_ALBUM_USERID));
			nAlbumDataId = json_integer_value(json_object_get(psValue, METADATA_ALBUM_DATAID));
			//删除指定的模板
			//rm jpg
			rmJPG(nAlbumUserId, nAlbumDataId, pHandles);
			//rm yuv
			rmYUV(nAlbumUserId, nAlbumDataId, pHandles);
			//Clear specified data from Album
			OKAO_FR_ClearData(pHandles->m_hAL, nAlbumUserId-1, nAlbumDataId-1);
			//清零被删除的dataid
			sprintf(pcSQL, "DELETE FROM ALBUM WHERE USERID=%d AND DATAID=%d;", nAlbumUserId-1, nAlbumDataId-1);
			sql_rm_record(pHandles->m_psSQLHandle, pcSQL);
		}

		//更新写入数据库文件
		BackupAlbum(pHandles->m_hAL);
		//是否需要check该userid下的模板图片都删除了
	}
	else
	{
		FACE_LOG("load json failed!\n");
		return 0;
	}

	nRet = 1;

ERR:

	if (NULL != psMetadata)
		json_decref(psMetadata);
	if (NULL != psCmd_id)
		json_decref(psCmd_id);
	if (NULL != psRoot)
		json_decref(psRoot);

	return nRet;
}

int encodeRmResponse(void* pcs, HANDLES* pHandles, unsigned char* pcReqStr)
{
	int nRet = 0;
	int nLen = 0;
	json_t *psRoot 					= NULL;
	json_t *psRoot2					= NULL;
	json_t *psCmd					= NULL;
	json_t* psCmd_id 				= NULL;
	json_t* psMetadata 				= NULL;
	json_t* psArrayMetadata 		= NULL;
	json_t* psArrayMetadataObj 		= NULL;
	struct STREAM_HD msg;
	unsigned char* pcTmp			= NULL;
	unsigned char* pcStr			= NULL;
	size_t index					= 0;
	json_t* psValue					= NULL;

	//FACE_LOG("request string:%s", pcReqStr);

	psRoot = json_object();
	if (NULL == psRoot)
	{
		FACE_LOG("create json failed!\n");
		nRet = 0;
		goto ERR;
	}

	psCmd = json_string(CMD_RM_RESPONSE);
	if (NULL == psCmd)
	{
		FACE_LOG("create %s failed!\n", CMD_CHGNAME_RESPONSE);
		nRet = 0;
		goto ERR;
	}
	json_object_set_new(psRoot, CMD_KEY, psCmd);

	psCmd_id = json_integer(pHandles->m_nCmdId);
	if (NULL == psCmd_id)
	{
		FACE_LOG("create %s failed!\n", CMD_CHGNAME_RESPONSE);
		nRet = 0;
		goto ERR;
	}
	json_object_set_new(psRoot, CMDID_KEY, psCmd_id);

#if 1
	psArrayMetadata = json_array();

	psRoot2 = load_json((const char*)pcReqStr);
	if (psRoot2)
	{
		psMetadata = json_object_get(psRoot2, METADATA_KEY);
		if (NULL == psMetadata)
		{
			FACE_LOG("no %s!", METADATA_KEY);
			nRet = 0;
			goto ERR;
		}

		json_array_foreach(psMetadata, index, psValue)
		{
			//FACE_LOG("index=%d\n", index);

			psArrayMetadataObj = json_object();
			json_object_set_new(psArrayMetadataObj, METADATA_ALBUM_USERID, json_object_get(psValue, METADATA_ALBUM_USERID));
			json_object_set_new(psArrayMetadataObj, METADATA_ALBUM_DATAID, json_object_get(psValue, METADATA_ALBUM_DATAID));
			json_object_set_new(psArrayMetadataObj, METADATA_NAME, json_object_get(psValue, METADATA_NAME));
			json_object_set_new(psArrayMetadataObj, METADATA_STATUS, json_integer(1));
						
			if (0 != json_array_append(psArrayMetadata, psArrayMetadataObj))
				FACE_LOG("array append failed!!!");

		}
	}
	json_object_set_new(psRoot, METADATA_KEY, psArrayMetadata);
#endif

	pcStr = json_dumps(psRoot, JSON_INDENT(4));
	FACE_LOG("string:%s\n", pcStr);
	nLen = strlen(pcStr);
	//FACE_LOG("response len=%d\n", nLen);

	pcTmp = malloc(nLen+4);
	memset(pcTmp,'\0',nLen+4);

	msg.tag = 0xB1;
	msg.lens = nLen;

	memcpy(pcTmp, &msg, sizeof(struct STREAM_HD));
	memcpy(pcTmp+4, pcStr, nLen);

	ssudp_send(pcs,pcTmp,nLen+4,SS_WAIT);
	nRet = 1;

ERR:

	if (NULL != psArrayMetadata)
		json_decref(psArrayMetadata);
	if (NULL != psMetadata)
		json_decref(psMetadata);
	if (NULL != psCmd_id)
		json_decref(psCmd_id);
	if (NULL != psCmd)
		json_decref(psCmd);
	if (NULL != psRoot2)
		json_decref(psRoot2);
	if (NULL != psRoot)
		json_decref(psRoot);

	free(pcTmp);
	return nRet;
}

int encodeRocResponse(void* pcs, HANDLES* pHandles, int nAlbumUserId)
{
	int nRet = 0;
	int nLen = 0;
	json_t *psRoot 					= NULL;
	json_t *psCmd					= NULL;
	json_t* psCmd_id 				= NULL;
	json_t* psMetadata 				= NULL;
	json_t* psArrayMetadata 		= NULL;
	json_t* psArrayMetadataObj 		= NULL;
	struct STREAM_HD msg;
	unsigned char* pcTmp			= NULL;
	unsigned char* pcStr			= NULL;
	char* pcSQL						= NULL;

	psRoot = json_object();
	if (NULL == psRoot)
	{
		FACE_LOG("create json failed!\n");
		nRet = 0;
		goto ERR;
	}

	psCmd = json_string(CMD_RECOG_RESULT_RESPONSE);
	if (NULL == psCmd)
	{
		FACE_LOG("create %s failed!\n", CMD_CHGNAME_RESPONSE);
		nRet = 0;
		goto ERR;
	}
	json_object_set_new(psRoot, CMD_KEY, psCmd);

	psCmd_id = json_integer(123);
	if (NULL == psCmd_id)
	{
		FACE_LOG("create %s failed!\n", CMD_CHGNAME_RESPONSE);
		nRet = 0;
		goto ERR;
	}
	json_object_set_new(psRoot, CMDID_KEY, psCmd_id);

	pcSQL = pHandles->m_psSQLHandle->m_sSqlCmd.m_pcSql;
	if (nAlbumUserId != 0)
	{
		sprintf(pcSQL, "SELECT DISTINCT NAME FROM ALBUM WHERE USERID=%d;", nAlbumUserId-1);
		sql_query_record(pHandles->m_psSQLHandle, pcSQL);

		//if (!checkUserDataId(nAlbumUserId-1, pHandles))
		//{
			psArrayMetadata = json_array();

			psArrayMetadataObj = json_object();
			json_object_set_new(psArrayMetadataObj, METADATA_ALBUM_USERID, json_integer(nAlbumUserId));
			json_object_set_new(psArrayMetadataObj, METADATA_NAME, json_string(&(pHandles->m_psSQLHandle->m_sPerson.m_acName[0])));
						
			json_array_append(psArrayMetadata, psArrayMetadataObj);

			json_object_set_new(psRoot, METADATA_KEY, psArrayMetadata);
		//}
		//else
		//{
		//	FACE_LOG("xxxxxxxxxxxxxxxall zero\n");
		//}
	}

	pcStr = json_dumps(psRoot, JSON_INDENT(4));
	if (pcStr == NULL)
	{
		nRet = 0;
		goto ERR;
	}

	FACE_LOG("string:%s\n", pcStr);
	nLen = strlen(pcStr);
	//FACE_LOG("response len=%d\n", nLen);

	pcTmp = malloc(nLen+4);
	memset(pcTmp,'\0',nLen+4);

	msg.tag = 0xB1;
	msg.lens = nLen;

	memcpy(pcTmp, &msg, sizeof(struct STREAM_HD));
	memcpy(pcTmp+4, pcStr, nLen);

	//FACE_LOG("pcs=%p\n", pcs);
	ssudp_send(pcs,pcTmp,nLen+4,SS_WAIT);
	nRet = 1;

ERR:

	if (NULL != psArrayMetadata)
		json_decref(psArrayMetadata);
	if (NULL != psMetadata)
		json_decref(psMetadata);
	if (NULL != psCmd_id)
		json_decref(psCmd_id);
	if (NULL != psCmd)
		json_decref(psCmd);
	if (NULL != psRoot)
		json_decref(psRoot);

	free(pcTmp);
	return nRet;

}


/*==========face handle API==========*/
int initFaceLibs(HANDLES* pHandles, UINT32 unDbSize)
{
	pHandles->m_hDT = OKAO_CreateDetection();
    pHandles->m_hDtResult = OKAO_CreateDtResult(10, 0);
    pHandles->m_hPT = OKAO_PT_CreateHandle();
    pHandles->m_hPtResult = OKAO_PT_CreateResultHandle();
    pHandles->m_hFD = OKAO_FR_CreateFeatureHandle();
    pHandles->m_hAL = CreateOrLoadAlbum(unDbSize);

    if ((pHandles->m_hDT == NULL) || (pHandles->m_hDtResult == NULL) ||
        (pHandles->m_hPT == NULL) || (pHandles->m_hPtResult == NULL) ||
        (pHandles->m_hFD == NULL) || (pHandles->m_hAL == NULL)) {
        /* Error handling(insufficient back-up memory) */
        if ( pHandles->m_hDT == NULL ) {
            FACE_LOG( "OKAO_CreateDetection() Error\n" );
        }
        if ( pHandles->m_hDtResult == NULL ) {
            FACE_LOG( "OKAO_CreateDtResult() Error\n" );
        }
        if ( pHandles->m_hPT == NULL ) {
            FACE_LOG( "OKAO_PT_CreateHandle() Error\n" );
        }
        if ( pHandles->m_hPtResult == NULL ) {
            FACE_LOG( "OKAO_PT_CreateResultHandle() Error\n" );
        }
        if ( pHandles->m_hFD == NULL ) {
            FACE_LOG( "OKAO_FR_CreateFeatureHandle() Error\n" );
        }
        if ( pHandles->m_hAL == NULL ) {
            FACE_LOG( "OKAO_FR_CreateAlbumHandle() Error\n" );
        }
        return 0;
    }
    return 1;
}

void uninitFaceLibs(HANDLES *pHandles)    /* Handles struct pointer */
{
	if ( pHandles->m_hAL != NULL ) {
		OKAO_FR_DeleteAlbumHandle(pHandles->m_hAL);
	}
	if ( pHandles->m_hFD != NULL ) {
		OKAO_FR_DeleteFeatureHandle(pHandles->m_hFD);
	}
	if ( pHandles->m_hPtResult != NULL ) {
		OKAO_PT_DeleteResultHandle(pHandles->m_hPtResult);
	}
	if ( pHandles->m_hPT != NULL ) {
		OKAO_PT_DeleteHandle(pHandles->m_hPT);
	}
	if ( pHandles->m_hDtResult != NULL ) {
		OKAO_DeleteDtResult(pHandles->m_hDtResult);
	}
	if ( pHandles->m_hDT != NULL ) {
		OKAO_DeleteDetection(pHandles->m_hDT);
	}
}

/* Album data size */
HALBUM CreateOrLoadAlbum(UINT32 unBufSize)
{
    HALBUM hAL = NULL;          /* Album handle */
    UINT8 *pucBuffer = NULL;    /* Album data buffer */
    FR_ERROR nError;            /* Error code */

    UINT32 unSize;
    FILE *pFile;
    const char *pAlbumFile = PATH_ALBUM_DB;

    if ( unBufSize == 0 /* If Library is run for the first time */ ) {
        hAL = OKAO_FR_CreateAlbumHandle(10, 5);
    } else { /* Incase Album is stored only during executing the library */
        pucBuffer = malloc(unBufSize);
        if ( pucBuffer == NULL ) {
            /* Error Handling */
            FACE_LOG( "%s : Malloc Error! \n", pAlbumFile);
            return NULL;
        }

        /* Set Album data in pucBuffer */
        if ( ( pFile = fopen( pAlbumFile , "rb" ) ) == NULL ) {
            FACE_LOG( "%s : File Open Error! \n", pAlbumFile);
            free(pucBuffer);
            return NULL;
        }
        unSize = fread(pucBuffer, 1, unBufSize, pFile);
        if ( unSize != unBufSize ) {
            FACE_LOG( "%s : Read Size Error [%d != %d] \n", pAlbumFile, unSize, unBufSize);
        }
        fclose(pFile);

        hAL = OKAO_FR_RestoreAlbum(pucBuffer, unBufSize, &nError);
        FACE_LOG( "\nLoad [%s] : OK\n", pAlbumFile );

        free(pucBuffer);
    }

   return hAL;
}

/* Set Face Detection parameters */
static BOOL SetDtParameters(HDETECTION hDT)
{
    INT32 nRet = OKAO_ERR_VARIOUS;
    RECT rcStillArea;
    RECT rcMotionArea;
    UINT32 anAngle[POSE_TYPE_COUNT];
    INT32 nMotionAngleExtension;

    /* Sets the Face Detection Mode */
    nRet = OKAO_SetDtMode(hDT, DT_MODE_MOTION1);
    if (nRet != OKAO_NORMAL) {
        printf( "OKAO_SetDtMode() Error : %d\n", nRet );
        return FALSE;
    }

    /* Sets the Minimum and Maximum face sizes to be detected */
    nRet = OKAO_SetDtFaceSizeRange(hDT, 40, 8192);
    if (nRet != OKAO_NORMAL) {
        printf( "OKAO_SetDtFaceSizeRange() Error : %d\n", nRet );
        return FALSE;
    }

    /* Sets the Angle settings for face detection */
    anAngle[POSE_FRONT] = ANGLE_0;
    anAngle[POSE_HALF_PROFILE] = ANGLE_NONE;
    anAngle[POSE_PROFILE] = ANGLE_NONE;
    nMotionAngleExtension = ANGLE_ROTATION_EXT1 | ANGLE_POSE_EXT1 | DETECT_HEAD_USE;
    nRet = OKAO_SetDtAngle(hDT, anAngle, nMotionAngleExtension);
    if (nRet != OKAO_NORMAL) {
        printf( "OKAO_SetDtAngle() Error : %d\n", nRet );
        return FALSE;
    }

    /* Sets the Direction Mask for Motion mode */
    nRet = OKAO_SetDtDirectionMask(hDT, TRUE);
    if (nRet != OKAO_NORMAL) {
        printf( "OKAO_SetDtDirectionMask() Error : %d\n", nRet );
        return FALSE;
    }

    /* Sets the timeout time for OKAO_Detection() */
    nRet = OKAO_SetDtTimeout(hDT, 0, 0);
    if (nRet != OKAO_NORMAL) {
        printf( "OKAO_SetDtTimeout() Error : %d\n", nRet );
        return FALSE;
    }

    /* Sets the Face Detction Rectangular Mask */
    rcStillArea.left = -1;
    rcStillArea.top = -1;
    rcStillArea.right = -1;
    rcStillArea.bottom = -1;
    rcMotionArea.left = -1;
    rcMotionArea.top = -1;
    rcMotionArea.right = -1;
    rcMotionArea.bottom = -1;
    nRet = OKAO_SetDtRectangleMask(hDT, rcStillArea, rcMotionArea);
    if (nRet != OKAO_NORMAL) {
        printf( "OKAO_SetDtRectangleMask() Error : %d\n", nRet );
        return FALSE;
    }

    /* Sets the Face Detection Threshold */
    nRet = OKAO_SetDtThreshold(hDT, 500, 500);
    if (nRet != OKAO_NORMAL) {
        printf( "OKAO_SetDtThreshold() Error : %d\n", nRet );
        return FALSE;
    }

    /* Sets the search density coefficient for face detection */
    nRet = OKAO_SetDtStep(hDT, 33, 33);
    if (nRet != OKAO_NORMAL) {
        printf( "OKAO_SetDtStep() Error : %d\n", nRet );
        return FALSE;
    }

    /* Sets Motion Face Detection Refresh Count for each Motion mode */
    nRet = OKAO_SetDtRefreshCount(hDT, DT_MODE_MOTION1, 15);
    if (nRet != OKAO_NORMAL) {
        printf( "OKAO_SetDtRefreshCount() Error : %d\n", nRet );
        return FALSE;
    }
    /* Sets Motion Face Detection Retry Count, Motion Head Detection Retry Count and Hold Count at lost time */
    nRet = OKAO_SetDtLostParam(hDT, 2, 3, 2);
    if (nRet != OKAO_NORMAL) {
        printf( "OKAO_SetDtLostParam() Error : %d\n", nRet );
        return FALSE;
    }

    /* Sets the motion mode face detection position correction parameter */
    nRet = OKAO_SetDtModifyMoveRate(hDT, 4);
    if (nRet != OKAO_NORMAL) {
        printf( "OKAO_SetDtModifyMoveRate() Error : %d\n", nRet );
        return FALSE;
    }

    return TRUE;
}

BOOL ExtractFeature(HANDLES *pHandles, RAWIMAGE *pImage, INT32 nWidth, INT32 nHeight)
{
    INT32 nRet;         /* Error code */
    INT32 nCount;       /* Number of faces detected */

    nRet = OKAO_Detection(pHandles->m_hDT, pImage, nWidth, nHeight, ACCURACY_NORMAL, pHandles->m_hDtResult);
    if ( nRet != OKAO_NORMAL ) {
        /* Error handling */
        FACE_LOG( "OKAO_Detection() Error : %d\n", nRet );
        return FALSE;
    }
    nRet = OKAO_GetDtFaceCount(pHandles->m_hDtResult, &nCount);
    if ( nRet != OKAO_NORMAL ) {
        /* Error handling */
        FACE_LOG( "OKAO_GetDtFaceCount() Error : %d\n", nRet );
        return FALSE;
    }

    FACE_LOG( "OKAO_GetDtFaceCount() OK : nFaceCount = %d\n", nCount );
    if ( nCount == 0 ) {
        /* Processing when no face is detected */
        return FALSE;
    }

    nRet = OKAO_PT_SetPositionFromHandle(pHandles->m_hPT, pHandles->m_hDtResult, 0);
    if ( nRet != OKAO_NORMAL ) {
        /* Error handling */
        FACE_LOG( "OKAO_PT_SetPositionFromHandle() Error : %d\n", nRet );
        return FALSE;
    }
    nRet = OKAO_PT_DetectPoint(pHandles->m_hPT, pImage, nWidth, nHeight, pHandles->m_hPtResult);
    if ( nRet != OKAO_NORMAL ) {
        /* Error handling */
        FACE_LOG( "OKAO_PT_DetectPoint() Error : %d\n", nRet );
        return FALSE;
    }

    nRet = OKAO_FR_ExtractFeatureFromPtHdl(pHandles->m_hFD, pImage, nWidth, nHeight, pHandles->m_hPtResult);
    if ( nRet != OKAO_NORMAL ) {
        /* Error handling */
        FACE_LOG( "OKAO_FR_ExtractFeatureFromPtHdl() Error : %d\n", nRet );
        return FALSE;
    }

    return TRUE;
}

BOOL RegisterData(HFEATURE hFD, HALBUM hAL, INT32 nUid, INT32 nDid)
{
    INT32 nRet;         /* Error code */
    BOOL bRegist;       /* Registration confirmation flag */
    UINT32 unSize;              /* Album size */

    nRet = OKAO_FR_IsRegistered(hAL, nUid, nDid, &bRegist);
    if ( nRet != OKAO_NORMAL ) {
        /* Error handling */
        FACE_LOG( "OKAO_FR_IsRegistered() Error : %d\n", nRet );
        return FALSE;
    }

    if (bRegist) {
        /* The specified nUid, nDid are already registered. If registered again,
        it will over-write the previous registration. */
    } else {
        /* Specified nUid, nDid are not registered. If registered,
        it will be a new registration. */
    }

    nRet = OKAO_FR_GetSerializedAlbumSize(hAL, &unSize);
    FACE_LOG( "frank---OKAO_FR_GetSerializedAlbumSize() unSize=%d\n", unSize );

    nRet = OKAO_FR_RegisterData(hAL, hFD, nUid, nDid);
    if ( nRet != OKAO_NORMAL ) {
        /* Error handling */
        FACE_LOG( "OKAO_FR_RegisterData() Error : %d\n", nRet );
        return FALSE;
    } else {
        /* Processing during end of registration */
    }

    nRet = OKAO_FR_GetSerializedAlbumSize(hAL, &unSize);
    FACE_LOG( "frank---OKAO_FR_GetSerializedAlbumSize() unSize=%d\n", unSize );

    return TRUE;
}

BOOL BackupAlbum(HALBUM hAL)
{
    INT32 nRet;                 /* Error code */

    UINT32 unSize;              /* Album size */
    UINT8 *pucBuffer = NULL;    /* Local Pointer to store Album data*/

    FILE *pFile;
    const char *pAlbumFile = PATH_ALBUM_DB;

	//取出album数据的总大小
    nRet = OKAO_FR_GetSerializedAlbumSize(hAL, &unSize);
    if ( nRet != OKAO_NORMAL ) {
        /* Error handling */
        FACE_LOG( "OKAO_FR_GetSerializedAlbumSize() Error : %d\n", nRet );
        return FALSE;
    }
	FACE_LOG ("album total size=%d\n", unSize);

    pucBuffer = malloc(unSize);
    if ( pucBuffer == NULL ) {
        /* Error handling */
        FACE_LOG( "%s : Malloc Error! \n", pAlbumFile);
        return FALSE;
    }

	//取出album中的数据暂存在pucBuffer中
    nRet = OKAO_FR_SerializeAlbum(hAL, pucBuffer, unSize);
    if ( nRet != OKAO_NORMAL ) {
        /* Error handling */
        FACE_LOG( "OKAO_FR_SerializeAlbum() Error : %d\n", nRet );
        free(pucBuffer);
        return FALSE;
    }

    if ( ( pFile = fopen( pAlbumFile , "wb" ) ) == NULL ) {
        FACE_LOG( "%s : File Open Error! \n", pAlbumFile);
        free(pucBuffer);
        return FALSE;
    }
	//将album数据保存到文件
    fwrite(pucBuffer, 1, unSize, pFile);
    fclose(pFile);
    FACE_LOG( "Save [%s] : OK\n", pAlbumFile );

    free(pucBuffer);
    return TRUE;
}

BOOL Verification(HFEATURE hFD, HALBUM hAL, INT32 nUid)
{
    INT32 nRet;         /* Error code */
    INT32 nScore;       /* Score level (rate of identicalness) */

    nRet = OKAO_FR_Verify(hFD, hAL, nUid, &nScore);
    if ( nRet != OKAO_NORMAL ) {
        /* Error handling */
        FACE_LOG( "OKAO_FR_Verify() Error : %d\n", nRet );
        return FALSE;
    }
    FACE_LOG("      eOkaoFr Verify (UserID = %d, Score = %d)\n", nUid, nScore );

    if ( nScore > 400 ) {
        /* Recognize as same person */
        FACE_LOG( "      This person is same as UserID:%d\n\n", nUid );
    } else {
        /* Recognize as different person */
        FACE_LOG( "      This person is different from UserID:%d\n\n", nUid );
    }

    return TRUE;
}
/*========== face handle API end==========*/
