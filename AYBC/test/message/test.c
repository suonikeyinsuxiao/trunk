#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define AYBC_IP_MAX_LEN (24)
#define AYBC_MSG_MAX_LEN (256)

typedef enum
{
	MSG_START,
	MSG_STOP,
	MSG_SEND,
	MSG_BEATHEART
}MessageType_E;

typedef struct _AYBC_Addr
{
	char m_acIp[AYBC_IP_MAX_LEN];	
	unsigned short  m_port;	
}AYBCAddr_S;

typedef struct _AYBC_Msg
{
	AYBCAddr_S 		m_sRemoteAddr;
	MessageType_E 	m_eMsgType;
//	char 			m_acMsgData[AYBC_MSG_MAX_LEN];
//	U32  			m_nMsgDataSize;
}AYBCMsg_S;

//消息起始符
//0x98 0x97 0x89 0x79
//消息数据格式
/*
*type=1&ip=192.168.1.1&port=123&datalen=10&data=
*type=1&ip=192.168.1.1&port=123
*/
//typedef void (*pfMsgCallback)(char* pcData);
//void registerMsgCB(pfMsgCallback pCb);

int decomposeMsg(char* pcMsg, AYBCMsg_S* psMsg)
{
	if (NULL == pcMsg || NULL == psMsg)
	{
		printf("decomposeMsg, NULL == pcMsg || NULL == psMsg\n");
		return -1;
	}

	char* pcStr1 = pcMsg;
	char* pcStr2 = NULL;
	char acType[8] ={0};
	char acPort[8] ={0};
	//printf("msg:%s\n", pcMsg);

	pcStr1 += 5;
	pcStr2 = strstr(pcMsg, "&ip=");
	memcpy(acType, pcStr1, pcStr2-pcStr1);
	psMsg->m_eMsgType = atoi(acType);
	//printf("msgtype:%d\n", psMsg->m_eMsgType);

	pcStr1 = strstr(pcMsg, "&ip=");
	pcStr2 = strstr(pcMsg, "&port=");
	pcStr1 += 4;
	memcpy(psMsg->m_sRemoteAddr.m_acIp, pcStr1, pcStr2-pcStr1);
	//printf("psMsg->m_sRemoteAddr.m_acIp=%s\n",psMsg->m_sRemoteAddr.m_acIp);


	pcStr1 = strstr(pcMsg, "&port=");
	pcStr2 = strstr(pcMsg, "#");
	pcStr1 += 6;
	memcpy(acPort, pcStr1, pcStr2-pcStr1);
	psMsg->m_sRemoteAddr.m_port = atoi(acPort);
	//printf("psMsg->m_sRemoteAddr.m_port=%d\n",psMsg->m_sRemoteAddr.m_port);

	return 0;
}
int main(void)
{
	char acSendMsg[AYBC_MSG_MAX_LEN] = {0};
	sprintf(acSendMsg,"type=%d&ip=%s&port=%u#",100,"192.168.1.1",1234);

	AYBCMsg_S msg;
	memset(&msg, 0, sizeof(AYBCMsg_S));

	decomposeMsg(acSendMsg,&msg);
	printf("ip=%s,port=%d,type=%d\n", msg.m_sRemoteAddr.m_acIp,msg.m_sRemoteAddr.m_port,msg.m_eMsgType);
	
	//int i =10;
	//char acType[16] = {0};
	//int nType =0;

	//sprintf(acType, "%d", i);

	//memcpy(&nType, acType, 4);
	//printf("nType=%d\n",nType);
	//printf("type0:%c\n",acType[0]);
	//printf("type1:%c\n",acType[1]);
	//printf("atoi=%d\n", atoi(acType));
	
	return 0;
}
