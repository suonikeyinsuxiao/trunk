
#include <stdio.h>
#include <unistd.h>

int main(void)
{
	pid_t  pid ; 

	//zombie  子进程先退出，父进程没有回收子进程资源

	pid = fork();	
	if(pid == 0)
	{
		printf("i am die ... \n");
		printf("pid:%d ppid:%d \n" , getpid() , getppid());
		return 0 ; 
	}

	while(1)
		sleep(1);

	return 0 ; 
}
