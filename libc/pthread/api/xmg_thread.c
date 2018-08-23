/***********************************************************************
*   Copyright (C) 2018 junfu0903@aliyun.com All rights reserved.
*   
*   File Name: xmg_thread.c 
*   Brief:      
*   Author: frank
*   Email: junfu0903@aliyun.com
*   Version: 1.0
*   Created Time:2018-08-23 22:31:39
*   Blog: http://www.cnblogs.com/black-mamba
*   Github: https://github.com/suonikeyinsuxiao
*   
***********************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include "xmg_thread.h"

typedef void *(*PTHREAD_ENTRY)(void*);

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
int xmg_create_thread(XMG_THREAD_ENTRY task_entry, UINT16 *stack, UINT16 stacksize, XMG_THREAD_PID *p_thread)
{
	//printf("task_entry=%x\n", task_entry);
    pthread_attr_t thread_attr;

    pthread_attr_init(&thread_attr);

    pthread_attr_setdetachstate(&thread_attr, PTHREAD_CREATE_DETACHED);
    pthread_attr_setstacksize(&thread_attr, 524288);
    
    if (pthread_create(p_thread, &thread_attr, (PTHREAD_ENTRY)task_entry, NULL) < 0 )
    {
        printf("pthread_create failed:%d", errno);
        return -1;
    }
    return 0;
}

/**
 * @brief    xmg_stop_thread
 *
 * @param[in]    thread
 *
 * @return
 */
int xmg_stop_thread(XMG_THREAD_PID thread)
{
    if (pthread_cancel(thread) < 0)
    {
        printf("pthread_cancel failed:%d", errno);
        return -1;
    }
    return 0;
}

