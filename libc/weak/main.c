#include <stdio.h>
	
//#include <linux/compiler-gcc.h>
#define __weak __attribute__((weak))

void __weak func()
{
	printf("weak hello world\n");
}

int main()
{
	func();

	return 0;
}
