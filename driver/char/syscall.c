#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/fcntl.h>


int mywrite(int fd, const char* buf, int size)
{
	int ret;
	__asm__ __volatile__(
		"svc #0x900004 \n"	
		"mov %0, r0"
		:"=r"(ret)
		:
		:"r0"	
	);

	if (ret < 0)
	{
		errno = -ret;
		ret = -1;
	}

	return ret;
}
int main(int argc, char** argv)
{
	int ret = 0;
//	ret = mywrite(10, "hello", 6);
	int fd = -1;	

	if (argc < 2)
	{
		printf("arg errs\n");
		return -1;
	}

	fd = open(argv[1], O_RDWR);
	if (-1 == fd)
	{
		perror("open");	
		return -1;
	}

	ret = write(fd, "hello", 6);
	printf("ret = %d\n", ret);

	while(1);

	close(fd);

	return 0;
}
