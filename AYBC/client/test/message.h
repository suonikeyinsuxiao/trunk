#ifndef __MESSAGE_H
#define __MESSAGE_H

#include "common.h"

#define AYBC_IP_MAX_LEN (24)
#define AYBC_MSG_MAX_LEN (256)

typedef enum
{
	MSG_START,
	MSG_STOP,
	MSG_SEND,
	MSG_EXIT,
	MSG_BEATHEART
}MessageType_E;

typedef struct _AYBC_Addr
{
	char m_acIp[AYBC_IP_MAX_LEN];	
	U16  m_port;	
}AYBCAddr_S;

typedef struct _AYBC_Msg
{
	AYBCAddr_S 		m_sRemoteAddr;
	MessageType_E 	m_eMsgType;
//	char 			m_acMsgData[AYBC_MSG_MAX_LEN];
//	U32  			m_nMsgDataSize;
}AYBCMsg_S;


//type=1&ip=192.168.1.1&port=123

//success return 0; otherwise -1
//pcMsg(in):message string
//psMsg(out): message stored in psMsg
int decomposeMsg(char* pcMsg, AYBCMsg_S* psMsg);

#endif		//__MESSAGE_H

//消息起始符
//0x98 0x97 0x89 0x79
//消息数据格式
/*
*type=1&ip=192.168.1.1&port=123&datalen=10&data=
*type=1&ip=192.168.1.1&port=123
*/
//typedef void (*pfMsgCallback)(char* pcData);
//void registerMsgCB(pfMsgCallback pCb);


