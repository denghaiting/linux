/****************************************Copyright (c)****************************************
 																							
                             Galaxywind Network System Co., Ltd.						    
                                      												
                                  http://www.galaxywind.com								
 																						
 Author:	Y
 Date  : 	2018-03-16
 *********************************************************************************************/ 
#ifndef _BSP_GPIO_H_
#define _BSP_GPIO_H_

#include "stm32f0xx.h"



void bsp_gpio_init(GPIO_TypeDef *gpio_x, u32 gpio_pin, GPIOMode_TypeDef mode, GPIOSpeed_TypeDef speed, GPIOOType_TypeDef otype, GPIOPuPd_TypeDef pupd);
void bsp_gpio_fini(GPIO_TypeDef *gpio_x, u32 gpio_pin);







#endif	// #ifndef _BSP_GPIO_H_

/* END OF FILE */

