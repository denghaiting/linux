/*###################################*
 *#                                 #*
 *#     Author: Yu                  #*
 *#     Email : 33076156@qq.com     #*
 *#     Date  : 2020.3.10           #*
 *#                                 #*
 *###################################*/
 
#include <stdio.h>
#include "sys_config.h"
#include "sys_nvic_config.h"
#include "sys.h"
#include "bsp_gpio.h"
#include "fifo.h"
#include "event.h"
#include "print_color.h"
#include "bsp_usart1_dma_txrx.h"


#if 1
#define ALIGN4 __align(4) 

// TODO: TX DMA
#define TX_DMA_BUF_SIZE 		512
#define TX_USER_BUF_SIZE		1024

ALIGN4 static u8 usart1_Tx_dma_buf[TX_DMA_BUF_SIZE] = {0};		// ����DMA���ͻ�����
ALIGN4 static u8 usart1_Tx_user_buf[TX_USER_BUF_SIZE] = {0};   // �û�����ѭ��������

fifo_t Tx_dma_fifo;
event_t Tx_dma_tc_event;

DMA_InitTypeDef Tx_dma_initstructure;

__IO stat_e usart1_Tx_dma_stat = eSTATE_IDLE;


// TODO: RX DMA
#define MAX_RX_DMA_BUF_SIZE 	256
ALIGN4 static u8 Rx_dma_buf[MAX_RX_DMA_BUF_SIZE];

event_t Rx_dma_idle_event;



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
	

	GPIO_PinAFConfig(USART1_TX_GPIO, USART1_TX_GPIO_PINSOURCE, USART1_TX_GPIO_AF);	// GPIO����
	GPIO_PinAFConfig(USART1_RX_GPIO, USART1_RX_GPIO_PINSOURCE, USART1_RX_GPIO_AF);	// GPIO����
}


static void usart1_config(void)
{
	USART_InitTypeDef USART_InitStructure;
	
	/* config USART clock */
	USART1_PERIPH_CLOCK_CMD_FUNC(USART1_RCC_PERIPH, ENABLE);   

	/* USART mode config */
	USART_InitStructure.USART_BaudRate = USART1_BAUDRATE;		// ������
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;	// ���ڴ����ֳ�
	USART_InitStructure.USART_StopBits = USART_StopBits_1;		// ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;			// ��żУ��
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	// ����Ӳ��������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	// ˫��ȫ˫��
	USART_Init(USART1, &USART_InitStructure);
	
	nvic_config(USART1_IRQCHANNEL, USART1_PRIOR, ENABLE);
	
	USART_ITConfig(USART1, USART_IT_IDLE, ENABLE);
	
	USART_Cmd(USART1, ENABLE); 									// ʹ�ܴ���

	USART_DMACmd(USART1, USART_DMAReq_Rx, ENABLE);

}


static void usart1_Tx_dma_config(void)
{	
	DMA_RemapConfig(USART1_USE_DMAX, USART1_TX_DMAX_CHY_REMAP);

	/* ����DMAʱ�� */
	USART1_DMA_PERIPH_CLOCK_CMD_FUNC(USART1_DMA_RCC_PERIPH, ENABLE);

	/* ����DMAԴ:�ڴ��ַ & �������ݼĴ�����ַ */  
	Tx_dma_initstructure.DMA_PeripheralBaseAddr = (u32)&USART1->TDR;
	
	/* �ڴ��ַ(Ҫ���������ָ��) */
	Tx_dma_initstructure.DMA_MemoryBaseAddr = (u32)usart1_Tx_dma_buf;
	
	/* ����:���ڴ浽���� ������ΪĿ���ַ */
	Tx_dma_initstructure.DMA_DIR = DMA_DIR_PeripheralDST;
	
	/* �����С */
	//dma_initstructure.DMA_BufferSize = sizeof(usart1_dma_tx_buff);
	Tx_dma_initstructure.DMA_BufferSize = 0; // ��ʼ��Ϊ0
	
	/* �����ַ�̶� */
	Tx_dma_initstructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	
	/* �ڴ��ַ���� */
	Tx_dma_initstructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	
	/* �������ݵ�λ */
	Tx_dma_initstructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	
	/* �ڴ����ݵ�λ */
	Tx_dma_initstructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	
	/* DMAģʽ Normal��ʾֻ����һ�� Circular��ʾ�ڷ�����һ�������Ժ������´��� */
	Tx_dma_initstructure.DMA_Mode = DMA_Mode_Normal;
	
	/* ���ȼ� */
	Tx_dma_initstructure.DMA_Priority = DMA_Priority_High;
	
	/* ��ֹ�ڴ浽�ڴ�Ĵ��� */
	Tx_dma_initstructure.DMA_M2M = DMA_M2M_Disable;

	DMA_Init(USART1_TX_DMAX_CHANNELX, &Tx_dma_initstructure);

	/* ʹ��DMA */
	DMA_Cmd(USART1_TX_DMAX_CHANNELX, ENABLE);
}



