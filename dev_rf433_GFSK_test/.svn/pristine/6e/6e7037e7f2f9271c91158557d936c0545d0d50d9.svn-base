/****************************************Copyright (c)****************************************
 																							
                             Galaxywind Network System Co., Ltd.						    
                                      												
                                  http://www.galaxywind.com								
 																						
 Author:	ZhangYu
 Date  : 	2015-06-01
 Email :	33076156@qq.com
 *********************************************************************************************/ 

#include "sys_nvic_config.h"


void nvic_config(u8 irq_channel, u8 prior, FunctionalState state)
{
    NVIC_InitTypeDef NVIC_InitStructure; 
     													
    NVIC_InitStructure.NVIC_IRQChannel = irq_channel;	  
    NVIC_InitStructure.NVIC_IRQChannelPriority = prior;	
    NVIC_InitStructure.NVIC_IRQChannelCmd = state;
	
    NVIC_Init(&NVIC_InitStructure);
}


/* END OF FILE */

