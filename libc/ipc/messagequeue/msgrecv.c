
#include <stdio.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>

int main(void)
{
	int msgid ; 
	
	msgid = msgget(0x12345678 , 0);
	if(msgid == -1)
	{
		perror("create msg queue fail");
		return -1 ; 
	}	
	printf("open msg success ... \n");


	int ret ; 	
	char buffer[1024] = {0};

	ret = msgrcv(msgid , buffer , 11 , 0 , 0);
	if(ret == -1)
	{
		perror("recv msgid fail");
		return -2 ; 
	}

	printf("ret: %d  buffer:%s \n" , ret , buffer);


	return 0 ; 
}
