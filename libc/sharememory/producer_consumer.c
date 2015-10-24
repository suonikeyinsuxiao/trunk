#include<stdio.h>
#include<string.h>
#include<errno.h>
#include<sys/ipc.h>
#include<sys/shm.h>
#include<unistd.h>
#include"shm.h"
#include"mutexsem.h"

#define QUIT "quit"

int main(void)
{
	int nShmId;
	int key = 0x12345678;
	struct shmid_ds shmDS;
	char* pShmAddr = NULL;
	pid_t pid;
	int nSemId;

	nShmId = createShm(key, SHMSIZE);	
	if (-1 == nShmId)
	{
		return -1;
	}
	
	statShm(nShmId, &shmDS);

	getShm(key);


	nSemId = initSem(key, 1);
	if (-1 == nSemId)
		return -1;

	pid = fork();
	if (pid == 0)
	{
		pShmAddr = shmat(nShmId, NULL, 0);
		if (NULL == pShmAddr)
		{
			fprintf(stderr,"attach shared memory failed:%s!\n",strerror(errno) );	
			return -1;
		}

		char buffer[1024];

		while(1)
		{
			sleep(1);
			pSem(nSemId);	

			//memcpy();	
//			printf("shm:%s,quit:%s, len=%d\n",pShmAddr, QUIT, strlen(QUIT));
			if (!strncmp(pShmAddr, QUIT, strlen(QUIT)))
			{
				printf("child exit...\n");
				vSem(nSemId);
				break;
			}


			if (pShmAddr[0] != '\n')
				printf("consumer:get %s\n",pShmAddr);

			memset(pShmAddr, 0, strlen(pShmAddr));
			vSem(nSemId);
		}

		shmdt(pShmAddr);

		return 0;
	}
	else if(pid > 0)
	{
		pShmAddr = shmat(nShmId, NULL, 0);
		if (NULL == pShmAddr)
		{
			fprintf(stderr,"attach shared memory failed:%s!\n",strerror(errno) );	
			goto END;
		}
		printf("main: pShmAddr=%p\n", pShmAddr);

		char buffer[1024];

		while(1)
		{
			pSem(nSemId);	
			
			memset(buffer,0,1024);
			// write to shared memory
			fgets(buffer, 1024, stdin);
			if (!strncmp(buffer, QUIT, strlen(QUIT)))
			{
				memcpy(pShmAddr, buffer, strlen(buffer));
				vSem(nSemId);
				printf("main exit while\n");
				break;
			}

			memcpy(pShmAddr, buffer, strlen(buffer));
			vSem(nSemId);
		}

END:
		printf("main: wait...\n");
		wait(NULL);

		shmdt(pShmAddr);

		printf("main exit...\n");
		rmShm(nShmId);
		rmSem(nSemId);
	}
	else
	{
		fprintf(stderr,"fork failed%s\n", strerror(errno));	
		return -1;
	}

	return 0;
}
