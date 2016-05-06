/***********************************************************************
*   Copyright (C) 2016 pilot-lab.inc All rights reserved.
*   
*   @file:       file.c
*   @brief:      
*   @author:     Pilot labs
*   @maintainer: frank.fu@pilot-lab.com.cn
*   @version:    1.0
*   @date:       2016-05-06
*   
***********************************************************************/
#include "file.h"
#include <stdio.h>
#include <stdlib.h>

int file_create(const char* pcPath, mode_t nMode)
{
	return (FILECREAT(pcPath, nMode));
}

int file_create_default(const char* pcPath)
{
	return (FILECREAT(pcPath, 0666));
}

int file_open(const char* pcPath, int nFlag, mode_t nMode)
{
	int nFd = -1;

	nFd = FILEOPEN(pcPath, nFlag, nMode);

	return nFd;
}

int file_open_r(const char* pcPath)
{
	int nFd = -1;

	nFd = FILEOPEN(pcPath, O_RDONLY, 0);

	return nFd;
}

int file_open_w(const char* pcPath)
{
	int nFd = -1;

	if (-1 == (nFd = FILEOPEN(pcPath, O_WRONLY, 0)))
		nFd = FILECREAT(pcPath, 0666);
	else
		FILESEEK(nFd, 0L, SEEK_END);

	return nFd;
}

int file_open_rw(const char* pcPath)
{
	int nFd = -1;

	nFd = FILEOPEN(pcPath, O_RDWR, 0);

	return nFd;
}

int file_close(int nFd)
{
	return FILECLOSE(nFd);
}


off_t file_seek(int nFd, off_t nOffset, int nWhere)
{
	return (FILESEEK(nFd, nOffset, nWhere));
}

int file_is_exist(const char* pcPath)
{
	return (FILEEXIST(pcPath));
}

int file_probe(const char* pcPath)
{
	int nFd = -1;

	nFd = FILEOPEN(pcPath, O_RDONLY, 0);

	if (-1 == nFd) return 0;
	
	FILECLOSE(nFd);		

	return 1;
}

size_t file_read(int nFd, void *pBuf, size_t nCount)
{
	return (FILEREAD(nFd, pBuf, nCount));
}

ssize_t file_write(int nFd, const void *pBuf, size_t nCount)
{
	return (FILEWRITE(nFd, pBuf, nCount));
}







static char* pc = "test.txt";

int main(int argc, char** argv)
{
	int nFd = -1;
	char acBuf[256] = {0};

	if (!file_probe(pc))
		nFd = file_create(pc, 0666);
	printf("create file fd=%d\n", nFd);

	if (0 != file_is_exist(pc))
		nFd = file_create_default(pc);
	printf("create default file fd=%d\n", nFd);

	nFd = file_open_r(pc);
	printf("file_open_r fd=%d\n", nFd);
	printf("file_read count=%d\n", (int)file_read(nFd, acBuf, 256));
	file_close(nFd);

	nFd = file_open_w(pc);
	printf("file_open_w fd=%d\n", nFd);
	printf("file_write count=%d\n", (int)file_write(nFd, acBuf, 256));
	file_close(nFd);

	return 0;
}
