#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>

//group ID:23456
//current process ID:23456
//parent process ID:23160
//group ID of parent process:23160
//group ID:23456

int main(int argc, char** argv)
{
	pid_t pid;

	if ((pid = fork()) < 0)
	{
		printf("fork child process error!\n");
		exit(1);	
	}
	else if (pid == 0)//child process
	{
		printf("child process ID:%d\n", getpid());
		printf("a group ID of child process:%d\n", getpgrp());	
		setpgid(pid, pid);//set up group ID of child process itself
		printf("b group ID of child process:%d\n", getpgrp());	
		sleep(5);

		printf("c group ID of child process:%d\n", getpgrp());	
		printf("child process exit\n");
	}
	else
	{
		sleep(1);
		printf("child process ID:%d\n", pid);
		printf("parent process ID:%d\n", getpid());
		printf("1group ID of parent process:%d\n", getpgrp());
		printf("parent process ID of parent process:%d\n", getppid());
		printf("group ID of grandpa process :%d\n", getpgid(getppid()));

		setpgid(getpid(), 0);//set up group ID of parent process itself
		printf("2group ID of parent process:%d\n", getpgrp());	
		setpgid(getppid(), 0);//set up group ID of grandpa process, not parent process, so donot change the group ID of parent process
		printf("group ID of grandpa process :%d\n", getpgid(getppid()));
		setpgid(getppid(), pid);//current process is parent process, so it only support to set up group ID of itself or child process, cannot set up the group ID of grandpa process.
		printf("group ID of grandpa process :%d\n", getpgid(getppid()));
		printf("3group ID of parent process:%d\n", getpgrp());
		setpgid(getpid(), getpgid(getppid()));//set up group ID of parent process belong to it's parent(grandpa) process
		printf("4group ID of parent process:%d\n", getpgrp());
		setpgid(pid, getpgrp());//set up group ID of child process
		sleep(10);
	}


#if 0
	//get process group ID of current process
	printf("group ID:%d\n", getpgrp());	
	printf("current process ID:%d\n", getpid());
	printf("parent process ID:%d\n", getppid());
	printf("group ID of parent process:%d\n", getpgid(getppid()));
	//printf("grandparent process ID :%d\n", getppid());

	setpgid(getpid(), 0);//pgid=0,
	printf("group ID:%d\n", getpgrp());	

	setpgid(0, 0);//pgid=0,
	printf("group ID:%d\n", getpgrp());	

	//setpgid(getpid(), getpgid(5265));
	//printf("group ID:%d\n", getpgrp());	
#endif
	return 0;
}
