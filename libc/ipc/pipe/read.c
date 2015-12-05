
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>

int main(void)
{
	int fd ;
		
	fd = open("hello" , O_RDWR );
	if(-1 == fd)
	{
		perror("open file fail");
		return -1 ; 
	}

	char buffer[1024] = {0};
	
	int ret ;
	ret = read(fd , buffer , 1024);
	
	printf("ret:%d  buffer:%s \n" , ret , buffer);
	
	close(fd);

	return 0 ; 
}
