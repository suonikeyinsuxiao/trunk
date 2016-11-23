/***********************************************************************
*   Copyright (C) 2016 pilot-lab.inc All rights reserved.
*   
*   @file:       dir.c
*   @brief:      
*   @author:     Pilot labs
*   @maintainer: frank.fu@pilot-lab.com.cn
*   @version:    1.0
*   @date:       2016-11-22
*   
***********************************************************************/
#include "dir.h"

#define DIR_PRINT(fmt, arg...)	\
		printf("[%s:%d]: "fmt"\n", __FUNCTION__, __LINE__, ##arg)

int dir_is_exist(const char* pcPath)
{
	if (NULL == pcPath)
	{
		DIR_PRINT("NULL == pcName");
		return -1;
	}

	if (NULL == opendir(pcPath))
	{
		DIR_PRINT("%s is not exist: %s", pcPath, strerror(errno));
		return -1;
	}

	return 0;
}

int isDir(const char* pcPath)
{
	if (NULL == pcPath)
	{
		DIR_PRINT("NULL == pcName");
		return -1;
	}

	struct stat statBuf;
	if ( -1 == stat(pcPath, &statBuf))
	{
		DIR_PRINT("%s is not exist: %s", pcPath, strerror(errno));
		return -1;
	}

	if (S_ISDIR(statBuf.st_mode))
		return 0;
	else return -1;
}

DIR* dir_open(const char* pcPath)
{
	DIR* psDir = NULL;

	if (NULL == pcPath)
	{
		DIR_PRINT("NULL == pcName");
		return NULL;
	}

	if (NULL == (psDir = opendir(pcPath)))
	{
		DIR_PRINT("open %s failed: %s", pcPath, strerror(errno));
		return NULL;
	}

	return psDir;
}

int dir_close(DIR* psDir)
{
	int nRet = -1;
	if (NULL == psDir)
	{
		DIR_PRINT("NULL == psDir");	
		return -1;
	}

	nRet = closedir(psDir);
	if (0 == nRet)
	{
		DIR_PRINT("close dir success!");	
		nRet = 0;
	}
	else
	{
		DIR_PRINT("close dir failed: %s", strerror(errno));
		nRet = -1;
	}

	return nRet;
}

struct dirent* dir_read(DIR* psDir)
{
	int nRet = -1;
	struct dirent * psEntry = NULL;

	if (NULL == psDir)
	{
		DIR_PRINT("NULL == psDir");	
		return NULL;
	}

	psEntry = readdir(psDir);
	if (NULL == psEntry && errno != 0)
	{
		DIR_PRINT("read dir failed:(%d) %s", errno, strerror(errno));
		return NULL;
	}

	return psEntry;
}

int main(int argc, char** argv)
{
	int nRet = -1;
	struct dirent * psEntry = NULL;
	DIR* psDir = NULL;
	struct stat statBuf;

	if (0 == isDir(argv[1]))
		DIR_PRINT("%s is dir\n", argv[1]);
	//DT_BLK      This is a block device.
	//DT_CHR      This is a character device.
	//DT_DIR      This is a directory.
	//DT_FIFO     This is a named pipe (FIFO).
	//DT_LNK      This is a symbolic link.
	//DT_REG      This is a regular file.
	//DT_SOCK     This is a UNIX domain socket.
	//DT_UNKNOWN  The file type is unknown.

	DIR_PRINT("DT_BLK      (%d)This is a block device.		", DT_BLK);
	DIR_PRINT("DT_CHR      (%d)This is a character device.	", DT_CHR);
	DIR_PRINT("DT_DIR      (%d)This is a directory.			", DT_DIR);
	DIR_PRINT("DT_FIFO     (%d)This is a named pipe (FIFO). ", DT_FIFO);
	DIR_PRINT("DT_LNK      (%d)This is a symbolic link.		", DT_LNK);
	DIR_PRINT("DT_REG      (%d)This is a regular file.		", DT_REG);
	DIR_PRINT("DT_SOCK     (%d)This is a UNIX domain socket.", DT_SOCK);
	DIR_PRINT("DT_UNKNOWN  (%d)The file type is unknown.	", DT_UNKNOWN);

	if (!dir_is_exist(argv[1]))
	{
		psDir = dir_open(argv[1]);	

		while(NULL != (psEntry = dir_read(psDir)))
		{
			if (0 == strcmp(psEntry->d_name, "."))
				continue;
			if (0 == strcmp(psEntry->d_name, ".."))
				continue;

			switch (psEntry->d_type)
			{
				case DT_BLK:
					{
					}
					break;
				case DT_CHR:
					{
					}
					break;
				case DT_DIR:
					{
						char acPathName[256] = {0};
						sprintf(acPathName, "%s/%s", argv[1], psEntry->d_name);
						DIR_PRINT("dir path=%s\n", acPathName);
						
						DIR_PRINT("d_name=%s\n", psEntry->d_name);
						DIR_PRINT("d_ino=%ld\n", psEntry->d_ino);
						//DIR_PRINT("d_off=%ld\n", psEntry->d_off);
						DIR_PRINT("d_reclen=%d\n", psEntry->d_reclen);
						if (-1 == lstat(acPathName, &statBuf))
						{
							DIR_PRINT("lstat(%s) failed:(%d) %s!\n", psEntry->d_name, errno, strerror(errno));
							continue;
						}
						else
						{
							DIR_PRINT("lstat(%s) success!\n", psEntry->d_name);
							DIR_PRINT("ID of device containing file:%d\n", statBuf.st_dev);
							DIR_PRINT("inode number:%ld\n", statBuf.st_ino);
							DIR_PRINT("protection:0x%x\n", statBuf.st_mode);
							DIR_PRINT("number of hard links:%ld\n", statBuf.st_nlink);
							DIR_PRINT("user ID of owner:%d\n", statBuf.st_uid);
							DIR_PRINT("group ID of owner:%d\n", statBuf.st_gid);
							DIR_PRINT("device ID (if special file):%ld\n", statBuf.st_rdev);
							DIR_PRINT("total size, in bytes:%ld\n", statBuf.st_size);
							DIR_PRINT("blocksize for filesystem I/O:%ld\n", statBuf.st_blksize);
							DIR_PRINT("number of 512B blocks allocated:%ld\n", statBuf.st_blocks);
							DIR_PRINT("time of last access:%ld\n", statBuf.st_atime);
							DIR_PRINT("time of last modification:%ld\n", statBuf.st_mtime);
							DIR_PRINT("time of last status change:%ld\n", statBuf.st_ctime);
						}
					}
					break;
				case DT_FIFO:
					{
					}
					break;
				case DT_LNK:
					{
					}
					break;
				case DT_REG:
					{
						char acPathName[256] = {0};
						sprintf(acPathName, "%s/%s", argv[1], psEntry->d_name);
						DIR_PRINT("dir path=%s\n", acPathName);
						
						DIR_PRINT("d_name=%s\n", psEntry->d_name);
						DIR_PRINT("d_ino=%ld\n", psEntry->d_ino);
						//DIR_PRINT("d_off=%ld\n", psEntry->d_off);
						DIR_PRINT("d_reclen=%d\n", psEntry->d_reclen);
						if (-1 == lstat(acPathName, &statBuf))
						{
							DIR_PRINT("lstat(%s) failed:(%d) %s!\n", psEntry->d_name, errno, strerror(errno));
							continue;
						}
						else
						{
							DIR_PRINT("lstat(%s) success!\n", psEntry->d_name);
							DIR_PRINT("ID of device containing file:%ld\n", statBuf.st_dev);
							DIR_PRINT("inode number:%ld\n", statBuf.st_ino);
							DIR_PRINT("protection:0x%x\n", statBuf.st_mode);
							DIR_PRINT("number of hard links:%ld\n", statBuf.st_nlink);
							DIR_PRINT("user ID of owner:%d\n", statBuf.st_uid);
							DIR_PRINT("group ID of owner:%d\n", statBuf.st_gid);
							DIR_PRINT("device ID (if special file):%ld\n", statBuf.st_rdev);
							DIR_PRINT("total size, in bytes:%ld\n", statBuf.st_size);
							DIR_PRINT("blocksize for filesystem I/O:%ld\n", statBuf.st_blksize);
							DIR_PRINT("number of 512B blocks allocated:%ld\n", statBuf.st_blocks);
							DIR_PRINT("time of last access:%ld\n", statBuf.st_atime);
							DIR_PRINT("time of last modification:%ld\n", statBuf.st_mtime);
							DIR_PRINT("time of last status change:%ld\n", statBuf.st_ctime);
						}
					}
					break;
				case DT_SOCK:
					{
					}
					break;
				case DT_UNKNOWN:
					{
					}
					break;
			}
		}
		dir_close(psDir);
	}

	return 0;
}
