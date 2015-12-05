
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>

int main(void)
{
	int ret ; 

	int fd1[2] ;
	int fd2[2] ;
	
	ret = pipe(fd1);
	if(ret == -1)
	{
		perror("mk pipe fail");
		return -1 ; 
	}

	ret = pipe(fd2);
	if(ret == -1)
	{
		perror("mk pipe fail");
		return -1 ; 
	}

	ret = fork();
	if(ret == 0)
	{
		//子进程
		char *p = "hello parent" ; 
		close(fd1[0]);  //fd1  只写
		write(fd1[1] , p , strlen(p));

		char buffer[1024] = {0};
						//fd2  只读
		close(fd2[1]);
		ret = read(fd2[0] , buffer , 1024);
	
		printf("child ret:%d buffer:%s \n" , ret , buffer);

		return 0 ; 
	}
	
	//父进程
	sleep(1);
	close(fd1[1]);
	char buffer[1024] = {0};
	ret = read(fd1[0] , buffer , 1024);
	
	printf("ret:%d buffer:%s \n" , ret , buffer);
	char *p = "hello son " ; 
	close(fd2[0]);
	write(fd2[1] , p , strlen(p));

	return 0 ;
}

