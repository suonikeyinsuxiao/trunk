
#include<pthread.h>
#include"mutexSem.h"

int nResource = 1;
int nSemId;

void* do_pthread(void*arg);
int main(int argc, char** argv)
{
	//create sem
	int key = 0x12345678;
	struct semid_ds semDS;

	nSemId = initSem(key, 1);
	if (-1 == nSemId)
		return -1;

	statSem(nSemId, &semDS); 
	printf("key:%#x,uid=%d,gid=%d,mode=%o,seq=%d,semnum=%ld\n", semDS.sem_perm.__key,semDS.sem_perm.uid,semDS.sem_perm.gid, semDS.sem_perm.mode, semDS.sem_perm.__seq ,semDS.sem_nsems);

	pthread_t tid; 
	pthread_create(&tid, NULL, do_pthread, NULL);
	pthread_detach(tid);


	while(1)
	{
		pSem(nSemId);
		nResource++;
		printf("main :a=%d\n",nResource);
		if (nResource >= 20)
		{
			vSem(nSemId);
			break;
		}
		vSem(nSemId);
		sleep(1);
	}

	
	rmSem(nSemId);

	return 0;
}

void* do_pthread(void*arg)
{
	while(1)
	{
		pSem(nSemId);
		nResource++;
		printf("pthread:a=%d\n",nResource);
		if (nResource >= 20)
		{
			vSem(nSemId);
			printf("child pthread exit...\n");
			break;
		}
		vSem(nSemId);
		sleep(1);
	}
}
