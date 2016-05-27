/***********************************************************************
*   Copyright (C) 2016 pilot-lab.inc All rights reserved.
*   
*   @file:       log.h
*   @brief:      
*   @author:     Pilot labs
*   @maintainer: frank.fu@pilot-lab.com.cn
*   @version:    1.0
*   @date:       2016-02-05
*   
***********************************************************************/
#ifndef _LOG_H
#define _LOG_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <syslog.h>
#include <stdio.h>

#define PL_LOG_LEVEL_CLOSE      0
#define PL_LOG_LEVEL_ERR        1
#define PL_LOG_LEVEL_WARNING    2
#define PL_LOG_LEVEL_INFO       3
#define PL_LOG_LEVEL_DEBUG      4
#define PL_LOG_LEVEL_VERBOSE    5

#define LOG_LEVEL_ERROR     "E/"
#define LOG_LEVEL_WARNING   "W/"
#define LOG_LEVEL_INFO      "I/"
#define LOG_LEVEL_DEBUG     "D/"
#define LOG_LEVEL_VERBOSE   "V/"


#ifndef CONFIG_PL_LOG_LEVEL
#ifdef PL_DEBUG
	#define CONFIG_PL_LOG_LEVEL PL_LOG_LEVEL_DEBUG
#else
	#define CONFIG_PL_LOG_LEVEL PL_LOG_LEVEL_INFO
#endif
#endif

