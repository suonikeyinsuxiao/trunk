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

void printPart(int* list, int begin, int end) {
	int i = 0;
	for (i = 0; i < begin; i++) {
		printf("\t");
	}
	for (i = begin; i <= end; i++) {
		printf("%d\t", list[i]);
	}
	printf("\n");
}

void HeapAdjust(int* array, int parent, int length) {
	int temp = array[parent]; // temp保存当前父节点
	int child = 2 * parent + 1; // 先获得左孩子

	while (child < length) {
		// 如果有右孩子结点，并且右孩子结点的值大于左孩子结点，则选取右孩子结点
		if (child + 1 < length && array[child] < array[child + 1]) {
			child++;
		}

		// 如果父结点的值已经大于孩子结点的值，则直接结束
		if (temp >= array[child])
			break;

		// 把孩子结点的值赋给父结点
		array[parent] = array[child];

		// 选取孩子结点的左孩子结点,继续向下筛选
		parent = child;
		child = 2 * child + 1;
	}

	array[parent] = temp;
}

#define LEFTCHILD(i) (2*(i)+1)
void percDown(int* pnArr, int parent, int nLen)
{
	int nChild;
	int tmp;

	for (tmp = pnArr[parent]; LEFTCHILD(parent) < nLen; parent=nChild)
	{
		nChild = LEFTCHILD(parent);//左孩子

		//如果有右孩子节点,并且右孩子节点的值大于左孩子节点,则选取右孩子节点
		if (nChild != nLen-1 && pnArr[nChild] < pnArr[nChild+1])
			nChild++;
		if (tmp < pnArr[nChild])
			pnArr[parent] = pnArr[nChild];//把孩子节点的值赋给父节点
		else
			break;//如果父节点的值大于孩子节点的值,则直接退出
	}
	pnArr[parent]=tmp;
}


int main(int argc, char** argv)
{
	int i = 0;
	long t = 0;
	//int anData[5] = {5,4,3,2,1};
	int anData[] = {1, 3, 4, 5, 2, 6, 9, 7, 8, 0, 10};
	int size = sizeof(anData)/sizeof(int);
	printf("sizeof=%d\n", size);

	t = get_time_us();
	for (i = size/2; i >= 0; i--)
		percDown(anData, i, size);

	for (i=size-1; i > 0; i--)
	{
		exchang2(&anData[0], &anData[i]);	
		percDown(anData, 0, i);
	}
	printf("time:%ld(us)\n", get_time_us()-t);

	for (i = 0; i < size; ++i)
	{
		printf("%d\n", anData[i]);	
	}

	return 0;
}
