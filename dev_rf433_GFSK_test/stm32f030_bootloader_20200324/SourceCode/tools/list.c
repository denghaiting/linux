/****************************************Copyright (c)****************************************
 																							
                             Galaxywind Network System Co., Ltd.						    
                                      												
                                  http://www.galaxywind.com								
 																						
 Author:	Y
 Date  : 	2018-03-16
 *********************************************************************************************/ 
#include "list.h"
#include "stm32f0xx.h"


static void __list_add(struct list_head *new_ex,
                	   struct list_head *prev,
                	   struct list_head *next)
{
    next->prev = new_ex;
    new_ex->next = next;
    new_ex->prev = prev;
    prev->next = new_ex;
}


static void __list_del(struct list_head *prev, struct list_head *next)
{
    next->prev = prev;
    prev->next = next;
}


void list_add(struct list_head *new_ex, struct list_head *head)
{
    __list_add(new_ex, head, head->next);
}


void list_add_tail(struct list_head *new_ex, struct list_head *head)
{
    __list_add(new_ex, head->prev, head);
}


void list_del(struct list_head *entry)
{
    __list_del(entry->prev, entry->next);
    entry->next = NULL;
    entry->prev = NULL;
}


void list_del_only(struct list_head *entry)
{
    __list_del(entry->prev, entry->next);
}



/* END OF FILE */

