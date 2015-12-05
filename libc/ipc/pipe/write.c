
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

	char *p = "hello world" ; 
	
	int ret ;
	write(fd , p ,strlen(p));
	
	
	close(fd);

	return 0 ; 
}
