#include <stdio.h>
#include <stdlib.h>

long get_time_us()
{
	struct timeval sTime;
	
	gettimeofday(&sTime, NULL);

	//printf("sTime.tv_sec=%ld, sTime.tv_usec=%ld",sTime.tv_sec,sTime.tv_usec);
	return sTime.tv_sec*1000000 + sTime.tv_usec;
}

void exchang2(int* a, int* b)
{
	if (a!=b)
	{
		*a = *a + *b;
		*b = *a - *b;
		*a = *a - *b;
	}
}

//1 9 8 15 4 11 7 12 13 6 5 3 16 2 10 14 17 19 18 
//1 2 8 15 4 11 7 12 13 6 5 3 16 9 10 14 17 19 18 
//1 2 3 15 4 11 7 12 13 6 5 8 16 9 10 14 17 19 18 
//1 2 3 4 15 11 7 12 13 6 5 8 16 9 10 14 17 19 18 
//1 2 3 4 5 11 7 12 13 6 15 8 16 9 10 14 17 19 18 
//1 2 3 4 5 6 7 12 13 11 15 8 16 9 10 14 17 19 18 
//1 2 3 4 5 6 0 12 13 11 15 8 16 9 10 14 17 19 18 
//1 2 3 4 5 6 0 8 13 11 15 12 16 9 10 14 17 19 18 
//1 2 3 4 5 6 0 8 9 11 15 12 16 13 10 14 17 19 18 
//1 2 3 4 5 6 0 8 9 10 15 12 16 13 11 14 17 19 18 
//1 2 3 4 5 6 0 8 9 10 11 12 16 13 15 14 17 19 18 
//1 2 3 4 5 6 0 8 9 10 11 0 16 13 15 14 17 19 18 
//1 2 3 4 5 6 0 8 9 10 11 0 13 16 15 14 17 19 18 
//1 2 3 4 5 6 0 8 9 10 11 0 13 14 15 16 17 19 18 
//1 2 3 4 5 6 0 8 9 10 11 0 13 14 0 16 17 19 18 
//1 2 3 4 5 6 0 8 9 10 11 0 13 14 0 0 17 19 18 
//1 2 3 4 5 6 0 8 9 10 11 0 13 14 0 0 0 19 18 
//1 2 3 4 5 6 0 8 9 10 11 0 13 14 0 0 0 18 19 
//1 2 3 4 5 6 0 8 9 10 11 0 13 14 0 0 0 18 0
void select_sort(int* pnData, int nDataSize)
{
	int i = 0;
	int j = 0;
	int min = 0;

	for (i = 0; i < nDataSize; i++)
	{
		min = i;
		printf("[%d]:\n", i);
		for (j = i+1; j < nDataSize; j++)
		{
			if (pnData[j] < pnData[min])	
				min = j;
		}
		printf("[%d]:pnData[%d]=%d, pnData[%d]=%d\n", min,i,pnData[i], min, pnData[min]);
		//if (i != min)
			//exchang2(pnData+i, pnData+min);
			exchang2(&pnData[i], &pnData[min]);
		printf("[%d]:pnData[%d]=%d, pnData[%d]=%d\n", min,i,pnData[i], min, pnData[min]);
		//else
		//	printf("i=%d, min=%d\n", i, min);
		printf("\n");
	}
}

int main(int argc, char** argv)
{
	int i = 0;
	long t = 0;
	int anData[] = {15, 9, 8, 1, 4, 11, 7, 12, 13, 6, 5, 3, 16, 2, 10, 14, 17,19,18};
	int size = sizeof(anData)/sizeof(int);
	printf("sizeof=%d\n", size);
	//for (i = 0; i < size; ++i)
	//{
	//	printf("[%d]:%d\n", i, anData[i]);	
	//}

	t = get_time_us();
	select_sort(anData, size);
	printf("time:%ld(us)\n", get_time_us()-t);

	for (i = 0; i < size; ++i)
	{
		printf("%d\n", anData[i]);	
	}

	return 0;
}
