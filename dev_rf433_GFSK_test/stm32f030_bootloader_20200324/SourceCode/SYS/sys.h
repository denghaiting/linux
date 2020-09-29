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



#define PCLR_DFT 	"\033[0m"

#define PCLR_RED 	"\033[31m"
#define PCLR_YEL 	"\033[33m"
#define PCLR_BLU 	"\033[34m"
#define PCLR_PUR 	"\033[35m"
#define PCLR_LBL 	"\033[36m"
#define PCLR_WHI 	"\033[37m"

#define PCLR_RED_B 	"\033[31m\033[1m"
#define PCLR_YEL_B 	"\033[33m\033[1m"
#define PCLR_BLU_B 	"\033[34m\033[1m"
#define PCLR_PUR_B 	"\033[35m\033[1m"
#define PCLR_LBL_B 	"\033[36m\033[1m"
#define PCLR_WHI_B 	"\033[37m\033[1m"

#define NEW_LINE 	"\r\n"


/* 喂狗 */
#define FEED_DOG() do { \
						IWDG_ReloadCounter(); \
				   } while (0)


static inline void __DISABLE_IRQ(void)
{
	__asm {
		CPSID I
		NOP 
	}
}

static inline void __ENABLE_IRQ(void)
{
	__asm {
		NOP 
		CPSIE I
	}
}



void watchdog_init(void);
void rcc_config(void);
void nvic_config(u8 irq_channel, u8 prior, FunctionalState state);
void Delay_10us(u32 t);
void Delay_ms(u32 t);
void Delay_s(u32 t);

void system_clk_config(void); 
void sys_init(void);



#endif /* #ifndef _SYS_H_ */

/* END OF FILE */

