#ifndef __RECVBROADCAST_H
#define __RECVBROADCAST_H

#include<stdio.h>
#include<string.h>
#include<errno.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<fcntl.h>
#include<netinet/in.h>

#define BRAODCASTPORT (12345)

int initSock();

int recvData(int nSockFd, char* pcDataBuf, int nDataBufLen, struct sockaddr* psSrcAddr); 

void closeSock(int nSockFd);

#endif
