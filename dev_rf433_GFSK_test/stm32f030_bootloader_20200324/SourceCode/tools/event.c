/****************************************Copyright (c)****************************************
 																							
                             Galaxywind Network System Co., Ltd.						    
                                      												
                                  http://www.galaxywind.com								
 																						
 Author:	Y
 Date  : 	2018-03-16
 *********************************************************************************************/ 
#include "event.h"






struct list_head event_manager[eEVENT_MAX];





int event_add(int event, event_t *node, event_func_t proc)
{
	if ((event < 0) || (event >= eEVENT_MAX) || ( ! proc ))
		return 1;
	
	event_del(event, proc);
	
	node->proc = proc;
	list_add_tail(&node->link, &event_manager[event]);

	return 0;
}


int event_del(int event, event_func_t proc)
{
	event_t *node;
	
	if (event < 0 || event >= eEVENT_MAX)
		return 1;

	list_for_each_entry(event_t, node, &event_manager[event], link) {
		if (node->proc == proc) {
			list_del_only(&node->link);
			return 0;
		}
	}

	return 1;
}

/*
int event_clear(int event)
{
	event_t *node, *next;
	
	if (event < 0 || event >= eEVENT_MAX)
		return 1;

	list_for_each_entry_safe(event_t, node, next, &event_manager[event], link) {
		list_del_only(&node->link);
	}
	
	return 0;	
}
*/

int event_call(int event, void *data, int data_len)
{
	event_t *node;

	if (event < 0 || event >= eEVENT_MAX)
		return 1;

	list_for_each_entry(event_t, node, &event_manager[event], link) {
		node->proc(event, data, data_len);
	}

	return 0;
}


void event_init(void)
{
	int i;

	for (i = 0; i < eEVENT_MAX; i++) {
		INIT_LIST_HEAD(&event_manager[i]);
	}
}

/*
void event_stop(void)
{
	int i;
	event_t *node, *next;

	for (i = 0; i < eEVENT_MAX; i++) {
		list_for_each_entry_safe(event_t, node, next, &event_manager[i], link) {
		}
	}
}
*/

/* END OF FILE */

