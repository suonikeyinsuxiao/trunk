#ifndef __SEM_H
#define __SEM_H

#include<stdio.h>
#include<string.h>
#include<errno.h>
#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/sem.h>

//mutex semaphore init
int initSem(int key, int semValue);

//print mutex semaphore information
void statSem(int semId, struct semid_ds* pSemDS);

//open a exit mutex semaphore
int openSem(int key);

//p Operation
void pSem(int semId);

//v Operation
void vSem(int semId);

// remove mutex semaphore
void rmSem(int semId);

#endif		//__SEM_H

