
#include <stdio.h>
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

	//删除
	int ret ; 
	
	ret = msgctl(msgid , IPC_RMID , NULL);
	if(ret != 0)
	{
		perror("remove msg queue fail");
		return -2 ;
	}

	printf("remove msgqueue success ... \n");

	return 0 ; 
}
