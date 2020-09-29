/****************************************Copyright (c)****************************************
 																							
                             Galaxywind Network System Co., Ltd.						    
                                      												
                                  http://www.galaxywind.com								
 																						
 Author:	ZhangYu
 Date  : 	2015-06-02
 Email :	33076156@qq.com
 *********************************************************************************************/ 
#include "bsp_tim15.h"
#include "sys_nvic_config.h"



#define TIM15_PERIOD		100		/* 100us */



/* +------------------------------------------+ *
 * |	  			tim15 中断配置        	  | *
 * +------------------------------------------+ */
static void __tim15_nvic_config(void)
{
	nvic_config(TIM15_IRQn, TIM15_PRIOR, ENABLE);
}


/* +------------------------------------------+ *
 * |	  			tim15  配置        	 	  | *
 * +------------------------------------------+ */
static void __tim15_config(void)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	u32 period = TIM15_PERIOD;
	
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM15, ENABLE);
    TIM_DeInit(TIM15);
	
    TIM_TimeBaseStructure.TIM_Period = period - 1;		 			
    /* 累计 TIM_Period个频率后产生一个更新或者中断 */
    TIM_TimeBaseStructure.TIM_Prescaler = 48 - 1;				    // 1M
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; 		// 采样分频 
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;     // 向上计数模式
    TIM_TimeBaseInit(TIM15, &TIM_TimeBaseStructure);
    TIM_ClearFlag(TIM15, TIM_FLAG_Update);							// 清除溢出中断标志
}


/* +------------------------------------------+ *
 * |	  		    TIM15 停止      	          | *
 * +------------------------------------------+ */
void bsp_tim15_stop(void)
{
    TIM_Cmd(TIM15, DISABLE);
    TIM_ITConfig(TIM15, TIM_IT_Update, DISABLE);
}


/* +------------------------------------------+ *
 * |	  		    TIM15 启动        	      | *
 * +------------------------------------------+ */
void bsp_tim15_start(void)
{
    TIM_ITConfig(TIM15, TIM_IT_Update, ENABLE);
    TIM_Cmd(TIM15, ENABLE);
}


/* +------------------------------------------+ *
 * |	  		     TIM15重新计数     	      | *
 * +------------------------------------------+ */
void bsp_tim15_clear(void)
{
	TIM_SetCounter(TIM15, 0);
}


/* +------------------------------------------+ *
 * |	  		     TIM15初始化      	      | *
 * +------------------------------------------+ */
void bsp_tim15_init(void)
{
	__tim15_nvic_config();
	__tim15_config();

	bsp_tim15_start();
}


/* +------------------------------------------+ *
 * |	  		     TIM15逆初始化     	      | *
 * +------------------------------------------+ */
void bsp_tim15_fini(void)
{
	bsp_tim15_stop();
	
    TIM_DeInit(TIM15);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM15, DISABLE);
}




/* END OF FILE */

