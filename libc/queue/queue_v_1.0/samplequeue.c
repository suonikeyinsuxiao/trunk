#include <stdio.h>
#define SIZE 5

int queue[SIZE] = {0};
int head = 0;
int tail = 0;

int is_empty()
{
	return head == tail;
}

int is_full()
{
	return (tail+1)%SIZE == head;
}

int enqueue(int val)
{
	if (is_full())
	{
		printf("queue is full...\n");
		return 0; 
	}

	queue[tail] = val;
	tail = (tail+1)%SIZE;
	return 1;
}

int dequeue(int *val)
{
	if (is_empty())
	{
		printf("queue is empty...\n");
		return 0;
	}

	*val = queue[head];
	head = (head+1)%SIZE;
	return 1;
}

int main()
{
	int i = 0;
	int val = 0;

	for (i = 0; i < SIZE; i++)
	{
		if (enqueue(i))
			printf("queue[%d]=%d\n", i, queue[i]);
	}

	for (i = 0; i < SIZE; i++)
	{
		if (dequeue(&val))	
			printf("val=%d\n", val);
	}

	for (i = 0; i < SIZE; i++)
	{
		if (enqueue(i))
			printf("queue[%d]=%d\n", i, queue[i]);
	}

	for (i = 0; i < SIZE; i++)
	{
		if (dequeue(&val))	
			printf("val=%d\n", val);
	}

	printf("head=%d, tail=%d\n", head, tail);
	return 0;
}

