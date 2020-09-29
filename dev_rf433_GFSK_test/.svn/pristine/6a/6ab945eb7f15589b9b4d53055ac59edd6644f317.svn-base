/*###################################*
 *#                                 #*
 *#     Author: Yu                  #*
 *#     Email : 33076156@qq.com     #*
 *#     Date  : 2020.3.10           #*
 *#                                 #*
 *###################################*/
 
#ifndef _BSP_USART2_H_
#define	_BSP_USART2_H_

#include "stm32f0xx.h"
#include <stdio.h>
#include <string.h>



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



#define __USART_GPIO_MODE				GPIO_Mode_AF		// 务必要这样设置，否则不生效
#define __USART_GPIO_SPEED				GPIO_Speed_50MHz	// 务必要这样设置，否则不生效
#define __USART_GPIO_OTYPE				GPIO_OType_PP		// 务必要这样设置，否则不生效
#define __USART_GPIO_PUPD				GPIO_PuPd_UP		// 务必要这样设置，否则不生效




void usart2_send_1byte(u8 byte);
void usart2_init(void);


#endif /* #ifndef _BSP_USART2_H_ */

