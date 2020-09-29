/****************************************Copyright (c)****************************************
 																							
                             Galaxywind Network System Co., Ltd.						    
                                      												
                                  http://www.galaxywind.com								
 																						
 Author:	Y
 Date  : 	2018-03-16
 *********************************************************************************************/ 
#include "usart_comm.h"
#include "version.h"
#include "sys.h"
#include "bsp_gpio.h"



void usart_common_init(void)
{
	bsp_tim16_init();

#if defined (USE_USART_1)
	bsp_usart1_config();
	bsp_usart1_init();

	
#elif defined (USE_USART_2)
	bsp_usart2_config();
	bsp_usart2_init();


#endif


}


void usart_common_fini(void)
{
	bsp_tim16_fini();

#if defined (USE_USART_1)
	bsp_usart1_fini();


#elif defined (USE_USART_2)
	bsp_usart2_fini();

#endif

}




/* +------------------------------------------+ *
 * |	          ÷ÿ∂®œÚ fputc  		      | *
 * +------------------------------------------+ */
int fputc(int ch, FILE *f)
{
#if defined (USE_USART_1)

	usart1_send_byte((u8)ch);

#elif defined (USE_USART_2)

	usart2_send_byte((u8)ch);

#endif

	return ch;
}



/* END OF FILE */

