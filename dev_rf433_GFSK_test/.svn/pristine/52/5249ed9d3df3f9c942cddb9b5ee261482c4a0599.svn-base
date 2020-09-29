/****************************************Copyright (c)****************************************
 																							
                             Galaxywind Network System Co., Ltd.						    
                                      												
                                  http://www.galaxywind.com								
 																						
 Author:	Y
 Date  : 	2018-03-16
 *********************************************************************************************/  
#include "sys.h"
#include "bsp_gpio.h"
#include "usart.h"



#define __USART_GPIO_MODE				GPIO_Mode_AF		// 务必要这样设置，否则不生效
#define __USART_GPIO_SPEED				GPIO_Speed_50MHz	// 务必要这样设置，否则不生效
#define __USART_GPIO_OTYPE				GPIO_OType_PP		// 务必要这样设置，否则不生效
#define __USART_GPIO_PUPD				GPIO_PuPd_UP		// 务必要这样设置，否则不生效



static void __usart_pin_init(GPIO_TypeDef *tx_gpio, u32 tx_pin, u16 tx_pinsource, u8 tx_af, 
                             GPIO_TypeDef *rx_gpio, u32 rx_pin, u16 rx_pinsource, u8 rx_af)
{
	bsp_gpio_init(tx_gpio, tx_pin, __USART_GPIO_MODE, __USART_GPIO_SPEED, __USART_GPIO_OTYPE, __USART_GPIO_PUPD);
	bsp_gpio_init(rx_gpio, rx_pin, __USART_GPIO_MODE, __USART_GPIO_SPEED, __USART_GPIO_OTYPE, __USART_GPIO_PUPD);	

	GPIO_PinAFConfig(tx_gpio, tx_pinsource, tx_af);	// GPIO复用
	GPIO_PinAFConfig(rx_gpio, rx_pinsource, rx_af);	// GPIO复用
}


static void _usart_pin_init(usart_t *pusart)
{
	if (NULL == pusart) {
		return;
	}
	
	__usart_pin_init(pusart->usart_pin_conf.tx_gpio,
		             pusart->usart_pin_conf.tx_pin,
		             pusart->usart_pin_conf.tx_pinsource,
		             pusart->usart_pin_conf.tx_af,
		             pusart->usart_pin_conf.rx_gpio,
		             pusart->usart_pin_conf.rx_pin,
		             pusart->usart_pin_conf.rx_pinsource,
		             pusart->usart_pin_conf.rx_af);
}




static void __usart_init(void (*usart_periphclockcmd_func)(u32, FunctionalState),
						 u32 usart_rcc_periph,
                         u32 baudrate, 
                         USART_TypeDef *usartx)
{
	USART_InitTypeDef USART_InitStructure;

	if ( ! usart_periphclockcmd_func) {
		return;
	}
	
	/* config USART clock */
	usart_periphclockcmd_func(usart_rcc_periph, ENABLE);   

	/* USART mode config */
	USART_InitStructure.USART_BaudRate = baudrate;		// 波特率
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;	// 串口传输字长
	USART_InitStructure.USART_StopBits = USART_StopBits_1;		// 停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;			// 奇偶校验
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	// 配置硬件控制流
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	// 双线全双工
	USART_Init(usartx, &USART_InitStructure);
	//USART_ITConfig(usartx, USART_IT_RXNE, ENABLE);			// 配置串口中断
	USART_Cmd(usartx, ENABLE); 									// 使能串口中断
}


static void _usart_init(usart_t *pusart)
{
	if (NULL == pusart) {
		return;
	}

	__usart_init(pusart->usart_conf.usart_periphclockcmd_func,
		         pusart->usart_conf.usart_rcc_periph,
		         pusart->usart_conf.baudrate,
		         pusart->usartx);
}


static void __usart_nvic_init(u8 irq_channel, u8 prior, FunctionalState state)
{
	nvic_config(irq_channel, prior, state);
}


