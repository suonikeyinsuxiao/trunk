#ifndef __LINK_P_H
#define __LINK_P_H
#include <stdio.h>
#include <stdlib.h>

typedef struct _NODE{
	int m_nData;
	struct _NODE* next;
}NODE_S;


void top_append(NODE_S** header, int val);
void tail_append(NODE_S** header, int val);
void tail_append_p(NODE_S** header, int val);
void print_link(NODE_S* header);
NODE_S* find_mid(NODE_S** header);
void turnover_link(NODE_S** header);
void del_link(NODE_S** header);
void del_node(NODE_S** header, int val);
#endif
