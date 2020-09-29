/****************************************Copyright (c)****************************************
 																							
                             Galaxywind Network System Co., Ltd.						    
                                      												
                                  http://www.galaxywind.com								
 																						
 Author:	Y
 Date  : 	2018-03-16
 *********************************************************************************************/ 
#include "stm32f0xx.h"
#include "systick.h"
#include "event.h"
#include "bsp_tim16.h"



#if 0
/* +------------------------------------------+ *
 * |	  		   系统时钟中断        	      | *
 * +------------------------------------------+ */
void SysTick_Handler(void)
{
	SYS_DELAY_TIMER++;
	SYS_USER_TIMER_TIMER++;
}
#endif


/* +------------------------------------------+ *
 * |	  	          TIM3中断                | *
 * +------------------------------------------+ */
void TIM3_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET) {
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
		
		event_call(eEVENT_TIM3_UPDATE_IRQ, NULL, 0);
	}
}



/* +------------------------------------------+ *
 * |	  	 TIM16中断驱动DMA发送串口          | *
 * +------------------------------------------+ */
void TIM16_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM16, TIM_IT_Update) != RESET) {
		TIM_ClearITPendingBit(TIM16, TIM_IT_Update);
		
		event_call(eEVENT_TIM16_UPDATE_IRQ, NULL, 0);
	}
}


/* +------------------------------------------+ *
 * |	  		  DMA发送完成中断        	  | *
 * +------------------------------------------+ */
void DMA1_Channel4_5_IRQHandler(void)
{	
	/* 判断是否为DMA发送完成中断 */
	if (DMA_GetFlagStatus(DMA1_FLAG_TC4) == SET) {   
	  	DMA_ClearFlag(DMA1_FLAG_TC4); 
		event_call(eEVENT_DMA1_CH4_TC, NULL, 0);
	}	
}


/* +------------------------------------------+ *
 * |	  		  DMA发送完成中断        	  | *
 * +------------------------------------------+ */
void DMA1_Channel2_3_IRQHandler(void)
{	
	/* 判断是否为DMA发送完成中断 */
	if (DMA_GetFlagStatus(DMA1_FLAG_TC2) == SET) {   
	  	DMA_ClearFlag(DMA1_FLAG_TC2); 
		event_call(eEVENT_DMA1_CH2_TC, NULL, 0);
	}	
}



/* +------------------------------------------+ *
 * |	  	 	串口1    RX中断         	  | *
 * +------------------------------------------+ */
void USART1_IRQHandler(void)
{
	u8 data;

	if (USART_GetITStatus(USART1, USART_IT_RXNE)) {
		USART_ClearITPendingBit(USART1, USART_IT_RXNE);
		data = USART_ReceiveData(USART1);
		
		event_call(eEVENT_USART1_RXNE, &data, 1);

	}
}



/* +------------------------------------------+ *
 * |	  	 	串口2    RX中断         	  | *
 * +------------------------------------------+ */
void USART2_IRQHandler(void)
{
	u8 data;

	if (USART_GetITStatus(USART2, USART_IT_RXNE)) {
		USART_ClearITPendingBit(USART2, USART_IT_RXNE);
		data = USART_ReceiveData(USART2);
		//usart2_send_byte(data);

		event_call(eEVENT_USART2_RXNE, &data, 1);

		
	}
}




/* END OF FILE */

