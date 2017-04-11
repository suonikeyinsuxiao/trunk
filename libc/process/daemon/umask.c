#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

#define RWRWRW (S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH|S_IWOTH)

int main(int argc, char** argv)
{
	mode_t mode;
	//mode = umask(0);
	//printf("original mode:0%o\n", mode);

	if (creat("foo", 0666)<0)
		printf("creat error for foo");

	mode = umask(S_IRGRP|S_IWGRP|S_IROTH|S_IWOTH);
	printf("original mode:%03o\n", mode);
	if (creat("bar", RWRWRW)<0)
		printf("creat error for foo");


	return 0;
}
