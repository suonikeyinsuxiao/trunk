/***********************************************************************
*   Copyright (C) 2018 junfu0903@aliyun.com All rights reserved.
*   
*   File Name: name_list.c 
*   Brief:      
*   Author: frank
*   Email: junfu0903@aliyun.com
*   Version: 1.0
*   Created Time:2018-07-08 22:26:07
*   Blog: http://www.cnblogs.com/black-mamba
*   Github: https://github.com/suonikeyinsuxiao
*   
***********************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mcheck.h>
#include "name_list.h"

LIST_HEAD(local_nl_header);				/*链表头*/
LIST_HEAD(remote_nl_header);			/*链表头*/
LIST_HEAD(local_no_sync_nl_header);		/*链表头*/
LIST_HEAD(remote_redundant_nl_header);	/*链表头*/

static int swap_int(int *a, int *b)
{
	*a = *a^*b;
	*b = *a^*b;
	*a = *a^*b;
	return 0;
}

//比较psLocal_name_list和psRemote_name_list中的name(不区分大小写),将psLocal_name_list中独有的name放入psLocal_no_sync_name_list中,将psRemote_name_list中独有的name放入psRemote_redundant_name_list中
void check_name_list(NAME_LIST_S* psLocal_name_list, NAME_LIST_S* psRemote_name_list, NAME_LIST_S* psLocal_no_sync_name_list, NAME_LIST_S* psRemote_redundant_name_list)
{
	NAME_NODE_S* pos_local = NULL;
	NAME_NODE_S* pos_local2 = NULL;
	NAME_NODE_S* pos_remote = NULL;
	NAME_NODE_S* pos_remote2 = NULL;
	struct list_head*  tmp = NULL;
	struct list_head*  tmp2 = NULL;
	struct list_head*  tmp3 = NULL;
	struct list_head*  tmp4 = NULL;

	//以loal name list 为参考,找出local有,remote 没有的name,然后存入psLocal_no_sync_nl_header
	list_for_each_entry_safe(pos_local, pos_local2, psLocal_name_list->m_psHead, m_sList)
	{
		list_for_each_entry_safe(pos_remote, pos_remote2, psRemote_name_list->m_psHead, m_sList)
		{
			if (strncasecmp(pos_local->m_pcName, pos_remote->m_pcName, strlen(pos_local->m_pcName)) == 0)
			{
				list_move_tail(&pos_local->m_sList, psLocal_no_sync_name_list->m_psHead);
				list_move_tail(&pos_remote->m_sList, psRemote_redundant_name_list->m_psHead);
				psLocal_no_sync_name_list->m_nNameNum++;
				psRemote_redundant_name_list->m_nNameNum++;
				psLocal_name_list->m_nNameNum--;
				psRemote_name_list->m_nNameNum--;
				break;
			}
		}
	}

	//交换local_name_list和local_no_sync_name_list的头节点
	//交换remote_name_list和remote_redundant_name_list的头节点
	tmp = psLocal_no_sync_name_list->m_psHead->next;
	tmp2 = psLocal_no_sync_name_list->m_psHead->prev;

	tmp3 = psRemote_redundant_name_list->m_psHead->next;
	tmp4 = psRemote_redundant_name_list->m_psHead->prev;

	psLocal_no_sync_name_list->m_psHead->next->prev = psLocal_name_list->m_psHead;
	psLocal_no_sync_name_list->m_psHead->prev->next = psLocal_name_list->m_psHead;
	psLocal_no_sync_name_list->m_psHead->next = psLocal_name_list->m_psHead->next;
	psLocal_no_sync_name_list->m_psHead->prev = psLocal_name_list->m_psHead->prev;

	psLocal_name_list->m_psHead->next->prev = psLocal_no_sync_name_list->m_psHead;
	psLocal_name_list->m_psHead->prev->next = psLocal_no_sync_name_list->m_psHead;
	psLocal_name_list->m_psHead->next = tmp;
	psLocal_name_list->m_psHead->prev = tmp2;
	swap_int(&psLocal_name_list->m_nNameNum, &psLocal_no_sync_name_list->m_nNameNum);
	
	psRemote_redundant_name_list->m_psHead->next->prev = psRemote_name_list->m_psHead;
	psRemote_redundant_name_list->m_psHead->prev->next = psRemote_name_list->m_psHead;
	psRemote_redundant_name_list->m_psHead->next = psRemote_name_list->m_psHead->next;
	psRemote_redundant_name_list->m_psHead->prev = psRemote_name_list->m_psHead->prev;

	psRemote_name_list->m_psHead->next->prev = psRemote_redundant_name_list->m_psHead;
	psRemote_name_list->m_psHead->prev->next = psRemote_redundant_name_list->m_psHead; 
	psRemote_name_list->m_psHead->next = tmp3;
	psRemote_name_list->m_psHead->prev = tmp4;
	swap_int(&psRemote_name_list->m_nNameNum, &psRemote_redundant_name_list->m_nNameNum);
}