static void _usart_nvic_init(usart_t *pusart)
{
	if (NULL == pusart) {
		return;
	}

	__usart_nvic_init(pusart->usart_conf.usart_irq_channel,
		              pusart->usart_conf.usart_prior,
		              pusart->usart_conf.usart_irq_state ? ENABLE : DISABLE);
	
	USART_ITConfig(pusart->usartx, USART_IT_RXNE, ENABLE);		// 配置串口中断

}


static void __usart_dma_init(USART_TypeDef *usartx,
							 void (*dma_periphclockcmd_func)(u32, FunctionalState),
						     u32 dma_rcc_periph, 
						     DMA_Channel_TypeDef *dmax_channelx,
							 u32 dma_memorybaseaddr,
							 u32 dma_buffersize,
                             DMA_InitTypeDef *p_dma_initstructure)
{
	if ( ! dma_periphclockcmd_func)
		return;
	
	/* 开启DMA时钟 */
	dma_periphclockcmd_func(dma_rcc_periph, ENABLE);

	/* 设置DMA源:内存地址 & 串口数据寄存器地址 */  
	p_dma_initstructure->DMA_PeripheralBaseAddr = (u32)&usartx->TDR;
	/* 内存地址(要传输变量的指针) */
	p_dma_initstructure->DMA_MemoryBaseAddr = dma_memorybaseaddr;	 
	/* 方向:从内存到外设 外设作为目标地址 */
	p_dma_initstructure->DMA_DIR = DMA_DIR_PeripheralDST;
	/* 传输大小 */
	p_dma_initstructure->DMA_BufferSize = dma_buffersize;
	/* 外设地址固定 */
	p_dma_initstructure->DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	/* 内存地址自增 */
	p_dma_initstructure->DMA_MemoryInc = DMA_MemoryInc_Enable;
	/* 外设数据单位 */
	p_dma_initstructure->DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	/* 内存数据单位 */
	p_dma_initstructure->DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	/* DMA模式 Normal表示只发送一次 Circular表示在发送完一轮数据以后再重新传输 */
	p_dma_initstructure->DMA_Mode = DMA_Mode_Normal;
	/* 优先级 */
	p_dma_initstructure->DMA_Priority = DMA_Priority_High;
	/* 禁止内存到内存的传输 */
	p_dma_initstructure->DMA_M2M = DMA_M2M_Disable;

	DMA_Init(dmax_channelx, p_dma_initstructure);

	/* 使能DMA */
	DMA_Cmd(dmax_channelx, ENABLE);

	/* 配置DMA发送完成后产生中断 */
	//DMA_ITConfig(dmax_channelx, DMA_IT_TC, ENABLE);	
}


static void _usart_dma_init(usart_t *pusart)
{
	if (NULL == pusart) {
		return;
	}

	__usart_dma_init(pusart->usartx,
					 pusart->usart_dma_conf.dma_periphclockcmd_func,
		             pusart->usart_dma_conf.dma_rcc_periph,
		             pusart->usart_dma_conf.dmax_channelx,
		             (u32)&pusart->usart_dma_conf.dma_tx_buf_ptr[0],
		             pusart->usart_dma_conf.dma_tx_buf_size,
		             &pusart->usart_dma_conf.dma_init_struct);
}


static void __usart_dma_nvic_init(u8 irq_channel, u8 prior, FunctionalState state)
{
	nvic_config(irq_channel, prior, state);
}


static void _usart_dma_nvic_init(usart_t *pusart)
{
	if (NULL == pusart) {
		return;
	}

	__usart_dma_nvic_init(pusart->usart_dma_conf.usart_dma_irq_channel,
			              pusart->usart_dma_conf.usart_dma_prior,
			              pusart->usart_dma_conf.usart_dma_irq_state ? ENABLE : DISABLE);

	/* 配置DMA发送完成后产生中断 */
	DMA_ITConfig(pusart->usart_dma_conf.dmax_channelx, DMA_IT_TC, ENABLE);	
}


