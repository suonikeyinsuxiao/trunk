#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <fcntl.h>

#define FIFO_PATH ("./tmp_fifo")

int main(int argc, char** argv)
{
	int nRet = 0;
	int fd = -1;
	char ch = '\0';
	char buf[128] = {0};
	int i = 0;

#if 0
	nRet = mkfifo(FIFO_PATH, 0);
	if (nRet == -1)
	{
		if (errno == EEXIST)
			printf("%s\n", strerror(errno));
		else
			printf("create fifo failed:%d(%s)\n", errno, strerror(errno));	

		//return -1;
	}
	printf("create fifo successfully\n");
#endif

	printf("client---before open\n");
	fd = open(FIFO_PATH, O_RDWR);
	if (-1 == fd)
	{
		printf("open %s failed\n", FIFO_PATH);
		return -1;
	}
	printf("client---after open\n");

	while(i < 20)
	{
		printf("aaaaa\n");
		i++;
		sleep(1);
	}

	nRet = read(fd, buf, 128);
	printf("read len:%d, %s\n", nRet, buf);

	i = 0;
	while(i < 10)
	{
		printf("ccccccc\n");
		i++;
		sleep(1);
	}

	nRet = write(fd, buf, strlen(buf));
	printf("write len:%d,%s\n", nRet,buf);


	close(fd);

	return 0;
}
