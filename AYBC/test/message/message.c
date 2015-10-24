#include "message.h"

int decomposeMsg(char* pcMsg, AYBCMsg_S* psMsg)
{
	if (NULL == pcMsg || NULL == psMsg)
	{
		printf("decomposeMsg error, NULL == pcMsg || NULL == psMsg\n");
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
