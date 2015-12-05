
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

int main(void)
{
	//进程成为守护进程的步骤
	//1.产生孤儿进程
	
	pid_t  pid ; 
	int fd ; 

	pid = fork();
	if(0 == pid)
	{
		//2.改变工作目录
		chdir("/");
		//3.成为进程组长
		setsid();
		//4 1>.关闭标准输入，输出，出错
		//close(0);
		//close(1);
		//close(2);

		//2>  重定向
	
		fd = open("/dev/null" , O_RDWR);
		if(-1 == fd)
		{
			perror("Open null file fail");
			return -1 ;
		}
		//禁掉
		dup2(fd , 0);
		dup2(fd , 1);
		dup2(fd , 2);

		while(1)
		{
		//main event loop
			printf("hello world \n");
			sleep(1);
		}

		return 0 ;
	}
	//父进程直接退出

	return 0 ;
}
