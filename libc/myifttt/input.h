/***********************************************************************
*   Copyright (C) 2016 pilot-lab.inc All rights reserved.
*   
*   @file:       input.h
*   @brief:      
*   @author:     Pilot labs
*   @maintainer: frank.fu@pilot-lab.com.cn
*   @version:    1.0
*   @date:       2016-12-20
*   
***********************************************************************/
#ifndef _INPUT_H
#define _INPUT_H

#include "klist.h"
#include "status.h"
#include "common.h"

#ifdef __cplusplus
extern "C"
{
#endif

#define INPUT_NAME_LEN			(256)
#define INPUT_CLASS_NAME_LEN	(64)


//一个condition必须含有一个已经存在的status， 一个status可以属于[0~n]个condition
typedef struct ifttt_condition_s
{
	int 					m_nCondition;
	int						m_nCount;
	int						m_nIndex;
	IFTTT_STATUS_S*			m_psSatus;
	struct list_head*		m_psList;
}IFTTT_CONDITION_S;

//一个input属于[0~n]个ifttt
typedef struct ifttt_input_cell_s
{
	int 	m_nInputId;//需要知道ID的分类和范围
	char 	m_acClassName[INPUT_CLASS_NAME_LEN];
	char 	m_acSubClassName[INPUT_CLASS_NAME_LEN];
	int 	m_nTrigger;//static_or_dynamic: (external trigger or system status trigger)
	int		m_nPriority;//1-8, 1 being the highest priority
	int		m_nRef;//count of reference
	//char 	m_acInputName[INPUT_NAME_LEN];//暂不使用
}IFTTT_INPUT_CELL_S;

//一个input可以包含[1~n]个condition， 一个condition可以属于[0~n]个input
typedef struct ifttt_input_s
{
	int					m_nCount;//number of cells
	IFTTT_INPUT_CELL_S* m_psCells;
	int					m_nResize;//number of resize
	struct list_head*	m_psList;
	struct list_head*	m_psFirst;
	struct list_head*	m_psLast;
}IFTTT_INPUT_S;

#ifdef __cplusplus
}
#endif
#endif // _INPUT_H
