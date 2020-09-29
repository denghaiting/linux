/****************************************Copyright (c)****************************************
 																							
                             Galaxywind Network System Co., Ltd.						    
                                      												
                                  http://www.galaxywind.com								
 																						
 Author:	Y
 Date  : 	2019-07-19
 *********************************************************************************************/ 
#ifndef _DEBUG_UART_H_
#define _DEBUG_UART_H_

#include <stdio.h>
#include <string.h>
#include "stm32f0xx.h"



enum {
	ePRINT_DEBUG = 0,
	ePRINT_INFO,
	ePRINT_REMIND,
	ePRINT_WARN,
	ePRINT_ERROR,
	ePRINT_HIGHEST_LEVEL,
};


enum {
	ePRT_COLOR_NONE = 0,
	ePRT_COLOR_RED,
	ePRT_COLOR_GRN,
	ePRT_COLOR_YEL,
	ePRT_COLOR_BLU,
	ePRT_COLOR_PUR,
	ePRT_COLOR_LBL,
	ePRT_COLOR_WHI,
	ePRT_COLOR_RED_B,
	ePRT_COLOR_GRN_B,
	ePRT_COLOR_YEL_B,
	ePRT_COLOR_BLU_B,
	ePRT_COLOR_PUR_B,
	ePRT_COLOR_LBL_B,
	ePRT_COLOR_WHI_B,
	ePRT_COLOR_MAX,
};

extern vu8 SYS_debug_level;
extern vu8 SYS_rf_dump;
extern vu8 SYS_upmo_dump;

extern const char *color_table[ePRT_COLOR_MAX];



#define  XX_PRINT(level, color, name, line, fmt, args...) \
	do {\
		if (level >= SYS_debug_level) {\
			printf("%s<%s:%03d> " fmt PCLR_DFT, color_table[color % ePRT_COLOR_MAX], name, line, ##args);\
		}\
	} while (0);



#define PCLR_DFT 	"\033[0m"

#define PCLR_RED 	"\033[31m"
#define PCLR_GRN 	"\033[32m"
#define PCLR_YEL 	"\033[33m"
#define PCLR_BLU 	"\033[34m"
#define PCLR_PUR 	"\033[35m"
#define PCLR_LBL 	"\033[36m"
#define PCLR_WHI 	"\033[37m"

#define PCLR_RED_B 	"\033[31m\033[1m"
#define PCLR_GRN_B 	"\033[32m\033[1m"
#define PCLR_YEL_B 	"\033[33m\033[1m"
#define PCLR_BLU_B 	"\033[34m\033[1m"
#define PCLR_PUR_B 	"\033[35m\033[1m"
#define PCLR_LBL_B 	"\033[36m\033[1m"
#define PCLR_WHI_B 	"\033[37m\033[1m"

#define NEW_LINE 	"\r\n"

#define CLEAR_SCREEN 	"\033[2J" 
#define ORIGINAL_POINT 	"\033[0;0H" 





//#define PRTS(str) 			print_string((const char *)(str))
#define PRTH(pre, x) 		print_02X(pre, x)

//#define PUT_CHAR(Byte) 		usart1_send_byte(Byte)


//void usart1_send_byte(u8 byte);
//void usart1_wait_send_finish(void);

void uart2_snd_fmt_hex(u8 x);
void print_string(const char *str);
void print_02X(BOOL pre, u8 num);
void print_DEC(u32 v);


void uart1_snd_fmt_hex(u8 x);
void u1_print_string(const char *str);
void u1_print_02X(BOOL pre, u8 num);
void u1_print_DEC(u32 v);


void debug_uart_init(void);


#endif	
