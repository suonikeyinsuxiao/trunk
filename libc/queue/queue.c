#include <stdio.h>

#define QUEUE_SIZE 5

static int head = 0;
static int tail = 0;
static int queue[QUEUE_SIZE];

int is_empty()
{
	return head==tail;
}

int is_full()
{
	return (tail+1)%QUEUE_SIZE == head;
}

int enqueue(int value)
{
	if (is_full())
	{
		printf("queue full...\n");	
		return 0;
	}
	queue[tail++] = value;
	return 1;
}

int dequeue(int *value)
{
	if (is_empty())
	{
		printf("queue empty...\n");	
		return 0;
	}
	
	*value = queue[head++];
	return 1;
}

int main()
{
	int i = 0;
	for (i = 0; i< QUEUE_SIZE; i++)
	{
		if (enqueue(i))
			printf("%d	", i/*queue[tail-1]*/);
	}

	int value;
	for (i = 0; i< QUEUE_SIZE; i++)
	{
		if (dequeue(&value))
			printf("%d	", value);
	}

	return 0;
}
