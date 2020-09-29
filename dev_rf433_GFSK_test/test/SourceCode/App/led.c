/****************************************Copyright (c)****************************************
 																							
                             Galaxywind Network System Co., Ltd.						    
                                      												
                                  http://www.galaxywind.com								
 																						
 Author:	ZhangYu
 Date  : 	2015-06-02
 Email :	33076156@qq.com
 *********************************************************************************************/ 
#include "led.h"
#include "bsp_gpio.h"
#include "sys_tools.h"


static void __led_high(GPIO_TypeDef* GPIOx, u16 GPIO_Pin)
{
	GPIO_SetBits(GPIOx, GPIO_Pin);
}


static void __led_low(GPIO_TypeDef* GPIOx, u16 GPIO_Pin)
{
	GPIO_ResetBits(GPIOx, GPIO_Pin);
}

/*
static void __led_turn(GPIO_TypeDef* GPIOx, u16 GPIO_Pin)
{
	GPIO_WriteBit(GPIOx, GPIO_Pin, (BitAction)((1 - GPIO_ReadOutputDataBit(GPIOx, GPIO_Pin))));
}
*/

#if defined (HIGH_LEVEL_VALID)
#define __SYS_LED_RED_ON()				__led_high(SYS_LED_RED_GPIO_GROUP, SYS_LED_RED_GPIO_PIN)	
#define __SYS_LED_RED_OFF()				__led_low(SYS_LED_RED_GPIO_GROUP, SYS_LED_RED_GPIO_PIN)

#define __SYS_LED_GREEN_ON()			__led_high(SYS_LED_GREEN_GPIO_GROUP, SYS_LED_GREEN_GPIO_PIN)	
#define __SYS_LED_GREEN_OFF()			__led_low(SYS_LED_GREEN_GPIO_GROUP, SYS_LED_GREEN_GPIO_PIN)

#define __SYS_LED_BLUE_ON()				//__led_high(SYS_LED_BLUE_GPIO_GROUP, SYS_LED_BLUE_GPIO_PIN)	
#define __SYS_LED_BLUE_OFF()			//__led_low(SYS_LED_BLUE_GPIO_GROUP, SYS_LED_BLUE_GPIO_PIN)

#elif defined (LOW_LEVEL_VALID)
#define __SYS_LED_RED_ON()				__led_low(SYS_LED_RED_GPIO_GROUP, SYS_LED_RED_GPIO_PIN)	
#define __SYS_LED_RED_OFF()				__led_high(SYS_LED_RED_GPIO_GROUP, SYS_LED_RED_GPIO_PIN)

#define __SYS_LED_GREEN_ON()			__led_low(SYS_LED_GREEN_GPIO_GROUP, SYS_LED_GREEN_GPIO_PIN)	
#define __SYS_LED_GREEN_OFF()			__led_high(SYS_LED_GREEN_GPIO_GROUP, SYS_LED_GREEN_GPIO_PIN)

#define __SYS_LED_BLUE_ON()				__led_low(SYS_LED_BLUE_GPIO_GROUP, SYS_LED_BLUE_GPIO_PIN)	
#define __SYS_LED_BLUE_OFF()			__led_high(SYS_LED_BLUE_GPIO_GROUP, SYS_LED_BLUE_GPIO_PIN)
#endif





/* +------------------------------------------+ *
 * |	  			系统LED配置        		  | *
 * +------------------------------------------+ */
static void __led_config()
{
	bsp_gpio_init(SYS_LED_RED_GPIO_GROUP,
				  SYS_LED_RED_GPIO_PIN, 
				  SYS_LED_RED_GPIO_MODE, 
				  SYS_LED_RED_GPIO_SPEED,
				  SYS_LED_RED_GPIO_OTYPE,
				  SYS_LED_RED_GPIO_PUPD);

	bsp_gpio_init(SYS_LED_GREEN_GPIO_GROUP,
				  SYS_LED_GREEN_GPIO_PIN, 
				  SYS_LED_GREEN_GPIO_MODE, 
				  SYS_LED_GREEN_GPIO_SPEED,
				  SYS_LED_GREEN_GPIO_OTYPE,
				  SYS_LED_GREEN_GPIO_PUPD);

	bsp_gpio_init(SYS_LED_BLUE_GPIO_GROUP,
				  SYS_LED_BLUE_GPIO_PIN, 
				  SYS_LED_BLUE_GPIO_MODE, 
				  SYS_LED_BLUE_GPIO_SPEED,
				  SYS_LED_BLUE_GPIO_OTYPE,
				  SYS_LED_BLUE_GPIO_PUPD);
}


/* +------------------------------------------+ *
 * |	  		     LED初始化      	      | *
 * +------------------------------------------+ */
void led_init(void)
{	
	__led_config();
	
	__SYS_LED_RED_ON();
	__SYS_LED_GREEN_ON();
	__SYS_LED_BLUE_ON();
}



/* +------------------------------------------+ *
 * |	  		   控制系统LED接口      	  | *
 * +------------------------------------------+ */
void led_ctrl(u8 value)
{
	__SYS_LED_RED_OFF();
	__SYS_LED_GREEN_OFF();
	__SYS_LED_BLUE_OFF();
	
	switch (value) {
	case eCOLOR_OFF:

		break;

	case eCOLOR_RED:
		__SYS_LED_RED_ON();
		break;
		
	case eCOLOR_GREEN:
		__SYS_LED_GREEN_ON();
		break;
		
	case eCOLOR_YELLOW:
		__SYS_LED_RED_ON();
		__SYS_LED_GREEN_ON();
		break;

	case eCOLOR_BLUE:
		__SYS_LED_BLUE_ON();
		break;

	case eCOLOR_PURPLE:
		__SYS_LED_RED_ON();
		__SYS_LED_BLUE_ON();
		break;

	case eCOLOR_LIGHT_BLUE:
		__SYS_LED_GREEN_ON();
		__SYS_LED_BLUE_ON();
		break;
		
	case eCOLOR_WHITE:
		__SYS_LED_RED_ON();
		__SYS_LED_GREEN_ON();
		__SYS_LED_BLUE_ON();
		break;
	default:

		break;
	}
}




