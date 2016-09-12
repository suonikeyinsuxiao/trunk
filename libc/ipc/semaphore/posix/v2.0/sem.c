#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <semaphore.h>
#include <errno.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>


#define handle_error(msg) \
	do { perror(msg); exit(EXIT_FAILURE); } while (0)

sem_t sem;

static void handler(int sig)
{
	write(STDOUT_FILENO, "sem_post() from handler\n", 24);
	if (sem_post(&sem) == -1) {
		write(STDERR_FILENO, "sem_post() failed\n", 18);
		_exit(EXIT_FAILURE);
	}
}

void* doFunc1(void* arg)
{
	int nRet;
	printf("doFunc1\n");
	printf("doFunc2\n");
}

int main(int argc, char** argv)
{
	struct sigaction sa;
	struct timespec ts;
	int s;

	if (argc != 3) {
		fprintf(stderr, "Usage: %s <alarm-secs> <wait-secs>\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	if (sem_init(&sem, 0, 0) == -1)
		handle_error("sem_init");

	sa.sa_handler = handler;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	if (sigaction(SIGALRM, &sa, NULL) == -1)
		handle_error("sigaction");

	alarm(atoi(argv[1]));

	if (clock_gettime(CLOCK_REALTIME, &ts) == -1)
		handle_error("clock_gettime");

	printf("ts.tv_sec=%ld, ts.tv_nsec=%ld\n", ts.tv_sec, ts.tv_nsec);

	ts.tv_sec += atoi(argv[2]);

	printf("main() about to call sem_timedwait()\n");
	while ((s = sem_timedwait(&sem, &ts)) == -1 && errno == EINTR)
		continue;       /* Restart if interrupted by handler */


	/* Check what happened */

	if (s == -1) {
		if (errno == ETIMEDOUT)
			printf("sem_timedwait() timed out\n");
		else
			perror("sem_timedwait");
	} else
		printf("sem_timedwait() succeeded\n");

	exit((s == 0) ? EXIT_SUCCESS : EXIT_FAILURE);

	int nRet = -1;
	pthread_t pid;

	nRet = sem_init(&sem, 0, 0);
	if(0 != nRet)
	{
		printf("init sem failed!\n");	
		return -1;
	}

	nRet = pthread_create(&pid, NULL, doFunc1, NULL);
	if (0 != nRet)
	{
		printf("create pthread failed!\n");
		return -1;
	}

	sleep(1);
	printf("aaaaaaaa1\n");
	printf("aaaaaaaa2\n");
	printf("aaaaaaaa3\n");
	sem_post(&sem);
	printf("bbbbbbbb\n");

	pthread_join(pid, NULL);

	sem_destroy(&sem);
	return 0;
}
