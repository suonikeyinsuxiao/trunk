/***********************************************************************
*   Copyright (C) 2016 pilot-lab.inc All rights reserved.
*   
*   @file:       main.c
*   @brief:      
*   @author:     Pilot labs
*   @maintainer: frank.fu@pilot-lab.com.cn
*   @version:    1.0
*   @date:       2016-11-15
*   
***********************************************************************/
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
  
int main()  
{  
	int sfd = dup(STDOUT_FILENO), testfd;  

	printf("sfd = [%d]\n", sfd);  

	testfd = open("./temp",O_CREAT | O_RDWR | O_APPEND);  
	if (-1 == testfd)  
	{  
		printf("open file error.\n");  
		exit(1);  
	}  

	/* 重定向 */  
	if (-1 == dup2(testfd,STDOUT_FILENO) ) {  
		printf("can't redirect fd error\n");  
		exit(1);  
	}  

	/* 此时向stdout写入应该输出到文件 */  
	write(STDOUT_FILENO,"file\n",5);  

	/* 恢复stdout */  
	if (-1 != dup2(sfd,STDOUT_FILENO) ) {  
		printf("recover fd ok \n");  

		/* 恢复后，写入stdout应该向屏幕输出 */  
		write(STDOUT_FILENO,"stdout\n",7);  
	}  

	printf("gogogogogogo!\n");  
	close(testfd);  
}  
