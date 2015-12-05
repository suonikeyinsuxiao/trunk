
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
	char *p = "hello world" ; 
	ret = msgsnd(msgid , p , strlen(p) , 0);
	if(ret == -1)
	{
		perror("send msgid fail");
		return -2 ; 
	}


	return 0 ; 
}
