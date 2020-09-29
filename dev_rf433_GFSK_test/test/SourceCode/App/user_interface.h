/*###################################*
 *#                                 #*
 *#     Author: Yu                  #*
 *#     Email : 33076156@qq.com     #*
 *#     Date  : 2020.3.10           #*
 *#                                 #*
 *###################################*/
 
#ifndef __USER_INTERFACE_H__
#define __USER_INTERFACE_H__


#include "stm32f0xx.h"



//#define MAX_START_WAIT_CNT			3 // 上电跳转APP 等待倒计时时间SEC
#define MAX_START_WAIT_CNT			1 // 上电跳转APP 等待倒计时时间SEC
#define MAX_WAIT_CMD_TIME 			200 // 1000ms 设置为1秒

#define START_CMD_GOTO_USER 		'1'


#define CAP2SMA(ch) 				(ch + ('a' - 'A'))


typedef enum {
    eLP_IDLE,
	eLP_COMMON_TERMINAL,	
	//eLP_COMMON_UPMO,		//上位机   
    eLP_MAX
} loop_stat_e;


typedef  void (*FunVoidType)(void);


void ui_init(void);
void ui_loop(void);



#endif /* #ifndef __USER_INTERFACE_H__ */


