#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <sys/un.h>
#include <unistd.h>
#include <stdlib.h>

#define BUFF_MAN_LEN (1280)

#define MSGTYPE_MAX_LEN (16)
#define MSGDATA_MAX_LEN (1024)

//msgType=xxx,msgData=xxx,msgLen=xxx
typedef struct LocalMsg
{
	char m_acMsgType[MSGTYPE_MAX_LEN+1];
	char m_acMsgData[MSGDATA_MAX_LEN+1];
	int  m_nMsgDataLen;
}LocalMsg_S;

int main (int argc, char *argv[])
{
	struct sockaddr_un address;
	int sockfd;
	int len;
	int i, bytes;
	int result;
	char ch_recv, ch_send;
	char* pcBuf = NULL;
	int nWriteLen = 0;
	LocalMsg_S* psMsgSend = NULL;

	/*创建socket,AF_UNIX通信协议,SOCK_STREAM数据方式*/
	if ((sockfd = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
		perror ("socket");
		exit (EXIT_FAILURE);
	}

	address.sun_family = AF_UNIX;
	strcpy (address.sun_path, "server_socket");
	len = sizeof (address);

	/*向服务器发送连接请求*/
	result = connect (sockfd, (struct sockaddr *)&address, len);
	if (result == -1) {
		printf ("ensure the server is up\n");
		perror ("connect");
		close (sockfd);
		exit (EXIT_FAILURE);
	}

	psMsgSend = (LocalMsg_S*)malloc(sizeof(LocalMsg_S));
	if (NULL == psMsgSend)
	{
		perror("malloc filed:");	
		close (sockfd);
		return -1;
	}

	if (strlen("facedetect") > MSGTYPE_MAX_LEN)
	{
		if (NULL != psMsgSend)
			free(psMsgSend);
	
		close (sockfd);
	}
	else
		memcpy(psMsgSend->m_acMsgType, "facedetet", strlen("facedetect"));
	memcpy(psMsgSend->m_acMsgData, "/home/fujun/face.jpg", strlen("/home/fujun/face.jpg"));
	psMsgSend->m_nMsgDataLen = strlen("/home/fujun/face.jpg");

	pcBuf = (char*)malloc(BUFF_MAN_LEN);	
	if (NULL == psMsgSend)
	{
		perror("malloc filed:");	
		return -1;
	}
	memset(pcBuf, 0, BUFF_MAN_LEN);

	snprintf(pcBuf, BUFF_MAN_LEN-1, "send msgType=%s,msgData=%s,msgLen=%d", psMsgSend->m_acMsgType, psMsgSend->m_acMsgData, psMsgSend->m_nMsgDataLen);

	nWriteLen = write(sockfd, pcBuf, strlen(pcBuf));
	printf("write len:%d\n", nWriteLen);

	if (NULL != psMsgSend)
		free(psMsgSend);

	if (NULL != pcBuf)
		free(pcBuf);

	//for (i = 0, ch_send = 'A'; i < 5; i++, ch_send++) {
	//	if ((bytes = write(sockfd, &ch_send, 1)) == -1) { /*发消息给服务器*/
	//		perror ("write");
	//		exit (EXIT_FAILURE);
	//	}

	//	sleep (2); /*休息二秒钟再发一次*/

	//	if ((bytes = read (sockfd, &ch_recv, 1)) == -1) { /*接收消息*/
	//		perror ("read");
	//		exit (EXIT_FAILURE);
	//	}

	//	printf ("receive from server data is %c\n", ch_recv);
	//}
	close (sockfd);

	return (0);
}
