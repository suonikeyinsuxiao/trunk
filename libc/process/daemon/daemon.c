#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/resource.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

#define RWRWRW (S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH|S_IWOTH)

int main(int argc, char** argv)
{
	int i, fd0, fd1, fd2, fd;
	pid_t pid;
	pid_t sid;
	struct rlimit rl;
	char acCwd[128]={0};

	//1. clear file creation mask
	umask(0);
	//2. fork()
	pid = fork();	
	if (pid < 0)
	{
		printf("fork error\n");
		exit(1);
	}
	else if (pid != 0)
		exit(0);
	
	printf("child process ID:%d\n", getpid());
	//3. setsid()
	sid = setsid();
	printf("child process session ID:%d\n", sid);
	//4. chdir()
	printf("cwd real len:%d", getcwd(acCwd, 128));
	printf("\npath:%s\n", acCwd);
	if (chdir("/") < 0)
	{
		printf("can't change current work dir\n");
		exit(1);
	}
	memset(acCwd, '\0', 128);
	getcwd(acCwd, 128);
	printf("after chdir, current work dir:%s\n", acCwd);
	
	//5. close all open file descriptor
	if (0 != getrlimit(RLIMIT_NOFILE, &rl))	
	{
		printf("get max number of fd failed\n");
		exit(1);
	}

	if (rl.rlim_max == RLIM_INFINITY)
		rl.rlim_max = 1024;
	for (i = 0; i < rl.rlim_max; ++i)
		close(i);
	
	//6. attach file descriptors 0 1 2 to /dev/null
	//way 1:
	//fd0 = open("/dev/null", O_RDWR);
	//fd1 = dup(0);
	//fd2 = dup(0);
	
	//way 2:
	fd = open("/dev/null", O_RDWR);
	dup2(fd, 1);
	dup2(fd, 2);

	while(1)
		sleep(999);

	return 0;
}