/* +------------------------------------------+ *
 * |	            初始化函数                | *
 * +------------------------------------------+ */
void usart_init(usart_t *pusart, init_mode_t mode)
{
	_usart_pin_init(pusart);	
	_usart_init(pusart);

	switch (mode) {
	case USART_INIT_DEFAULT:
		_usart_nvic_init(pusart);
		break;

	case USART_INIT_PRINT_ONLY:

		break;
		
	default:
		break;
	}
	
	_usart_dma_init(pusart);
	_usart_dma_nvic_init(pusart);
}


/* +------------------------------------------+ *
 * |	            逆初始化函数              | *
 * +------------------------------------------+ */
void usart_fini(usart_t *pusart)
{
	if (NULL == pusart) {
		return;
	}
	
	DMA_ITConfig(pusart->usart_dma_conf.dmax_channelx, DMA_IT_TC, DISABLE);	
	DMA_Cmd(pusart->usart_dma_conf.dmax_channelx, DISABLE);
	DMA_DeInit(pusart->usart_dma_conf.dmax_channelx);
	
	__usart_nvic_init(pusart->usart_dma_conf.usart_dma_irq_channel,
		              pusart->usart_dma_conf.usart_dma_prior,
		              DISABLE);

	USART_ITConfig(pusart->usartx, USART_IT_RXNE, DISABLE);				
	USART_Cmd(pusart->usartx, DISABLE);
	USART_DeInit(pusart->usartx);

	__usart_nvic_init(pusart->usart_conf.usart_irq_channel,
		              pusart->usart_conf.usart_prior,
		              DISABLE);
	
	pusart->usart_conf.usart_periphclockcmd_func(pusart->usart_conf.usart_rcc_periph,
		                                         DISABLE);	
	
	bsp_gpio_fini(pusart->usart_pin_conf.tx_gpio, 
		          pusart->usart_pin_conf.tx_pin);

	bsp_gpio_fini(pusart->usart_pin_conf.rx_gpio, 
		          pusart->usart_pin_conf.rx_pin);

}


/* +------------------------------------------+ *
 * |	         关闭接收中断函数             | *
 * +------------------------------------------+ */
void usart_irq_close(usart_t *pusart)
{
	__usart_nvic_init(pusart->usart_dma_conf.usart_dma_irq_channel,
		              pusart->usart_dma_conf.usart_dma_prior,
		              DISABLE);

	USART_ITConfig(pusart->usartx, USART_IT_RXNE, DISABLE);
}


/* +------------------------------------------+ *
 * |	         开启接收中断函数             | *
 * +------------------------------------------+ */
void usart_irq_open(usart_t *pusart)
{
	__usart_nvic_init(pusart->usart_dma_conf.usart_dma_irq_channel,
		              pusart->usart_dma_conf.usart_dma_prior,
		              ENABLE);

	USART_ITConfig(pusart->usartx, USART_IT_RXNE, ENABLE);
}



/* +------------------------------------------+ *
 * |	       	 Rx Tx  引脚配置              | *
 * +------------------------------------------+ */
void usart_pin_config(usart_t *pusart, 
                      GPIO_TypeDef *tx_gpio, u32 tx_pin, u16 tx_pinsource, u8 tx_af, 
                      GPIO_TypeDef *rx_gpio, u32 rx_pin, u16 rx_pinsource, u8 rx_af)
{
	if (NULL == pusart) {
		return;
	}
	
	pusart->usart_pin_conf.tx_gpio = tx_gpio;
	pusart->usart_pin_conf.tx_pin = tx_pin;
	pusart->usart_pin_conf.tx_pinsource = tx_pinsource;
	pusart->usart_pin_conf.tx_af = tx_af;
	pusart->usart_pin_conf.rx_gpio = rx_gpio;
	pusart->usart_pin_conf.rx_pin = rx_pin;
	pusart->usart_pin_conf.rx_pinsource = rx_pinsource;
	pusart->usart_pin_conf.rx_af = rx_af;
}


