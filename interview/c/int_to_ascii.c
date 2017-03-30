#include <stdio.h>

//1234
void int_to_ascii(int nVal)
{
	//printf("%d\n", nVal);
	int n;

	n = nVal/10;
	if (n != 0)
		int_to_ascii(n);
	printf("%c", (nVal%10)+48);
}

int main(int argc, char** argv)
{
	if (argc != 2)
	{
		printf("usage:./a.out int");
		return -1;
	}

	int_to_ascii(atoi(argv[1]));	

	return 0;
}
