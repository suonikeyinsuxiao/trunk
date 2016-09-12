
#include"mutexSem.h"

//mutex semaphore init
int initSem(int key, int semValue)
{
	int nSemId;
	nSemId = semget(key, 1, IPC_CREAT|IPC_EXCL|0777);
	if (-1 == nSemId)
	{
		fprintf(stderr, "create mutex semaphore failed:%s!\n", strerror(errno));	
		return -1;
	}

	if (0 != semctl(nSemId, 0, SETVAL, semValue))
	{
		fprintf(stderr, "set sem value failed:%s!\n", strerror(errno));	
		return -1;
	}

	return nSemId;
}

//open a exsit mutex semaphore
int openSem(int key)
{
	int nSemId;
	nSemId = semget(key, 0, 0);
	if (-1 == nSemId)
	{
		fprintf(stderr, "open sem(key=%#x) failed:%s\n", key, strerror(errno));	
		return -1;
	}
	return nSemId;
}

//print mutex semaphore information
void statSem(int semId, struct semid_ds* pSemDS)
{
	if (0 != semctl(semId, 0, IPC_STAT, pSemDS))
	{
		fprintf(stderr, "stat sem failed:%s!\n", strerror(errno));	
		return ;
	}
}

//p Operation
void pSem(int semId)
{
	struct sembuf semBuf;
	semBuf.sem_num = 0;//编号
	semBuf.sem_op = -1;//加锁
	semBuf.sem_flg = SEM_UNDO;
	
	if (-1 == semop(semId, &semBuf, 1))
	{
		fprintf(stderr, "P operation to sem failed:%s!\n", strerror(errno));	
		return ;
	}
}

//v Operation
void vSem(int semId)
{
	struct sembuf semBuf;
	semBuf.sem_num = 0;
	semBuf.sem_op = 1;
	semBuf.sem_flg = SEM_UNDO;
	
	if (-1 == semop(semId, &semBuf, 1))
	{
		fprintf(stderr, "v operation to sem failed:%s!\n", strerror(errno));	
		return ;
	}
}

// remove mutex semaphore
void rmSem(int semId)
{
	if (0 != semctl(semId, 0, IPC_RMID))
	{
		fprintf(stderr, "stat sem failed:%s!\n", strerror(errno));	
		return ;
	}
}