/* +------------------------------------------+ *
 * |	       	     串口配置                 | *
 * +------------------------------------------+ */
void usart_config(usart_t *pusart,
	              void (*usart_periphclockcmd_func)(u32, FunctionalState),
 				  u32 usart_rcc_periph,
                  u32 baudrate, 
                  USART_TypeDef *usartx)
{
	if (NULL == pusart) {
		return;
	}
	
	pusart->usart_conf.usart_periphclockcmd_func = usart_periphclockcmd_func;
	pusart->usart_conf.usart_rcc_periph = usart_rcc_periph;
	pusart->usart_conf.baudrate = baudrate;
	pusart->usartx = usartx;
}


/* +------------------------------------------+ *
 * |	       	   串口中断配置               | *
 * +------------------------------------------+ */
void usart_nvic_config(usart_t *pusart,
                       u8 irq_channel, 
                       u8 prior, 
                       FunctionalState state)
{
	if (NULL == pusart) {
		return;
	}

	pusart->usart_conf.usart_irq_channel = irq_channel;
	pusart->usart_conf.usart_prior = prior;
	pusart->usart_conf.usart_irq_state = (u8)state;
}


/* +------------------------------------------+ *
 * |	        DMA 串口的初始化配置          | *
 * +------------------------------------------+ */
void usart_dma_config(usart_t *pusart,
                      USART_TypeDef *usartx,
 					  void (*dma_periphclockcmd_func)(u32, FunctionalState),
 				      u32 dma_rcc_periph, 
 				      DMA_Channel_TypeDef *dmax_channelx,
 				      u32 dmay_flag,
 					  void *dma_tx_buf_ptr,
 					  u32 dma_buffersize)
{
	if (NULL == pusart) {
		return;
	}
	
	pusart->usartx = usartx;
	pusart->usart_dma_conf.dma_periphclockcmd_func = dma_periphclockcmd_func;
	pusart->usart_dma_conf.dma_rcc_periph = dma_rcc_periph;
	pusart->usart_dma_conf.dmax_channelx = dmax_channelx;
	pusart->usart_dma_conf.dmay_flag = dmay_flag;
	pusart->usart_dma_conf.dma_tx_buf_ptr = (u8 *)dma_tx_buf_ptr;
	pusart->usart_dma_conf.dma_tx_buf_size = dma_buffersize;
}


/* +------------------------------------------+ *
 * |	          DMA 中断配置                | *
 * +------------------------------------------+ */
void usart_dma_nvic_config(usart_t *pusart,
	                       u8 irq_channel, 
	                       u8 prior, 
	                       FunctionalState state)
{
	if (NULL == pusart) {
		return;
	}

	pusart->usart_dma_conf.usart_dma_irq_channel = irq_channel;
	pusart->usart_dma_conf.usart_dma_prior = prior;
	pusart->usart_dma_conf.usart_dma_irq_state = (u8)state;
}


/* +------------------------------------------+ *
 * |	            串口FIFO配置              | *
 * +------------------------------------------+ */
void usart_fifo_config(usart_t *pusart, void *buffer, u32 buffer_size)
{
	if (NULL == pusart) {
		return;
	}

	/* 检查是否是2的幂次 */
	if (buffer_size & (buffer_size - 1)) {
		return;
	}

	fifo_init(&pusart->fifo, (u8 *)buffer, buffer_size);
}


/* +------------------------------------------+ *
 * |	        TIM调用 DMA发送数据           | *
 * +------------------------------------------+ */
