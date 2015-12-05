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
	if (SIG_ERR == signal(SIGINT, do_handle))
	{
		printf("register signal failed...\n");	
		return -1;
	}

	if (SIG_ERR == signal(SIGTERM, do_handle))
	{
		printf("register signal failed...\n");	
		return -1;
	}

	while(1)
		sleep(10);

	return 0;
}
