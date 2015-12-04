#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define NR(x) sizeof(x)/sizeof(x[0])


typedef struct _NODE
{
    int m_nData;
	struct _NODE* m_pNext;
}NODE_S;

void link_top_append_p(NODE_S** ppHeader, int nData);
void link_print_p(NODE_S* pHeader);
void link_tail_append_p(NODE_S** ppHeader, int nData);
void link_node_delete_p(NODE_S** ppHeader, int nData);
void link_turn_over_p(NODE_S** ppHeader);

int main(void)
{
	int i = 0;
	int array[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
	//int array[] = {1, 1, 1, 1, 1, 1, 1, 1, 1};

	NODE_S* psHeader = NULL;

	for (i = 0; i < NR(array); i++)
	{
		//link_top_append_p(&psHeader, array[i]);	
		link_tail_append_p(&psHeader, array[i]);
	}

	link_print_p(psHeader);

	int num = 0;
	printf("please input a number:");
	scanf("%d", &num);
	link_node_delete_p(&psHeader, num);

	link_print_p(psHeader);

	link_turn_over_p(&psHeader);
	link_print_p(psHeader);
	return 0;
}

void link_top_append_p(NODE_S** ppHeader, int nData)
{

	NODE_S* pNew = malloc(sizeof(NODE_S));
	if (NULL == pNew)
	{
		printf("NULL == pNew\n");	
		return ;
	}

	pNew->m_nData = nData;
	pNew->m_pNext = *ppHeader;
	*ppHeader = pNew;
}

void link_print_p(NODE_S* pHeader)
{
	NODE_S* pTmp = pHeader;
    while(pTmp)
	{
		printf("%d ", pTmp->m_nData);	
		pTmp = pTmp->m_pNext;
	}
	putchar('\n');
}

void link_tail_append_p(NODE_S** ppHeader, int nData)
{
    NODE_S* pNew = malloc(sizeof(NODE_S));
	if (NULL == pNew)
	{
		printf("NULL == pNew\n");	
		return ;
	}
	
	pNew->m_nData = nData;

	if (NULL == *ppHeader)
	{
		pNew->m_pNext = *ppHeader;
		*ppHeader = pNew;
		return ;
	}

	NODE_S* pTmp = *ppHeader;

	while(pTmp->m_pNext && (pTmp = pTmp->m_pNext));

	pNew->m_pNext = pTmp->m_pNext;
	pTmp->m_pNext = pNew;
}
void link_node_delete_p(NODE_S** ppHeader, int nData)
{
	NODE_S* pBefore = *ppHeader;
	NODE_S* pAfter = *ppHeader;

	while(pAfter)
	{
		if ( pAfter->m_nData == nData)	
		{
			if (pAfter == *ppHeader)
			{
				*ppHeader = (*ppHeader)->m_pNext;		
				pBefore = *ppHeader;
				pAfter->m_pNext = NULL;
				free(pAfter);
				pAfter = *ppHeader;
				continue;
			}
			
			pBefore->m_pNext = pAfter->m_pNext;
			pAfter->m_pNext = NULL;
			free(pAfter);
			pAfter = pBefore->m_pNext;
			continue;
		}

		pBefore = pAfter;
		pAfter = pAfter->m_pNext;
	}
}

void link_turn_over_p(NODE_S** ppHeader)
{
    NODE_S* pTmp = *ppHeader;
	NODE_S* pCur = NULL;
	*ppHeader = NULL;

	while(pCur = pTmp)
	{
		pTmp = pTmp->m_pNext;
		pCur->m_pNext = *ppHeader;
		*ppHeader = pCur;
	}
}	
