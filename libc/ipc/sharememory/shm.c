#include"shm.h"

int getShm(int key)
{
	int nShmId;
	nShmId = shmget(key, 0, 0);
	if (-1 == nShmId)
	{
		fprintf(stderr,"get shared memory failed:%s\n", strerror(errno));	
		return -1;
	}

	printf("getShm ID=%d\n",nShmId);

	return nShmId;
}

int createShm(int key, int size)
{
	int nShmId;
	nShmId = shmget(key, size, IPC_CREAT|IPC_EXCL|0777);
	if (-1 == nShmId)
	{
		fprintf(stderr,"create shared memory failed:%s\n", strerror(errno));	
		return -1;
	}

	return nShmId;
}

void rmShm(int nShmId)
{
	struct shmid_ds shmDS;

	if (-1 == shmctl(nShmId, IPC_RMID, &shmDS))
	{
		fprintf(stderr, "remove shared memory failed!\n");	
		return;
	}
}

void statShm(int nShmId, struct shmid_ds* pShmDS)
{
	if (NULL == pShmDS)
	{
		printf("NULL ==  pShmDS\n");
		return ;
	}

	if (-1 == shmctl(nShmId, IPC_STAT, pShmDS))
	{
		fprintf(stderr, "stat shared memory failed!\n");	
		return;
	}
	printf("shmID=%d, key=%#x,shmsize=%d,creatorPID=%d,lastPID=%d,atnum=%ld\n", nShmId, pShmDS->shm_perm.__key, pShmDS->shm_segsz, pShmDS->shm_cpid, pShmDS->shm_lpid, pShmDS->shm_nattch);
}
