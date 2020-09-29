/****************************************Copyright (c)****************************************
 																							
                             Galaxywind Network System Co., Ltd.						    
                                      												
                                  http://www.galaxywind.com								
 																						
 Author:	Y
 Date  : 	2018-03-16
 *********************************************************************************************/ 
#include "bsp_tim3.h"
#include "sys_config.h"



#define TIM3_PERIOD		5000		/* 5ms */



/* +------------------------------------------+ *
 * |	  			tim3 �ж�����        	  | *
 * +------------------------------------------+ */
static void __tim3_nvic_config(void)
{
	nvic_config(TIM3_IRQn, TIM3_PRIOR, ENABLE);
}


/* +------------------------------------------+ *
 * |	  			tim3  ����        	 	  | *
 * +------------------------------------------+ */
static void __tim3_config(void)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure = {0};
	u32 period = TIM3_PERIOD;
	
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
    TIM_DeInit(TIM3);
	
    TIM_TimeBaseStructure.TIM_Period = period - 1;		 			
    /* �ۼ� TIM_Period��Ƶ�ʺ����һ�����»����ж� */
    TIM_TimeBaseStructure.TIM_Prescaler = 48 - 1;				    // 1M
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; 		// ������Ƶ 
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;     // ���ϼ���ģʽ
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
    TIM_ClearFlag(TIM3, TIM_FLAG_Update);							// �������жϱ�־
}


/* +------------------------------------------+ *
 * |	  		    TIM3 ֹͣ      	          | *
 * +------------------------------------------+ */
void bsp_tim3_stop(void)
{
    TIM_Cmd(TIM3, DISABLE);
    TIM_ITConfig(TIM3, TIM_IT_Update, DISABLE);
}


/* +------------------------------------------+ *
 * |	  		    TIM3 ����        	      | *
 * +------------------------------------------+ */
void bsp_tim3_start(void)
{
    TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
    TIM_Cmd(TIM3, ENABLE);
}


/* +------------------------------------------+ *
 * |	  		     TIM3���¼���     	      | *
 * +------------------------------------------+ */
void bsp_tim3_clear(void)
{
	TIM_SetCounter(TIM3, 0);
}


/* +------------------------------------------+ *
 * |	  		     TIM3��ʼ��      	      | *
 * +------------------------------------------+ */
void bsp_tim3_init(void)
{
	__tim3_nvic_config();
	__tim3_config();

	bsp_tim3_start();
}


/* +------------------------------------------+ *
 * |	  		     TIM3���ʼ��     	      | *
 * +------------------------------------------+ */
void bsp_tim3_fini(void)
{
	bsp_tim3_stop();
	
    TIM_DeInit(TIM3);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, DISABLE);
}




/* END OF FILE */

