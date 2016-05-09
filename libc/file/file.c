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

#define FILECREAT(path, mode) 			creat((path), (mode))
#define FILEOPEN(path, flag, mode)  	open((path), (flag), (mode))
#define FILECLOSE(fd)  					close(fd)
#define FILESEEK(fd, offset, where)		lseek((fd), (offset), (where))
#define FILEEXIST(path)					access((path), F_OK)
#define FILEREAD(fd, buf, count)		read((fd), (buf), (count))
#define FILEWRITE(fd, buf, count)		write((fd), (buf), (count))


/**
 * @brief    file_create 
 *
 * @param[in]    pcPath
 * @param[in]    nMode
 *
 * @return   success return the new file descriptor, -1 if an error occurred 
 */
int file_create(const char* pcPath, mode_t nMode)
{
	return (FILECREAT(pcPath, nMode));
}

/**
 * @brief    file_create_default, mode is 0666 
 *
 * @param[in]    pcPath
 *
 * @return    success return the new file descriptor, -1 if an error occurred
 */
int file_create_default(const char* pcPath)
{
	return (FILECREAT(pcPath, 0666));
}

/**
 * @brief    file_open 
 *
 * @param[in]    pcPath
 * @param[in]    nFlag
 * @param[in]    nMode
 *
 * @return    success return the new file descriptor, -1 if an error occurred
 */
int file_open(const char* pcPath, int nFlag, mode_t nMode)
{
	int nFd = -1;

	nFd = FILEOPEN(pcPath, nFlag, nMode);

	return nFd;
}

/**
 * @brief    file_open_r read only 
 *
 * @param[in]    pcPath
 *
 * @return    success return the new file descriptor, -1 if an error occurred
 */
int file_open_r(const char* pcPath)
{
	int nFd = -1;

	nFd = FILEOPEN(pcPath, O_RDONLY, 0);

	return nFd;
}

/**
 * @brief    file_open_w write only
 *
 * @param[in]    pcPath
 *
 * @return    success return the new file descriptor, -1 if an error occurred
 */
int file_open_w(const char* pcPath)
{
	int nFd = -1;

	if (-1 == (nFd = FILEOPEN(pcPath, O_WRONLY, 0)))
		nFd = FILECREAT(pcPath, 0666);
	else
		FILESEEK(nFd, 0L, SEEK_END);

	return nFd;
}

/**
 * @brief    file_open_rw  read and write
 *
 * @param[in]    pcPath
 *
 * @return    success return the new file descriptor, -1 if an error occurred
 */
int file_open_rw(const char* pcPath)
{
	int nFd = -1;

	nFd = FILEOPEN(pcPath, O_RDWR, 0);

	return nFd;
}

/**
 * @brief    file_close 
 *
 * @param[in]    nFd
 *
 * @return   zero on success.  On error, -1 is returned, and errno is set appropriately 
 */
int file_close(int nFd)
{
	return FILECLOSE(nFd);
}

/**
 * @brief    file_seek 
 *
 * @param[in]    nFd
 * @param[in]    nOffset
 * @param[in]    nWhere
 *
 * @return   the resulting offset location as measured in bytes from the beginning of the file,On error, the value (off_t) -1 is returned and errno is set to
 *        indicate the error
 */
off_t file_seek(int nFd, off_t nOffset, int nWhere)
{
	return (FILESEEK(nFd, nOffset, nWhere));
}

/**
 * @brief    file_is_exist 
 *
 * @param[in]    pcPath
 *
 * @return   the file exists, 0 is returned;On error, -1 is returned
 */
int file_is_exist(const char* pcPath)
{
	return (FILEEXIST(pcPath));
}

/**
 * @brief    file_probe 
 *
 * @param[in]    pcPath
 *
 * @return   1 file exit, 0 file is not exit 
 */
int file_probe(const char* pcPath)
{
	int nFd = -1;

	nFd = FILEOPEN(pcPath, O_RDONLY, 0);

	if (-1 == nFd) return 0;
	
	FILECLOSE(nFd);		

	return 1;
}

/**
 * @brief    file_read 
 * 				read up to nCount bytes from file descriptor nFd into the buffer starting at pBuf 
 *
 * @param[in]    nFd
 * @param[out]    pBuf
 * @param[in]    nCount
 *
 * @return  	On  success, the number of bytes read is returned (zero indicates end of file), and the file position is advanced by this number. 
 * 				It is not an error if this number is smaller than the number of bytes requested; 
 * 				this may happen for example because fewer bytes are actually available right now (maybe because we were close to end-of-file, or because we are reading from a pipe,  
 * 				or  from  a terminal),  or  because  read()  was  interrupted  by a signal.  
 * 				On error, -1 is returned, and errno is set appropriately.  In this case it is left unspecified whether the file position (if any) changes.
 */
size_t file_read(int nFd, void *pBuf, size_t nCount)
{
	return (FILEREAD(nFd, pBuf, nCount));
}

/**
 * @brief    file_write 
 * 				writes up to nCount bytes from the buffer pointed pBuf to the file referred to by the file descriptor nFd.
 *
 * @param[in]    nFd
 * @param[in]    pBuf
 * @param[in]    nCount
 *
 * @return   On success, the number of bytes written is returned (zero indicates nothing was written).  On error, -1 is returned, and errno is set appropriately. 
 */
ssize_t file_write(int nFd, const void *pBuf, size_t nCount)
{
	return (FILEWRITE(nFd, pBuf, nCount));
}

//0 clear success, -1 file is not exist, -2 clear failed
int file_clear(const char* pcPath)
{
	if (-1 == file_is_exist(pcPath))
		return -1;

	int nFd = -1;
	nFd = FILEOPEN(pcPath, O_RDWR|O_TRUNC, 0);	
	if (-1 != nFd)
		FILECLOSE(nFd);
	else
		return -2;

	return 0;
}




#if 0
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

	file_clear(pc);

	return 0;
}
#endif
