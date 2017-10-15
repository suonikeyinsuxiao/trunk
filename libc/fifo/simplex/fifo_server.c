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
	int i = 0;
	int nRet = 0;
	int fd = -1;
	char ch = 'A';
	char buf[128] = "hello fifo";
	char buf2[128] = {0};

	nRet = mkfifo(FIFO_PATH, 0600);
	if (nRet == -1)
	{
		if (errno == EEXIST)
			printf("%s\n", strerror(errno));
		else
			printf("create fifo failed:%d(%s)\n", errno, strerror(errno));	

		//return -1;
	}

	printf("create fifo successfully\n");

	printf("server---before open\n");
	fd = open(FIFO_PATH, O_WRONLY);
	if (-1 == fd)
	{
		printf("open %s failed\n", FIFO_PATH);
		return -1;
	}
	printf("server---after open\n");

	while(i < 10)
	{
		printf("bbbbbbb\n");
		i++;
		sleep(1);
	}

	//nRet = write(fd, &ch, 1);
	nRet = write(fd, buf, strlen(buf));
	printf("write len:%d\n", nRet);

	close(fd);

	return 0;
}
