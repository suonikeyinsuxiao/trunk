#include <stdio.h>

#define STACK_SIZE 5

int stacktop = 0;
int stack[STACK_SIZE];

int is_empty()
{
	return stacktop==0;
}

int is_full()
{
	return stacktop == STACK_SIZE;
}

int push(int value)
{
	if (is_full())
	{
		printf("stack full...\n");	
		return 0;
	}
	stack[stacktop++] = value;

	return 1;
}

int pop(int *value)
{
	if(is_empty())
	{
		printf("stack empty...\n");	
		return 0;
	}
	*value = stack[--stacktop];

	return 1;
}

int main()
{
	int i;
	for(i=0; i<STACK_SIZE+1; i++)	
	{
		if (push(i))	
			printf("%d	", stack[i]);
	}

	int value;
	for(i=0; i<STACK_SIZE+1; i++)	
	{
		if (pop(&value))	
			printf("%d	", value);
	}

	return 0;
}

