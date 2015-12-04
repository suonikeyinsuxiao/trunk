#include "link_p.h"


void top_append(NODE_S** header, int val)
{
	NODE_S* new = malloc(sizeof(NODE_S));
	if (NULL == new)
	{
		printf("malloc new node failed...\n");	
		return;
	}

	new->m_nData = val;

	new->next = (*header);
	*header = new;
}

void tail_append(NODE_S** header, int val)
{
	NODE_S* new = malloc(sizeof(NODE_S));
	if (NULL == new)
	{
		printf("malloc new node failed...\n");	
		return;
	}

	new->m_nData = val;

	if (NULL == *header)
	{
		new->next = *header;
		*header = new;
		return;
	}

	NODE_S* tmp = *header;
	while(tmp->next && (tmp = tmp->next))
		;

	new->next = tmp->next;
	tmp->next = new;

}

void tail_append_p(NODE_S** header, int val)
{
	NODE_S* new = malloc(sizeof(NODE_S));
	if (NULL == new)
	{
		printf("malloc new node failed...\n");	
		return;
	}

	new->m_nData = val;

	NODE_S** tmp = header;
	NODE_S* cur = NULL;

	while(cur = *tmp)
	{
		//*tmp = cur->next;
		tmp = &(cur->next);//头指针不能跟着向后移动
	}

	new->next = *tmp;
	*tmp = new;

}

void print_link(NODE_S* header)
{
	if (NULL == header)
	{
		printf("empty link...\n");
		return;
	}

	NODE_S* tmp = header;

	while(tmp)
	{
		printf("%d\n",tmp->m_nData);	
		tmp = tmp->next;
	}
}

void del_link(NODE_S** header)
{
	NODE_S* tmp = *header;
	
	while(*header)
	{
		*header = (*header)->next;	
		free(tmp);
		tmp = *header;
	}
}

NODE_S* find_mid(NODE_S** header)
{
	NODE_S* slow = *header;
	NODE_S* fast = *header;

	if (NULL == *header)
	{
		printf("empty link...\n");
		return;
	}

	while(fast->next)
	{
		slow = slow->next;	
		fast = fast->next;
		if(fast->next != NULL)
			fast = fast->next;
	}

	return slow;
}

void turnover_link(NODE_S** header)
{
	NODE_S* tmp = *header;
	NODE_S* next = NULL;

	tmp = (*header)->next;
	(*header)->next = NULL;

	while(tmp)
	{
		next = tmp->next;
		tmp->next = (*header);
		(*header) = tmp;
		tmp = next;
	}

}

void turnover_link1(NODE_S** header)
{
	NODE_S* tmp = *header;
	NODE_S* cur = NULL;
	*header = NULL;

	while(cur = tmp)
	{
		tmp = tmp->next;
		cur->next = *header;
		*header = cur;
	}
}

void del_node(NODE_S** header, int val)
{
	NODE_S* front = *header;
	NODE_S* back = NULL;

	while(front)
	{
		if (front->m_nData == val)	
		{
			if (front == *header)	
			{
				*header = front->next;
				front->next = NULL;
				free(front);
				front = *header;
				continue;
			}
			
			back->next = front->next;
			front->next = NULL;
			free(front);
			front = back->next;
			continue;
		}

		back = front;
		front = front->next;
	}
}
