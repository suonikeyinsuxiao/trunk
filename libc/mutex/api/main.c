/***********************************************************************
*   Copyright (C) 2018 junfu0903@aliyun.com All rights reserved.
*   *   File Name: main.c 
*   Brief:  本程序模拟简单的音乐播放器的控制流程     
*   Author: frank
*   Email: junfu0903@aliyun.com
*   Version: 1.0
*   Created Time:2018-08-23 22:48:22
*   Blog: http://www.cnblogs.com/black-mamba
*   Github: https://github.com/suonikeyinsuxiao
*   
***********************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "xmg_mutex.h"
#include "xmg_thread.h"

static int s_playstate = 0;//0,stop;1,start;2,pause;
xmg_mutex_s test_mutex1;
XMG_THREAD_PID app_uipc_tx_thread_struct;

static void app_uipc_tx_thread(void)
{
	int status;
	printf("starting app_uipc_tx_thread\n");
	while(1)
	{
		while (s_playstate != 1)//!start
		{
			status = xmg_lock_mutex(&test_mutex1);
			if (status < 0)
			{
				printf("xmg_lock_mutex failed\n");	
				break;
			}
		}

		printf("playing...\n");
		//sleep(1);
	}

	printf("thread exit\n");
	pthread_exit(NULL);
}

int main(int argc, char** argv)
{
	//int te =1;
	int status;

	xmg_init_mutex(&test_mutex1);

	//printf("app_uipc_tx_thread=%x\n", app_uipc_tx_thread);
	status = xmg_create_thread(app_uipc_tx_thread, 0, 0, &app_uipc_tx_thread_struct);

	printf("main thread sleep 1s...\n");
	sleep(1);

	//play start
	s_playstate = 1;
	status = xmg_unlock_mutex(&test_mutex1);
	if (status < 0)
	{
		printf("xmg_unlock_mutex failed\n");	
	}
	printf("main thread play 5s...\n");
	sleep(15);
	
	//play pause
	s_playstate = 2;
	printf("main thread pause 5s...\n");
	sleep(5);

	//play start
	s_playstate = 1;
	status = xmg_unlock_mutex(&test_mutex1);
	if (status < 0)
	{
		printf("xmg_unlock_mutex failed\n");	
	}
	printf("main thread play 5s...\n");
	sleep(5);

	//play stop
	s_playstate = 0;
	xmg_stop_thread(app_uipc_tx_thread_struct);
	printf("main thread stop 5s...\n");
	sleep(5);


	//(void)te;

	return 0;
}
