/****************************************Copyright (c)****************************************
 																							
                             Galaxywind Network System Co., Ltd.						    
                                      												
                                  http://www.galaxywind.com								
 																						
 Author:	Y
 Date  : 	2018-03-16
 *********************************************************************************************/ 
#include "sys.h"
#include "sys_config.h"
#include "usart.h"
#include "bsp_usart1.h"



/* 串口初始化方式 */
#define USART_1_MODE			USART_INIT_DEFAULT


/* USART1  */
#define USART1_PERIPH_CLOCK_CMD_FUNC 		RCC_APB2PeriphClockCmd
#define USART1_RCC_PERIPH 					RCC_APB2Periph_USART1
#define USART1_BAUDRATE 					SYS_USART1_BAUDRATE
#define USART1_IRQCHANNEL 					USART1_IRQn


#if 0

/* USART1 Tx */
#define USART1_TX_GPIO						GPIOA
#define USART1_TX_GPIO_PIN					GPIO_Pin_9
#define USART1_TX_GPIO_PINSOURCE 			GPIO_PinSource9
#define USART1_TX_GPIO_AF					GPIO_AF_1

/* USART1 Rx */
#define USART1_RX_GPIO						GPIOA
#define USART1_RX_GPIO_PIN					GPIO_Pin_10
#define USART1_RX_GPIO_PINSOURCE 			GPIO_PinSource10
#define USART1_RX_GPIO_AF					GPIO_AF_1

#else

/* USART1 Tx */
#define USART1_TX_GPIO						GPIOB
#define USART1_TX_GPIO_PIN					GPIO_Pin_6
#define USART1_TX_GPIO_PINSOURCE 			GPIO_PinSource6
#define USART1_TX_GPIO_AF					GPIO_AF_0

/* USART1 Rx */
#define USART1_RX_GPIO						GPIOB
#define USART1_RX_GPIO_PIN					GPIO_Pin_7
#define USART1_RX_GPIO_PINSOURCE 			GPIO_PinSource7
#define USART1_RX_GPIO_AF					GPIO_AF_0


#endif


/* USART1 DMA */
#define USART1_DMA_PERIPH_CLOCK_CMD_FUNC 	RCC_AHBPeriphClockCmd
#define USART1_DMA_RCC_PERIPH 				RCC_AHBPeriph_DMA1
#define USART1_DMAX_CHANNELX 				DMA1_Channel2
#define USART1_DMA_IRQCHANNEL 				DMA1_Channel2_3_IRQn
#define USART1_DMAY_FLAG					DMA1_FLAG_TC2



#define USART1_DMA_TX_BUFF_SIZE 			512
#define USART1_USER_DATA_BUFF_SIZE			1024

static u8 usart1_dma_tx_buff[USART1_DMA_TX_BUFF_SIZE] = {0};			// 串口DMA发送缓冲区
static u8 usart1_user_data_buff[USART1_USER_DATA_BUFF_SIZE] = {0};   	// 用户数据循环缓冲区


static usart_t __usart_1;

            

static void __usart1_dma_send(int event, void *data, int data_len)
{
	usart_dma_send_data(&__usart_1);
}

static void __usart1_set_dma_idle(int event, void *data, int data_len)
{
	set_dma_state_idle(&__usart_1);
}

static void __usart1_init(void)
{
	usart_init(&__usart_1, USART_1_MODE);
}

static void __usart1_fini(void)
{
	usart_fini(&__usart_1);
}


/* +------------------------------------------+ *
 * |	         main调用串口配置             | *
 * +------------------------------------------+ */
void bsp_usart1_config(void)
{
	usart_pin_config(&__usart_1,
                     USART1_TX_GPIO, 
                     USART1_TX_GPIO_PIN, 
                     USART1_TX_GPIO_PINSOURCE,
                     USART1_TX_GPIO_AF,
                     USART1_RX_GPIO, 
                     USART1_RX_GPIO_PIN,
                     USART1_RX_GPIO_PINSOURCE,
                     USART1_RX_GPIO_AF);

	usart_config(&__usart_1,
                 USART1_PERIPH_CLOCK_CMD_FUNC, 
                 USART1_RCC_PERIPH,
                 USART1_BAUDRATE, 
                 USART1);

	usart_nvic_config(&__usart_1,
                      USART1_IRQCHANNEL, 
                      USART1_PRIOR, 
                      ENABLE);

	usart_dma_config(&__usart_1,
					 USART1,
                     USART1_DMA_PERIPH_CLOCK_CMD_FUNC, 
                     USART1_DMA_RCC_PERIPH,
                     USART1_DMAX_CHANNELX,
                     USART1_DMAY_FLAG,
                     usart1_dma_tx_buff,
                     sizeof(usart1_dma_tx_buff));

 	usart_dma_nvic_config(&__usart_1,
                          USART1_DMA_IRQCHANNEL, 
                          USART_DMA_PRIOR, 
                          ENABLE);

	usart_fifo_config(&__usart_1, usart1_user_data_buff, sizeof(usart1_user_data_buff));

	usart_event_config(eEVENT_TIM16_UPDATE_IRQ, &__usart_1.dma_send_event, __usart1_dma_send);

	usart_event_config(eEVENT_DMA1_CH2_TC, &__usart_1.dma_tc, __usart1_set_dma_idle);

	usart_init_config(&__usart_1, __usart1_init);

	usart_fini_config(&__usart_1, __usart1_fini);

}



/* +------------------------------------------+ *
 * |	            串口初始化                | *
 * +------------------------------------------+ */
void bsp_usart1_init(void)
{
	__usart_1.init();
}


/* +------------------------------------------+ *
 * |	           串口逆初始化               | *
 * +------------------------------------------+ */
void bsp_usart1_fini(void)
{
	__usart_1.fini();
}


/* +------------------------------------------+ *
 * |	         串口发送一字节               | *
 * +------------------------------------------+ */
void usart1_send_byte(uc8 byte)
{
	usart_send_byte(&__usart_1, byte);
}


/* +------------------------------------------+ *
 * |	         串口发送N字节                | *
 * +------------------------------------------+ */
void usart1_send_data(const u8 *src, u32 data_len)
{
	usart_send_data(&__usart_1, src, data_len);
}


/* +------------------------------------------+ *
 * |	         关闭接收中断函数             | *
 * +------------------------------------------+ */
void usart1_irq_close(void)
{
	usart_irq_close(&__usart_1);
}


/* +------------------------------------------+ *
 * |	         开启接收中断函数             | *
 * +------------------------------------------+ */
void usart1_irq_open(void)
{
	usart_irq_open(&__usart_1);
}


/* END OF FILE */

