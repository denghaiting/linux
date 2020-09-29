/****************************************Copyright (c)****************************************
 																							
                             Galaxywind Network System Co., Ltd.						    
                                      												
                                  http://www.galaxywind.com								
 																						
 Author:	Y
 Date  : 	2018-03-16
 *********************************************************************************************/ 
#ifndef _LIST_H_
#define _LIST_H_

struct list_head
{
    struct list_head *next;
	struct list_head *prev;
};

#define LIST_HEAD_INIT(name) { &(name), &(name) }

#define LIST_HEAD(name) \
	struct list_head name = LIST_HEAD_INIT(name)

#define INIT_LIST_HEAD(ptr) \
	do { \
		(ptr)->next = (ptr); (ptr)->prev = (ptr); \
	} while (0)


#define prefetch(x) x


#define list_entry(ptr, type, member) \
	((type *)((char *)(ptr) - (char *)(&((type *)0)->member)))


#define list_for_each_entry(type, pos, head, member) \
	for (pos = list_entry((head)->next, type, member), \
		     prefetch(pos->member.next); \
	     &pos->member != (head); \
	     pos = list_entry(pos->member.next, type, member), \
		     prefetch(pos->member.next))


#define list_for_each_entry_safe(type, pos, n, head, member) \
	for (pos = list_entry((head)->next, type, member), \
		n = list_entry(pos->member.next, type, member); \
	     &pos->member != (head); \
	     pos = n, n = list_entry(n->member.next, type, member))



void list_add_tail(struct list_head *new_ex, struct list_head *head);
void list_del_only(struct list_head *entry);


#endif	// #ifndef _LIST_H_

/* END OF FILE */

