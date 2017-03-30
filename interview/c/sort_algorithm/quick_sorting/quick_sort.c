#include <stdio.h>
#include <stdlib.h>

long get_time_us()
{
	struct timeval sTime;
	
	gettimeofday(&sTime, NULL);

	//printf("sTime.tv_sec=%ld, sTime.tv_usec=%ld",sTime.tv_sec,sTime.tv_usec);
	return sTime.tv_sec*1000000 + sTime.tv_usec;
}

void quick_sort(int* pnData, int nLeft, int nRight)
{
	if (nLeft > nRight)
		return;

	int i = nLeft;
	int j = nRight;
	int key = pnData[i];

	while(i < j)
	{
		while(i < j && key <= pnData[j])
			j--;
		pnData[i] = pnData[j];

		while(i < j && key >= pnData[i])
			i++;
		pnData[j] = pnData[i];
	}

	pnData[i] = key;
	quick_sort(pnData, nLeft, i-1);
	quick_sort(pnData, i+1, nRight);

}

int main(int argc, char** argv)
{
	int i = 0;
	long t = 0;
	int anData[] = {15, 9, 8, 1, 4, 11, 7, 12, 13, 6, 5, 3, 16, 2, 10, 14, 17,19,18};
	int size = sizeof(anData)/sizeof(int);

	t = get_time_us();
	quick_sort(anData, 0, size-1);
	printf("time:%ld(us)\n", get_time_us()-t);

	for (i = 0; i < size; ++i)
	{
		printf("%d\n", anData[i]);	
	}

	return 0;
}
