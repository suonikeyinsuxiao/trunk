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
//#include "dir.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>
#include <errno.h>

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

	if (!dir_is_exist(argv[1]))
	{
		psDir = dir_open(argv[1]);	

		while(NULL != (psEntry = dir_read(psDir)))
		{
			//DIR_PRINT("d_info=%ld\n", psEntry->d_ino);
			//DIR_PRINT("d_off=%ld\n", psEntry->d_off);
			//DIR_PRINT("d_reclen=%d\n", psEntry->d_reclen);
			DIR_PRINT("d_type=%d\n", psEntry->d_type);
			DIR_PRINT("d_name=%s\n", psEntry->d_name);

		}
		dir_close(psDir);
	}


	return 0;
}
