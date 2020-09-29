/****************************************Copyright (c)****************************************
 																							
                             Galaxywind Network System Co., Ltd.						    
                                      												
                                  http://www.galaxywind.com								
 																						
 Author:	Y
 Date  : 	2018-03-16
 *********************************************************************************************/ 
#ifndef _USART_H_
#define	_USART_H_

#include "stm32f0xx.h"
#include "event.h"
#include "fifo.h"



typedef enum {
	USART_INIT_DEFAULT = 0,
	USART_INIT_PRINT_ONLY
} init_mode_t;


typedef enum {
	STATE_IDLE = 0,
	STATE_BUSY = !STATE_IDLE
} stat_t;



typedef struct usart_conf_s {
	void (*usart_periphclockcmd_func)(u32, FunctionalState);
	u32 usart_rcc_periph;
    u32 baudrate;
	u8 usart_irq_channel;
	u8 usart_prior;
	u8 usart_irq_state;
	u8 pad;
} usart_conf_t;


typedef struct usart_pin_conf_s {
	GPIO_TypeDef *tx_gpio;
	u32 tx_pin;
	u16 tx_pinsource;
	u8 tx_af;
	u8 txpad;
	GPIO_TypeDef *rx_gpio;
	u32 rx_pin;
	u16 rx_pinsource;
	u8 rx_af;
	u8 rxpad;
} usart_pin_conf_t;


typedef struct usart_dma_conf_s {
	void (*dma_periphclockcmd_func)(u32, FunctionalState);
	u32 dma_rcc_periph;
	USART_TypeDef *usartx;
	DMA_Channel_TypeDef *dmax_channelx;
	u32 dmay_flag;
	u8 usart_dma_irq_channel;
	u8 usart_dma_prior;
	u8 usart_dma_irq_state;
	u8 usart_dma_pad;
	u8 *dma_tx_buf_ptr;	
	u32 dma_tx_buf_size;
	DMA_InitTypeDef dma_init_struct;
} usart_dma_conf_t;

typedef struct usart_s {
	USART_TypeDef *usartx;
	usart_conf_t usart_conf;
	usart_pin_conf_t usart_pin_conf;
	usart_dma_conf_t usart_dma_conf;
	__IO stat_t dma_tx_stat;
	__IO stat_t user_put_stat;
	fifo_t fifo;
	event_t dma_send_event;
	event_t dma_tc;
	void (*init)(void);
	void (*fini)(void);
} usart_t; 






void usart_pin_config(usart_t *pusart, 
                      GPIO_TypeDef *tx_gpio, u32 tx_pin, u16 tx_pinsource, u8 tx_af, 
                      GPIO_TypeDef *rx_gpio, u32 rx_pin, u16 rx_pinsource, u8 rx_af);

void usart_config(usart_t *pusart,
	              void (*usart_periphclockcmd_func)(u32, FunctionalState),
 				  u32 usart_rcc_periph,
                  u32 baudrate, 
                  USART_TypeDef *usartx);

void usart_nvic_config(usart_t *pusart,
                       u8 irq_channel, 
                       u8 prior, 
                       FunctionalState state);

void usart_dma_config(usart_t *pusart,
                      USART_TypeDef *usartx,
 					  void (*dma_periphclockcmd_func)(u32, FunctionalState),
 				      u32 dma_rcc_periph, 
 				      DMA_Channel_TypeDef *dmax_channelx,
 				      u32 dmay_flag,
 					  void *dma_tx_buf_ptr,
 					  u32 dma_buffersize);

void usart_dma_nvic_config(usart_t *pusart,
	                       u8 irq_channel, 
	                       u8 prior, 
	                       FunctionalState state);

void usart_init(usart_t *pusart, init_mode_t mode);
void usart_fini(usart_t *pusart);

void usart_irq_close(usart_t *pusart);
void usart_irq_open(usart_t *pusart);

void usart_dma_send_data(usart_t *pusart);
void set_dma_state_idle(usart_t *pusart);

void usart_fifo_config(usart_t *pusart, void *buffer, u32 buffer_size);
void usart_event_config(int wh_event, event_t *node, event_func_t proc);
void usart_init_config(usart_t *pusart, void (*func)(void));
void usart_fini_config(usart_t *pusart, void (*func)(void));

void usart_send_byte(usart_t *pusart, const u8 byte);
void usart_send_data(usart_t *pusart, const u8 *src, u32 data_len);

#endif /* #ifndef _USART_H_ */

/* END OF FILE */

