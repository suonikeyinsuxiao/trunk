/***********************************************************************
*   Copyright (C) 2016 pilot-lab.inc All rights reserved.
*   
*   @file:       ifttt.h
*   @brief:      
*   @author:     Pilot labs
*   @maintainer: frank.fu@pilot-lab.com.cn
*   @version:    1.0
*   @date:       2016-12-20
*   
***********************************************************************/
#ifndef _IFTTT_H
#define _IFTTT_H

#include "klist.h"
#include "common.h"
#include "status.h"
#include "input.h"
#include "output.h"

#ifdef __cplusplus
extern "C"
{
#endif

#define IFTTT_NAME_LEN			(256)
#define IFTTT_CLASS_NAME_LEN	(64)

//一个input属于[0~n]个ifttt,一个ifttt只能包含一个input
//一个output属于[0~n]个ifttt,一个ifttt只能包含一个output。 
typedef struct ifttt_entry_s
{
	char 					m_acIftttName[IFTTT_NAME_LEN];
	char 					m_acClassName[IFTTT_CLASS_NAME_LEN];
	char 					m_acSubClassName[IFTTT_CLASS_NAME_LEN];
	int						m_nId;
	int						m_nRef;//count of reference
	IFTTT_CONDITION_S*		m_psCondition;
	IFTTT_INPUT_S*			m_psInput;
	IFTTT_OUTPUT_S*			m_psOutput;
}IFTTT_ENTRY_S;

//一个scene可包含[1~n] 个ifttt, 一个ifttt可以属于[0~n]个scene
typedef struct ifttt_scene_s
{
	int						m_nCount;//number of ifttt entries
	int						m_nIndex;//index of ifttt entry
	IFTTT_ENTRY_S*			m_psIfttt;
	struct list_head*		m_psList;
}IFTTT_SCENE_S;

#ifdef __cplusplus
}
#endif
#endif // _IFTTT_H
