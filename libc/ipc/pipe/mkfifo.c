
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>

int main(void)
{
	int ret ; 
	
	ret = mkfifo("hello" , 0777);
	if(-1 == ret)
	{
		perror("创建fifo fail");
		return -1 ;
	}

	printf("create fifo success ... \n");

	return 0 ; 
}
