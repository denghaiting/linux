/****************************************Copyright (c)****************************************
 																							
                             Galaxywind Network System Co., Ltd.						    
                                      												
                                  http://www.galaxywind.com								
 																						
 Author:	Y
 Date  : 	2018-03-16
 *********************************************************************************************/ 
#ifndef _BSP_USART2_H_
#define	_BSP_USART2_H_

#include "stm32f0xx.h"
#include <stdio.h>



void bsp_usart2_config(void);
void bsp_usart2_init(void);
void bsp_usart2_fini(void);
void usart2_send_byte(uc8 byte);
void usart2_send_data(const u8 *src, u32 data_len);




#endif /* #ifndef _BSP_USART2_H_ */

/* END OF FILE */

