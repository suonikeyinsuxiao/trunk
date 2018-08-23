/***********************************************************************
*   Copyright (C) 2018 junfu0903@aliyun.com All rights reserved.
*   
*   File Name: xmg_mutex.h 
*   Brief:  xmg means my bias(xiaomage)
*           本系列API是互斥锁和条件变量的综合应用,多线程同步;
*           可以应用到任何需要状态控制,而不是简单的轮询控制
*   Author: frank
*   Email: junfu0903@aliyun.com
*   Version: 1.0
*   Created Time:2018-08-23 22:38:52
*   Blog: http://www.cnblogs.com/black-mamba
*   Github: https://github.com/suonikeyinsuxiao
*   
***********************************************************************/

#ifndef _XMG_MUTEX_H
#define _XMG_MUTEX_H

#include <pthread.h>

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct
{
    pthread_mutex_t pthread_mutex;
    pthread_cond_t  pthread_cond;
    int             n_count;
}xmg_mutex_s;

/**
 * @brief    xmg_init_mutex
 *
 * @param[in]    p_mutex
 *
 * @return
 */
int xmg_init_mutex(xmg_mutex_s *p_mutex);

/**
 * @brief    xmg_delete_mutex
 *
 * @param[in]    p_mutex
 *
 * @return
 */
int xmg_delete_mutex(xmg_mutex_s *p_mutex);

/**
 * @brief    xmg_lock_mutex
 *
 * @param[in]    p_mutex
 *
 * @return
 */
int xmg_lock_mutex(xmg_mutex_s *p_mutex);

/**
 * @brief    xmg_unlock_mutex
 *
 * @param[in]    p_mutex
 *
 * @return
 */
int xmg_unlock_mutex(xmg_mutex_s *p_mutex);


#ifdef __cplusplus
}
#endif
#endif //_XMG_MUTEX_H

