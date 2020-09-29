/****************************************Copyright (c)****************************************
 																							
                             Galaxywind Network System Co., Ltd.						    
                                      												
                                  http://www.galaxywind.com								
 																						
 Author:	Y
 Date  : 	2018-03-16
 *********************************************************************************************/ 
#ifndef _USART_COMM_H_
#define _USART_COMM_H_


#include "stm32f0xx.h"
#include "sys_config.h"
#include "event.h"
#include "bsp_tim3.h"
#include "bsp_tim16.h"

#if defined (USE_USART_1)
#include "bsp_usart1.h"
#elif defined (USE_USART_2)
#include "bsp_usart2.h"
#endif

#include <string.h>



#if defined (USE_USART_1)
#define PUT_CHAR(x)				do {\
									usart1_send_byte(x);\
								} while (0);
#elif defined (USE_USART_2)
#define PUT_CHAR(x)				do {\
									usart2_send_byte(x);\
								} while (0);
#endif





void usart_common_init(void);
void usart_common_fini(void);


#endif	// #ifndef _USART_COMM_H_

/* END OF FILE */

