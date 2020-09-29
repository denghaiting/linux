/****************************************Copyright (c)****************************************
 																							
                             Galaxywind Network System Co., Ltd.						    
                                      												
                                  http://www.galaxywind.com								
 																						
 Author:	Y
 Date  : 	2020-03-25
 *********************************************************************************************/ 
#include "sys.h"
#include "systick.h"
#include "sys_config.h"



/* +------------------------------------------+ *
 * |	 		   初始化看门狗         	  | *
 * +------------------------------------------+ */
void watchdog_init(void)
{
#if defined(ENABLE_IWDG)
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);	//关闭IWDG_PR和IWDG_RLR的写保护
	IWDG_SetReload(0xFFF);							//设置重装载值为0xfff
	IWDG_SetPrescaler(__CFG__WATCHDOG_PRESCALER);	//设置预分频系数为128， 约12秒
	IWDG_ReloadCounter();
	IWDG_Enable();
#endif
}


/* +------------------------------------------+ *
 * |  内部时钟初始化，STM32F030最高频率48MHz  | *
 * +------------------------------------------+ */
void system_clk_config(void)
{	
#if defined(EXTERN_CLOCK)
	// TODO: startup_stm32f030.s 取消注释, 恢复使用SystemInit 
#elif defined(INNER_CLOCK)
	
	RCC_DeInit();//将外设 RCC寄存器重设为缺省值
 
	RCC_HSICmd(ENABLE);//使能HSI  HSI为固定8MHz
	
	while (RCC_GetFlagStatus(RCC_FLAG_HSIRDY) == RESET);//等待HSI使能成功

    RCC_HCLKConfig(RCC_SYSCLK_Div1); //HCLK == SYSCLK  
    RCC_PCLKConfig(RCC_HCLK_Div1);	 //PCLK == HCLK == SYSCLK 	
    
   	/* 设置 PLL 时钟源及倍频系数 HSI / 2 * 12 = 48MHz*/
    RCC_PLLConfig(RCC_PLLSource_HSI_Div2, RCC_PLLMul_12);  
	
    /* 使能或者失能 PLL,这个参数可以取：ENABLE或者DISABLE */
    RCC_PLLCmd(ENABLE);//如果PLL被用于系统时钟,那么它不能被失能
    
    /* 等待指定的 RCC 标志位设置成功 等待PLL初始化成功 */
    while (RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET);
 
    /* 设置系统时钟（SYSCLK） 设置PLL为系统时钟源 */
    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);  

	/* 等待PLL成功用作于系统时钟的时钟源 
	 * +--------+------------------+ *
 	 * |  0x00	| HSI 作为系统时钟 | *
	 * +--------+------------------+ *
	 * |  0x04	| HSE 作为系统时钟 | *
 	 * +--------+------------------+ *
 	 * |  0x08	| PLL 作为系统时钟 | *
	 * +--------+------------------+ */
    while (RCC_GetSYSCLKSource() != 0x08);
#endif
}


void feed_iwdg(void)
{
#if defined(ENABLE_IWDG)
	IWDG_ReloadCounter();
#endif
}


void sys_init(void)
{
	watchdog_init();

	system_clk_config();

	systick_init();
}




/* END OF FILE */

