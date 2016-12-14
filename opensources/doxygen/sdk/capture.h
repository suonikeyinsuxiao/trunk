/***********************************************************************
*   Copyright (C) 2016 pilot-lab.inc All rights reserved.
*   
*   @file:       capture.h
*   @brief:      head file 
*   @author:     Pilot labs
*   @maintainer: frank.fu@pilot-lab.com.cn
*   @version:    1.0
*   @date:       2016-03-07
*   
***********************************************************************/
#ifndef _CAPTURE_H
#define _CAPTURE_H
#ifdef __cplusplus
extern "C"
{
#endif

#include <pthread.h>

/*
 * comment:此代码架构只适用一个video设备,/dev/videox                                
 *         若要支持多个video设备,需要做适当修改                                    
 *         从目前产品应用来看,不会用到多video设备,时间关系,就先不做进一步架构优化*/

typedef enum
{
	e_CALLBACK_OK = 0x30,
	e_CALLBACK_FULL, 
	e_CALLBACK_EXIST,
	e_CALLBACK_NOT_EXIST,
}CAPCALLBACK_E;

/**
 * @brief   video capture stream status
 */
typedef enum
{
	e_STREAM_ON,
	e_STREAM_OFF,
}CAPSTREAMSTATUS_E;

/**
 * @brief   video capture status
 */
typedef enum 
{
	e_CAP_INIT,			//init
	e_CAP_START,		
	e_CAP_STOP,			
	e_CAP_EXIT,			//exit
	
}CAPSTATUS_E;

/**
 * @brief    video capture buffer
 */
typedef struct _buffer_S
{
	void* 			m_pStart;
	unsigned int 	m_nLength;
	unsigned int 	m_PhyAddr;
}CAPBUFFER_S;

/**
 * @brief   video capture operations 
 */
typedef struct _captureOpts_S
{
	int (*initCap)	(void* pCapHandle);
	int (*startCap)	(void* pCapHandle);
	int (*stopCap)	(void* pCapHandle);
	int (*exitCap)	(void* pCapHandle);
	int (*setcap)	(void* pCapHandle);
}CAPOPTS_S;

/**
 * @brief   video capture handle 
 */
typedef struct _captureHandle_S
{
	int 				m_nVideoNum;		/** /dev/deviceX device */
	int 				m_nDevFd;			/** device file description*/
	int 				m_nWidth;			/** request target capture size */
	int 				m_nHeight;			
	int 				m_nFmt;				/** capture format */
	float 				m_fFps;				/** capture framerate */
	CAPOPTS_S* 			m_psOps;			/** operations*/
	//下面两个status是否禁止外部可控,暂不处理
	CAPSTATUS_E			m_eCapStatus;		/** capture status*/
	CAPSTREAMSTATUS_E 	m_eCapStmStatus;	/** capture stream status*/
	int					m_nSizeImage;		/** one frame length*/
	pthread_t 			m_sCapThread; 		/** video capture pthread */
	pthread_mutex_t 	m_sMutex;
}CAPTUREHANDLE_S;

typedef void (*CaptureDataCallback_F) (CAPTUREHANDLE_S* psCapHandle, unsigned char* pcVideoData, void* pUserData);

typedef struct _captureCallbackInfo_S
{
	CAPTUREHANDLE_S*		m_pCapHandle;
	CaptureDataCallback_F 	m_pfCallback;
	void*					m_pUserData;
}CAPTURECALLCBACKINFO_S;


/**
 * @brief    registerCapture 
 *
 * @param[in]    psCapHandle
 *
 * @return   0 is success, -1 is error 
 */
int registerCapture(CAPTUREHANDLE_S* psCapHandle);

/**
 * @brief    addDataCallBack 
 *
 * @param[in]    psCapHandle
 * @param[in]    pfCb
 * @param[in]    pUserData
 *
 * @return   -1 bad parameters, else CAPCALLBACK_E
 */
int addDataCallBack(CAPTUREHANDLE_S* psCapHandle, CaptureDataCallback_F pfCb, void* pUserData);

/**
 * @brief    delDataCallBack 
 *
 * @param[in]    psCapHandle
 * @param[in]    pfCb
 * @param[in]    pUserData
 *
 * @return    -1 bad parameters, else CAPCALLBACK_E
 */
int delDataCallBack(CAPTUREHANDLE_S* psCapHandle, CaptureDataCallback_F pfCb, void* pUserData);

#ifdef __cplusplus
}
#endif
#endif // _CAPTURE_H