void usart_dma_send_data(usart_t *pusart)
{
	u32 send_len = 0;

	/* 判断用户发送缓冲区是否有数据要发送 */
	if (get_fifo_len(&pusart->fifo) == 0) {
		return;
	}

	/* 判断用户是否正在往缓冲区里写数据 */
	if (pusart->user_put_stat== STATE_BUSY) {
		return;
	}

	/* 判断上一轮发送是否完毕 */
	if (pusart->dma_tx_stat== STATE_BUSY) {
		return;
	}

	/* 发送的时候置为BUSY */
	pusart->dma_tx_stat = STATE_BUSY;

	/* 计算出本次要发送的数据长度 */
	send_len = __MIN(get_fifo_len(&pusart->fifo), pusart->usart_dma_conf.dma_tx_buf_size);

	/* 从用户的发送缓冲区往串口发送缓冲区拷数据出来 */
	fifo_get(&pusart->fifo, pusart->usart_dma_conf.dma_tx_buf_ptr, send_len);

	/* 告诉DMA本次发送的数据首地址和数据长度 */
	pusart->usart_dma_conf.dma_init_struct.DMA_MemoryBaseAddr = (u32)(pusart->usart_dma_conf.dma_tx_buf_ptr);
	pusart->usart_dma_conf.dma_init_struct.DMA_BufferSize = send_len;

	USART_DMACmd(pusart->usartx, USART_DMAReq_Tx, DISABLE);	// Stop DMA transfer
	DMA_Cmd(pusart->usart_dma_conf.dmax_channelx, DISABLE);
	DMA_Init(pusart->usart_dma_conf.dmax_channelx, &pusart->usart_dma_conf.dma_init_struct);
	DMA_ClearFlag(pusart->usart_dma_conf.dmay_flag);
	DMA_Cmd(pusart->usart_dma_conf.dmax_channelx, ENABLE);
	USART_DMACmd(pusart->usartx, USART_DMAReq_Tx, ENABLE);	// Start DMA transfer


}



/* +------------------------------------------+ *
 * |	       DMA发送完以后置空闲            | *
 * +------------------------------------------+ */
void set_dma_state_idle(usart_t *pusart)
{
	pusart->dma_tx_stat = STATE_IDLE;
}


/* +------------------------------------------+ *
 * |	            事件回调配置              | *
 * +------------------------------------------+ */
void usart_event_config(int wh_event, event_t *node, event_func_t proc)
{
	event_add(wh_event, node, proc);
}


/* +------------------------------------------+ *
 * |	          初始化函数配置              | *
 * +------------------------------------------+ */
void usart_init_config(usart_t *pusart, void (*func)(void))
{
	pusart->init = func;
}


/* +------------------------------------------+ *
 * |	          逆初始化函数配置            | *
 * +------------------------------------------+ */
void usart_fini_config(usart_t *pusart, void (*func)(void))
{
	pusart->fini = func;
}


/* +------------------------------------------+ *
 * |	  提供给用户的发送接口,发一个字节     | *
 * +------------------------------------------+ */
void usart_send_byte(usart_t *pusart, const u8 byte)
{	
	/* 写满了就等DMA消费一些 */
	while (get_fifo_len(&pusart->fifo) == pusart->fifo.size);
	
	pusart->user_put_stat = STATE_BUSY;
	fifo_put(&pusart->fifo, &byte, 1);
	pusart->user_put_stat = STATE_IDLE;
}


/* +------------------------------------------+ *
 * |  提供给用户的发送接口,发data_len个字节   | *
 * +------------------------------------------+ */
void usart_send_data(usart_t *pusart, const u8 *src, u32 data_len)
{	
	u32 len = 0, send_len = 0, i = 0;

	len = data_len;

	while (len) {
		/* 写满了就等DMA消费一些 */
		while (get_fifo_len(&pusart->fifo) == pusart->fifo.size);
		
		pusart->user_put_stat = STATE_BUSY;
		send_len = fifo_put(&pusart->fifo, &src[i], len);
		pusart->user_put_stat = STATE_IDLE;
		
		i += send_len;
		len -= send_len;
	}
}




/* END OF FILE */

