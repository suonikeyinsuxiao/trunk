#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv)
{
	int pid1,pid2;

	pid1 = fork();
	pid2 = fork();

	printf("pid1=%d, pid2=%d\n", pid1, pid2);

	return 0;
}

