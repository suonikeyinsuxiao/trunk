/***********************************************************************
*   Copyright (C) 2016 pilot-lab.inc All rights reserved.
*   
*   @file:       dir.h
*   @brief:      
*   @author:     Pilot labs
*   @maintainer: frank.fu@pilot-lab.com.cn
*   @version:    1.0
*   @date:       2016-11-21
*   
***********************************************************************/
#ifndef _DIR_H
#define _DIR_H

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>


#ifdef __cplusplus
extern "C"
{
#endif

int dir_is_exist(const char* pcPath);
DIR* dir_open(const char* pcPath);
int dir_close(DIR* psDir);
struct dirent* dir_read(DIR* psDir);

#ifdef __cplusplus
}
#endif
#endif // _DIR_H
