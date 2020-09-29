/****************************************Copyright (c)****************************************
 																							
                             Galaxywind Network System Co., Ltd.						    
                                      												
                                  http://www.galaxywind.com								
 																						
 Author:	Y
 Date  : 	2018-03-16
 *********************************************************************************************/ 
#include "bsp_tim16.h"
#include "sys_config.h"



#define TIM16_PERIOD		100		/* 100us */



/* +------------------------------------------+ *
 * |	  			tim16 中断配置        	  | *
 * +------------------------------------------+ */
static void __tim16_nvic_config(void)
{
	nvic_config(TIM16_IRQn, TIM16_PRIOR, ENABLE);
}


/* +------------------------------------------+ *
 * |	  			tim16  配置        	 	  | *
 * +------------------------------------------+ */
static void __tim16_config(void)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure = {0};
	u32 period = TIM16_PERIOD;
	
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM16, ENABLE);
    TIM_DeInit(TIM16);
	
    TIM_TimeBaseStructure.TIM_Period = period - 1;		 			
    /* 累计 TIM_Period个频率后产生一个更新或者中断 */
    TIM_TimeBaseStructure.TIM_Prescaler = 48 - 1;				    // 1M
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; 		// 采样分频 
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;     // 向上计数模式
    TIM_TimeBaseInit(TIM16, &TIM_TimeBaseStructure);
    TIM_ClearFlag(TIM16, TIM_FLAG_Update);							// 清除溢出中断标志
}


/* +------------------------------------------+ *
 * |	  		    TIM16 停止      	          | *
 * +------------------------------------------+ */
void bsp_tim16_stop(void)
{
    TIM_Cmd(TIM16, DISABLE);
    TIM_ITConfig(TIM16, TIM_IT_Update, DISABLE);
}


/* +------------------------------------------+ *
 * |	  		    TIM16 启动        	      | *
 * +------------------------------------------+ */
void bsp_tim16_start(void)
{
    TIM_ITConfig(TIM16, TIM_IT_Update, ENABLE);
    TIM_Cmd(TIM16, ENABLE);
}


/* +------------------------------------------+ *
 * |	  		     TIM16重新计数     	      | *
 * +------------------------------------------+ */
void bsp_tim16_clear(void)
{
	TIM_SetCounter(TIM16, 0);
}


/* +------------------------------------------+ *
 * |	  		     TIM16初始化      	      | *
 * +------------------------------------------+ */
void bsp_tim16_init(void)
{
	__tim16_nvic_config();
	__tim16_config();

	bsp_tim16_start();
}


/* +------------------------------------------+ *
 * |	  		     TIM16逆初始化     	      | *
 * +------------------------------------------+ */
void bsp_tim16_fini(void)
{
	bsp_tim16_stop();
	
    TIM_DeInit(TIM16);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM16, DISABLE);
}




/* END OF FILE */

