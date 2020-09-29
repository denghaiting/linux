/*###################################*
 *#                                 #*
 *#     Author: Yu                  #*
 *#     Email : 33076156@qq.com     #*
 *#     Date  : 2020.3.10           #*
 *#                                 #*
 *###################################*/
 
#include <stdio.h>
#include "bsp_usart2.h"
#include "sys_config.h"
#include "sys_nvic_config.h"
#include "sys.h"
#include "bsp_gpio.h"
#include "fifo.h"
#include "event.h"
#include "print_color.h"



static void usart2_pin_init(void)
{
	bsp_gpio_init(USART2_TX_GPIO, 
				  USART2_TX_GPIO_PIN, 
				  __USART_GPIO_MODE, 
				  __USART_GPIO_SPEED, 
				  __USART_GPIO_OTYPE, 
				  __USART_GPIO_PUPD);
	
	bsp_gpio_init(USART2_RX_GPIO, 
				  USART2_RX_GPIO_PIN, 
				  __USART_GPIO_MODE, 
				  __USART_GPIO_SPEED, 
				  __USART_GPIO_OTYPE, 
				  __USART_GPIO_PUPD);
	

	GPIO_PinAFConfig(USART2_TX_GPIO, USART2_TX_GPIO_PINSOURCE, USART2_TX_GPIO_AF);	// GPIO复用
	GPIO_PinAFConfig(USART2_RX_GPIO, USART2_RX_GPIO_PINSOURCE, USART2_RX_GPIO_AF);	// GPIO复用
}


static void usart2_config(void)
{
	USART_InitTypeDef USART_InitStructure;
	
	/* config USART clock */
	USART2_PERIPH_CLOCK_CMD_FUNC(USART2_RCC_PERIPH, ENABLE);   

	/* USART mode config */
	USART_InitStructure.USART_BaudRate = USART2_BAUDRATE;		// 波特率
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;	// 串口传输字长
	USART_InitStructure.USART_StopBits = USART_StopBits_1;		// 停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;			// 奇偶校验
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	// 配置硬件控制流
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	// 双线全双工
	USART_Init(USART2, &USART_InitStructure);

	USART_Cmd(USART2, ENABLE); 									// 使能串口

}




/* +------------------------------------------+ *
 * |	  提供给用户的发送接口,发一个字节     | *
 * +------------------------------------------+ */
void usart2_send_1byte(u8 byte)
{	
	u16 x = byte;
	
	USART_SendData(USART2, x);

	while ( ! USART_GetFlagStatus(USART2, USART_FLAG_TC) );
}


 
void usart2_init(void)
{
	usart2_pin_init();
	
	usart2_config();
}



