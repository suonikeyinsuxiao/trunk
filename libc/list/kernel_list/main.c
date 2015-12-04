#include <stdio.h>
#include <stdlib.h>

#include "klist.h"

struct node
{
	int num;
	struct list_head list;
};

int main()
{
	int i = 0;
	LIST_HEAD(header);	

	struct node* new = NULL;
	struct node* p = NULL;

	for(i = 0; i < 10; i++)
	{
	
		new = malloc(sizeof(struct node));
		printf("new=%p\n", new);
		new->num = i;
		list_add(&new->list, &header);
	}


	p = list_entry(header.next, struct node, list);
	printf("p=%p,p->num=%d\n", p, p->num);

	p = list_first_entry(&header, struct node, list);
	printf("p=%p,p->num=%d\n", p, p->num);

#if 0
	struct list_head* pos = NULL;
	list_for_each(pos, &header)
	{
		printf("num=%d\n",list_entry(pos, struct node, list)->num);
	}

	list_for_each_prev(pos, &header)
	{
		printf("num=%d\n",list_entry(pos, struct node, list)->num);
	}
#endif

	list_for_each_entry(p, &header, list)
	{
		printf("num=%d\n", p->num);	
	}

	return 0;
}
