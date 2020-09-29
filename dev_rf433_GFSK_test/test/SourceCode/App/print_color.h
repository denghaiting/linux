/****************************************Copyright (c)****************************************
 																							
                             Galaxywind Network System Co., Ltd.						    
                                      												
                                  http://www.galaxywind.com								
 																						
 Author:	Y
 Date  : 	2018-05-10
 *********************************************************************************************/ 
     
#ifndef _PRINTCOLOR_H_
#define _PRINTCOLOR_H_


#define PCLR_DFT 	"\033[0m"

#define PCLR_RED 	"\033[31m"
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

#define CURSOR_CLEAR_LINE 	"\033[K"  // ����ӹ�굽��β������ 

#define HIDE_CURSOR			"\033[?25l"  // ���ع��   
#define DISPLAY_CURSOR		"\033[?25h"  // ��ʾ���   


#define SAVE_CURSOR			"\033[s"  // ������λ�� 
#define RESTORE_CURSOR 		"\033[u"  // �ָ����λ�� 


#endif	// #ifndef _PRINTCOLOR_H_

/* END OF FILE */

