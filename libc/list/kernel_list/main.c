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
	struct node* n = NULL;
	struct list_head* p1 = NULL;
	struct list_head* p2 = NULL;

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

	//way1: delete list
	list_for_each_entry_safe(p, n, &header, list)
	{
		list_del(&p->list);	
		free(p);
	}
//way2: delete list
//	list_for_each_safe(p1, p2, &header)
//	{
//		p = list_entry(p1, struct node, list);
//		list_del(p1);	
//		free(p);
//	}

	if (list_empty_careful(&header))
		printf("list empty\n");

	return 0;
}
