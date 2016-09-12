#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <semaphore.h>
#include <errno.h>

sem_t sem;

void* doFunc1(void* arg)
{
	int nRet;
	printf("doFunc1\n");
	//nRet = sem_wait(&sem);
	while(1)
	{
		nRet = sem_trywait(&sem);
		if (-1 == nRet)
		{
			if (errno == EAGAIN)
			{
				printf("no sem, again\n");
				continue;
			}
		}
		else if (0 == nRet)
			break;
	}
	printf("doFunc2\n");
}

int main(int argc, char** argv)
{
	int nRet = -1;
	pthread_t pid;

//int sem_init(sem_t *sem, int pshared, unsigned int value);	
	nRet = sem_init(&sem, 0, 0);
	if(0 != nRet)
	{
		printf("init sem failed!\n");	
		return -1;
	}

	nRet = pthread_create(&pid, NULL, doFunc1, NULL);
	if (0 != nRet)
	{
		printf("create pthread failed!\n");
		return -1;
	}

	sleep(1);
	printf("aaaaaaaa1\n");
	printf("aaaaaaaa2\n");
	printf("aaaaaaaa3\n");
	sem_post(&sem);
	printf("bbbbbbbb\n");

	pthread_join(pid, NULL);

	sem_destroy(&sem);
	return 0;
}
