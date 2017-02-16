#include<stdio.h>
#include<stdlib.h>

//n! = n*(n-1)!
int factorial_recursive(int nNum)
{
	if (0 == nNum || 1 == nNum)
		return 1;

	return nNum * factorial_recursive(nNum-1);
}

int main(int argc, char** argv)
{

	printf("sum=%d\n", factorial_recursive(4));

	return 0;
}

