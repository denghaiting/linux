/****************************************Copyright (c)****************************************
 																							
                             Galaxywind Network System Co., Ltd.						    
                                      												
                                  http://www.galaxywind.com								
 																						
 Author:	Y
 Date  : 	2018-03-16
 *********************************************************************************************/ 
#include "bsp_gpio.h"


/* GPIO Group Periph Manager*/
typedef struct ggm_s {
	GPIO_TypeDef *gpio_x;
	u32 periph;
	vu32 bit_map;
} ggpm_t;


#define GPIO_PIN_NUM_MASK	0x0000FFFF

enum {
	GPROUP_A = 0,
	GPROUP_B,
	GPROUP_C,
	GPROUP_D,
	GPROUP_E,
	GPROUP_F,
	MAX_GPIO_GROUP_NUM
};

static ggpm_t ggpm[MAX_GPIO_GROUP_NUM] = {
	{GPIOA, RCC_AHBPeriph_GPIOA, 0x0}, 
	{GPIOB, RCC_AHBPeriph_GPIOB, 0x0}, 
	{GPIOC, RCC_AHBPeriph_GPIOC, 0x0}, 
	{GPIOD, RCC_AHBPeriph_GPIOD, 0x0},
	{GPIOE, RCC_AHBPeriph_GPIOE, 0x0}, 
	{GPIOF, RCC_AHBPeriph_GPIOF, 0x0}, 
};



/* +------------------------------------------+ *
 * |	      封装了一层GPIO初始化函数        | *
 * +------------------------------------------+ */
void bsp_gpio_init(GPIO_TypeDef *gpio_x, u32 gpio_pin, GPIOMode_TypeDef mode, GPIOSpeed_TypeDef speed, GPIOOType_TypeDef otype, GPIOPuPd_TypeDef pupd)
{
	GPIO_InitTypeDef gpio_init_struct;
	int i = 0;

	for (i = 0; i < (sizeof(ggpm) / sizeof(ggpm_t)); i++) {
		if (ggpm[i].gpio_x == gpio_x) {
			if ( ! (ggpm[i].bit_map & GPIO_PIN_NUM_MASK) ) {
				RCC_AHBPeriphClockCmd(ggpm[i].periph, ENABLE);
			}
			
			ggpm[i].bit_map |= gpio_pin;
			
			gpio_init_struct.GPIO_Pin = gpio_pin;
			gpio_init_struct.GPIO_Mode = mode;
			gpio_init_struct.GPIO_Speed = speed;
			gpio_init_struct.GPIO_OType = otype;
			gpio_init_struct.GPIO_PuPd = pupd;
			
			GPIO_Init(gpio_x, &gpio_init_struct);
		
			break;
		}
	}	

	
}


/* +------------------------------------------+ *
 * |	     封装了一层GPIO逆初始化函数       | *
 * +------------------------------------------+ */
void bsp_gpio_fini(GPIO_TypeDef *gpio_x, u32 gpio_pin)
{
	GPIO_InitTypeDef gpio_init_struct;
	int i = 0;

	for (i = 0; i < (sizeof(ggpm) / sizeof(ggpm_t)); i++) {
		if (ggpm[i].gpio_x == gpio_x) {
			gpio_init_struct.GPIO_Pin = gpio_pin;
			gpio_init_struct.GPIO_Mode = GPIO_Mode_IN;
			GPIO_Init(gpio_x, &gpio_init_struct);
			
			ggpm[i].bit_map &= ~gpio_pin;
			
			if ( ! (ggpm[i].bit_map & GPIO_PIN_NUM_MASK) ) {
				RCC_AHBPeriphClockCmd(ggpm[i].periph, DISABLE);
				GPIO_DeInit(gpio_x);
			}

			break;
		}
	}	
}






/* END OF FILE */

