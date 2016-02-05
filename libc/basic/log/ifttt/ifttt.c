/***********************************************************************
*   Copyright (C) 2016 pilot-lab.inc All rights reserved.
*   
*   @file:       ifttt.c
*   @brief:      
*   @author:     Pilot labs
*   @maintainer: frank.fu@pilot-lab.com.cn
*   @version:    1.0
*   @date:       2016-02-05
*   
***********************************************************************/
#include "ifttt.h"

#ifndef PL_TAG
#define PL_TAG "IFTTT"
#endif

void ifttt_print(char* pcStr)
{
	PL_LOGE(PL_TAG, "%s",pcStr);
	PL_LOGW(PL_TAG, "%s",pcStr);	
	PL_LOGD(PL_TAG, "%s",pcStr);
	PL_LOGI(PL_TAG, "%s",pcStr);
	PL_LOGV(PL_TAG, "%s",pcStr);
}
