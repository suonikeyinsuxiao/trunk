/***********************************************************************
*   Copyright (C) 2016 pilot-lab.inc All rights reserved.
*   
*   @file:       file.h
*   @brief:      
*   @author:     Pilot labs
*   @maintainer: frank.fu@pilot-lab.com.cn
*   @version:    1.0
*   @date:       2016-05-06
*   
***********************************************************************/
#ifndef _FILE_H
#define _FILE_H
#ifdef __cplusplus
extern "C"
{
#endif

#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>
#include <unistd.h>

/**
 * @brief    file_create 
 *
 * @param[in]    pcPath
 * @param[in]    nMode
 *
 * @return   success return the new file descriptor, -1 if an error occurred 
 */
int file_create(const char* pcPath, mode_t nMode);

/**
 * @brief    file_create_default, mode is 0666 
 *
 * @param[in]    pcPath
 *
 * @return    success return the new file descriptor, -1 if an error occurred
 */
int file_create_default(const char* pcPath);

/**
 * @brief    file_open 
 *
 * @param[in]    pcPath
 * @param[in]    nFlag
 * @param[in]    nMode
 *
 * @return    success return the new file descriptor, -1 if an error occurred
 */
int file_open(const char* pcPath, int nFlag, mode_t nMode);

/**
 * @brief    file_open_r read only 
 *
 * @param[in]    pcPath
 *
 * @return    success return the new file descriptor, -1 if an error occurred
 */
int file_open_r(const char* pcPath);

/**
 * @brief    file_open_w write only
 *
 * @param[in]    pcPath
 *
 * @return    success return the new file descriptor, -1 if an error occurred
 */
int file_open_w(const char* pcPath);

/**
 * @brief    file_open_rw  read and write
 *
 * @param[in]    pcPath
 *
 * @return    success return the new file descriptor, -1 if an error occurred
 */
int file_open_rw(const char* pcPath);

/**
 * @brief    file_close 
 *
 * @param[in]    nFd
 *
 * @return   zero on success.  On error, -1 is returned, and errno is set appropriately 
 */
int file_close(int nFd);

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
off_t file_seek(int nFd, off_t nOffset, int nWhere);

/**
 * @brief    file_is_exist 
 *
 * @param[in]    pcPath
 *
 * @return   the file exists, 0 is returned;On error, -1 is returned
 */
int file_is_exist(const char* pcPath);

/**
 * @brief    file_probe 
 *
 * @param[in]    pcPath
 *
 * @return   1 file exit, 0 file is not exit 
 */
int file_probe(const char* pcPath);

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
size_t file_read(int nFd, void *pBuf, size_t nCount);

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
ssize_t flle_write(int nFd, const void *pBuf, size_t nCount);


int file_clear(const char* pcPath);

#ifdef __cplusplus
}
#endif
#endif // _FILE_H
