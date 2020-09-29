/****************************************Copyright (c)****************************************
 																							
                             Galaxywind Network System Co., Ltd.						    
                                      												
                                  http://www.galaxywind.com								
 																						
 Author:	Y
 Date  : 	2018-03-16
 *********************************************************************************************/ 
#include "sys.h"
#include "sys_config.h"
#include "usart.h"
#include "bsp_usart2.h"


/* 串口初始化方式 */
#define USART_2_MODE			USART_INIT_DEFAULT


/* +------------------------------------------+ *
 * |	   	  		串口相关配置        	  | *
 * +------------------------------------------+ */

/* USART2  */
#define USART2_PERIPH_CLOCK_CMD_FUNC 		RCC_APB1PeriphClockCmd
#define USART2_RCC_PERIPH 					RCC_APB1Periph_USART2
#define USART2_BAUDRATE 					SYS_USART2_BAUDRATE
#define USART2_IRQCHANNEL 					USART2_IRQn

/* USART2 Tx */
#define USART2_TX_GPIO						GPIOA
#define USART2_TX_GPIO_PIN					GPIO_Pin_2
#define USART2_TX_GPIO_PINSOURCE 			GPIO_PinSource2
#define USART2_TX_GPIO_AF					GPIO_AF_1

/* USART2 Rx */
#define USART2_RX_GPIO						GPIOA
#define USART2_RX_GPIO_PIN					GPIO_Pin_3
#define USART2_RX_GPIO_PINSOURCE 			GPIO_PinSource3
#define USART2_RX_GPIO_AF					GPIO_AF_1

/* USART2 DMA */
#define USART2_DMA_PERIPH_CLOCK_CMD_FUNC 	RCC_AHBPeriphClockCmd
#define USART2_DMA_RCC_PERIPH 				RCC_AHBPeriph_DMA1
#define USART2_DMAX_CHANNELX 				DMA1_Channel4
#define USART2_DMA_IRQCHANNEL 				DMA1_Channel4_5_IRQn
#define USART2_DMAY_FLAG					DMA1_FLAG_TC4


#define USART2_DMA_TX_BUFF_SIZE 			256
#define USART2_USER_DATA_BUFF_SIZE			512

static u8 usart2_dma_tx_buff[USART2_DMA_TX_BUFF_SIZE] = {0};			// 串口DMA发送缓冲区
static u8 usart2_data_fifo_buff[USART2_USER_DATA_BUFF_SIZE] = {0};   	// 用户数据循环缓冲区


static usart_t __usart_2;

            

static void __usart2_dma_send(int event, void *data, int data_len)
{
	usart_dma_send_data(&__usart_2);
}

static void __usart2_set_dma_idle(int event, void *data, int data_len)
{
	set_dma_state_idle(&__usart_2);
}

static void __usart2_init(void)
{
	usart_init(&__usart_2, USART_2_MODE);
}

static void __usart2_fini(void)
{
	usart_fini(&__usart_2);
}


/* +------------------------------------------+ *
 * |	         main调用串口配置             | *
 * +------------------------------------------+ */
void bsp_usart2_config(void)
{
	usart_pin_config(&__usart_2,
                     USART2_TX_GPIO, 
                     USART2_TX_GPIO_PIN, 
                     USART2_TX_GPIO_PINSOURCE,
                     USART2_TX_GPIO_AF,
                     USART2_RX_GPIO, 
                     USART2_RX_GPIO_PIN,
                     USART2_RX_GPIO_PINSOURCE,
                     USART2_RX_GPIO_AF);

	usart_config(&__usart_2,
                 USART2_PERIPH_CLOCK_CMD_FUNC, 
                 USART2_RCC_PERIPH,
                 USART2_BAUDRATE, 
                 USART2);

	usart_nvic_config(&__usart_2,
                      USART2_IRQCHANNEL, 
                      USART2_PRIOR,
                      ENABLE);

	usart_dma_config(&__usart_2,
					 USART2,
                     USART2_DMA_PERIPH_CLOCK_CMD_FUNC, 
                     USART2_DMA_RCC_PERIPH,
                     USART2_DMAX_CHANNELX,
                     USART2_DMAY_FLAG,
                     usart2_dma_tx_buff,
                     sizeof(usart2_dma_tx_buff));

 	usart_dma_nvic_config(&__usart_2,
                          USART2_DMA_IRQCHANNEL, 
                          USART_DMA_PRIOR,
                          ENABLE);

	usart_fifo_config(&__usart_2, usart2_data_fifo_buff, sizeof(usart2_data_fifo_buff));

	usart_event_config(eEVENT_TIM16_UPDATE_IRQ, &__usart_2.dma_send_event, __usart2_dma_send);

	usart_event_config(eEVENT_DMA1_CH4_TC, &__usart_2.dma_tc, __usart2_set_dma_idle);

	usart_init_config(&__usart_2, __usart2_init);

	usart_fini_config(&__usart_2, __usart2_fini);

}



/* +------------------------------------------+ *
 * |	            串口初始化                | *
 * +------------------------------------------+ */
void bsp_usart2_init(void)
{
	__usart_2.init();
}


/* +------------------------------------------+ *
 * |	           串口逆初始化               | *
 * +------------------------------------------+ */
void bsp_usart2_fini(void)
{
	__usart_2.fini();
}


/* +------------------------------------------+ *
 * |	         串口发送一字节               | *
 * +------------------------------------------+ */
void usart2_send_byte(uc8 byte)
{
	usart_send_byte(&__usart_2, byte);
}


/* +------------------------------------------+ *
 * |	         串口发送N字节                | *
 * +------------------------------------------+ */
void usart2_send_data(const u8 *src, u32 data_len)
{
	usart_send_data(&__usart_2, src, data_len);
}





/* END OF FILE */