static void __usart1_set_Tx_dma_idle(int event, void *data, int data_len)
{
	usart1_Tx_dma_stat = eSTATE_IDLE;
}



/* +------------------------------------------+ *
 * |	           DMA��������                | *
 * +------------------------------------------+ */
void usart1_Tx_dma_driver(void)
{
	u32 send_len = 0;

	/* �ж���һ�ַ����Ƿ���� */
	if (usart1_Tx_dma_stat == eSTATE_BUSY) {
		return;
	}

	/* �ж��û����ͻ������Ƿ�������Ҫ���� */
	if (get_fifo_len(&Tx_dma_fifo) == 0) {
		return;
	}

	/* ���������Ҫ���͵����ݳ��� */
	send_len = __MIN(get_fifo_len(&Tx_dma_fifo), sizeof(usart1_Tx_dma_buf));
	//if (0 == send_len) {
	//	return;
	//}

	/* ���û��ķ��ͻ����������ڷ��ͻ����������ݳ��� */
	fifo_get(&Tx_dma_fifo, usart1_Tx_dma_buf, send_len);

	/* ���͵�ʱ����ΪBUSY */
	usart1_Tx_dma_stat = eSTATE_BUSY;

	/* ����DMA���η��͵������׵�ַ�����ݳ��� */
	//Tx_dma_initstructure.DMA_MemoryBaseAddr = (u32)&usart1_Tx_dma_buf[0];
	Tx_dma_initstructure.DMA_BufferSize = send_len;

	USART_DMACmd(USART1, USART_DMAReq_Tx, DISABLE);	// Stop DMA transfer
	DMA_Cmd(USART1_TX_DMAX_CHANNELX, DISABLE);
	DMA_Init(USART1_TX_DMAX_CHANNELX, &Tx_dma_initstructure);
	DMA_ClearFlag(USART1_TX_DMAY_FLAG);
	DMA_Cmd(USART1_TX_DMAX_CHANNELX, ENABLE);
	USART_DMACmd(USART1, USART_DMAReq_Tx, ENABLE);	// Start DMA transfer

}


/* +------------------------------------------+ *
 * |	  �ṩ���û��ķ��ͽӿ�,��һ���ֽ�     | *
 * +------------------------------------------+ */
void usart1_send_1byte_with_dma(u8 byte)
{	
	u8 x = byte;
	
	if (get_fifo_len(&Tx_dma_fifo) == Tx_dma_fifo.size) { // ����
		while (usart1_Tx_dma_stat == eSTATE_BUSY); // ��DMA�������
		usart1_Tx_dma_driver();
	}
	
	fifo_put(&Tx_dma_fifo, &x, 1);
}

#if 0
void flush_tx_data(void)
{
	while (usart1_Tx_dma_stat == eSTATE_BUSY); // ��DMA�������
	usart1_Tx_dma_driver();
}
#endif


/* +------------------------------------------+ *
 * |  �ṩ���û��ķ��ͽӿ�,��data_len���ֽ�   | *
 * +------------------------------------------+ */
void usart1_send_data_with_dma(u8 *src, u32 data_len)
{	
	u32 len = 0, send_len = 0, i = 0;

	len = data_len;

	while (len) {
		/* д���˾͵�DMA����һЩ */
		if (get_fifo_len(&Tx_dma_fifo) == Tx_dma_fifo.size) { // ����
			while (usart1_Tx_dma_stat == eSTATE_BUSY); // ��DMA�������
			usart1_Tx_dma_driver();
		}

		send_len = fifo_put(&Tx_dma_fifo, &src[i], len);
	
		i += send_len;
		len -= send_len;
	}
}


void flush_tx_data(void)
{
	while (usart1_Tx_dma_stat == eSTATE_BUSY); // ��DMA�������
	usart1_Tx_dma_driver();
}



void usart1_Tx_dma_init(void)
{
	fifo_init(&Tx_dma_fifo, (u8 *)usart1_Tx_user_buf, sizeof(usart1_Tx_user_buf));

	event_add(eEVENT_DMA1_CH2_TC, &Tx_dma_tc_event, __usart1_set_Tx_dma_idle);

	usart1_Tx_dma_config();

	nvic_config(USART1_TX_DMA_IRQCHANNEL, USART_DMA_PRIOR, ENABLE);
		
	/* ����TX DMA������ɺ�����ж� */
	DMA_ITConfig(USART1_TX_DMAX_CHANNELX, DMA_IT_TC, ENABLE);	
}


