#include <stdio.h>
#include <signal.h>

typedef void (*sighandler_t)(int);

sighandler_t signal(int signum, sighandler_t handler);

void do_handle(int num)
{
	printf("signal(%d) coming...\n", num);
}

int main()
{
	printf("SIGINT=%d\n", SIGINT);
	if (SIG_ERR == signal(SIGINT, do_handle))
	{
		printf("register signal failed...\n");	
		return -1;
	}

	printf("SIGTERM=%d\n", SIGTERM);
	if (SIG_ERR == signal(SIGTERM, do_handle))
	{
		printf("register signal failed...\n");	
		return -1;
	}

	alarm(5);
	printf("SIGALRM=%d\n", SIGALRM);
	if (SIG_ERR == signal(SIGALRM, do_handle))
	{
		printf("register signal failed...\n");	
		return -1;
	}
	
	sleep(10);

	return 0;
}
