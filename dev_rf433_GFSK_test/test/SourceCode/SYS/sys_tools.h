/*###################################*
 *#                                 #*
 *#     Author: Yu                  #*
 *#     Email : 33076156@qq.com     #*
 *#     Date  : 2020.3.10           #*
 *#                                 #*
 *###################################*/
#ifndef _SYS_TOOLS_H_
#define _SYS_TOOLS_H_

#include "stm32f0xx.h"



#define IS_AF(c)  				((c >= 'A') && (c <= 'F'))
#define IS_af(c)  				((c >= 'a') && (c <= 'f'))
#define IS_09(c)  				((c >= '0') && (c <= '9'))
#define ISVALIDHEX(ch) 			(IS_AF(ch) || IS_af(ch) || IS_09(ch))
#define ISVALIDDEC(c)  			IS_09(c)
#define CONVERTDEC(c)  			(c - '0')

#define CONVERTHEX_alpha(c)  	(IS_AF(c) ? (c - 'A' + 10) : (c - 'a' + 10))
#define CONVERTHEX(c)   		(IS_09(c) ? (c - '0') : CONVERTHEX_alpha(c))\




#define htonl(A) 	((((u32)(A) & 0xff000000) >> 24) | \
					(((u32)(A) & 0x00ff0000) >> 8) | \
					(((u32)(A) & 0x0000ff00) << 8) | \
					(((u32)(A) & 0x000000ff) << 24))

#define htons(A) 	((((u16)(A) & 0xff00) >> 8) | \
					(((u16)(A) & 0x00ff) << 8))

	
#define SIMPLE_DELAY_TIME 		(0x07) // 大约1us




void delay_us(u32 n);
void delay_ms(u32 n);
void delay_s(u32 n);
void simple_delay_us(u32 t);

u32 Str2Int(u8 *inputstr, int *intnum);
u32 HEXStr2Int(u8 *inputstr, int *intnum);
int check_str_is_valid_hex(const char *str, int len);



static inline void DISABLE_IRQ(void)
{
	__asm {
		CPSID I
		NOP 
	}
}

static inline void ENABLE_IRQ(void)
{
	__asm {
		NOP 
		CPSIE I
	}
}

static inline u8 __get_PRIMASK_DISABLE_IRQ(void)
{
	register u8 __regPriMask;
	__asm{
		MRS __regPriMask,primask
		CPSID I
		NOP 
	}
	return(__regPriMask);
}


#define __MIN(a, b)		((a) <= (b) ? (a) : (b))




#define ONE_DAY_MIN			(1440U)
#define ONE_DAY_SEC			(86400UL)
#define ONE_HOUR_MIN		(60)
#define ONE_HOUR_SEC		(3600U)
#define ONE_MIN_SEC			(60)

typedef u32  time_t;

struct tm {
	u32 tm_sec;
	u32 tm_min;
	u32 tm_hour;
	u32 tm_mday;
	u32 tm_mon;
	u32 tm_year;
	u32 tm_wday;
	u32 tm_yday;
	u32 tm_isdst;
};

/* 默认时区 */
#define DEFAULT_T_ZONE 	8 	//东8区


struct tm *simple_localtime_r(const time_t t, struct tm *ret_tm, s8 t_zone);


#endif	// #ifndef _SYS_TOOLS_H_

/* END OF FILE */