static void usart1_Rx_dma_config(void)
{		
	DMA_InitTypeDef dma_initstructure;

	DMA_RemapConfig(USART1_USE_DMAX, USART1_RX_DMAX_CHY_REMAP);

	/* ����DMAʱ�� */
	USART1_DMA_PERIPH_CLOCK_CMD_FUNC(USART1_DMA_RCC_PERIPH, ENABLE);

	/* ����DMAԴ:�ڴ��ַ & �������ݼĴ�����ַ */  
	dma_initstructure.DMA_PeripheralBaseAddr = (u32)&USART1->RDR;
	/* �ڴ��ַ(Ҫ���������ָ��) */
	dma_initstructure.DMA_MemoryBaseAddr = (u32)&Rx_dma_buf[0];	 
	/* ����:�����赽�ڴ� ������ΪԴ��ַ */
	dma_initstructure.DMA_DIR = DMA_DIR_PeripheralSRC;
	/* �����С */
	dma_initstructure.DMA_BufferSize = sizeof(Rx_dma_buf); 
	/* �����ַ�̶� */
	dma_initstructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	/* �ڴ��ַ���� */
	//dma_initstructure.DMA_MemoryInc = DMA_MemoryInc_Disable;
	dma_initstructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	/* �������ݵ�λ */
	dma_initstructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	/* �ڴ����ݵ�λ */
	dma_initstructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	/* DMAģʽ Normal��ʾֻ����һ�� Circular��ʾ�ڷ�����һ�������Ժ������´��� */
	dma_initstructure.DMA_Mode = DMA_Mode_Circular;
	//dma_initstructure.DMA_Mode = DMA_Mode_Normal;
	/* ���ȼ� */
	dma_initstructure.DMA_Priority = DMA_Priority_High;
	/* ��ֹ�ڴ浽�ڴ�Ĵ��� */
	dma_initstructure.DMA_M2M = DMA_M2M_Disable;

	DMA_Init(USART1_RX_DMAX_CHANNELX, &dma_initstructure);

	/* ʹ��DMA */

	//DMA_Cmd(USART1_RX_DMAX_CHANNELX, DISABLE ); 
	//DMA_SetCurrDataCounter(USART1_RX_DMAX_CHANNELX, 16);
	DMA_Cmd(USART1_RX_DMAX_CHANNELX, ENABLE); 
}




typedef enum {
	eUSART_RX_STATE_WAIT_DMA = 0,	
	eUSART_RX_STATE_PROCESS
} usart_rx_state_e;


vu8 dma_rx_state = eUSART_RX_STATE_WAIT_DMA;
u16 dma_rx_len = 0;



void usart1_Rx_dma_IT_IDLE(int event, void *data, int data_len)
{
	u16 dma_recv_len;

    DMA_Cmd(USART1_RX_DMAX_CHANNELX, DISABLE);  /* ��ʱ�ر�dma��������δ���� */
	
	dma_recv_len = sizeof(Rx_dma_buf) - DMA_GetCurrDataCounter(USART1_RX_DMAX_CHANNELX);
		
	dma_rx_len = dma_recv_len;

	//PUT_CHAR(dma_rx_len + '0');
	
	dma_rx_state = eUSART_RX_STATE_PROCESS;
}

//extern vu32 SYSTICK_TIMER;
void test_show_rx(u8 *data, u16 data_len)
{
	u32 len;

	//printf(PCLR_LBL_B "recv[%u]" NEW_LINE, data_len);
	
	for (len = 0; len < data_len; len++) {
		printf("%c", data[len]);
	}

	//printf("SYSTICK_TIMER = %u"NEW_LINE, SYSTICK_TIMER);

	//printf(PCLR_DFT);

}


void usart1_Rx_proc(void)
{	
	if (dma_rx_state != eUSART_RX_STATE_PROCESS) {
		return;
	}
	
	//test_show_rx(Rx_dma_buf, dma_rx_len);
	
	event_call(eEVENT_USART1_READ_DATA, Rx_dma_buf, dma_rx_len);
	
	dma_rx_state = eUSART_RX_STATE_WAIT_DMA;
	dma_rx_len = 0;
	DMA_SetCurrDataCounter(USART1_RX_DMAX_CHANNELX, sizeof(Rx_dma_buf));
	DMA_Cmd(USART1_RX_DMAX_CHANNELX, ENABLE);
}




void usart1_Rx_dma_init(void)
{
	event_add(eEVENT_USART1_IDLE, &Rx_dma_idle_event, usart1_Rx_dma_IT_IDLE);
	
	usart1_Rx_dma_config();

	//nvic_config(USART1_IRQCHANNEL, USART1_PRIOR, ENABLE);
	
	//USART_ITConfig(USART1, USART_IT_IDLE, ENABLE);

	//USART_DMACmd(USART1, USART_DMAReq_Rx, ENABLE);

}


void usart1_Rx_disable(void)
{
	event_del(eEVENT_USART1_IDLE, usart1_Rx_dma_IT_IDLE);
	USART_ITConfig(USART1, USART_IT_IDLE, DISABLE);
	DMA_Cmd(USART1_RX_DMAX_CHANNELX, DISABLE); 
	USART_DMACmd(USART1, USART_DMAReq_Rx, DISABLE);
}

 
void usart1_init(void)
{
	usart1_pin_init();
	
	usart1_Tx_dma_init(); // ʹ�ܷ���DMA

	usart1_Rx_dma_init(); // ʹ�ܽ���DMA

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


#endif

