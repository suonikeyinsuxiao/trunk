/***********************************************************************
*   Copyright (C) 2016 pilot-lab.inc All rights reserved.
*   
*   @file:       tfcard.h
*   @brief:      
*   @author:     Pilot labs
*   @maintainer: frank.fu@pilot-lab.com.cn
*   @version:    1.0
*   @date:       2016-05-10
*   
***********************************************************************/
#ifndef _TFCARD_H
#define _TFCARD_H
#ifdef __cplusplus
extern "C"
{
#endif

#define TFCARDPATH "/mnt/mmcblk0"

#define NK (1024)
#define FK (1000.0)

#define KBYTES (1 << 10)
#define MBYTES (1 << 20)
#define GBYTES (1 << 30)

#define KB(X) ((X) >> 10)
#define MB(X) ((X) >> 20)
#define GB(X) ((X) >> 30)

typedef enum
{
	e_B = 0x01,
	e_K,
	e_M,
	e_G,
}SIZETYPE_E;

typedef struct _size_S
{
	float 		m_fSize;
	SIZETYPE_E 	m_eSizeType;
}SIZE_S;


/**
 * @brief    tfcard_is_exist 
 *
 * @param[in]    pcPath
 *
 * @return   1 file exist, 0 file is not exist, -1 error 
 */
int tfcard_is_exist(const char* pcPath);

/**
 * @brief    getUsedSize 
 *
 * @param[in]    pcPath
 * @param[out]    psSize
 *
 * @return   0 get used size of tf-card success, -1 failed 
 */
int getUsedSize(const char* pcPath, SIZE_S* psSize);

/**
 * @brief    getTotalSize 
 *
 * @param[in]    pcPath
 * @param[out]    psSize
 *
 * @return    0 get total size of tf-card success, -1 failed 
 */
int getTotalSize(const char* pcPath, SIZE_S* psSize);

/**
 * @brief    getAvailableSize 
 *
 * @param[in]    pcPath
 * @param[out]    psSize
 *
 * @return    0 get available size of tf-card success, -1 failed
 */
int getAvailableSize(const char* pcPath, SIZE_S* psSize);

/**
 * @brief    getUsedPercent 
 *
 * @param[in]    pcPath
 * @param[out]    pfValPercent
 *
 * @return   success return 0, failed return -1 
 */
int getUsedPercent(const char* pcPath, float* pfValPercent);
#ifdef __cplusplus
}
#endif
#endif // _TFCARD_H
