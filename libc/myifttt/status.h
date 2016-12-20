/***********************************************************************
*   Copyright (C) 2016 pilot-lab.inc All rights reserved.
*   
*   @file:       status.h
*   @brief:      
*   @author:     Pilot labs
*   @maintainer: frank.fu@pilot-lab.com.cn
*   @version:    1.0
*   @date:       2016-12-20
*   
***********************************************************************/
#ifndef _STATUS_H
#define _STATUS_H

#include "common.h"

#ifdef __cplusplus
extern "C"
{
#endif

#define STATUS_NAME_LEN			(64)
#define STATUS_CLASS_NAME_LEN	(64)

typedef struct ifttt_status_s
{
	char 	m_acStatusName[STATUS_NAME_LEN];
	char 	m_acClassName[STATUS_CLASS_NAME_LEN];
	char 	m_acSubClassName[STATUS_CLASS_NAME_LEN];
	int 	m_nValue;//使用宏定义
	int		m_nRef;//count of reference
}IFTTT_STATUS_S;

#ifdef __cplusplus
}
#endif
#endif // _STATUS_H
