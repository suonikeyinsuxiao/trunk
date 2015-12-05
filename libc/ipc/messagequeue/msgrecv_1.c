
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
	//有过滤效果
	long mtype ; 
	struct student stu ; 
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
	struct buf Data ; 
	
	ret = msgrcv(msgid , &Data , sizeof(struct buf) , NUM , 0);
	if(ret == -1)
	{
		perror("recv msgid fail");
		return -2 ; 
	}

	printf("mtype:%d  name:%s id:%d  mach:%f \n" , 
	Data.mtype , Data.stu.name , Data.stu.id , Data.stu.mach);

	return 0 ; 
}
