/****************************************Copyright (c)****************************************
 																							
                             Galaxywind Network System Co., Ltd.						    
                                      												
                                  http://www.galaxywind.com								
 																						
 Author:	Y
 Date  : 	2018-03-16
 *********************************************************************************************/ 
#ifndef _EVENT_H_
#define _EVENT_H_

#include "list.h"



typedef enum {
	eEVENT_EXITLINE0_IRQ,
	eEVENT_EXITLINE8_IRQ,
	eEVENT_EXITLINE9_IRQ,
	eEVENT_EXITLINE11_IRQ,
	eEVENT_EXITLINE12_IRQ,
	eEVENT_EXITLINE14_IRQ,
	eEVENT_TIM3_UPDATE_IRQ,
	eEVENT_TIM15_UPDATE_IRQ,
	eEVENT_TIM16_UPDATE_IRQ,
	eEVENT_TIM17_UPDATE_IRQ,
	eEVENT_DMA1_CH1_TC,
	eEVENT_DMA1_CH2_TC,
	eEVENT_DMA1_CH4_TC,
	eEVENT_USART1_RXNE,
	eEVENT_USART2_RXNE,
	eEVENT_MAX
} event_e;



typedef void (* event_func_t)(int event, void *data, int data_len);


typedef struct {
	struct list_head link;
	event_func_t proc;
} event_t;



int event_add(int event, event_t *node, event_func_t proc);
int event_del(int event, event_func_t proc);
int event_call(int event, void *data, int data_len);
void event_init(void);



#endif	// #ifndef _EVENT_H_

/* END OF FILE */

