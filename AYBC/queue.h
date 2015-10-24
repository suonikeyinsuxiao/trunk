#ifndef __QUEUE_H
#define __QUEUE_H

#include "common.h"


typedef struct {
   U32 m_nRealDataLen;     //实际数据大小 
   U8* m_pcBuffer;		   //数据存储buffer
}AYBCQueueBuffer_S;


typedef struct {
   U32 m_nQueueUnitNum;					//队列的长度，以一个queuebuffer为单位；若为16，表示一个队列右16个buffer
   U32 m_nQueueBufferSize;				//一个queuebuffer的长度
   U32 m_nHeadIndex；					//队列头索引
   U32 m_nTailIndex；   				//队列尾索引
   AYBCQueueBuffer_S* m_psBuffers;						
}AYBCQueue_S;

/*
*创建队列
*nQueueSize(in): 队列的大小
*psQueHandle(int): 队列的句柄
*return 0 success, otherwise -1
*/
int createQueue(int nQueueSize,AYBCQueue_S* psQueHandle);

/*
*销毁队列
*/
void destroyQueue(AYBCQueue_S* psQueHandle);

/*判断队列是否为满
*psQueHandle(in):队列的句柄
*return 1 is full,otherwise 0
*/
int isFull(AYBCQueue_S* psQueHandle);

/*判断队列是否为空
*psQueHandle(in):队列的句柄
*return 1 is empty,otherwise 0
*/
int isEmpty(AYBCQueue_S* psQueHandle);

/*入队
*psQueHandle(in):队列的句柄
*psData(in):入队的数据 
*return 0 success, otherwise -1 is full, -2 is other error
*/
int enqueue(AYBCQueue_S* psQueHandle, AYBCQueueBuffer_S* psData);

/*
*出队
*psQueHandle(in): 队列的句柄
*psData(out):出队的数据 
*return 0 success, otherwise -1 is empty, -2 is other error
*/
int dequeue(AYBCQueue_S* psQueHandle, AYBCQueueBuffer_S* psData);

#endif		//__QUEUE_H