#define PL_LOG(tag, level, fmt, arg...)  \
		printf("[%s][%s:%d]:%s "fmt"\n", tag, __FUNCTION__, __LINE__, level, ##arg)


#define PL_LOG_FILE(pri, tag, level, fmt, arg...)  \
        syslog(pri, "%s:%s <%s:%u>: "fmt"\n", tag, level, __FUNCTION__, __LINE__, ##arg)


#if CONFIG_PL_LOG_LEVEL == PL_LOG_LEVEL_CLOSE
#define PL_LOGE(tag, fmt, arg...) 
#define PL_LOGW(tag, fmt, arg...)
#define PL_LOGD(tag, fmt, arg...)
#define PL_LOGI(tag, fmt, arg...)
#define PL_LOGV(tag, fmt, arg...)
#define PL_LOGE_FILE(tag, fmt, arg...) 
#define PL_LOGW_FILE(tag, fmt, arg...)
#define PL_LOGD_FILE(tag, fmt, arg...)
#define PL_LOGI_FILE(tag, fmt, arg...)
#define PL_LOGV_FILE(tag, fmt, arg...)

#elif CONFIG_PL_LOG_LEVEL == PL_LOG_LEVEL_ERR
#define PL_LOGE(tag, fmt, arg...) PL_LOG(tag, LOG_LEVEL_ERROR, fmt, ##arg)
#define PL_LOGW(tag, fmt, arg...)
#define PL_LOGD(tag, fmt, arg...)
#define PL_LOGI(tag, fmt, arg...)
#define PL_LOGV(tag, fmt, arg...)
#define PL_LOGE_FILE(tag, fmt, arg...) PL_LOG_FILE(LOG_ERR, tag, LOG_LEVEL_ERROR, fmt, ##arg)
#define PL_LOGW_FILE(tag, fmt, arg...)
#define PL_LOGD_FILE(tag, fmt, arg...)
#define PL_LOGI_FILE(tag, fmt, arg...)
#define PL_LOGV_FILE(tag, fmt, arg...)

#elif CONFIG_PL_LOG_LEVEL == PL_LOG_LEVEL_WARNING
#define PL_LOGE(tag, fmt, arg...) PL_LOG(tag, LOG_LEVEL_ERROR, fmt, ##arg)
#define PL_LOGW(tag, fmt, arg...) PL_LOG(tag, LOG_LEVEL_WARNING, fmt, ##arg)
#define PL_LOGD(tag, fmt, arg...) 
#define PL_LOGI(tag, fmt, arg...)
#define PL_LOGV(tag, fmt, arg...)
#define PL_LOGE_FILE(tag, fmt, arg...) PL_LOG_FILE(LOG_ERR, tag, LOG_LEVEL_ERROR, fmt, ##arg)
#define PL_LOGW_FILE(tag, fmt, arg...) PL_LOG_FILE(LOG_WARNING, tag, LOG_LEVEL_WARNING, fmt, ##arg)
#define PL_LOGD_FILE(tag, fmt, arg...)
#define PL_LOGI_FILE(tag, fmt, arg...)
#define PL_LOGV_FILE(tag, fmt, arg...)

#elif CONFIG_PL_LOG_LEVEL == PL_LOG_LEVEL_INFO
#define PL_LOGE(tag, fmt, arg...) PL_LOG(tag, LOG_LEVEL_ERROR, fmt, ##arg)
#define PL_LOGW(tag, fmt, arg...) PL_LOG(tag, LOG_LEVEL_WARNING, fmt, ##arg)
#define PL_LOGI(tag, fmt, arg...) PL_LOG(tag, LOG_LEVEL_INFO, fmt, ##arg)
#define PL_LOGD(tag, fmt, arg...) 
#define PL_LOGV(tag, fmt, arg...)
#define PL_LOGE_FILE(tag, fmt, arg...) PL_LOG_FILE(LOG_ERR, tag, LOG_LEVEL_ERROR, fmt, ##arg)
#define PL_LOGW_FILE(tag, fmt, arg...) PL_LOG_FILE(LOG_WARNING, tag, LOG_LEVEL_WARNING, fmt, ##arg)
#define PL_LOGI_FILE(tag, fmt, arg...) PL_LOG_FILE(LOG_INFO, tag, LOG_LEVEL_INFO, fmt, ##arg)
#define PL_LOGD_FILE(tag, fmt, arg...)
#define PL_LOGV_FILE(tag, fmt, arg...)

#elif CONFIG_PL_LOG_LEVEL == PL_LOG_LEVEL_DEBUG
#define PL_LOGE(tag, fmt, arg...) PL_LOG(tag, LOG_LEVEL_ERROR, fmt, ##arg)
#define PL_LOGW(tag, fmt, arg...) PL_LOG(tag, LOG_LEVEL_WARNING, fmt, ##arg)
#define PL_LOGI(tag, fmt, arg...) PL_LOG(tag, LOG_LEVEL_INFO, fmt, ##arg)
#define PL_LOGD(tag, fmt, arg...) PL_LOG(tag, LOG_LEVEL_DEBUG, fmt, ##arg)
#define PL_LOGV(tag, fmt, arg...)
#define PL_LOGE_FILE(tag, fmt, arg...) PL_LOG_FILE(LOG_ERR, tag, LOG_LEVEL_ERROR, fmt, ##arg)
#define PL_LOGW_FILE(tag, fmt, arg...) PL_LOG_FILE(LOG_WARNING, tag, LOG_LEVEL_WARNING, fmt, ##arg)
#define PL_LOGI_FILE(tag, fmt, arg...) PL_LOG_FILE(LOG_INFO, tag, LOG_LEVEL_INFO, fmt, ##arg)
#define PL_LOGD_FILE(tag, fmt, arg...) PL_LOG_FILE(LOG_DEBUG, tag, LOG_LEVEL_DEBUG, fmt, ##arg)
#define PL_LOGV_FILE(tag, fmt, arg...)

#elif CONFIG_PL_LOG_LEVEL == PL_LOG_LEVEL_VERBOSE
#define PL_LOGE(tag, fmt, arg...) PL_LOG(tag, LOG_LEVEL_ERROR, fmt, ##arg)
#define PL_LOGW(tag, fmt, arg...) PL_LOG(tag, LOG_LEVEL_WARNING, fmt, ##arg)
#define PL_LOGI(tag, fmt, arg...) PL_LOG(tag, LOG_LEVEL_INFO, fmt, ##arg)
#define PL_LOGD(tag, fmt, arg...) PL_LOG(tag, LOG_LEVEL_DEBUG, fmt, ##arg)
#define PL_LOGV(tag, fmt, arg...) PL_LOG(tag, LOG_LEVEL_VERBOSE, fmt, ##arg)
#define PL_LOGE_FILE(tag, fmt, arg...) PL_LOG_FILE(LOG_ERR, tag, LOG_LEVEL_ERROR, fmt, ##arg)
#define PL_LOGW_FILE(tag, fmt, arg...) PL_LOG_FILE(LOG_WARNING, tag, LOG_LEVEL_WARNING, fmt, ##arg)
#define PL_LOGI_FILE(tag, fmt, arg...) PL_LOG_FILE(LOG_INFO, tag, LOG_LEVEL_INFO, fmt, ##arg)
#define PL_LOGD_FILE(tag, fmt, arg...) PL_LOG_FILE(LOG_DEBUG, tag, LOG_LEVEL_DEBUG, fmt, ##arg)
#define PL_LOGV_FILE(tag, fmt, arg...) PL_LOG_FILE(LOG_INFO, tag, LOG_LEVEL_VERBOSE, fmt, ##arg)

#endif

#ifdef __cplusplus
}
#endif
#endif // _LOG_H
