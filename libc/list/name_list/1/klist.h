#ifndef __KLIST_H
#define __KLIST_H

#define offsetof(TYPE, MEMBER) ((size_t) &((TYPE *)0)->MEMBER)

#define container_of(ptr, type, member) ({		      \
	const typeof(((type *)0)->member)*__mptr = (ptr);    \
	(type *)((char *)__mptr - offsetof(type, member)); })

struct list_head
{
	struct list_head* prev;
	struct list_head* next;
};

#define LIST_HEAD_INIT(name) {&(name), &(name)}
#define LIST_HEAD(name) \
	struct list_head name = LIST_HEAD_INIT(name) 


static inline void INIT_LIST_HEAD(struct list_head* list)
{
	list->prev = list;
	list->next = list;
}

static inline void __list_add(struct list_head* new, struct list_head* prev, struct list_head* next)
{
	prev->next = new;
	new->next = next;
	new->prev = prev;
	next->prev = new;
}

static void list_add(struct list_head* new, struct list_head* head) 
{
	__list_add(new, head, head->next);
}

static inline void list_add_tail(struct list_head* new, struct list_head* head)
{
	__list_add(new, head->prev, head);
}

static inline void __list_del(struct list_head* prev, struct list_head* next)
{
	prev->next = next;
	next->prev = prev;
}

static inline void list_del(struct list_head* entry)
{
	__list_del(entry->prev, entry->next);
	entry->next = (void*)0x00100100;
	entry->prev = (void*)0x00200200;
}

static inline void __list_del_entry(struct list_head* entry)
{
	__list_del(entry->prev, entry->next);
}

static inline void list_del_init(struct list_head* entry)
{
	__list_del_entry(entry);
	INIT_LIST_HEAD(entry);
}

static inline void list_replace(struct list_head* new, struct list_head* old)
{
	new->next = old->next;
	new->prev = old->prev;
	old->next->prev = new;
	old->prev->next = new;
}

static inline void list_replace_init(struct list_head* new, struct list_head* old)
{
	list_replace(new, old);
	INIT_LIST_HEAD(old);
}

//move a entry in one list to another list
static inline void list_move(struct list_head* entry, struct list_head* head)
{
	__list_del_entry(entry);
	list_add(entry, head);
}

static inline void list_move_tail(struct list_head* entry, struct list_head* head)
{
	__list_del_entry(entry);
	list_add_tail(entry, head);
}

static inline int list_is_last(struct list_head* entry, struct list_head* head)
{
	return entry->next == head;
}

static inline int list_is_empty(struct list_head* head)
{
	return head->next == head;
}

static inline int list_empty_careful(struct list_head* head)
{
	struct list_head* next = head->next;
	return (next == head) && (next == head->prev);
}

/*
 * 两种表述:
 * 1.链表节点左移一次, 即看成是链表头不动,链表上的节点循环左移一次
 *		h 1 2 3
 *		h 2 3 1
 * 2.链表头和第一个节点交换一次
 * 		h 1 2 3
 * 		1 h 2 3
 */
static inline void list_rotate_left(struct list_head* head)
{
	struct list_head* first;
	if (!list_empty_careful(head))
	{
		first = head->next;
		list_move_tail(first, head);
	}
}

//tests whether a list has just one entry
static inline int list_is_singular(struct list_head* head)
{
	return (!list_empty_careful(head)) && (head->next == head->prev);
}

static inline void __list_cut_position(struct list_head* list, struct list_head* head, struct list_head* entry)
{
	struct list_head* new_first = entry->next;
	list->next = head->next;
	head->next->prev = list;
	list->prev = entry;
	entry->next = list;

	head->next = new_first;
	new_first->prev = head;
}

