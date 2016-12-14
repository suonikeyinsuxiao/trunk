/***********************************************************************
*   Copyright (C) 2016 pilot-lab.inc All rights reserved.
*   
*   @file:       videoencodecomponent.h
*   @brief:      
*   @author:     Pilot labs
*   @maintainer: frank.fu@pilot-lab.com.cn
*   @version:    1.0
*   @date:       2016-03-28
*   
***********************************************************************/
#ifndef __ENCODER_COMP_H__
#define __ENCODER_COMP_H__

#include <semaphore.h>
#include "vencoder.h"



enum VIDEOENCODERNOTIFY
{
	VIDEO_ENCODE_NOTIFY_ERROR,
	VIDEO_ENCODE_NOTIFY_CRASH,
	VIDEO_ENCODE_NOTIFY_RETURN_BUFFER,
	VIDEO_ENCODE_NOTIFY_ENCODED_BUFFER,
};

//typedef enum
//{
//	e_IFrame;
//	e_PFrame;
//	e_BFrame;
//}VIDEO_FRAME_TYPE_E;

//videoencodecomponent---VENCCPT

typedef void* VENCCPT_P;

typedef int (*VENCCPTCB_F)(void* pUserData, int eMessageId, void* param);

/**
 * @brief   video encoder config struct 
 */
typedef struct _videoEncodeConfig_S
{
	VENC_CODEC_TYPE		m_eType;
	int             	m_nFrameRate;
	int             	m_nBitRate;
	int             	m_nOutWidth;
	int             	m_nOutHeight;
	int             	m_nSrcFrameRate;
	int             	m_nSrcWidth;
	int             	m_nSrcHeight;
	VENC_PIXEL_FMT  	m_eInputFormat;
	VencH264Param		m_sH264Param;
}VENC_CONFIG_S;

/**
 * @brief    video input buffer
 */
typedef struct _VideoInputBuffer_S
{
	unsigned char*	m_pcData;		//data buff
	int				m_nLen; 		//data len
	long long		m_nPts; 		//pts
}VIDEO_INPUTBUFFER_S;

/**
 * @brief    createVenc: create video encoder 
 *
 * @return    success, return pointer of video encoder handle, otherwise NULL
 */
VENCCPT_P createVenc();

/**
 * @brief    initVenc: init video encoder 
 *
 * @param[in]    pVencComp: video encoder handle
 * @param[in]    pVencConfig: video encoder config
 *
 * @return   success 0, otherwise -1 
 */
int initVenc(VENCCPT_P pVencComp, VENC_CONFIG_S* pVencConfig);

/**
 * @brief    destroyVenc: destroy video encoder 
 *
 * @param[in]    pVencComp: video encoder handle
 */
void destroyVenc(VENCCPT_P pVencComp);

/**
 * @brief    startVenc: video encode start 
 *
 * @param[in]    pVencComp: video encoder handle
 *
 * @return    0 success, -1 error
 */
int startVenc(VENCCPT_P pVencComp);
//int stopVenc(VENCCPT_P pVencComp);
//int resetVenc(VENCCPT_P pVencComp);

//int setFrameRate4Venc(VENCCPT_P pVencComp, int nFrameRate);

//int setBitRate4Venc(VENCCPT_P pVencComp, int nBitRate);

/**
 * @brief    getInputBufferFromVenc 
 *
 * @param[in]    pVencComp
 * @param[in]    psInputBuffer
 *
 * @return   0 success, -1 error 
 */
int getInputBufferFromVenc(VENCCPT_P pVencComp, VIDEO_INPUTBUFFER_S* psInputBuffer);

/**
 * @brief    getOneVideoFrame 
 *
 * @param[in]    pVencComp
 * @param[in]    psOutputBuffer
 *
 * @return   0 success , -1 error 
 */
int getOneVideoFrame(VENCCPT_P pVencComp, VencOutputBuffer *psOutputBuffer);

/**
 * @brief    freeOneVideoFrame 
 *
 * @param[in]    pVencComp
 * @param[in]    psOutputBuffer
 *
 * @return    0 success , -1 error
 */
int freeOneVideoFrame(VENCCPT_P pVencComp, VencOutputBuffer *psOutputBuffer);

/**
 * @brief    getSPS_PPS 
 *
 * @param[in]    pVencComp
 * @param[in]    ppcBuf
 * @param[in]    pnLen
 *
 * @return    0 success , -1 error
 */
int getSPS_PPS(VENCCPT_P pVencComp, unsigned char** ppcBuf, unsigned int* pnLen);

/**
 * @brief    setCallBack4Venc 
 *
 * @param[in]    pVencComp
 * @param[in]    pfVencCallBack
 * @param[in]    pUserData
 *
 * @return    0 success , -1 error
 */
int setCallBack4Venc(VENCCPT_P pVencComp, VENCCPTCB_F pfVencCallBack, void* pUserData);

/**
 * @brief    isKeyFrame 
 *
 * @param[in]    psOutputBuffer
 *
 * @return   1 is IDR frame; 0 is not IDR frame 
 */
int isKeyFrame(VencOutputBuffer* psOutputBuffer);







#if 0
 typedef void* VideoEncodeComp;
//int EncodeCompSetEncodeType(VideoEncodeComp *p, OMX_VIDEO_CODINGTYPE nType, int nFrameRate, int nBitRate, int nOutWidth, int nOutHeight, int nSrcFrameRate);
VideoEncodeComp* VideoEncodeCompCreate();
int VideoEncodeCompInit(VideoEncodeComp* p, VideoEncodeConfig* config);
void VideoEncodeCompDestory(VideoEncodeComp* p);

int VideoEncodeCompStart(VideoEncodeComp *p);
int VideoEncodeCompStop(VideoEncodeComp *p);
int VideoEncodeCompReset(VideoEncodeComp* p)  ;

int VideoEncodeCompSetFrameRate(VideoEncodeComp *p, int framerate);
int VideoEncodeCompSetBitRate(VideoEncodeComp *p, int bitrate);
int VideoEncodeCompInputBuffer(VideoEncodeComp* v, VideoInputBuffer *buf);
int VideoEncodeCompRequestVideoFrame(VideoEncodeComp *p, VencOutputBuffer *pBuffer);
int VideoEncodeCompReturnVideoFrame(VideoEncodeComp *v, VencOutputBuffer* outputBuffer);
int VideoEncodeCompSetCallback(VideoEncodeComp *p, VideoEncodeCallback notifier, void* pUserData);
int VideoEncodeCompGetExtradata(VideoEncodeComp *p, unsigned char** buf, unsigned int* length);
#endif


#endif 

