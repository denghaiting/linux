/*###################################*
 *#                                 #*
 *#     Author: Yu                  #*
 *#     Email : 33076156@qq.com     #*
 *#     Date  : 2020.3.10           #*
 *#                                 #*
 *###################################*/
 
#include <stdio.h>
#include "bsp_usart1.h"
#include "sys_config.h"
#include "sys_nvic_config.h"
#include "sys.h"
#include "bsp_gpio.h"
#include "fifo.h"
#include "event.h"
#include "print_color.h"



// TODO: RX DMA
#define U1_MAX_RX_DMA_BUF_SIZE 	128
static u8 U1_Rx_dma_buf[U1_MAX_RX_DMA_BUF_SIZE];

event_t U1_Rx_dma_idle_event;


static void usart1_pin_init(void)
{
	bsp_gpio_init(USART1_TX_GPIO, 
				  USART1_TX_GPIO_PIN, 
				  __USART_GPIO_MODE, 
				  __USART_GPIO_SPEED, 
				  __USART_GPIO_OTYPE, 
				  __USART_GPIO_PUPD);
	
	bsp_gpio_init(USART1_RX_GPIO, 
				  USART1_RX_GPIO_PIN, 
				  __USART_GPIO_MODE, 
				  __USART_GPIO_SPEED, 
				  __USART_GPIO_OTYPE, 
				  __USART_GPIO_PUPD);
	

	GPIO_PinAFConfig(USART1_TX_GPIO, USART1_TX_GPIO_PINSOURCE, USART1_TX_GPIO_AF);	// GPIO复用
	GPIO_PinAFConfig(USART1_RX_GPIO, USART1_RX_GPIO_PINSOURCE, USART1_RX_GPIO_AF);	// GPIO复用
}


static void usart1_config(void)
{
	USART_InitTypeDef USART_InitStructure;
	
	/* config USART clock */
	USART1_PERIPH_CLOCK_CMD_FUNC(USART1_RCC_PERIPH, ENABLE);   

	/* USART mode config */
	USART_InitStructure.USART_BaudRate = USART1_BAUDRATE;		// 波特率
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;	// 串口传输字长
	USART_InitStructure.USART_StopBits = USART_StopBits_1;		// 停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;			// 奇偶校验
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	// 配置硬件控制流
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	// 双线全双工
	USART_Init(USART1, &USART_InitStructure);
	
	nvic_config(USART1_IRQCHANNEL, USART1_PRIOR, ENABLE);
	
	USART_ITConfig(USART1, USART_IT_IDLE, ENABLE);
	
	USART_Cmd(USART1, ENABLE); 									// 使能串口

	USART_DMACmd(USART1, USART_DMAReq_Rx, ENABLE);

}




/* +------------------------------------------+ *
 * |	  提供给用户的发送接口,发一个字节     | *
 * +------------------------------------------+ */
void usart1_send_1byte(u8 byte)
{	
	u16 x = byte;
	
	USART_SendData(USART1, x);

	while ( ! USART_GetFlagStatus(USART1, USART_FLAG_TC) );
}

void usart1_send_data(u8 *src, u32 data_len)
{	
	u32 i = 0;

	for (i = 0; i < data_len; i++) {
		usart1_send_1byte(src[i]);
	}
}


static void usart1_Rx_dma_config(void)
{		
	DMA_InitTypeDef dma_initstructure;

	DMA_RemapConfig(USART1_USE_DMAX, USART1_RX_DMAX_CHY_REMAP);

	/* 开启DMA时钟 */
	USART1_DMA_PERIPH_CLOCK_CMD_FUNC(USART1_DMA_RCC_PERIPH, ENABLE);

	/* 设置DMA源:内存地址 & 串口数据寄存器地址 */  
	dma_initstructure.DMA_PeripheralBaseAddr = (u32)&USART1->RDR;
	/* 内存地址(要传输变量的指针) */
	dma_initstructure.DMA_MemoryBaseAddr = (u32)&U1_Rx_dma_buf[0];	 
	/* 方向:从外设到内存 外设作为源地址 */
	dma_initstructure.DMA_DIR = DMA_DIR_PeripheralSRC;
	/* 传输大小 */
	dma_initstructure.DMA_BufferSize = sizeof(U1_Rx_dma_buf); 
	/* 外设地址固定 */
	dma_initstructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	/* 内存地址自增 */
	//dma_initstructure.DMA_MemoryInc = DMA_MemoryInc_Disable;
	dma_initstructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	/* 外设数据单位 */
	dma_initstructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	/* 内存数据单位 */
	dma_initstructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	/* DMA模式 Normal表示只发送一次 Circular表示在发送完一轮数据以后再重新传输 */
	dma_initstructure.DMA_Mode = DMA_Mode_Circular;
	//dma_initstructure.DMA_Mode = DMA_Mode_Normal;
	/* 优先级 */
	dma_initstructure.DMA_Priority = DMA_Priority_High;
	/* 禁止内存到内存的传输 */
	dma_initstructure.DMA_M2M = DMA_M2M_Disable;

	DMA_Init(USART1_RX_DMAX_CHANNELX, &dma_initstructure);

	/* 使能DMA */

	//DMA_Cmd(USART1_RX_DMAX_CHANNELX, DISABLE ); 
	//DMA_SetCurrDataCounter(USART1_RX_DMAX_CHANNELX, 16);
	DMA_Cmd(USART1_RX_DMAX_CHANNELX, ENABLE); 
}




