/***********************************************************************
*   Copyright (C) 2018 junfu0903@aliyun.com All rights reserved.
*   
*   File Name: name_list.h 
*   Brief: 基于双向循环链表的name list     
*   		 应用场景:远程服务器的name db与本地name db的同步
*   Author: frank
*   Email: junfu0903@aliyun.com
*   Version: 1.0
*   Created Time:2018-07-08 22:21:20
*   Blog: http://www.cnblogs.com/black-mamba
*   Github: https://github.com/suonikeyinsuxiao
*   
***********************************************************************/

#ifndef _NAME_LIST_H
#define _NAME_LIST_H


#ifdef __cplusplus
extern "C"
{
#endif

#include "klist.h"

#define NAME_LEN (64)

typedef struct _name_node_s
{
	struct list_head m_sList;   /* node */
	char* m_pcName;             /* name */
}NAME_NODE_S;

typedef struct _name_list_s
{
	struct list_head* m_psHead;     /* name list head */
	int m_nNameNum;                 /* number of name in name list*/
}NAME_LIST_S;


/**
 * @brief print_name_list 
 *          打印psNameList中所有的名字
 *
 * @param psNameList
 */
void print_name_list(NAME_LIST_S* psNameList);


/**
 * @brief init_name_list 
 *          初始化psNameList
 *
 * @param psNameList
 * @param psHead
 */
void init_name_list(NAME_LIST_S* psNameList, struct list_head* psHead);


/**
 * @brief add_name 
 *          向psNameList中添加新的名字
 *
 * @param psNameList
 * @param pcName
 * @param nNameLen
 *
 * @return 0,表示添加失败;1,表示添加成功
 */
int add_name(NAME_LIST_S* psNameList, char* pcName, int nNameLen);

/**
 * @brief del_name 
 *          删除psNameList中pcName指定的名字
 *
 * @param psNameList
 * @param pcName
 * @param nNameLen
 */
void del_name(NAME_LIST_S* psNameList, char* pcName, int nNameLen);

/**
 * @brief    check_name_list 
 * 			 
 *			  比较psLocal_name_list和psRemote_name_list中的name(不区分大小写),将psLocal_name_list中独有的name放入psLocal_no_sync_name_list中,将psRemote_name_list中独有的name放入psRemote_redundant_name_list中
 *
 * @param[in]    psLocal_name_list 	
 * @param[in]    psRemote_name_list
 * @param[in]    psLocal_no_sync_name_list
 * @param[in]    psRemote_redundant_name_list
 */
void check_name_list(NAME_LIST_S* psLocal_name_list, NAME_LIST_S* psRemote_name_list, NAME_LIST_S* psLocal_no_sync_name_list, NAME_LIST_S* psRemote_redundant_name_list);


/**
 * @brief del_name_list 
 *          删除name list
 *
 * @param psNameList
 */
void del_name_list(NAME_LIST_S* psNameList);


/**
 * @brief is_name_exist 
 *          判断psNameList中是否存在pcName
 *
 * @param psNameList
 * @param pcName
 *
 * @return 
 */
int is_name_exist(NAME_LIST_S* psNameList, char* pcName);


/**
 * @brief get_size_of_name_list 
 *          获取psNameList中名字的个数
 *
 * @param psNameList
 *
 * @return name list中名字的个数
 */
int get_size_of_name_list(NAME_LIST_S* psNameList);

#ifdef __cplusplus
}
#endif
#endif //_NAME_LIST_H
