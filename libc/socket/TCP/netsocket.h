#ifndef __NETSOCKET_H
#define __NETSOCKET_H

#include "common.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define CLIENT_PORT 12270
#define SERVER_PORT 12280
#define SERVER_UDP_PORT 12281
#define CLIENT_UDP_PORT 12271

typedef enum _NetSocketType
{
	NET_SOCKET_UDP,
	NET_SOCKET_TCP
}NetSocketType_E;

//eSocketType
//success return socket fd;otherwise -1
int netSocketCreate(NetSocketType_E eSocketType);

void netSocketDestroy(int* pnSocketFd);

//success return 0; otherwise -1
int netSocketBind(int nSocketFd, int nPort);

//success return 0; otherwise -1
int netSocketListen(int nSocketFd, int nMaxConnectNum);

//success return 0; otherwise -1
//support pcIp = NULL
int netSocketConnect(int nSocketFd, char* pcIp, int nPort);

//success return accepted fd(>0); otherwise -1
//*nTimeout_ms(in):nTimeout_ms=0 no timeout
int netSocketAccept(int nSocketFd, struct sockaddr* psPeer, U32 nTimeout_ms);

//success return 0; otherwise -1
int netSetsockopt(int nSocketFd, int nLevel, int nOptName, const void* pOptVal, socklen_t nOptLen);

//success return 0; otherwise -1
int netGetsockopt(int nSocketFd, int nLevel, int nOptName, void* pOptVal, socklen_t* pnOptLen);


/*success return 0; otherwise -1
*nSocketFd(in): socket descriptor
*pcBuf(out):recieve data stored in pcBuf
*nBufSize(in): buffer size
*pnRecvLen(out): realsize of recieved
*nTimeout_ms(in):nTimeout_ms=0 no timeout
*/
int netSocketRecv(int nSocketFd, char* pcBuf, int nBufSize, int* pnRecvLen, U32 nTimeout_ms);

/*success return 0; otherwise -1
*nSocketFd(in): socket descriptor
*pcBuf(out):recieve data stored in pcBuf
*nBufSize(in): buffer size
*pcIp(out): peer ip
*pnPort(out):peer port 
*pnRecvLen(out): realsize of recieved
*nTimeout_ms(in):nTimeout_ms=0 no timeout
*/
int netSocketRecvfrom(int nSocketFd, char* pcBuf, int nBufSize, char* pcIp, int* pnPort, int* pnRecvLen, U32 nTimeout_ms);

/*
*success return 0; otherwise -1
*nSocketFd(in): socket descriptor
*pcBuf(in):send data stored in pcBuf
*nBufSize(in): buffer size
*pnSendLen(out): realsize of send
*nTimeout_ms(in):nTimeout_ms=0 no timeout
*/
int netSocketSend(int nSocketFd, char *pcBuf, int nBufSize, int *pnSendLen, U32 nTimeout_ms);

/*
*success return 0; otherwise -1
*nSocketFd(in): socket descriptor
*pcBuf(in):send data stored in pcBuf
*nBufSize(in): buffer size
*pcIp(in): peer ip,pcIp==NULL <-->INADDR_ANY
*pnPort(in):peer port 
*pnSendLen(out): realsize of send
*nTimeout_ms(in):nTimeout_ms=0 no timeout
*/
int netSocketSendto(int nSocketFd, char *pcBuf, int nBufSize, int* pnSenLen, char* pcIp, int nPort, U32 nTimeout_ms);


#endif	//__NETSOCKET_H