/*
 *	list: 	被剪切后节点新加入的链表
 *  head: 	被剪切的链表
 *  entry:	head链表中的节点
 *
 *  说明:	
 *  		1.entry: head链表中必须得有这个节点,由调用者保证,若head链表中没有entry这个节点,会产生未知结果.
 *  		1.将从entry节点(including)之前的所有节点剪切, 加入到list
 *          2.list链表应该为空链表或里面的数据无关紧要的链表
 *          3.本函数可检测:
 *          				head为空;
 *          				head为单节点链表且不含entry;
 *          				head == entry
 *
 *
 *  	head 1 2 3 4 5 6 7 8
 *  	entry假设为第5个节点
 *
 *  结果:
 *  	head 6 7 8 
 *  	list 1 2 3 4 5  
 */
static inline void list_cut_position(struct list_head* list, struct list_head* head, struct list_head* entry)
{
	if (list_is_empty(head))
		return;
	if (list_is_singular(head) && (entry != head && entry != head->next))
		return;

	if (entry == head)
		INIT_LIST_HEAD(list);
	else
		__list_cut_position(list, head, entry);
}


static inline void __list_splice(const struct list_head* list, struct list_head* prev, struct list_head* next)
{
	struct list_head* first = list->next;
	struct list_head* last = list->prev;

	first->prev = prev;
	prev->next = first;

	last->next = next;
	next->prev = last;
}

//将list链表中的节点全部拼接到head链表中,类似头插
static inline void list_splice(struct list_head* list, struct list_head* head)
{
	if (!list_is_empty(list))
		__list_splice(list, head, head->next);
}

//将list链表中的节点全部拼接到head链表中,类似尾插
static inline void list_splice_tail(struct list_head* list, struct list_head* head)
{
	if (!list_is_empty(list))
		__list_splice(list, head->prev, head);
}

static inline void list_splice_init(struct list_head* list, struct list_head* head)
{
	if (!list_is_empty(list))
	{
		__list_splice(list, head, head->next);
		INIT_LIST_HEAD(list);//reinitialise the emptied list
	}
}

static inline void list_splice_tail_init(struct list_head* list, struct list_head* head)
{
	if (!list_is_empty(list))
	{
		__list_splice(list, head->prev, head);
		INIT_LIST_HEAD(list);//reinitialise the emptied list
	}
}

/*
 *	ptr:	struct list_head指针
 *	type:	container struct type
 *	name:	struct list_head 成员名
 *
 *	获得ptr所在container结构体的首地址.
 */
#define list_entry(ptr, type, name) \
	container_of(ptr, type, name)

#define list_first_entry(ptr, type, name) \
	list_entry((ptr)->next, type, name)

//遍历每个struct list_head
#define list_for_each(pos, head) \
	for (pos = (head)->next; pos != (head); pos = pos->next)

#define list_for_each_prev(post, head)	\
	for (pos = (head)->prev; pos != (head); pos = pos->prev)

//遍历每个container结构体
#define list_for_each_entry(pos, head, name)	\
	for (pos = list_entry((head)->next, typeof(*pos), name);	\
			&(pos->name) != (head);	\
			pos = list_entry(pos->name.next, typeof(*pos), name))	

/**
 * list_for_each_entry_safe - iterate over list of given type safe against removal of list entry
 * @pos:	the type * to use as a loop cursor.
 * @n:		another type * to use as temporary storage
 * @head:	the head for your list.
 * @member:	the name of the list_struct within the struct.
 */
#define list_for_each_entry_safe(pos, n, head, member)			\
	for (pos = list_entry((head)->next, typeof(*pos), member),	\
		n = list_entry(pos->member.next, typeof(*pos), member);	\
	     &pos->member != (head); 					\
	     pos = n, n = list_entry(n->member.next, typeof(*n), member))



/**
* list_for_each_safe - iterate over a list safe against removal of list entry
* @pos:    the &struct list_head to use as a loop cursor.
* @n:      another &struct list_head to use as temporary storage
* @head:   the head for your list.
*/
#define list_for_each_safe(pos, n, head) \
	for (pos = (head)->next, n = pos->next; pos != (head); \
		pos = n, n = pos->next)


#endif
