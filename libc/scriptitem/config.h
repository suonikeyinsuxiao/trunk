/***********************************************************************
*   Copyright (C) 2016 pilot-lab.inc All rights reserved.
*   
*   @file:       config.h
*   @brief:      
*   @author:     Pilot labs
*   @maintainer: frank.fu@pilot-lab.com.cn
*   @version:    1.0
*   @date:       2016-08-18
*   
***********************************************************************/
#ifndef _CONFIG_H
#define _CONFIG_H
#ifdef __cplusplus
extern "C"
{
#endif

#define ITEM_NAME_SIZE_MAX    (32)

#define CONFIG_MALLOC(size)			malloc((size_t)size)
#define CONFIG_FREE(addr)			free((void*)addr)


#define ITEM_TYPE_TO_STR(type)  ((CONFIG_ITEM_VALUE_TYPE_INT == (type)) ?  "int" : \
		((CONFIG_ITEM_VALUE_TYPE_STR == (type))  ?  "string" : \
		 "invalid"))


#pragma pack(1)
typedef struct
{
	char name[32];
	int  sub_cnt;
	int  offset;
} config_origin_main_key_t;
#pragma pack()

/*
 * define origin sub key data structure in cript buffer
 * @name: sub key name, defined left of "="
 * @offset: sub key value position, in dword
 * @type: type of sub key, int / string / gpio
 * @cnt:  length of the value area, in dword
 */
#pragma pack(1)
typedef struct
{
	char name[32];
	int  offset;
	struct {
		u32 cnt : 16;
		u32 type: 16;
	}pattern;
} config_origin_sub_key_t;
#pragma pack()

/*
 * define origin header of the script in cript buffer
 * @main_cnt: count of main keys
 * @version: script version
 * @main_key: fist main key
 */
#pragma pack(1)
	typedef struct
	{
		int  main_cnt;
		int  version[3];
		script_origin_main_key_t    main_key;
	} script_origin_head_t;
#pragma pack()

typedef enum 
{          
	CONFIG_ITEM_VALUE_TYPE_INVALID = 0,
	CONFIG_ITEM_VALUE_TYPE_INT,
	CONFIG_ITEM_VALUE_TYPE_STR,
}CONFIG_ITEM_VALUE_TYPE_E;

typedef union 
{
	int                 m_nVal;
	char*				m_pcStr;
}CONFIG_ITEM_U;

typedef enum 
{
	CONFIG_PARSER_VALUE_TYPE_INVALID = 0,
	CONFIG_PARSER_VALUE_TYPE_SINGLE_WORD,
	CONFIG_PARSER_VALUE_TYPE_STRING,
	CONFIG_PARSER_VALUE_TYPE_MULTI_WORD
}CONFIG_PARSER_VALUE_TYPE_E;

typedef struct 
{
	char 						m_acName[ITEM_NAME_SIZE_MAX];
	CONFIG_ITEM_U*				m_puValue;
	CONFIG_ITEM_VALUE_TYPE_E    m_eType;
	int                         m_nHash;
	void*                       m_pNext;
}CONFIG_SUB_KEY_S;

typedef struct 
{
	char 				m_acName[ITEM_NAME_SIZE_MAX];
	CONFIG_SUB_KEY_S*	m_psSubKey;
	CONFIG_ITEM_U*		m_puSubKeyVal;
	int                 m_nHash;
	void*				m_pNext;
}CONFIG_MAIN_KEY_S;

#ifdef __cplusplus
}
#endif
#endif // _CONFIG_H
