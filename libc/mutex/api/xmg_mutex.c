/***********************************************************************
*   Copyright (C) 2018 junfu0903@aliyun.com All rights reserved.
*   
*   File Name: xmg_mutex.c 
*   Brief:  xmg means my bias(xiaomage)
*           本系列API是互斥锁和条件变量的综合应用,多线程同步;
*           可以应用到任何需要状态控制,而不是简单的轮询控制
*   Author: frank
*   Email: junfu0903@aliyun.com
*   Version: 1.0
*   Created Time:2018-08-23 22:39:23
*   Blog: http://www.cnblogs.com/black-mamba
*   Github: https://github.com/suonikeyinsuxiao
*   
***********************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "xmg_mutex.h"

/**
 * @brief    xmg_init_mutex 
 *
 * @param[in]    p_mutex
 *
 * @return    
 */
int xmg_init_mutex(xmg_mutex_s *p_mutex)
{
    int status;
    memset(p_mutex, 0, sizeof(xmg_mutex_s));
    status = pthread_mutex_init(&(p_mutex->pthread_mutex), NULL);
	if (status)
    {
        perror("xmg_init_mutex pthread_mutex_init failed Reason:");
        return status;
    }

	status = pthread_cond_init(&(p_mutex->pthread_cond), NULL);
    if (status)
    {
        perror("xmg_init_mutex pthread_cond_init failed Reason:");
        pthread_mutex_destroy(&(p_mutex->pthread_mutex));
        return status;
    }
    p_mutex->n_count = 0;

	return status;
}

/**
 * @brief    xmg_delete_mutex 
 *
 * @param[in]    p_mutex
 *
 * @return    
 */
int xmg_delete_mutex(xmg_mutex_s *p_mutex)
{
	int status;
    int ret = 0;

    status = pthread_mutex_destroy(&(p_mutex->pthread_mutex));
    if (status)
    {
        perror("xmg_delete_mutex pthread_mutex_destroy failure while destroying Reason:");
        ret = -1;
    }
    status = pthread_cond_destroy(&(p_mutex->pthread_cond));
    if (status)
    {
        perror("xmg_delete_mutex pthread_cond_destroy failure while destroying Reason:");
        ret = -1;
    }
    p_mutex->n_count = 0;

    return(ret);
}

/**
 * @brief    xmg_lock_mutex 
 *
 * @param[in]    p_mutex
 *
 * @return    
 */
int xmg_lock_mutex(xmg_mutex_s *p_mutex)
{
	int status = -1;

    /* locking the mutex */
    status = pthread_mutex_lock(&(p_mutex->pthread_mutex));
    if (status)
    {
        perror("xmg_lock_mutex pthread_mutex_lock failure Reason:");
    }
    p_mutex->n_count--;
    if (p_mutex->n_count < 0)
    {
		printf("waitting...\n");
        status = pthread_cond_wait(&(p_mutex->pthread_cond), &(p_mutex->pthread_mutex));
        if (status)
        {
            perror("xmg_lock_mutex pthread_cond_wait failure Reason:");
            pthread_mutex_unlock(&(p_mutex->pthread_mutex));
            return -1;
        }
    }
    status = pthread_mutex_unlock(&(p_mutex->pthread_mutex));
    if (status)
    {
        perror("xmg_lock_mutex pthread_unlock_mutex failure Reason:");
    }
    return status;
}

/**
 * @brief    xmg_unlock_mutex
 *
 * @param[in]    p_mutex
 *
 * @return
 */
int xmg_unlock_mutex(xmg_mutex_s *p_mutex)
{
	int status;

    /* unlocking the mutex */
    status = pthread_mutex_lock(&(p_mutex->pthread_mutex));
    if (status)
    {
        perror("xmg_unlock_mutex pthread_mutex_lock failure Reason:");
        return -1;
    }
    p_mutex->n_count++;
    if (p_mutex->n_count < 1)
	{
		printf("signal...\n");
        status = pthread_cond_signal(&(p_mutex->pthread_cond));
        if (status)
        {
            perror("xmg_unlock_mutex pthread_cond_signal failure Reason:");
            pthread_mutex_unlock(&(p_mutex->pthread_mutex));
            return -1;
        }
    }
    status = pthread_mutex_unlock(&(p_mutex->pthread_mutex));
    if (status)
    {
        perror("xmg_unlock_mutex pthread_mutex_unlock failure Reason:");
    }
    return status;
}

