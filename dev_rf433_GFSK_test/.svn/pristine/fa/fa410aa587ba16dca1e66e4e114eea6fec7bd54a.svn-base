/****************************************Copyright (c)****************************************
 																							
                             Galaxywind Network System Co., Ltd.						    
                                      												
                                  http://www.galaxywind.com								
 																						
 Author:	Y
 Date  : 	2018-03-16
 *********************************************************************************************/ 
#ifndef _FIFO_H_
#define _FIFO_H_

#include "stm32f0xx.h"

/* 当in==out时，说明缓冲区为空；
   当(in - out) == size时，说明缓冲区已满。
 */
typedef struct fifo { 
	u8 *buffer; 		// buffer指向存放数据的缓冲区 
	u32 size;        	// size是缓冲区的大小 
	u32 in;          	// in是写指针下标 
	u32 out;			// out是读指针下标 
} fifo_t; 




#define __MIN(a, b)		((a) <= (b) ? (a) : (b))





void clear_fifo(fifo_t *fifo);
u32 fifo_put(fifo_t *fifo, const u8 *buffer, u32 len);
u32 fifo_get(fifo_t *fifo, u8 *buffer, u32 len);
u32 get_fifo_len(fifo_t *fifo);
void fifo_init(fifo_t *fifo, u8 *buffer, u32 size);





#endif	// #ifndef _USART_FIFO_H_

/* END OF FILE */

