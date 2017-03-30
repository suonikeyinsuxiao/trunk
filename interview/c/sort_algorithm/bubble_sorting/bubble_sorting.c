#include <stdio.h>
#include<stdlib.h>

long get_time_us()
{
	struct timeval sTime;
	
	gettimeofday(&sTime, NULL);

	//printf("sTime.tv_sec=%ld, sTime.tv_usec=%ld",sTime.tv_sec,sTime.tv_usec);
	return sTime.tv_sec*1000000 + sTime.tv_usec;
}

void exchang2(int* a, int* b)
{
	*a = *a + *b;
	*b = *a - *b;
	*a = *a - *b;
}

void bubble_sort(int* pnData, int nDataSize)
{
	int i,j = 0;
	for (i = 0; i < nDataSize; ++i)
	{
		for (j = 1; j < nDataSize-i; ++j)	
		{
			if (pnData[j] < pnData[j-1])
				exchang2(pnData+j, pnData+j-1);
		}
	}
}

int main(int argc, char** argv)
{
	int i = 0;
	long t = 0;
	int anData[] = {15, 9, 8, 1, 4, 11, 7, 12, 13, 6, 5, 3, 16, 2, 10, 14, 17,19,18};
	int size = sizeof(anData)/sizeof(int);

	t = get_time_us();
	bubble_sort(anData, size);
	printf("time:%ld(us)\n", get_time_us()-t);

	for (i = 0; i < size; ++i)
	{
		printf("%d\n", anData[i]);	
	}

	return 0;
}
