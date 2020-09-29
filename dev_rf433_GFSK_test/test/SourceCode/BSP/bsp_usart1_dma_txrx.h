/*###################################*
 *#                                 #*
 *#     Author: Yu                  #*
 *#     Email : 33076156@qq.com     #*
 *#     Date  : 2020.3.10           #*
 *#                                 #*
 *###################################*/
 
#ifndef _BSP_USART1_DMA_TXRX_H_
#define	_BSP_USART1_DMA_TXRX_H_

#include "stm32f0xx.h"
#include <stdio.h>
#include <string.h>


#define PUT_CHAR(x)				do {\
									usart1_send_1byte_with_dma(x);\
								} while (0);



/* USART1  */
#define USART1_PERIPH_CLOCK_CMD_FUNC 		RCC_APB2PeriphClockCmd
#define USART1_RCC_PERIPH 					RCC_APB2Periph_USART1
#define USART1_BAUDRATE 					921600
#define USART1_IRQCHANNEL 					USART1_IRQn


/* USART1 参数 */
#define USART1_WL 							USART_WordLength_8b			// 串口传输字长
#define USART1_SB 							USART_StopBits_1 			// 停止位
#define USART1_PN 							USART_Parity_No 				// 奇偶校验
#define USART1_HFC 							USART_HardwareFlowControl_None 	// 配置硬件控制流
#define USART1_MODE 						(USART_Mode_Rx | USART_Mode_Tx)	// 双线全双工


/* USART1 DMA */
#define USART1_DMA_PERIPH_CLOCK_CMD_FUNC 	RCC_AHBPeriphClockCmd
#define USART1_DMA_RCC_PERIPH 				RCC_AHBPeriph_DMA1


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

/* USART1 DMA */
#define USART1_DMA_PERIPH_CLOCK_CMD_FUNC 	RCC_AHBPeriphClockCmd
#define USART1_DMA_RCC_PERIPH 				RCC_AHBPeriph_DMA1
#define USART1_USE_DMAX 					DMA1

/* TX DMA */
#define USART1_TX_DMAX_CHY_REMAP 			DMA1_CH2_USART1_TX
#define USART1_TX_DMAX_CHANNELX 			DMA1_Channel2
#define USART1_TX_DMA_IRQCHANNEL 			DMA1_Channel2_3_IRQn
#define USART1_TX_DMAY_FLAG					DMA1_FLAG_TC2

/* RX DMA */
#define USART1_RX_DMAX_CHY_REMAP 			DMA1_CH3_USART1_RX
#define USART1_RX_DMAX_CHANNELX 			DMA1_Channel3
#define USART1_RX_DMA_IRQCHANNEL 			DMA1_Channel2_3_IRQn
#define USART1_RX_DMAY_FLAG					DMA1_FLAG_TC3


#define __USART_GPIO_MODE				GPIO_Mode_AF		// 务必要这样设置，否则不生效
#define __USART_GPIO_SPEED				GPIO_Speed_50MHz	// 务必要这样设置，否则不生效
#define __USART_GPIO_OTYPE				GPIO_OType_PP		// 务必要这样设置，否则不生效
#define __USART_GPIO_PUPD				GPIO_PuPd_UP		// 务必要这样设置，否则不生效



typedef enum {
	eSTATE_IDLE = 0,
	eSTATE_BUSY = ! eSTATE_IDLE
} stat_e;





void usart1_Tx_dma_driver(void);
void usart1_Rx_proc(void);
void usart1_send_1byte_with_dma(u8 byte);
void usart1_send_data_with_dma(u8 *src, u32 data_len);
void usart1_init(void);
void usart1_fini(void);
void usart1_Rx_disable(void);
void flush_tx_data(void);


#endif /* #ifndef _BSP_USART1_DMA_TXRX_H_ */

/* END OF FILE */

