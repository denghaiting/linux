/*###################################*
 *#                                 #*
 *#     Author: Yu                  #*
 *#     Email : 33076156@qq.com     #*
 *#     Date  : 2020.3.26           #*
 *#                                 #*
 *###################################*/
#include "stm32f0xx.h"
#include "sys_config.h"
#include "systick.h"



/* +------------------------------------------+ *
 * |	  		     精确延时         	      | *
 * +------------------------------------------+ */
// TODO: 使用这个函数时注意量程 最大349毫秒
void systick_delay(u32 delay_time)
{
	u32 temp;

	if (delay_time > MAX_LOAD_TICKS) {
		delay_time = MAX_LOAD_TICKS;
	}
	
    SysTick->LOAD = delay_time; 				//时间加载             
    SysTick->VAL = 0x00; 						//清空计数器  
    SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk; 	//开始倒数   

    do {  
        temp = SysTick->CTRL;  
    } while ((temp & 0x01) && ! (temp & (1 << 16)));     //等待时间到达 
    
    SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk; 		//关闭计数器  
    SysTick->VAL = 0x00;                           //清空计数器       
}


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
	if (SysTick_Config(SystemCoreClock / 1000)) { 
		while (1);
	}
}



/* END OF FILE */

