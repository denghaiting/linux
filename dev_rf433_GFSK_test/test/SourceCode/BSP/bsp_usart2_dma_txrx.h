/*###################################*
 *#                                 #*
 *#     Author: Yu                  #*
 *#     Email : 33076156@qq.com     #*
 *#     Date  : 2020.3.10           #*
 *#                                 #*
 *###################################*/
 
#ifndef _BSP_USART2_DMA_TXRX_H_
#define	_BSP_USART2_DMA_TXRX_H_

#include "stm32f0xx.h"
#include <stdio.h>
#include <string.h>


#define PUT_CHAR(x)				do {\
									usart2_send_1byte_with_dma(x);\
								} while (0);


#define USART2_BAUDRATE					921600


/* USART2  */
#define USART2_PERIPH_CLOCK_CMD_FUNC 		RCC_APB1PeriphClockCmd
#define USART2_RCC_PERIPH 					RCC_APB1Periph_USART2
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
#define USART2_USE_DMAX 					DMA1


// TX 
#define USART2_TX_DMAX_CHY_REMAP 			DMA1_CH4_USART2_TX
#define USART2_TX_DMAX_CHANNELX 			DMA1_Channel4
#define USART2_TX_DMA_IRQCHANNEL 			DMA1_Channel4_5_IRQn
#define USART2_TX_DMAY_FLAG					DMA1_FLAG_TC4

// RX 
#define USART2_RX_DMAX_CHY_REMAP 			DMA1_CH5_USART2_RX
#define USART2_RX_DMAX_CHANNELX 			DMA1_Channel5
#define USART2_RX_DMA_IRQCHANNEL 			DMA1_Channel4_5_IRQn
#define USART2_RX_DMAY_FLAG					DMA1_FLAG_TC5


#define __USART_GPIO_MODE				GPIO_Mode_AF		// 务必要这样设置，否则不生效
#define __USART_GPIO_SPEED				GPIO_Speed_50MHz	// 务必要这样设置，否则不生效
#define __USART_GPIO_OTYPE				GPIO_OType_PP		// 务必要这样设置，否则不生效
#define __USART_GPIO_PUPD				GPIO_PuPd_UP		// 务必要这样设置，否则不生效



typedef enum {
	eSTATE_IDLE = 0,
	eSTATE_BUSY = ! eSTATE_IDLE
} stat_e;





void usart2_Tx_dma_driver(void);
void usart2_Rx_proc(void);
void usart2_send_1byte_with_dma(u8 byte);
void usart2_send_data_with_dma(u8 *src, u32 data_len);
void usart2_init(void);
void usart2_fini(void);
void usart2_Rx_disable(void);
void flush_tx_data(void);

#endif /* #ifndef _BSP_USART2_DMA_TXRX_H_ */


