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
	int   m_nMsgDataLen;
}LocalMsg_S;

int main (int argc, char *argv[])
{
	int server_sockfd, client_sockfd;
	int server_len, client_len;
	struct sockaddr_un server_address; /*声明一个UNIX域套接字结构*/
	struct sockaddr_un client_address;
	int i, bytes;
	char ch_send, ch_recv;
	char* pcBuf = NULL;
	LocalMsg_S* psMsgSend = NULL;

	unlink ("server_socket"); /*删除原有server_socket对象*/

	/*创建 socket, 通信协议为AF_UNIX, SCK_STREAM 数据方式*/
	server_sockfd = socket (AF_UNIX, SOCK_STREAM, 0);

	/*配置服务器信息(通信协议)*/
	server_address.sun_family = AF_UNIX;

	/*配置服务器信息(socket 对象)*/
	strcpy (server_address.sun_path, "server_socket");

	/*配置服务器信息(服务器地址长度)*/
	server_len = sizeof (server_address);

	/*绑定 socket 对象*/
	bind (server_sockfd, (struct sockaddr *)&server_address, server_len);

	/*监听网络,队列数为5*/
	listen (server_sockfd, 5);

	printf ("Server is waiting for client connect...\n");

	client_len = sizeof (client_address);

	/*接受客户端请求; 第2个参数用来存储客户端地址; 第3个参数用来存储客户端地址的大小*/
	/*建立(返回)一个到客户端的文件描述符,用以对客户端的读写操作*/
	client_sockfd = accept (server_sockfd, (struct sockaddr *)&server_address, (socklen_t *)&client_len);
	if (client_sockfd == -1) {
		perror ("accept");
		exit (EXIT_FAILURE);
	}

	printf ("The server is waiting for client data...\n");

	psMsgSend = (LocalMsg_S*)malloc(sizeof(LocalMsg_S));
	if (NULL == psMsgSend)
	{
		perror("malloc filed:");	
		return -1;
	}
	memset(psMsgSend, 0, sizeof(LocalMsg_S));

	pcBuf = (char*)malloc(BUFF_MAN_LEN);	
	if (NULL == pcBuf)
	{
		perror("malloc filed:");	
		return -1;
	}
	memset(pcBuf, 0, BUFF_MAN_LEN);

	bytes = read(client_sockfd, pcBuf, BUFF_MAN_LEN);
	printf("recv buf:%s, bytes=%d\n", pcBuf, bytes);

	do
	{
		char* str1 = NULL;
		char* str2 = NULL;

		if (NULL == (str1 = strstr(pcBuf, "msgType=")))
		{
			printf("error message format\n");	
			break;
		}
		//printf("pcBuf:%s\n", pcBuf);
		printf("str1:%s\n", str1);
		str2 = str1 + strlen("msgType="); 
		printf("str2:%s\n", str2);
		str1 = strstr(str1, ",msgData=");
		if (str1-str2 > MSGTYPE_MAX_LEN)
		{
			printf("msg type is too large\n");	
			break;
		}
		memcpy(psMsgSend->m_acMsgType, str2, str1-str2);
		printf("test-msgType:%s\n", psMsgSend->m_acMsgType);

		str2 = str1 + strlen(",msgData=");
		printf("str2:%s\n", str2);
		str1 = strstr(str1, ",msgLen=");
		if (str1-str2 > MSGDATA_MAX_LEN)
		{
			printf("msg data is too large\n");	
			break;
		}
		memcpy(psMsgSend->m_acMsgData, str2, str1-str2);
		printf("test-msgData:%s\n", psMsgSend->m_acMsgData);

		str2 = str1 + strlen(",msgLen=");
		psMsgSend->m_nMsgDataLen = atoi(str2);
		printf("test-msgLen:%d\n", psMsgSend->m_nMsgDataLen);
		if (strlen(psMsgSend->m_acMsgData) != psMsgSend->m_nMsgDataLen)
		{
			printf("error: data size is error");
			break;
		}

	
	}while(0);

	if (NULL != psMsgSend)
		free(psMsgSend);

	if (NULL != pcBuf)
		free(pcBuf);
	//for (i = 0, ch_send = '1'; i < 5; i++, ch_send++) {
	//	if ((bytes = read (client_sockfd, &ch_recv, 1)) == -1) {
	//		perror ("read");
	//		exit (EXIT_FAILURE);
	//	}

	//	printf ("The character receiver from client is %c\n", ch_recv);

	//	sleep (1);

	//	if ((bytes = write (client_sockfd, &ch_send, 1)) == -1) {
	//		perror ("read");
	//		exit (EXIT_FAILURE);
	//	}
	//}

	close (client_sockfd);
	unlink ("server socket");
}

