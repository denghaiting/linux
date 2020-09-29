/****************************************Copyright (c)****************************************
 																							
                             Galaxywind Network System Co., Ltd.						    
                                      												
                                  http://www.galaxywind.com								
 																						
 Author:	Y
 Date  : 	2020-03-25
 *********************************************************************************************/ 
#include "sys.h"
#include "systick.h"
#include "sys_config.h"



/* +------------------------------------------+ *
 * |	 		   ��ʼ�����Ź�         	  | *
 * +------------------------------------------+ */
void watchdog_init(void)
{
#if defined(ENABLE_IWDG)
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);	//�ر�IWDG_PR��IWDG_RLR��д����
	IWDG_SetReload(0xFFF);							//������װ��ֵΪ0xfff
	IWDG_SetPrescaler(__CFG__WATCHDOG_PRESCALER);	//����Ԥ��Ƶϵ��Ϊ128�� Լ12��
	IWDG_ReloadCounter();
	IWDG_Enable();
#endif
}


/* +------------------------------------------+ *
 * |  �ڲ�ʱ�ӳ�ʼ����STM32F030���Ƶ��48MHz  | *
 * +------------------------------------------+ */
void system_clk_config(void)
{	
#if defined(EXTERN_CLOCK)
	// TODO: startup_stm32f030.s ȡ��ע��, �ָ�ʹ��SystemInit 
#elif defined(INNER_CLOCK)
	
	RCC_DeInit();//������ RCC�Ĵ�������Ϊȱʡֵ
 
	RCC_HSICmd(ENABLE);//ʹ��HSI  HSIΪ�̶�8MHz
	
	while (RCC_GetFlagStatus(RCC_FLAG_HSIRDY) == RESET);//�ȴ�HSIʹ�ܳɹ�

    RCC_HCLKConfig(RCC_SYSCLK_Div1); //HCLK == SYSCLK  
    RCC_PCLKConfig(RCC_HCLK_Div1);	 //PCLK == HCLK == SYSCLK 	
    
   	/* ���� PLL ʱ��Դ����Ƶϵ�� HSI / 2 * 12 = 48MHz*/
    RCC_PLLConfig(RCC_PLLSource_HSI_Div2, RCC_PLLMul_12);  
	
    /* ʹ�ܻ���ʧ�� PLL,�����������ȡ��ENABLE����DISABLE */
    RCC_PLLCmd(ENABLE);//���PLL������ϵͳʱ��,��ô�����ܱ�ʧ��
    
    /* �ȴ�ָ���� RCC ��־λ���óɹ� �ȴ�PLL��ʼ���ɹ� */
    while (RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET);
 
    /* ����ϵͳʱ�ӣ�SYSCLK�� ����PLLΪϵͳʱ��Դ */
    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);  

	/* �ȴ�PLL�ɹ�������ϵͳʱ�ӵ�ʱ��Դ 
	 * +--------+------------------+ *
 	 * |  0x00	| HSI ��Ϊϵͳʱ�� | *
	 * +--------+------------------+ *
	 * |  0x04	| HSE ��Ϊϵͳʱ�� | *
 	 * +--------+------------------+ *
 	 * |  0x08	| PLL ��Ϊϵͳʱ�� | *
	 * +--------+------------------+ */
    while (RCC_GetSYSCLKSource() != 0x08);
#endif
}


void feed_iwdg(void)
{
#if defined(ENABLE_IWDG)
	IWDG_ReloadCounter();
#endif
}


void sys_init(void)
{
	watchdog_init();

	system_clk_config();

	systick_init();
}




/* END OF FILE */

