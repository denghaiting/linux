/****************************************Copyright (c)****************************************
 																							
                             Galaxywind Network System Co., Ltd.						    
                                      												
                                  http://www.galaxywind.com								
 																						
 Author:	Y
 Date  : 	2020-03-25
 *********************************************************************************************/ 


// TODO:                                        WARNING: 
// TODO:           如果添加了新代码，务必在每次编译完成以后查看Project/stm32f030c8.map文件
// TODO:           计算内存使用情况，若超过8192BYTE，请作相应调整


#include "stm32f0xx.h"
#include "remap_sram.h"
#include "systick.h"
#include "sys_tools.h"
#include "event.h"
#include "led.h"
#include "sys.h"
#include "bsp_usart2.h"
#include "bsp_usart1_dma_txrx.h"
#include "debug_uart.h"
#include "print_color.h"
#include "user_interface.h"
#include "rf433.h"



// TODO: 跳转APP后预处理
static void __pre_handle(void)
{
	ENABLE_IRQ();
	
	//FEED_DOG();

	sys_remap_sram();

}


static void __sys_init(void)
{	
	event_init();
	
	sys_init();

	usart1_init();
	usart2_init();

	printf("APP!"NEW_LINE);
	printf("INIT Usart2!"NEW_LINE);

	print_string("Usart2."NEW_LINE);
	print_02X(TRUE, 0x88);
	print_string(NEW_LINE);
	
}


static void __user_init(void)
{
	led_init();

	ui_init();

	rf433_init();

#if 0 // TODO: 上电一直发送
	udelay(100000);
	do_rf_start_tx(NULL, 0);
#endif

#if defined (IS_DELAY)
	udelay(100000);
	start_relay();
#endif


}


static void __main_loop(void)
{	
	for ( ; ; ) {  		
		ui_loop();
		
		usart1_Tx_dma_driver();

		rf_loop_proc();
		
		//usart1_Rx_proc();	

		//led_ctrl(i++ & 7);
		//delay_s(1);
		//rx_proc();
		
#if defined(ENABLE_IWDG)
		IWDG_ReloadCounter();
#endif
	}

}


int main(void)
{	
	__pre_handle();
	
	__sys_init();

	__user_init();

//	led_ctrl(eCOLOR_RED);
//	while(1){
//		usart2_send_1byte('a');
//	}

	__main_loop();
}





/* END OF FILE */

