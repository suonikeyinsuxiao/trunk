#include <stdio.h>
#include <stdlib.h>

long get_time_us()
{
	struct timeval sTime;
	
	gettimeofday(&sTime, NULL);

	//printf("sTime.tv_sec=%ld, sTime.tv_usec=%ld",sTime.tv_sec,sTime.tv_usec);
	return sTime.tv_sec*1000000 + sTime.tv_usec;
}

static void Merge(int* pnArr, int* pnTmpArr, int nLPos, int nRPos, int nRightEnd)
{
	int i = 0;
	int nLeftEnd = nRPos-1;
	int nTmpPos = nLPos;
	int nNum = nRightEnd-nLPos+1;

	while(nLPos <= nLeftEnd && nRPos <= nRightEnd)
		if (pnArr[nLPos] <= pnArr[nRPos])
			pnTmpArr[nTmpPos++] = pnArr[nLPos++];
		else
			pnTmpArr[nTmpPos++] = pnArr[nRPos++];
	
	while(nLPos <= nLeftEnd)
			pnTmpArr[nTmpPos++] = pnArr[nLPos++];
	while(nRPos <= nRightEnd)
			pnTmpArr[nTmpPos++] = pnArr[nRPos++];
	
	//printf("nRightEnd=%d, nTmpPos=%d\n", nRightEnd, nTmpPos);
	for (i = 0; i < nNum; i++, nRightEnd--)
		pnArr[nRightEnd] = pnTmpArr[nRightEnd];
}

static void MSort(int* pnArr, int* pnTmpArr, int left, int right)
{
	int nCenter;

	if (left < right)
	{
		nCenter = (left+right)/2;	
		MSort(pnArr, pnTmpArr, left, nCenter);
		MSort(pnArr, pnTmpArr, nCenter+1, right);
		Merge(pnArr, pnTmpArr, left, nCenter+1, right);
	}
}

void merge_sort(int* pnArr, int nSize)
{
	int* pnTmpArr = malloc(nSize*sizeof(int));
	if (NULL != pnTmpArr)
	{
		MSort(pnArr, pnTmpArr, 0, nSize-1);
		free(pnTmpArr);
	}
	else
		printf("no space for tmp array\n");
}


int main(int argc, char** argv)
{
	int i = 0;
	long t = 0;
	int anData[] = {1, 3, 4, 5, 2, 6, 9, 7, 8, 0, 10};
	int size = sizeof(anData)/sizeof(int);
	printf("sizeof=%d\n", size);

	t = get_time_us();
	merge_sort(anData, size);
	printf("time:%ld(us)\n", get_time_us()-t);

	for (i = 0; i < size; ++i)
	{
		printf("%d\n", anData[i]);	
	}
	return 0;
}
