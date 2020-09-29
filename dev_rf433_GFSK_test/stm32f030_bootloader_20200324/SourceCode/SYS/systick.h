/****************************************Copyright (c)****************************************
 																							
                             Galaxywind Network System Co., Ltd.						    
                                      												
                                  http://www.galaxywind.com								
 																						
 Author:	Y
 Date  : 	2018-03-16
 *********************************************************************************************/ 
#ifndef _SYSTICK_H_
#define _SYSTICK_H_

#include "stm32f0xx.h"



extern u32 SYS_DELAY_TIMER;
extern u32 SYS_USER_TIMER_TIMER;



void systick_init(void);



#endif /* #ifndef _SYSTICK_H_ */


/* END OF FILE */

