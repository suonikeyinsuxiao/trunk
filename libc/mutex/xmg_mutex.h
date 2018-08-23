/***********************************************************************
*   Copyright (C) 2018 pilot-lab.inc All rights reserved.
*   
*   @file:       xmg_mutex.h
*   @brief:      
*   @author:     Pilot labs
*   @maintainer: frank.fu@pilot-lab.com.cn
*   @version:    1.0
*   @date:       2018-08-22
*   
***********************************************************************/
#ifndef _XMG_MUTEX_H
#define _XMG_MUTEX_H
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
#endif // _XMG_MUTEX_H
