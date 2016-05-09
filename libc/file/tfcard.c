/***********************************************************************
*   Copyright (C) 2016 pilot-lab.inc All rights reserved.
*   
*   @file:       tfcard.c
*   @brief:      
*   @author:     Pilot labs
*   @maintainer: frank.fu@pilot-lab.com.cn
*   @version:    1.0
*   @date:       2016-05-09
*   
***********************************************************************/
#include "file.h"
#include "log.h"
#include <stdio.h>
#include <sys/vfs.h>
#include <errno.h>


#define TFCARDPATH "/mnt/mmcblk0"

#ifndef PL_TAG
#define PL_TAG "TFCARD"
#endif

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



int tfcard_is_exist(const char* pcPath)
{
	if (NULL == pcPath) 
	{
		PL_LOGE(PL_TAG, "NULL == pcPath\n");
		return -1;
	}

    return	(!file_is_exist(pcPath));
}


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
		printf("xxxxxxxxxxxused_size=%d\n", used_size);
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

int main(int argc, char** argv)
{
	SIZE_S size;
	
	getTotalSize(argv[1], &size);
	printf("tatal size =%.2f, type=%x\n", size.m_fSize, size.m_eSizeType);

	getUsedSize(argv[1], &size);
	printf("used size =%.2f, type=%x\n", size.m_fSize, size.m_eSizeType);

	getAvailableSize(argv[1], &size);
	printf("available size =%.2f, type=%x\n", size.m_fSize, size.m_eSizeType);

	return 0;
}

#if 0
int main(int argc, char** argv)
{

	if (-1 == tfcard_is_exist(TFCARDPATH))
	{
		PL_LOGE(PL_TAG, "%s is not exist\n", TFCARDPATH);
		return -1;
	}

	struct statfs disk_info;

	if (statfs(TFCARDPATH, &disk_info) == -1)
    {
		PL_LOGE(PL_TAG, "Failed to get file disk infomation,errno:%u, reason:%s\n", errno, strerror(errno));
		return -1;
	}
		
	unsigned long total_size = (disk_info.f_blocks>>10) * (disk_info.f_bsize>>10);
	unsigned long available_size = (disk_info.f_bavail >> 10)* (disk_info.f_bsize >> 10);
	unsigned long free_size = (disk_info.f_bfree >> 10)* (disk_info.f_bsize >> 10);
	unsigned long used_size = (disk_info.f_blocks - disk_info.f_bavail)
	printf("disk_info.f_bsize >> 10=%lu\n",disk_info.f_bsize >> 10);
	printf("disk_info.f_bavail >> 10=%lu\n",disk_info.f_bavail >> 10 );
	printf("disk_info.f_bfree >> 20=%lu\n",disk_info.f_bfree >> 20);

	//输出每个块的长度，linux下内存块为4KB
	printf("block size: %ld bytes\n", disk_info.f_bsize);
	//输出块个数
	printf("total data blocks: %ld \n", disk_info.f_blocks);
	//输出path所在磁盘的大小
	printf("total file disk size: %ld MB\n",total_size);
	//输出非root用户可以用的磁盘空间大小
	printf("avaiable size: %ld MB\n",available_size);
	//输出硬盘的所有剩余空间
	printf("free size: %ld MB\n",free_size);
	//输出磁盘上文件节点个数
	printf("total file nodes: %ld\n", disk_info.f_files);
	//输出可用文件节点个数
	printf("free file nodes: %ld\n", disk_info.f_ffree);
	//输出文件名最大长度
	printf("maxinum length of file name: %ld\n", disk_info.f_namelen);

	return 0;

}
#endif
