/*###################################*
 *#                                 #*
 *#     Author: Yu                  #*
 *#     Email : 33076156@qq.com     #*
 *#     Date  : 2020.3.10           #*
 *#                                 #*
 *###################################*/
     
#ifndef _SYS_CONFIG_H_
#define _SYS_CONFIG_H_






// TODO: No.0 NVIC_PRIORITY
/*###########################################*
 *#                                         #*
 *#                配置NVIC                 #*
 *#                                         #*
 *###########################################*/


/* 中断优先等级划分 STM32F030只有0 - 3 */
typedef enum {
	PRIORITY_HIGHEST = 0,
	PRIORITY_1,
	PRIORITY_2,
	PRIORITY_3,
} NVIC_PRIORITY;



/* 此处配置所有中断的优先级 */
#define USART1_PRIOR			PRIORITY_1
#define USART2_PRIOR			PRIORITY_1
#define USART_DMA_PRIOR			PRIORITY_1
#define RF_IRQ_PRIOR			PRIORITY_1
#define TIM17_PRIOR				PRIORITY_2	//usart DMA update
#define TIM16_PRIOR				PRIORITY_2	//usart DMA update
#define TIM15_PRIOR				PRIORITY_2	
#define TIM3_PRIOR				PRIORITY_2	
#define ADC_DMA_PRIOR			PRIORITY_2	//ADC



// TODO: No.1 SYSTICK
/*###########################################*
 *#                                         #*
 *#                配置时钟                 #*
 *#                                         #*
 *###########################################*/

/* 配置选择内部或外部时钟 */
#if 0
#define EXTERN_CLOCK
#else
#define INNER_CLOCK
#endif



/*---------------------------------- 分割线 ---------------------------------- */

/* === Watch Dog 分频系数 === */
#define __CFG__WATCHDOG_PRESCALER 	IWDG_Prescaler_128 


// TODO: 看门狗配置
/* 看门狗开关 */
#if 1
#define ENABLE_IWDG
#endif

/*---------------------------------- 分割线 ---------------------------------- */

// TODO: SYS_LED

/*  ============== led bit ===================  
 *	+---+---+---+---+---+---+---+---+ *
 *	| 7 | 6 | 5 | 4 | 3 | 2 | 1 | 0 | *
 *	+---+---+---+---+---+---+---+---+ *
 *	| / | / | / | / | / | / | R | G | *
 *	+---+---+---+---+---+---+---+---+ */
//#define LED_GREEN_BIT 	0x01
//#define LED_RED_BIT 	0x02


#if 0 // 开发板
#define SYS_LED_RED_GPIO_GROUP					GPIOB
#define SYS_LED_RED_GPIO_PIN					GPIO_Pin_6
#define SYS_LED_RED_GPIO_MODE					GPIO_Mode_OUT
#define SYS_LED_RED_GPIO_SPEED					GPIO_Speed_50MHz
#define SYS_LED_RED_GPIO_OTYPE					GPIO_OType_PP
#define SYS_LED_RED_GPIO_PUPD					GPIO_PuPd_UP

#define SYS_LED_GREEN_GPIO_GROUP				GPIOB
#define SYS_LED_GREEN_GPIO_PIN					GPIO_Pin_7
#define SYS_LED_GREEN_GPIO_MODE					GPIO_Mode_OUT
#define SYS_LED_GREEN_GPIO_SPEED				GPIO_Speed_50MHz
#define SYS_LED_GREEN_GPIO_OTYPE				GPIO_OType_PP
#define SYS_LED_GREEN_GPIO_PUPD					GPIO_PuPd_UP

#define SYS_LED_BLUE_GPIO_GROUP					GPIOB
#define SYS_LED_BLUE_GPIO_PIN					GPIO_Pin_8
#define SYS_LED_BLUE_GPIO_MODE					GPIO_Mode_OUT
#define SYS_LED_BLUE_GPIO_SPEED					GPIO_Speed_50MHz
#define SYS_LED_BLUE_GPIO_OTYPE					GPIO_OType_PP
#define SYS_LED_BLUE_GPIO_PUPD					GPIO_PuPd_UP

#else
#define SYS_LED_RED_GPIO_GROUP					GPIOC
#define SYS_LED_RED_GPIO_PIN					GPIO_Pin_0
#define SYS_LED_RED_GPIO_MODE					GPIO_Mode_OUT
#define SYS_LED_RED_GPIO_SPEED					GPIO_Speed_50MHz
#define SYS_LED_RED_GPIO_OTYPE					GPIO_OType_PP
#define SYS_LED_RED_GPIO_PUPD					GPIO_PuPd_UP

#define SYS_LED_GREEN_GPIO_GROUP				GPIOF
#define SYS_LED_GREEN_GPIO_PIN					GPIO_Pin_1
#define SYS_LED_GREEN_GPIO_MODE					GPIO_Mode_OUT
#define SYS_LED_GREEN_GPIO_SPEED				GPIO_Speed_50MHz
#define SYS_LED_GREEN_GPIO_OTYPE				GPIO_OType_PP
#define SYS_LED_GREEN_GPIO_PUPD					GPIO_PuPd_UP

#define SYS_LED_BLUE_GPIO_GROUP					GPIOF
#define SYS_LED_BLUE_GPIO_PIN					GPIO_Pin_1
#define SYS_LED_BLUE_GPIO_MODE					GPIO_Mode_OUT
#define SYS_LED_BLUE_GPIO_SPEED					GPIO_Speed_50MHz
#define SYS_LED_BLUE_GPIO_OTYPE					GPIO_OType_PP
#define SYS_LED_BLUE_GPIO_PUPD					GPIO_PuPd_UP
#endif


#if 1
#define HIGH_LEVEL_VALID
#else
#define LOW_LEVEL_VALID
#endif
/*---------------------------------- 分割线 ---------------------------------- */



#endif	// #ifndef _SYS_CONFIG_H_