void print_name_list(NAME_LIST_S* psNameList)
{
	NAME_NODE_S* pos = NULL;
	//list_for_each_entry(pos, psHeader, m_sList)
	list_for_each_entry(pos, psNameList->m_psHead, m_sList)
	{
		printf("name:%s\n", pos->m_pcName);
	}
	printf("name num:%d\n", psNameList->m_nNameNum);
}

//向链表中新增一个名字
int add_name(NAME_LIST_S* psNameList, char* pcName, int nNameLen)
{
	int nRet = 0;
	NAME_NODE_S* psNew = NULL;;

	if (!is_name_exist(psNameList, pcName))
	{
		printf("%s is not exist\n", pcName);
		if (nNameLen < NAME_LEN)
		{
			psNew = malloc(sizeof(NAME_NODE_S));
			psNew->m_pcName = (char*)malloc(NAME_LEN);
			memset(psNew->m_pcName, '\0', NAME_LEN);
			memcpy(psNew->m_pcName, pcName, nNameLen);
			list_add_tail(&psNew->m_sList, psNameList->m_psHead);
			psNameList->m_nNameNum++;
			nRet =1;
		}
	}
	else
	{
		printf("%s is exist\n", pcName);
		nRet = 0;
	}

	return nRet;
}

//删除pcName指定的名字
void del_name(NAME_LIST_S* psNameList, char* pcName, int nNameLen)
{
	NAME_NODE_S* pos = NULL;
	list_for_each_entry(pos, psNameList->m_psHead, m_sList)
	{
		if (strncmp(pcName, pos->m_pcName, nNameLen) == 0)
		{
			list_del(&pos->m_sList);
			free(pos->m_pcName);
			free(pos);
			break;
		}
	}
}

//清空名字链表
void del_name_list(NAME_LIST_S* psNameList)
{
	NAME_NODE_S* pos = NULL;
	NAME_NODE_S* tmp = NULL;

	list_for_each_entry_safe(pos, tmp, psNameList->m_psHead, m_sList)
	{
		list_del(&pos->m_sList);
		free(pos->m_pcName);
		free(pos);
	}
}

void init_name_list(NAME_LIST_S* psNameList, struct list_head* psHead)
{
	psNameList->m_psHead = psHead;	   
	psNameList->m_nNameNum = 0;	   
}

int is_name_exist(NAME_LIST_S* psNameList, char* pcName)
{
	int nRet = 0;
	NAME_NODE_S* pos = NULL;
	NAME_NODE_S* tmp = NULL;

	list_for_each_entry_safe(pos, tmp, psNameList->m_psHead, m_sList)
	{
		if (strncasecmp(pcName, pos->m_pcName, strlen(pcName)) == 0)
		{
			nRet = 1;
			break;
		}
	}
	return nRet;
}

int get_size_of_name_list(NAME_LIST_S* psNameList)
{
	return psNameList->m_nNameNum;
}

int main(int argc, char** argv)
{
	NAME_LIST_S local_name_list;
	NAME_LIST_S remote_name_list;
	NAME_LIST_S local_no_sync_name_list;
	NAME_LIST_S remote_redundant_name_list;

	init_name_list(&local_name_list, &local_nl_header);
	init_name_list(&remote_name_list, &remote_nl_header);
	init_name_list(&local_no_sync_name_list, &local_no_sync_nl_header);
	init_name_list(&remote_redundant_name_list, &remote_redundant_nl_header);

	add_name(&local_name_list, "Frank", strlen("frank"));
	add_name(&local_name_list, "tim", strlen("Tim"));
	add_name(&local_name_list, "lass", strlen("lass"));
	add_name(&local_name_list, "Mike", strlen("Mike"));
	add_name(&local_name_list, "Sam", strlen("Sam"));
	add_name(&local_name_list, "Sim", strlen("Sim"));

	printf("local name list:\n");
	print_name_list(&local_name_list);

	add_name(&remote_name_list, "frank", strlen("frank"));
	add_name(&remote_name_list, "Tim", strlen("Tim"));
	add_name(&remote_name_list, "lass", strlen("lass"));
	add_name(&remote_name_list, "blue", strlen("blue"));

	printf("\nremote name list:\n");
	print_name_list(&remote_name_list);

#if 1
	if (is_name_exist(&local_name_list, "frank"))
		printf("frank is exist\n");
	else
		printf("frank is not exist\n");
#endif

	check_name_list(&local_name_list, &remote_name_list, &local_no_sync_name_list, &remote_redundant_name_list);

	printf("\n\n\nlocal_same:\n");
	print_name_list(&local_name_list);
	printf("\nremote_same:\n");
	print_name_list(&remote_name_list);
	printf("\nlocal_no_sync:\n");
	print_name_list(&local_no_sync_name_list);
	printf("\nremote_redundant:\n");
	print_name_list(&remote_redundant_name_list);


	del_name_list(&local_name_list);
	del_name_list(&remote_name_list);
	del_name_list(&local_no_sync_name_list);
	del_name_list(&remote_redundant_name_list);

	return 0;

}

