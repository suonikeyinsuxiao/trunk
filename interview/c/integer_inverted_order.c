#include <stdio.h>
#include <stdlib.h>

int integet_inverted_order(int n)
{
	int ret = 0;
	int a = 0;

	while(n)
	{
		a = n%10;
		n = n/10;
		ret = ret * 10 + a;	
	}

	return ret;
}

int main(int argc, char** argv)
{
	printf("ret=%d\n", integet_inverted_order(atoi(argv[1])));

	return 0;
}

