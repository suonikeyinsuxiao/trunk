/***********************************************************************
*   Copyright (C) 2016 pilot-lab.inc All rights reserved.
*   
*   @file:       log.c
*   @brief:      
*   @author:     Pilot labs
*   @maintainer: frank.fu@pilot-lab.com.cn
*   @version:    1.0
*   @date:       2016-02-05
*   
***********************************************************************/
#include "log.h"
#include "ifttt.h"
#include "init.h"


#ifndef PL_TAG
#define PL_TAG  "LOG"
#endif

int main(int argc, char** argv)
{
	ifttt_print("hello");
	init_print("hello");

	PL_LOGE(PL_TAG, "hello");
	PL_LOGW(PL_TAG, "hello");	
	PL_LOGD(PL_TAG, "hello");
	PL_LOGI(PL_TAG, "hello");
	PL_LOGV(PL_TAG, "hello");

	PL_LOGE_FILE(PL_TAG, "hello");
	PL_LOGW_FILE(PL_TAG, "hello");
	PL_LOGD_FILE(PL_TAG, "hello");
	PL_LOGI_FILE(PL_TAG, "hello");
	PL_LOGV_FILE(PL_TAG, "hello");



	return 0;
}
