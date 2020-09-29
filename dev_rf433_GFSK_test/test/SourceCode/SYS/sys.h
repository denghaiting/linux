/****************************************Copyright (c)****************************************
 																							
                             Galaxywind Network System Co., Ltd.						    
                                      												
                                  http://www.galaxywind.com								
 																						
 Author:	Y
 Date  : 	2020-03-25
 *********************************************************************************************/ 
#ifndef _SYS_H_
#define _SYS_H_


#include "stm32f0xx.h"



/* 中断优先等级划分 STM32F030只有0 - 3 */
enum {
	ePRIORITY_HIGHEST = 0,
	ePRIORITY_1,
	ePRIORITY_2,
	ePRIORITY_3,
};


void watchdog_init(void);
void feed_iwdg(void);


void system_clk_config(void); 
void sys_init(void);



#endif /* #ifndef _SYS_H_ */

/* END OF FILE */