typedef enum {
	eUSART_RX_STATE_WAIT_DMA = 0,	
	eUSART_RX_STATE_PROCESS
} usart_rx_state_e;


vu8 u1_dma_rx_state = eUSART_RX_STATE_WAIT_DMA;
u16 u1_dma_rx_len = 0;



void usart1_Rx_dma_IT_IDLE(int event, void *data, int data_len)
{
	u16 dma_recv_len;

    DMA_Cmd(USART1_RX_DMAX_CHANNELX, DISABLE);  /* 暂时关闭dma，数据尚未处理 */
	
	dma_recv_len = sizeof(U1_Rx_dma_buf) - DMA_GetCurrDataCounter(USART1_RX_DMAX_CHANNELX);
		
	u1_dma_rx_len = dma_recv_len;

	//PUT_CHAR(u1_dma_rx_len + '0');
	
	u1_dma_rx_state = eUSART_RX_STATE_PROCESS;
}


void usart1_Rx_proc(void)
{	
	if (u1_dma_rx_state != eUSART_RX_STATE_PROCESS) {
		return;
	}
	
	//test_show_rx(U1_Rx_dma_buf, u1_dma_rx_len);
	
	event_call(eEVENT_USART1_READ_DATA, U1_Rx_dma_buf, u1_dma_rx_len);
	
	u1_dma_rx_state = eUSART_RX_STATE_WAIT_DMA;
	u1_dma_rx_len = 0;
	DMA_SetCurrDataCounter(USART1_RX_DMAX_CHANNELX, sizeof(U1_Rx_dma_buf));
	DMA_Cmd(USART1_RX_DMAX_CHANNELX, ENABLE);
}




void usart1_Rx_dma_init(void)
{
	event_add(eEVENT_USART1_IDLE, &U1_Rx_dma_idle_event, usart1_Rx_dma_IT_IDLE);
	
	usart1_Rx_dma_config();

	//nvic_config(USART1_IRQCHANNEL, USART1_PRIOR, ENABLE);
	
	//USART_ITConfig(USART1, USART_IT_IDLE, ENABLE);

	//USART_DMACmd(USART1, USART_DMAReq_Rx, ENABLE);

}


 
void usart1_init(void)
{
	usart1_pin_init();
	
	usart1_Rx_dma_init(); // 使能接收DMA

	usart1_config();


}



#if 0
void usart1_fini(void)
{	
	DMA_ITConfig(USART1_TX_DMAX_CHANNELX, DMA_IT_TC, DISABLE);	
	DMA_Cmd(USART1_TX_DMAX_CHANNELX, DISABLE);	
	DMA_Cmd(USART1_RX_DMAX_CHANNELX, DISABLE);
	
	USART_DMACmd(USART1, USART_DMAReq_Tx, DISABLE);
	USART_DMACmd(USART1, USART_DMAReq_Rx, DISABLE);
	
	nvic_config(USART1_IRQCHANNEL, USART1_GRAB_PRIOR, USART1_RESPONSE_PRIOR, DISABLE);
	nvic_config(USART1_TX_DMA_IRQCHANNEL, USART_TX_DMA_GRAB_PRIOR, USART_TX_DMA_RESPONSE_PRIOR, DISABLE);
		
	USART_ITConfig(USART1, USART_IT_IDLE, DISABLE);
	USART_Cmd(USART1, DISABLE); 

	USART1_PERIPH_CLOCK_CMD_FUNC(USART1_RCC_PERIPH, DISABLE);

	bsp_gpio_fini(USART1_TX_GPIO, USART1_TX_GPIO_PIN);
	bsp_gpio_fini(USART1_RX_GPIO, USART1_RX_GPIO_PIN);	


}
#endif



