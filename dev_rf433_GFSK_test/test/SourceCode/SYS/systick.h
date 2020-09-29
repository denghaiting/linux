/*###################################*
 *#                                 #*
 *#     Author: Yu                  #*
 *#     Email : 33076156@qq.com     #*
 *#     Date  : 2020.3.10           #*
 *#                                 #*
 *###################################*/
#ifndef _SYSTICK_H_
#define _SYSTICK_H_

#include "stm32f0xx.h"

#define MAX_LOAD_TICKS 	((u32)0xFFFFFF) // 48M������ 349525΢��
// TODO: 48M�µ�����349���� �ǳ���

#define US_TO_TICKS(us) 	(us * 48)
#define MS_TO_TICKS(ms) 	(ms * 48000U)

// TODO: ʹ���������ʱע������ ���349����
void systick_delay(u32 delay_time);

void enable_systick(void);
void disable_systick(void);
void systick_init(void);


#endif /* #ifndef _SYSTICK_H_ */


/* END OF FILE */

