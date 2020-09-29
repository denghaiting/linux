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

#define MAX_LOAD_TICKS 	((u32)0xFFFFFF) // 48M算下来 349525微秒
// TODO: 48M下的量程349毫秒 非常短

#define US_TO_TICKS(us) 	(us * 48)
#define MS_TO_TICKS(ms) 	(ms * 48000U)

// TODO: 使用这个函数时注意量程 最大349毫秒
void systick_delay(u32 delay_time);

void enable_systick(void);
void disable_systick(void);
void systick_init(void);


#endif /* #ifndef _SYSTICK_H_ */


/* END OF FILE */

