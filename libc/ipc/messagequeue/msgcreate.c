
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/msg.h>

int main(void)
{
	int msgid ; 
	
	msgid = msgget(0x12345678 , IPC_CREAT | 0777);
	if(msgid == -1)
	{
		perror("create msg queue fail");
		return -1 ; 
	}	
	printf("create msg success ...msgid=%d \n", msgid);
	return 0 ; 
}
