/****************************************Copyright (c)****************************************
 																							
                             Galaxywind Network System Co., Ltd.						    
                                      												
                                  http://www.galaxywind.com								
 																						
 Author:	ZhangYu
 Date  : 	2015-06-03
 Email :	33076156@qq.com
 *********************************************************************************************/ 
#ifndef _BSP_TIM15_H_
#define _BSP_TIM15_H_

#include "stm32f0xx.h"



enum {
	SAMPLING_RATE_1K = 0,	
	SAMPLING_RATE_2K,
	SAMPLING_RATE_4K,
	SAMPLING_RATE_8K,
	SAMPLING_PERIOD_168us
};



typedef void (*tim15_callback_t)(void);





void tim15_config(int rate);
void bsp_tim15_stop(void);
void bsp_tim15_start(void);
void bsp_tim15_clear(void);
void bsp_tim15_init(void);
void bsp_tim15_fini(void);
void tim15_set_irq_callback(tim15_callback_t func);


#endif	// #ifndef _BSP_TIM15_H_

/* END OF FILE */


