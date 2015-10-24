#ifndef __SHM_H
#define __SHM_H

#include<stdio.h>
#include<string.h>
#include<errno.h>
#include<sys/ipc.h>
#include<sys/shm.h>

#define SHMSIZE 1024	

int createShm(int key, int size);
void statShm(int nShmId, struct shmid_ds* pShmDS);
int getShm(int key);
void rmShm(int nShmId);


#endif		//__SHM_H
