/****************************************Copyright (c)****************************************
 																							
                             Galaxywind Network System Co., Ltd.						    
                                      												
                                  http://www.galaxywind.com								
 																						
 Author:	Y
 Date  : 	2018-03-16
 *********************************************************************************************/ 
#include "stm32f0xx.h"



#define SYSTICK_DIV		1000		// 10us 
//#define SYSTICK_DIV		100		// 10ms


vu32 SYS_DELAY_TIMER = 0;
vu32 SYS_USER_TIMER_TIMER = 0;




/* 使能滴答定时器 */
void enable_systick(void)
{
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;
}

/* 关闭滴答定时器 */
void disable_systick(void)
{
	SysTick->CTRL &= ~(SysTick_CTRL_ENABLE_Msk);
}



/*  ======================= 系统时钟初始化 ======================== *
 *	+------------------------------------+------------------------+ *
 *	|     SystemCoreClock / 1000         |   interrupt for 1ms    | *
 *	+------------------------------------+------------------------+ *
 *	|     SystemCoreClock / 100000       |   interrupt for 10us   | *
 *	+------------------------------------+------------------------+ *
 *	|     SystemCoreClock / 1000000      |   interrupt for 1us    | *
 *	+------------------------------------+------------------------+ *
 *                在STM32库开发指南PAGE 147 有具体讲解              */



/* +------------------------------------------+ *
 * |	  	   	  系统时钟初始化        	  | *
 * +------------------------------------------+ */
void systick_init(void)
{
	enable_systick();
	
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK);
	
	if (SysTick_Config(SystemCoreClock / SYSTICK_DIV)) { 
		while (1);
	}
}



/* END OF FILE */

