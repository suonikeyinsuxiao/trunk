/***********************************************************************
*   Copyright (C) 2016 pilot-lab.inc All rights reserved.
*   
*   @file:       output.h
*   @brief:      
*   @author:     Pilot labs
*   @maintainer: frank.fu@pilot-lab.com.cn
*   @version:    1.0
*   @date:       2016-12-20
*   
***********************************************************************/
#ifndef _OUTPUT_H
#define _OUTPUT_H

#include "klist.h"
#include "common.h"

#ifdef __cplusplus
extern "C"
{
#endif

/*output class name*/
#define	OUTPUT_CLASS_WORK_MODE					("work_mode") 
#define	OUTPUT_CLASS_EXECUTE_COMMAND			("execute_command") 
#define OUTPUT_CLASS_MECHANICS					("mechanics")
#define OUTPUT_CLASS_ZIGBEE_CO					("zigbee_co")
#define OUTPUT_CLASS_ZIGBEE_DEV					("zigbee_dev")
#define OUTPUT_CLASS_SDCARD						("sdcard")
#define OUTPUT_CLASS_LED						("led")
#define OUTPUT_CLASS_EXECUTE_CLOUD_FUNC			("exe_cloud_func")
#define OUTPUT_CLASS_AUDIO_REPLY				("audio_reply")

#define OUTPUT_NAME_LEN			(256)
#define OUTPUT_CLASS_NAME_LEN	(64)

//一个output属于[0~n]个ifttt,一个ifttt只能包含一个output。 
//备注： 通过ifttt配置后， input和putput的关系其实是多对多， 即一个input可
//以引发多条output(可并行/顺序执行)， 多个input可以对应一个output。
typedef struct ifttt_output_cell_s
{
	char	m_acOutputName[OUTPUT_NAME_LEN];
	char	m_acClassName[OUTPUT_CLASS_NAME_LEN];
	char	m_acSubClassName[OUTPUT_CLASS_NAME_LEN];
	char*	m_pcCmd;
	int 	m_nValue;
	int		m_nPriority;//1-8, 1 being the highest priority
	int 	m_nEndCondition;
	int 	m_nRef;//count of reference
}IFTTT_OUTPUT_CELL_S;

typedef struct ifttt_output_s
{
	int						m_nCount;//number of cells
	int						m_nResize;//number of resize
	IFTTT_OUTPUT_CELL_S* 	m_psCells;
	struct list_head*		m_psList;
	struct list_head*		m_psFirst;
	struct list_head*		m_psLast;
}IFTTT_OUTPUT_S;

#ifdef __cplusplus
}
#endif
#endif // _OUTPUT_H
