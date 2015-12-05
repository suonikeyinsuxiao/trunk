
#include"mutexSem.h"



int main(int argc, char**argv)
{
	int key = 0x12345678;
	initSem(key, 1);
	printf("semID:%d\n", openSem(key));
//	rmSem(atoi(argv[1]));

	return 0;
}
