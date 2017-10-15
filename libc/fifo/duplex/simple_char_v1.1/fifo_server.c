#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <fcntl.h>
#include <pthread.h>

#define FIFO_PATH_R ("./r_fifo")
#define FIFO_PATH_W ("./w_fifo")

void* read_thread(void* argv)
{
	int nRet = 0;
	char buf[128] = {0};
	int fd_r = *(int*)argv;

	while(1)
	{
		bzero(buf, 128);
		nRet = read(fd_r, buf, 128);
		printf("read len:%d, %s\n", nRet, buf);
	}
}

int main(int argc, char** argv)
{
	int i = 0;
	int nRet = 0;
	int fd_w = -1;
	int fd_r = -1;
	char ch = 'A';
	char buf[128] = "hello fifo";

	pthread_t pid;

	nRet = mkfifo(FIFO_PATH_R, 0600);
	if (nRet == -1)
	{
		if (errno == EEXIST)
			printf("%s\n", strerror(errno));
		else
			printf("create fifo failed:%d(%s)\n", errno, strerror(errno));	

		//return -1;
	}

	nRet = mkfifo(FIFO_PATH_W, 0600);
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
	fd_w = open(FIFO_PATH_W, O_WRONLY);
	if (-1 == fd_w)
	{
		printf("open %s failed\n", FIFO_PATH_W);
		return -1;
	}
	fd_r = open(FIFO_PATH_R, O_RDONLY);
	if (-1 == fd_w)
	{
		printf("open %s failed\n", FIFO_PATH_R);
		return -1;
	}
	printf("server---after open\n");

	nRet = pthread_create(&pid, NULL, read_thread, &fd_r);

	while(1)
	{
		fgets(buf, 128, stdin);

		nRet = write(fd_w, buf, strlen(buf));
		printf("write len:%d\n", nRet);

	}

	pthread_join(pid,NULL);

	close(fd_w);
	close(fd_r);

	return 0;
}
