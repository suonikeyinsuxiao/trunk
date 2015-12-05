
#include <stdio.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#define NUM 10
#define COUNT 123

struct student 
{
	int id ; 
	char name[20];
	float mach ; 
};

struct buf
{
	long mtype ; 
	struct student stu ; 
};

struct buf Data = {
	NUM , { COUNT, "hello" , 44.444}
};

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
	ret = msgsnd(msgid , &Data, sizeof(struct buf) , 0);
	if(ret == -1)
	{
		perror("send msgid fail");
		return -2 ; 
	}


	return 0 ; 
}
