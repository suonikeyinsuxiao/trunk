#include <stdio.h>

#define SWAP(a,b) ((a)^=(b)^=(a)^=b)

void exchang(int* a, int* b)
{
	int tmp = 0;

	tmp = *a;
	*a = *b;
	*b = tmp;
}

void exchang2(int* a, int* b)
{
	*a = *a + *b;
	*b = *a - *b;
	*a = *a - *b;
}

int main(int argc, char** argv)
{
	int a = 10;
	int b = 12;

	SWAP(a, b);
	printf("a=%d, b=%d\n", a,b);

	exchang(&a, &b);
	printf("a=%d, b=%d\n", a,b);
	exchang2(&a, &b);
	printf("a=%d, b=%d\n", a,b);
	return 0;
}
