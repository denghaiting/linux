/**
  ******************************************************************************
  * @file    Project/STM32F0xx_StdPeriph_Templates/stm32f0xx_it.c 
  * @author  MCD Application Team
  * @version V1.3.1
  * @date    17-January-2014
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2014 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f0xx_it.h"
#include "sys_tools.h"
#include "event.h"
#include "led.h"
#include "bsp_usart2.h"
#include "bsp_usart1_dma_txrx.h"
#include "rf433.h"


/** @addtogroup Template_Project
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/


//vu32 SYS_DELAY_TIMER = 0;
//vu32 SYSTICK_TIMER = 0;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M0 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */


/* +------------------------------------------+ *
 * |	  		   系统时钟中断        	      | *
 * +------------------------------------------+ */
void SysTick_Handler(void)
{
	//SYSTICK_TIMER++;
	
	//SYS_DELAY_TIMER++;
}

/******************************************************************************/
/*                 STM32F0xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f0xx.s).                                            */
/******************************************************************************/

/* +------------------------------------------+ *
 * |	  		  DMA发送完成中断        	  | *
 * +------------------------------------------+ */
void DMA1_Channel1_IRQHandler(void)
{
	/* 判断是否为DMA传输完成中断 */
	if (DMA_GetFlagStatus(DMA1_FLAG_TC1)) {
        DMA_ClearFlag(DMA1_FLAG_TC1);
		event_call(eEVENT_DMA1_CH1_TC, NULL, 0);
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



/**
  * @brief  This function handles TIM4 receive Handler.
  * @param  None
  * @retval None
  */

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
 * |	  	 TIM15中断控制ADC采样率           | *
 * +------------------------------------------+ */
void TIM15_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM15, TIM_IT_Update) != RESET) {
		TIM_ClearITPendingBit(TIM15, TIM_IT_Update);
		
		event_call(eEVENT_TIM15_UPDATE_IRQ, NULL, 0);
	}
}



/* +------------------------------------------+ *
 * |	  	 TIM16中断驱动DMA发送串口         | *
 * +------------------------------------------+ */
void TIM16_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM16, TIM_IT_Update) != RESET) {
		TIM_ClearITPendingBit(TIM16, TIM_IT_Update);
		
		//event_call(eEVENT_TIM16_UPDATE_IRQ, NULL, 0);
	}
}

/* +------------------------------------------+ *
 * |	  	 TIM17中断驱动DMA发送串口         | *
 * +------------------------------------------+ */
void TIM17_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM17, TIM_IT_Update) != RESET) {
		TIM_ClearITPendingBit(TIM17, TIM_IT_Update);
		
		//event_call(eEVENT_TIM17_UPDATE_IRQ, NULL, 0);
	}
}


/* +------------------------------------------+ *
 * |	  	 	串口1    RX中断         	  | *
 * +------------------------------------------+ */
void USART1_IRQHandler(void)
{
	//u8 data;

	if (USART_GetITStatus(USART1, USART_IT_IDLE)) {
		USART_ClearITPendingBit(USART1, USART_IT_IDLE);

		event_call(eEVENT_USART1_IDLE, NULL, 1);

	}
}



/* +------------------------------------------+ *
 * |	  	 	串口2    RX中断         	  | *
 * +------------------------------------------+ */
void USART2_IRQHandler(void)
{
	//u8 data;

	if (USART_GetITStatus(USART2, USART_IT_IDLE)) {
		USART_ClearITPendingBit(USART2, USART_IT_IDLE);

		event_call(eEVENT_USART2_IDLE, NULL, 1);

	}
}



void EXTI0_1_IRQHandler(void)
{
	if (EXTI_GetITStatus(EXTI_Line0) != RESET) {	// 确保是否产生了EXTI Line中断
		EXTI_ClearITPendingBit(EXTI_Line0);     	// 清除中断标志位

		//event_call(eEVENT_EXITLINE0_IRQ, NULL, 0);

		rx_pkt_isr();
	}

	if (EXTI_GetITStatus(EXTI_Line1) != RESET) {	// 确保是否产生了EXTI Line中断
		EXTI_ClearITPendingBit(EXTI_Line1);     	// 清除中断标志位

		tx_done_isr();
	}
}



void EXTI2_3_IRQHandler(void)
{


}



void EXTI4_15_IRQHandler(void)//中断服务函数的名字在启动函数中有定义
{
/*
	if (EXTI_GetITStatus(EXTI_Line8) != RESET) {	// 确保是否产生了EXTI Line中断
		EXTI_ClearITPendingBit(EXTI_Line8);     	// 清除中断标志位

		event_call(eEVENT_EXITLINE8_IRQ, NULL, 0);
	}
	
	if (EXTI_GetITStatus(EXTI_Line9) != RESET) {	// 确保是否产生了EXTI Line中断
		EXTI_ClearITPendingBit(EXTI_Line9);     	// 清除中断标志位
		
		event_call(eEVENT_EXITLINE9_IRQ, NULL, 0);
	}
*/

	if (EXTI_GetITStatus(EXTI_Line11) != RESET) {	// 确保是否产生了EXTI Line中断
		EXTI_ClearITPendingBit(EXTI_Line11);     	// 清除中断标志位
		
		event_call(eEVENT_EXITLINE11_IRQ, NULL, 0);
	}

/*
	if (EXTI_GetITStatus(EXTI_Line12) != RESET) {	// 确保是否产生了EXTI Line中断
		EXTI_ClearITPendingBit(EXTI_Line12);     	// 清除中断标志位
		
		event_call(eEVENT_EXITLINE12_IRQ, NULL, 0);
  	}

	if (EXTI_GetITStatus(EXTI_Line14) != RESET) {	// 确保是否产生了EXTI Line中断
		EXTI_ClearITPendingBit(EXTI_Line14);     	// 清除中断标志位

		event_call(eEVENT_EXITLINE14_IRQ, NULL, 0);
	}
*/
}





/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */ 


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
