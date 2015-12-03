#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#define swap(a, b) {a = a + b; b = a - b; a = a - b;}
#define ARR_SIZE(x) (sizeof(x)/sizeof(x[0]))

typedef enum{
	ASCENDING,
	DESCENTING
}SORT_TYPE;

void print_data(int* pnArr, int len)
{
	int i = 0;

	for (i= 0;i < len; i++)
		printf("%d	", pnArr[i]);
}

void get_data(int* pnArr, int len)
{
	int i = 0;
	srand((unsigned int)time(NULL));

	for (i=0; i < len; i++)
		pnArr[i] = rand()%1000;
}

void chioce_sort(int* pnArr, int len)
{
	int i = 0;
	int j = 0;
	int min_index = 0;
		
	for (i = 0; i < len-1; i++)
	{
		min_index = i;
		for (j = i + 1; j < len; j++)
		{
			if (pnArr[j] < pnArr[min_index])	
			{
				min_index = j;
			}
		}
		if (i != min_index)
			swap(pnArr[i],pnArr[min_index]);
	}
}

void chioce_sort2(int* pnArr, int len, SORT_TYPE type)
{
	int i = 0;
	int j = 0;
	int index = 0;
		
	for (i = 0; i < len-1; i++)
	{
		index = i;
		for (j = i + 1; j < len; j++)
		{
			switch(type)
			{
				case ASCENDING:	
					if (pnArr[j] < pnArr[index])	
					{
						index = j;
					}
					break;
					
				case DESCENTING:
					if (pnArr[j] > pnArr[index])
					{
						index = j;		
					}
			}
		}
		if (i != index)
			swap(pnArr[i],pnArr[index]);
	}
}

void bubble_sort(int* pnArr, int len)
{
	int i = 0;
	int j = 0;

	for (i = 0; i < len-1; i++)
	{
		for (j = 0; j < len-i-1; j++)	
		{
			if (pnArr[j] > pnArr[j+1])		
				swap(pnArr[j],pnArr[j+1]);
		}
	}
}

void bubble_sort2(int* pnArr, int len, SORT_TYPE type)
{
	int i = 0;
	int j = 0;

	for (i = 0; i < len-1; i++)
	{
		for (j = 0; j < len-i-1; j++)	
		{
			switch(type)
			{
				case ASCENDING:
					if (pnArr[j] > pnArr[j+1])		
						swap(pnArr[j],pnArr[j+1]);
					break;
				case DESCENTING:
					if (pnArr[j] < pnArr[j+1])		
						swap(pnArr[j],pnArr[j+1]);
			}
		}
	}
}

//插入排序思想:将每个元素插入到待排队列中,每插入一个元素就和前面的元素进行比较,保证在插入新元素前这些元素是有序的
void insert_sort(int* pnArr, int len)
{
	int i = 0;
	int j = 0;

	for (i =1; i < len; i++)
	{
		for (j = 0; j < i; j++)	
		{
			if (pnArr[i] < pnArr[j])	
				swap(pnArr[i],pnArr[j]);
		}
	}
}

//双向交替扫描
void quick_sort(int* pnArr, int left, int right)
{
	int i = left;
	int j = right;
	int key = pnArr[i];//取出枢纽值

	if (i >= j)
		return;

	//一趟扫描
	while(i<j)
	{
		while(i<j && pnArr[j] >= key)	
			j--;
		pnArr[i] = pnArr[j];

		while(i<j && pnArr[i] <= key)
			i++;
		pnArr[j] = pnArr[i];
	}
	
	pnArr[i] = key;//找到枢纽值的最终位置
	quick_sort(pnArr, left, i-1);
	quick_sort(pnArr, i+1, right);
}

int main(int argc, char** argv)
{
	int i = 0;

	if (argc < 2)
	{
		printf("usage: ./sort size\n");
		return -1;
	}

	int size = atoi(argv[1]);
	printf("size:%d\n", size);

	int a[size];
	//int* a = malloc(size*sizeof(int));

	get_data(a, size);
	printf("origin data:\n");
	print_data(a,size);

	printf("\nquick ascenging data:\n");
	quick_sort(a, 0, size-1);
	print_data(a, size);

	printf("\nchioce ascenging data:\n");
	chioce_sort(a, size);//ascending
	print_data(a, size);

	printf("\nchioce descending data:\n");
	chioce_sort2(a, size, DESCENTING);
	print_data(a, size);

	printf("\nbubble ascending data:\n");
	bubble_sort(a, size);
	print_data(a, size);

	printf("\nbubble descending data:\n");
	bubble_sort2(a, size, DESCENTING);
	print_data(a, size);

	printf("\ninsert ascending data:\n");
	insert_sort(a, size);
	print_data(a, size);


	printf("\n");

	return 0;
}
