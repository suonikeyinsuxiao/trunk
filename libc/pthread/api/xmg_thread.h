/***********************************************************************
*   Copyright (C) 2018 junfu0903@aliyun.com All rights reserved.
*   
*   File Name: xmg_thread.h 
*   Brief:  pthread api    
*   Author: frank
*   Email: junfu0903@aliyun.com
*   Version: 1.0
*   Created Time:2018-08-23 22:29:55
*   Blog: http://www.cnblogs.com/black-mamba
*   Github: https://github.com/suonikeyinsuxiao
*   
***********************************************************************/

#ifndef _XMG_THREAD_H
#define _XMG_THREAD_H


#ifdef __cplusplus
extern "C"
{
#endif

#include <pthread.h>

typedef void (XMG_THREAD_ENTRY)(void);
typedef pthread_t XMG_THREAD_PID;
typedef unsigned short UINT16;

/**
 * @brief    xmg_create_thread
 *
 * @param[in]    task_entry
 * @param[in]    stack
 * @param[in]    stacksize
 * @param[out]    p_thread
 *
 * @return
 */
int xmg_create_thread(XMG_THREAD_ENTRY task_entry, UINT16 *stack, UINT16 stacksize, XMG_THREAD_PID *p_thread);

/**
 * @brief    xmg_stop_thread
 *
 * @param[in]    thread
 *
 * @return
 */
int xmg_stop_thread(XMG_THREAD_PID thread);

#ifdef __cplusplus
}
#endif
#endif //_XMG_THREAD_H

