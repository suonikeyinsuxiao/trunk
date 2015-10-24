#ifndef __SENDBROADCAST_H
#define __SENDBROADCAST_H

#include<stdio.h>
#include<string.h>
#include<errno.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<fcntl.h>
#include<netinet/in.h>
//#include<arpa/inet.h>


#define BROADCASTPORT (12345)

//∑µªÿsocket fd
int initBroadSock();

//pcDstIP:π„≤•µÿ÷∑
int sendBroadData(int nSockFd, char* pcDstIP, char* pcData, int nDataLen);


void closeBroadSock(int nSockFd);

#endif
