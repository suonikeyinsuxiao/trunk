#include <stdio.h>
#include <stdlib.h>

#define SWAP(a,b) ((a)^=(b)^=(a)^=b)

long get_time_us()
{
	struct timeval sTime;
	
	gettimeofday(&sTime, NULL);

	//printf("sTime.tv_sec=%ld, sTime.tv_usec=%ld",sTime.tv_sec,sTime.tv_usec);
	return sTime.tv_sec*1000000 + sTime.tv_usec;
}

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

void insert_sort(int* pnData, int nDataSize)
{
	int i = 0;
	int j = 0;

	for (i = 1; i < nDataSize; ++i)
	{
		j = i;
		while(j>0 && pnData[j] < pnData[j-1])
		{
			//printf("before:pnData[%d]=%d, pnData[%d]=%d\n", j, pnData[j], j-1, pnData[j-1]);
			exchang2(pnData+j, pnData+j-1);	
			//printf("after:pnData[%d]=%d, pnData[%d]=%d\n", j, pnData[j], j-1, pnData[j-1]);
			j--;
		}
	}
}

void insert_sort2(int* pnData, int nDataSize)
{
	int i,j = 0;

	for (i = 1; i < nDataSize; ++i)
		for(j = i; j > 0 && pnData[j] < pnData[j-1]; --j)	
			exchang2(pnData+j, pnData+j-1);	
}

int main(int argc, char** argv)
{
	int i = 0;
	long t = 0;
	//int anData[5] = {5,4,3,2,1};
	int anData[] = {15, 9, 8, 1, 4, 11, 7, 12, 13, 6, 5, 3, 16, 2, 10, 14, 17,19,18};
	int size = sizeof(anData)/sizeof(int);
	printf("sizeof=%d\n", size);

	t = get_time_us();
	//insert_sort(anData, size);
	insert_sort2(anData, size);
	printf("time:%ld(us)\n", get_time_us()-t);

	for (i = 0; i < size; ++i)
	{
		printf("%d\n", anData[i]);	
	}

	return 0;
}
