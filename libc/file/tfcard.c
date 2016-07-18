/***********************************************************************
*   Copyright (C) 2016 pilot-lab.inc All rights reserved.
*   
*   @file:       tfcard.c
*   @brief:      
*   @author:     Pilot labs
*   @maintainer: frank.fu@pilot-lab.com.cn
*   @version:    1.0
*   @date:       2016-05-10
*   
***********************************************************************/
#include "tfcard.h"
#include "file.h"
#include "log.h"
#include <stdio.h>
#include <sys/vfs.h>
#include <errno.h>

#ifndef PL_TAG
#define PL_TAG "TFCARD"
#endif

#define PERCENT (100)

int tfcard_is_exist(const char* pcPath)
{
	if (NULL == pcPath) 
	{
		PL_LOGE(PL_TAG, "NULL == pcPath\n");
		return -1;
	}

    return	(!file_is_exist(pcPath));
}

/**
 * @brief    getUsedSize 
 *
 * @param[in]    pcPath
 * @param[out]    psSize
 *
 * @return   0 get used size of tf-card success, -1 failed 
 */
int getUsedSize(const char* pcPath, SIZE_S* psSize)
{
	if (-1 == tfcard_is_exist(pcPath))
	{
		PL_LOGE(PL_TAG, "%s is not exist\n", pcPath);
		return -1;
	}

	struct statfs disk_info;

	if (statfs(pcPath, &disk_info) == -1)
    {
		PL_LOGE(PL_TAG, "failed to get %s infomation,errno:%u, reason:%s\n",pcPath, errno, strerror(errno));
		return -1;
	}

	unsigned long used_size = (KB(disk_info.f_blocks - disk_info.f_bfree)) * (KB(disk_info.f_bsize));

	if (used_size >= NK)
	{
		psSize->m_fSize = (used_size/FK);
		psSize->m_eSizeType = e_G;
		return 0;
	}
	else if (used_size < NK && used_size > 0)
	{
		psSize->m_fSize = (float)used_size;
		psSize->m_eSizeType = e_M;
		return 0;
	}
	else
	{
		unsigned int uUsedSizeK = (disk_info.f_blocks - disk_info.f_bfree)* (KB(disk_info.f_bsize));
		if (uUsedSizeK > 0 && uUsedSizeK < NK)
		{
			psSize->m_fSize = (float)uUsedSizeK;
			psSize->m_eSizeType = e_K;
			return 0;
		}
		else
		{
			psSize->m_fSize = (float)uUsedSizeK;
			psSize->m_eSizeType = e_B;
			return 0;
		}
	}
	return 0;
}

/**
 * @brief    getTotalSize 
 *
 * @param[in]    pcPath
 * @param[out]    psSize
 *
 * @return    0 get total size of tf-card success, -1 failed 
 */
int getTotalSize(const char* pcPath, SIZE_S* psSize)
{
	if (-1 == tfcard_is_exist(pcPath))
	{
		PL_LOGE(PL_TAG, "%s is not exist\n", pcPath);
		return -1;
	}

	struct statfs disk_info;

	if (statfs(pcPath, &disk_info) == -1)
    {
		PL_LOGE(PL_TAG, "failed to get %s infomation,errno:%u, reason:%s\n",pcPath, errno, strerror(errno));
		return -1;
	}

	unsigned int uTotalSize = (KB(disk_info.f_blocks)) * (KB(disk_info.f_bsize));
	if (uTotalSize >= NK)
	{
		psSize->m_fSize = (uTotalSize/FK);
		psSize->m_eSizeType = e_G;
	}
	else
	{
		psSize->m_fSize = (float)uTotalSize;
		psSize->m_eSizeType = e_M;
	}

	return 0;
}

/**
 * @brief    getAvailableSize 
 *
 * @param[in]    pcPath
 * @param[out]    psSize
 *
 * @return    0 get available size of tf-card success, -1 failed
 */
int getAvailableSize(const char* pcPath, SIZE_S* psSize)
{
	if (-1 == tfcard_is_exist(pcPath))
	{
		PL_LOGE(PL_TAG, "%s is not exist\n", pcPath);
		return -1;
	}

	struct statfs disk_info;

	if (statfs(pcPath, &disk_info) == -1)
    {
		PL_LOGE(PL_TAG, "failed to get %s infomation,errno:%u, reason:%s\n",pcPath, errno, strerror(errno));
		return -1;
	}

	unsigned int uAvailableSize = (KB(disk_info.f_bavail))* (KB(disk_info.f_bsize));
	if (uAvailableSize >= NK)
	{
		psSize->m_fSize = (float)(uAvailableSize/FK);
		psSize->m_eSizeType = e_G;
		return 0;
	}
	else if (uAvailableSize < NK && uAvailableSize > 0)
	{
		psSize->m_fSize = (float)uAvailableSize;
		psSize->m_eSizeType = e_M;
		return 0;
	}
	else
	{
		unsigned int uAvailableSizeK = (disk_info.f_bavail)* (KB(disk_info.f_bsize));
		if (uAvailableSizeK > 0 && uAvailableSizeK < NK)
		{
			psSize->m_fSize = (float)uAvailableSizeK;
			psSize->m_eSizeType = e_K;
			return 0;
		}
		else
		{
			psSize->m_fSize = (float)uAvailableSizeK;
			psSize->m_eSizeType = e_B;
			return 0;
		}
	}

	return 0;
}

int getUsedPercent(const char* pcPath, float* pfValPercent)
{
	SIZE_S sUsedSize;
	SIZE_S sTotalSize;

	if (-1 == getUsedSize(pcPath, &sUsedSize))
	{
		PL_LOGE(PL_TAG, "get used size failed\n");
		return -1;
	}

	if (getTotalSize(pcPath, &sTotalSize))
	{
		PL_LOGE(PL_TAG, "get total size failed\n");
		return -1;
	}

	if (sTotalSize.m_eSizeType == e_G)
	{
		if (sUsedSize.m_eSizeType == e_G)	
		{
			PL_LOGD(PL_TAG, "sUsedSize.m_fSize=%f,sTotalSize.m_fSize=%f",sUsedSize.m_fSize,sTotalSize.m_fSize);
			*pfValPercent = PERCENT * (sUsedSize.m_fSize/sTotalSize.m_fSize);	
		}
		else if (sUsedSize.m_eSizeType == e_M)
		{
			*pfValPercent = PERCENT * (sUsedSize.m_fSize/(KBYTES*sTotalSize.m_fSize));	
				
		}
		else if (sUsedSize.m_eSizeType == e_K)
		{
			*pfValPercent = PERCENT * (sUsedSize.m_fSize/(MBYTES*sTotalSize.m_fSize));	
		
		}
		else if (sUsedSize.m_eSizeType == e_B)
		{ 
			*pfValPercent = PERCENT * (sUsedSize.m_fSize/(GBYTES*sTotalSize.m_fSize));	
		}
	}
	else if (sTotalSize.m_eSizeType == e_M)
	{
		if (sUsedSize.m_eSizeType == e_M)
		{
			*pfValPercent = PERCENT * (sUsedSize.m_fSize/sTotalSize.m_fSize);	
		
		}
		else if (sUsedSize.m_eSizeType == e_K)
		{
			*pfValPercent = PERCENT * (sUsedSize.m_fSize/(KBYTES*sTotalSize.m_fSize));	
		
		}
		else if (sUsedSize.m_eSizeType == e_B)
		{
			*pfValPercent = PERCENT * (sUsedSize.m_fSize/(MBYTES*sTotalSize.m_fSize));	
		
		}
	
	}

	return 0;
}
