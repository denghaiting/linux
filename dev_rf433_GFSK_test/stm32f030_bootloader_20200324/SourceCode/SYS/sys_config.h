/****************************************Copyright (c)****************************************
 																							
                             Galaxywind Network System Co., Ltd.						    
                                      												
                                  http://www.galaxywind.com								
 																						
 Author:	Y
 Date  : 	2020-03-25
 *********************************************************************************************/ 
#ifndef __SYS_CONFIG_H__
#define __SYS_CONFIG_H__


/*
==============================================================
 	STM32F030C8
          |  ||
          |  |+-----------------------FLASH=64K,RAM=8K
          |  +-------------- 48PIN
          +----------M0 core
==============================================================
 */


// TODO: FLASH 划分
/*  =====================     内存使用说明  总大小 8K Byte     ===================  
 *	+----------------------------------+-------------------+-------------------+ *
 *	|                                  |        说明       |     大小(OCT)     | *
 *	+----------------------------------+-------------------+-------------------+ *
 *	|            Stack_Size            |         栈        |       3072        | *
 *	+----------------------------------+-------------------+-------------------+ *
 *	|            Heap_Size             |         堆        |          0        | *
 *  +----------------------------------+-------------------+-------------------+ *
 *	|      USART_DMA_TX_BUFF_SIZE      | 串口DMA发送缓冲区 |        512        | *
 *	+----------------------------------+-------------------+-------------------+ *
 *	|        TX_FIFO_BUFF_SIZE         |        FIFO       |       1024        | *
 *	+----------------------------------+-------------------+-------------------+ */

/*  =====================    FLSAH使用说明  总大小 64K Byte    ===================  
 *	+-------------+--------------------+-------------------+--------------------+ *
 *	|   起始地址  |        备注        |     大小(HEX)     |        说明        | *
 *	+-------------+--------------------+-------------------+--------------------+ *
 *	|  0x08000000 |       固定 16K     |      0x4000       |      BOOT镜像      | *
 *	+-------------+--------------------+-------------------+--------------------+ *
 *	|  0x08004000 |       固定 46K     |      0xC000       |       APP镜像      | *
 *	+-------------+--------------------+-------------------+--------------------+ *
 *	|  0x0800F800 |       固定 1K      |      0x400        |   ADDR存储页(保留) | *
 *	+-------------+--------------------+-------------------+--------------------+ *
 *	|  0x0800FC00 |       固定 1K      |      0x400        |       ADDR存储页   | *
 *	+-------------+--------------------+-------------------+--------------------+ *
 *	|  0x0800FFFF |    FLASH最高地址   |      --  --       |       --  --       | *
 *  +-------------+--------------------+-------------------+--------------------+ *
 *  =====================         以上空间可以重新规划        =================== */ 



// TODO: 内存大小
#define RAM_TOTAL_SIZE 						((u32)0x2000UL) 

// TODO: FLASH大小
#define FLASH_TOTAL_SIZE 					((u32)0x10000UL) /*  64 KBytes FLASH总大小 */

// TODO: FLASH起始地址
#define FLASH_HEAD_ADDR 					((u32)0x08000000UL) 

// TODO: FLASH结束地址
#define FLASH_END_ADDR						((u32)0x0800FFFFUL)	


// TODO: BOOT镜像占用空间
#define BOOT_IMAGE_SIZE						(0x3C00UL)   /* 15 KBytes BOOT镜像最大空间 */

// TODO: BOOT占用空间
#define BOOT_USE_SIZE						(0x4000UL)   /* 16 KBytes BOOT镜像 + 存储 */

// TODO: BOOT存储页1
#define BOOT_USE_PAGE0						(FLASH_HEAD_ADDR + BOOT_IMAGE_SIZE)   


// TODO: APP镜像占用空间
#define APP_IMAGE_SIZE						(0xB800UL)  /* 46 KBytes APP镜像最大空间 */

// TODO: APP程序首地址
#define APP_IMAGE_START						(FLASH_HEAD_ADDR + BOOT_USE_SIZE)   

// TODO: 跳转至APP运行地址
#define APP_IMAGE_ADDR      				(APP_IMAGE_START)  


// TODO: LOADER 升级时能够使用的起始地址
#define USER_FLASH_START_ADDR				(FLASH_HEAD_ADDR + BOOT_IMAGE_SIZE)	

// TODO: LOADER 升级时能够使用的结束地址
#define USER_FLASH_END_ADDR 				FLASH_END_ADDR






/*---------------------------------- 分割线 ---------------------------------- */

/* === Watch Dog 分频系数 === */
#define __CFG__WATCHDOG_PRESCALER 	IWDG_Prescaler_128 


/* === 系统时钟配置 === */
#if 1	
#define INNER_CLOCK	 // 选择内部时钟 目前都用的是内部时钟
#else	
#define EXTERN_CLOCK // 选择外部时钟 且必须在startup_stm32f030.s 恢复使用SystemInit
#endif

/*---------------------------------- 分割线 ---------------------------------- */

// TODO:  配置串口 
/* 波特率配置 */
#define SYS_USART1_BAUDRATE		921600
//#define SYS_USART1_BAUDRATE		115200

#define SYS_USART2_BAUDRATE		921600	
//#define SYS_USART2_BAUDRATE		115200	

#if 1
#define USE_USART_1
#else
#define USE_USART_2
#endif

/*---------------------------------- 分割线 ---------------------------------- */

// TODO: 看门狗配置
/* 看门狗开关 */
#if 0 
#define ENABLE_IWDG
#endif

#if defined(ENABLE_IWDG)
#define IWDG_TIME_STEP 		eIWDG_TIME_STEP_3P2_MS	
#define IWDG_RELOAD_VALUE 	MAX_IWDG_RELOAD_VALUE	
/* (0xFFF + 1) * 3.2ms = 13107.2S */
#endif

/*---------------------------------- 分割线 ---------------------------------- */







/* 中断优先等级划分 STM32F030只有0 - 3 */
typedef enum {
	PRIORITY_HIGHEST = 0,
	PRIORITY_1,
	PRIORITY_2,
	PRIORITY_3,
} NVIC_PRIORITY;



/* 此处配置智能插座所有中断的优先级 */
#define USART1_PRIOR			PRIORITY_1
#define USART2_PRIOR			PRIORITY_1
#define USART_DMA_PRIOR			PRIORITY_1
#define SPI_IRQ_PRIOR			PRIORITY_1
#define TIM17_PRIOR				PRIORITY_2	//usart DMA update
#define TIM16_PRIOR				PRIORITY_2	//usart DMA update
#define TIM15_PRIOR				PRIORITY_2	
#define TIM3_PRIOR				PRIORITY_2	
#define ADC_DMA_PRIOR			PRIORITY_2	//ADC



#endif /* #ifndef __SYS_CONFIG_H__ */

/* END OF FILE */


