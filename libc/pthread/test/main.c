/***********************************************************************
*   Copyright (C) 2016 pilot-lab.inc All rights reserved.
*   
*   @file:       main.c
*   @brief:      
*   @author:     Pilot labs
*   @maintainer: frank.fu@pilot-lab.com.cn
*   @version:    1.0
*   @date:       2016-12-02
*   
***********************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/prctl.h>

void *print_msg1(void* arg)
{
	//prctl(PR_SET_NAME,"THREAD1");
	pthread_setname_np(pthread_self(), "THREAD1");
	while(1)
	{
		printf("thread1...\n");
		sleep(1);
	}
}

void *print_msg2(void* arg)
{
	//prctl(PR_SET_NAME,"THREAD2");
	pthread_setname_np(pthread_self(), "THREAD2");

	while(1)
	{
		printf("thread2...\n");
		sleep(1);
	}
}

int main(int argc, void **argv)
{  
	pthread_t id1;
	pthread_t id2;

	pthread_create(&id1, NULL, print_msg1, NULL);
	pthread_create(&id2, NULL, print_msg2, NULL);

	pthread_join(id1, NULL);
	pthread_join(id2, NULL);


	return 0;
}
