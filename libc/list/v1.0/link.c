#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define NR(x) sizeof(x)/sizeof(x[0])

typedef struct _NODE 
{
    int m_nData;
	struct _NODE* m_pNext;
}NODE_S;



void link_top_append(NODE_S* pHeader, int nData);
void link_print(NODE_S* pHeader);
void link_tail_append(NODE_S* pHeader, int nData);
int link_node_delete(NODE_S* pHeader, int nData); 
int link_sort(NODE_S* pHeader, int flag);//flag=1 s->b;flag=0 b->s
void link_node_turn_over(NODE_S* pHeader);

int main(void)
{
	int i = 0;
	int array[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};

	NODE_S sHeader = {.m_pNext = NULL};

	for (i = 0; i < NR(array); i++)
	{
		//link_top_append(&sHeader, array[i]);	
		link_tail_append(&sHeader, array[i]);	
	}

	link_print(&sHeader);

	int num;
	printf("please input a number:");
	scanf("%d", &num);
	link_node_delete(&sHeader, num);
	link_print(&sHeader);

	link_sort(&sHeader, 0);
	link_print(&sHeader);

	link_node_turn_over(&sHeader);
	link_print(&sHeader);

	return 0;
}


void link_top_append(NODE_S* pHeader, int nData)
{
    NODE_S* pTmp = malloc(sizeof(NODE_S));
	if (pTmp == NULL)
	{
		printf("pTmp==NULL\n");	
		return;
	}

	pTmp->m_nData = nData;
	pTmp->m_pNext = pHeader->m_pNext;
	pHeader->m_pNext = pTmp;
}

void link_print(NODE_S* pHeader)
{
	NODE_S* pTmp = pHeader;
	while(pTmp = pTmp->m_pNext)
	{
		printf("%d ", pTmp->m_nData);	
	}

	putchar('\n');
}

void link_tail_append(NODE_S* pHeader, int nData)
{
    NODE_S* pTmp = malloc(sizeof(NODE_S));
	if (pTmp == NULL)
	{
		printf("pTmp==NULL\n");	
		return;
	}

	NODE_S* pCur = pHeader;
    while(pCur->m_pNext && (pCur = pCur->m_pNext));

	pTmp->m_nData = nData;
    pTmp->m_pNext = NULL;
	pCur->m_pNext = pTmp;
}

int link_node_delete(NODE_S* pHeader, int nData)
{
    NODE_S* pBefore = pHeader;
    NODE_S* pAfter = pHeader->m_pNext;

	while(pAfter)
	{
		if (nData == pAfter->m_nData)	
		{
			pBefore->m_pNext = pAfter->m_pNext;
			pAfter->m_pNext = NULL;
			free(pAfter);
			pAfter = pBefore->m_pNext;
			continue;
		}

		pBefore = pAfter;
		pAfter = pAfter->m_pNext;
	}

	return 1;
}

int link_sort(NODE_S* pHeader, int flag)
{
	//choice sort
	NODE_S* pTmp1 = NULL;
	NODE_S* pTmp2 = NULL;
	int tmp = 0;

	pTmp1 = pHeader;
	while(pTmp1 = pTmp1->m_pNext)
	{
		pTmp2 = pTmp1->m_pNext;	
		while(pTmp2)
		{
			if (flag?(pTmp1->m_nData > pTmp2->m_nData) : (pTmp1->m_nData < pTmp2->m_nData))	
			{
				memcpy(&tmp, &pTmp1->m_nData, sizeof(int));	//当m_nData为结构体时,用memcpy;
				memcpy(&pTmp1->m_nData, &pTmp2->m_nData, sizeof(int));	
				memcpy(&pTmp2->m_nData, &tmp, sizeof(int));	
			}
			pTmp2 = pTmp2->m_pNext;
		}
	}

	return 1;
}


void link_node_turn_over(NODE_S* pHeader)
{
    NODE_S* pTmp = pHeader->m_pNext;
    NODE_S* pNew = NULL;
	pHeader->m_pNext = NULL;

	while(pNew = pTmp)
	{
		pTmp = pTmp->m_pNext;
		pNew->m_pNext = pHeader->m_pNext; 
		pHeader->m_pNext = pNew;
	}
}
