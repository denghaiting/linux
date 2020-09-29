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


// TODO: FLASH ����
/*  =====================     �ڴ�ʹ��˵��  �ܴ�С 8K Byte     ===================  
 *	+----------------------------------+-------------------+-------------------+ *
 *	|                                  |        ˵��       |     ��С(OCT)     | *
 *	+----------------------------------+-------------------+-------------------+ *
 *	|            Stack_Size            |         ջ        |       3072        | *
 *	+----------------------------------+-------------------+-------------------+ *
 *	|            Heap_Size             |         ��        |          0        | *
 *  +----------------------------------+-------------------+-------------------+ *
 *	|      USART_DMA_TX_BUFF_SIZE      | ����DMA���ͻ����� |        512        | *
 *	+----------------------------------+-------------------+-------------------+ *
 *	|        TX_FIFO_BUFF_SIZE         |        FIFO       |       1024        | *
 *	+----------------------------------+-------------------+-------------------+ */

/*  =====================    FLSAHʹ��˵��  �ܴ�С 64K Byte    ===================  
 *	+-------------+--------------------+-------------------+--------------------+ *
 *	|   ��ʼ��ַ  |        ��ע        |     ��С(HEX)     |        ˵��        | *
 *	+-------------+--------------------+-------------------+--------------------+ *
 *	|  0x08000000 |       �̶� 16K     |      0x4000       |      BOOT����      | *
 *	+-------------+--------------------+-------------------+--------------------+ *
 *	|  0x08004000 |       �̶� 46K     |      0xC000       |       APP����      | *
 *	+-------------+--------------------+-------------------+--------------------+ *
 *	|  0x0800F800 |       �̶� 1K      |      0x400        |   ADDR�洢ҳ(����) | *
 *	+-------------+--------------------+-------------------+--------------------+ *
 *	|  0x0800FC00 |       �̶� 1K      |      0x400        |       ADDR�洢ҳ   | *
 *	+-------------+--------------------+-------------------+--------------------+ *
 *	|  0x0800FFFF |    FLASH��ߵ�ַ   |      --  --       |       --  --       | *
 *  +-------------+--------------------+-------------------+--------------------+ *
 *  =====================         ���Ͽռ�������¹滮        =================== */ 



// TODO: �ڴ��С
#define RAM_TOTAL_SIZE 						((u32)0x2000UL) 

// TODO: FLASH��С
#define FLASH_TOTAL_SIZE 					((u32)0x10000UL) /*  64 KBytes FLASH�ܴ�С */

// TODO: FLASH��ʼ��ַ
#define FLASH_HEAD_ADDR 					((u32)0x08000000UL) 

// TODO: FLASH������ַ
#define FLASH_END_ADDR						((u32)0x0800FFFFUL)	


// TODO: BOOT����ռ�ÿռ�
#define BOOT_IMAGE_SIZE						(0x3C00UL)   /* 15 KBytes BOOT�������ռ� */

// TODO: BOOTռ�ÿռ�
#define BOOT_USE_SIZE						(0x4000UL)   /* 16 KBytes BOOT���� + �洢 */

// TODO: BOOT�洢ҳ1
#define BOOT_USE_PAGE0						(FLASH_HEAD_ADDR + BOOT_IMAGE_SIZE)   


// TODO: APP����ռ�ÿռ�
#define APP_IMAGE_SIZE						(0xB800UL)  /* 46 KBytes APP�������ռ� */

// TODO: APP�����׵�ַ
#define APP_IMAGE_START						(FLASH_HEAD_ADDR + BOOT_USE_SIZE)   

// TODO: ��ת��APP���е�ַ
#define APP_IMAGE_ADDR      				(APP_IMAGE_START)  


// TODO: LOADER ����ʱ�ܹ�ʹ�õ���ʼ��ַ
#define USER_FLASH_START_ADDR				(FLASH_HEAD_ADDR + BOOT_IMAGE_SIZE)	

// TODO: LOADER ����ʱ�ܹ�ʹ�õĽ�����ַ
#define USER_FLASH_END_ADDR 				FLASH_END_ADDR






/*---------------------------------- �ָ��� ---------------------------------- */

/* === Watch Dog ��Ƶϵ�� === */
#define __CFG__WATCHDOG_PRESCALER 	IWDG_Prescaler_128 


/* === ϵͳʱ������ === */
#if 1	
#define INNER_CLOCK	 // ѡ���ڲ�ʱ�� Ŀǰ���õ����ڲ�ʱ��
#else	
#define EXTERN_CLOCK // ѡ���ⲿʱ�� �ұ�����startup_stm32f030.s �ָ�ʹ��SystemInit
#endif

/*---------------------------------- �ָ��� ---------------------------------- */

// TODO:  ���ô��� 
/* ���������� */
#define SYS_USART1_BAUDRATE		921600
//#define SYS_USART1_BAUDRATE		115200

#define SYS_USART2_BAUDRATE		921600	
//#define SYS_USART2_BAUDRATE		115200	

#if 1
#define USE_USART_1
#else
#define USE_USART_2
#endif

/*---------------------------------- �ָ��� ---------------------------------- */

// TODO: ���Ź�����
/* ���Ź����� */
#if 0 
#define ENABLE_IWDG
#endif

#if defined(ENABLE_IWDG)
#define IWDG_TIME_STEP 		eIWDG_TIME_STEP_3P2_MS	
#define IWDG_RELOAD_VALUE 	MAX_IWDG_RELOAD_VALUE	
/* (0xFFF + 1) * 3.2ms = 13107.2S */
#endif

/*---------------------------------- �ָ��� ---------------------------------- */







/* �ж����ȵȼ����� STM32F030ֻ��0 - 3 */
typedef enum {
	PRIORITY_HIGHEST = 0,
	PRIORITY_1,
	PRIORITY_2,
	PRIORITY_3,
} NVIC_PRIORITY;



/* �˴��������ܲ��������жϵ����ȼ� */
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


