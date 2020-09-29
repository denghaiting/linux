/****************************************Copyright (c)****************************************
 																							
                             Galaxywind Network System Co., Ltd.						    
                                      												
                                  http://www.galaxywind.com								
 																						
 Author:	Y
 Date  : 	2018-03-16
 *********************************************************************************************/ 
#ifndef __USER_INTERFACE_H__
#define __USER_INTERFACE_H__


#include "usart_comm.h"
#include "ymodem.h"


#define REBOOT_BYTE 				'#'
#define USER_START_CMD 				'C'
#define UPMO_START_CMD 				'D'
#define BACK_TO_USER_CMD 			'$'
#define BACK_TO_UPMO_CMD 			'@'
#define USER_START_CMD_CNT 			2
#define UPMO_START_CMD_CNT 			2
#define MAX_WAIT_CMD_TIME 			100 // 500ms

#define PAD 						0

/* 这里注意 一定要用大写字母 */
#define CH_CMD_HELP 	 			'H'
#define EXEG_HELP 	 				"help"
	
#define CH_CMD_INFO 	 			'I'
#define EXEG_INFO 	 				"info"
	
#define CH_CMD_JUMP 	 			'J'
#define EXEG_JUMP 	 				"jump"

#define CH_CMD_UPGRADE 				'U'
#define EXEG_UPGRADE 				"upgrade"

#define CH_CMD_VERSION 	 			'V'
#define EXEG_VERSION 	 			"version"


#define CAP2SMA(ch) 				(ch + ('a' - 'A'))


typedef enum {
    eLP_START,
	eLP_COMMON_TERMINAL,	
	//eLP_COMMON_UPMO,		//上位机   
    eLP_YMODEM,
    eLP_JUMP_APP,
    eLP_MAX
} loop_stat_e;


typedef struct {
	u8 check_ret;
	u8 pad;
	u8 crc[2];
	u16 size;
	u8 pad1[2];
	image_file_t imfl;
} app_image_info_t;


typedef  void (*FunVoidType)(void);








void reboot(void);

void ui_init(void);
void ui_loop(void);







#endif /* #ifndef __USER_INTERFACE_H__ */

/* END OF FILE */


