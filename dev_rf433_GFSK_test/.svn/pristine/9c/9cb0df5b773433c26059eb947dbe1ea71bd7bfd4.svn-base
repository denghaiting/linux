/****************************************Copyright (c)****************************************
 																							
                             Galaxywind Network System Co., Ltd.						    
                                      												
                                  http://www.galaxywind.com								
 																						
 Author:	Y
 Date  : 	2019-07-19
 *********************************************************************************************/ 
#include "debug_uart.h"
#include "bsp_usart2.h"
#include "bsp_usart1_dma_txrx.h"
#include "bsp_gpio.h"
#include "sys_nvic_config.h"



#define __USART_GPIO_MODE				GPIO_Mode_AF		// 务必要这样设置，否则不生效
#define __USART_GPIO_SPEED				GPIO_Speed_50MHz	// 务必要这样设置，否则不生效
#define __USART_GPIO_OTYPE				GPIO_OType_PP		// 务必要这样设置，否则不生效
#define __USART_GPIO_PUPD				GPIO_PuPd_UP		// 务必要这样设置，否则不生效


vu8 SYS_debug_level = ePRINT_DEBUG;

const char *color_table[ePRT_COLOR_MAX] = {
	PCLR_DFT,
	PCLR_RED,
	PCLR_GRN,
	PCLR_YEL,
	PCLR_BLU,
	PCLR_PUR,
	PCLR_LBL,
	PCLR_WHI,
	PCLR_RED_B,
	PCLR_GRN_B,
	PCLR_YEL_B,
	PCLR_BLU_B,
	PCLR_PUR_B,
	PCLR_LBL_B,
	PCLR_WHI_B,
};


void print_string(const char *str)
{
	int i = 0;

	while (str[i] != '\0') {
		usart2_send_1byte((u8)(str[i++]));
	}

}

void u1_print_string(const char *str)
{
	int i = 0;

	while (str[i] != '\0') {
		usart1_send_1byte_with_dma((u8)(str[i++]));
	}

}

static u8 hex_table[16] = {
	'0', '1', '2', '3', 
	'4', '5', '6', '7',	
	'8', '9', 'A', 'B',	
	'C', 'D', 'E', 'F'	
};

void uart2_snd_fmt_hex(u8 x)
{	
	usart2_send_1byte(hex_table[x & 0xF]);
}

void print_02X(BOOL pre, u8 num)
{	
	if (pre) {
		usart2_send_1byte('0');
		usart2_send_1byte('x');
	}
	
	uart2_snd_fmt_hex(num >> 4);
	uart2_snd_fmt_hex(num);
}


void uart1_snd_fmt_hex(u8 x)
{	
	usart1_send_1byte_with_dma(hex_table[x & 0xF]);
}

void u1_print_02X(BOOL pre, u8 num)
{	
	if (pre) {
		usart1_send_1byte_with_dma('0');
		usart1_send_1byte_with_dma('x');
	}
	
	uart1_snd_fmt_hex(num >> 4);
	uart1_snd_fmt_hex(num);
}



#define POWERS_OF_TEN_9 		(1000000000UL)
#define POWERS_OF_TEN_8 		(100000000UL)
#define POWERS_OF_TEN_7 		(10000000UL)
#define POWERS_OF_TEN_6 		(1000000UL)
#define POWERS_OF_TEN_5 		(100000UL)
#define POWERS_OF_TEN_4 		(10000U)
#define POWERS_OF_TEN_3 		(1000U)
#define POWERS_OF_TEN_2 		(100U)
#define POWERS_OF_TEN_1 		(10U)
//#define POWERS_OF_TEN_0 		(1U)


static u32 PO10_32bit_table[9] = {
	POWERS_OF_TEN_9,
	POWERS_OF_TEN_8,
	POWERS_OF_TEN_7,
	POWERS_OF_TEN_6,
	POWERS_OF_TEN_5,
	POWERS_OF_TEN_4,
	POWERS_OF_TEN_3,
	POWERS_OF_TEN_2,
	POWERS_OF_TEN_1,
};


void print_DEC(u32 v)
{
	u8 i;
	u8 result;
	u8 pad_zero = 0;

	for (i = 0; i < 9; i++) {
		if (v >= PO10_32bit_table[i]) {
			if ( ! pad_zero ) {
				pad_zero = 1;
			}

			result = (u8)(v / PO10_32bit_table[i]);
			
			uart2_snd_fmt_hex(result);
			v = v - result * PO10_32bit_table[i];
		}
		else {
			if (pad_zero) {
				uart2_snd_fmt_hex(0);
			}
		}
	}
	
	uart2_snd_fmt_hex((u8)v);
}

void u1_print_DEC(u32 v)
{
	u8 i;
	u8 result;
	u8 pad_zero = 0;

	for (i = 0; i < 9; i++) {
		if (v >= PO10_32bit_table[i]) {
			if ( ! pad_zero ) {
				pad_zero = 1;
			}

			result = (u8)(v / PO10_32bit_table[i]);
			
			uart1_snd_fmt_hex(result);
			v = v - result * PO10_32bit_table[i];
		}
		else {
			if (pad_zero) {
				uart1_snd_fmt_hex(0);
			}
		}
	}
	
	uart1_snd_fmt_hex((u8)v);
}

/* +------------------------------------------+ *
 * |	          重定向 fputc  		      | *
 * +------------------------------------------+ */
int fputc(int ch, FILE *f)
{
	usart1_send_1byte_with_dma((u8)ch);

//	usart2_send_1byte(ch);

	return ch;
}


