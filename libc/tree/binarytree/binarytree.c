#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>

/*树的一种特殊形式，它的每个节点之多有两个子节点(left node, right node),
 * 每个节点的值比它的左子树的所有节点的值都要大，但比它的右子树的所有节点的值都要小，
 * 那么这种树中就不存在值相同的节点了。
 * 
*/

typedef struct _binary_tree
{
	int 			m_nVal;
	struct _binary_tree* 	m_psLeftNode;
	struct _binary_tree* 	m_psRightNode;
}binary_tree_s;

static binary_tree_s* psTree = NULL;

/**
 * @brief    myrand 
 *
 * @param[in]    range
 *
 * @return   random number 
 */
int myrand(int range)
{
	srand((unsigned int)time(NULL));
	return (rand()%range);
}

void insert(int nVal)
{
	printf("insert:%d", nVal);

	binary_tree_s* psCurrent = NULL;
	binary_tree_s** ppsLink = NULL;

	ppsLink = &psTree;

	
	while((psCurrent = *ppsLink) != NULL)
	{
		if (nVal < psCurrent->m_nVal)
		{
			printf("left ");
			ppsLink = &psCurrent->m_psLeftNode;
		}
		else
		{
			if (nVal == psCurrent->m_nVal)
			{
				printf("nVal=%d, m_nVal=%d\n", nVal, psCurrent->m_nVal);
				return;
			}
			printf("right ");
			ppsLink = &psCurrent->m_psRightNode;	
		}
	
	}

	psCurrent = (binary_tree_s*)malloc(sizeof(binary_tree_s));
	psCurrent->m_nVal = nVal;
	psCurrent->m_psLeftNode = NULL;
	psCurrent->m_psRightNode = NULL;
	*ppsLink = psCurrent;
	printf("\n");
}

binary_tree_s* find(int nVal)
{
	binary_tree_s* psCurrent = NULL;

	psCurrent = psTree;

	while((NULL != psCurrent) && (nVal != psCurrent->m_nVal))
	{
		if (nVal < psCurrent->m_nVal)
			psCurrent = psCurrent->m_psLeftNode;
		else
			psCurrent = psCurrent->m_psRightNode;
	}
	
	if (psCurrent == NULL)
		return NULL;
	else
		return psCurrent;
}

void myprint(int nVal)
{
	printf("val:%d\n", nVal);
}

static void do_pre_order_traverse(binary_tree_s* psCurrent, void(*callback)(int nVal))
{
	if (NULL != psCurrent)
	{
		callback(psCurrent->m_nVal);
		do_pre_order_traverse(psCurrent->m_psLeftNode, callback);
		do_pre_order_traverse(psCurrent->m_psRightNode, callback);
	}
}

void pre_order_traverse(binary_tree_s* psTree)
{
	do_pre_order_traverse(psTree, myprint);
}

static void do_mid_order_traverse(binary_tree_s* psCurrent, void(*callback)(int nVal))
{
	if (NULL != psCurrent)
	{
		do_mid_order_traverse(psCurrent->m_psLeftNode, callback);
		callback(psCurrent->m_nVal);
		do_mid_order_traverse(psCurrent->m_psRightNode, callback);
	}
}

void mid_order_traverse(binary_tree_s* psTree)
{
	do_mid_order_traverse(psTree, myprint);
}


static void do_aft_order_traverse(binary_tree_s* psCurrent, void(*callback)(int nVal))
{
	if (NULL != psCurrent)
	{
		do_aft_order_traverse(psCurrent->m_psLeftNode, callback);
		do_aft_order_traverse(psCurrent->m_psRightNode, callback);
		callback(psCurrent->m_nVal);
	}
}

void aft_order_traverse(binary_tree_s* psTree)
{
	do_aft_order_traverse(psTree, myprint);
}

void destroy_tree(binary_tree_s* psNode)
{
	binary_tree_s** ppsCur;

	ppsCur = &psNode;

	if (*ppsCur != NULL)
	{
		destroy_tree((*ppsCur)->m_psLeftNode);	
		destroy_tree((*ppsCur)->m_psRightNode);	
		free((*ppsCur));
		ppsCur = NULL;
	}
}

void destroy_tree2(binary_tree_s* psNode)
{

	if (psNode != NULL)
	{
		destroy_tree(psNode->m_psLeftNode);	
		destroy_tree(psNode->m_psRightNode);	
		free(psNode);
		psNode = NULL;
	}
}

int main(int argc, char** argv)
{
	srand((unsigned int)time(NULL));

	//printf("rand:%d", rand()%100);
	//printf("rand:%d", rand()%100);
	//printf("rand:%d", rand()%100);
	insert(rand()%100);
	insert(rand()%100);
	insert(rand()%100);
	insert(rand()%100);
	insert(rand()%100);
	insert(rand()%100);
	//insert(myrand(100));
	//insert(myrand(100));

#if 0
	binary_tree_s* psTmp = find(49);
	if (NULL != psTmp)
		printf("val:%d\n", psTmp->m_nVal);
#endif

	printf("pre ord:\n");
	pre_order_traverse(psTree);
	printf("\nmid ord:\n");
	mid_order_traverse(psTree);
	printf("\naft ord:\n");
	aft_order_traverse(psTree);

	destroy_tree(psTree);

	printf("xxxxxxxx\n");
	//pre_order_traverse(psTree);

	return 0;
}
