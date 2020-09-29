/*###################################*
 *#                                 #*
 *#     Author: Yu                  #*
 *#     Email : 33076156@qq.com     #*
 *#     Date  : 2020.3.10           #*
 *#                                 #*
 *###################################*/

#include "sys_tools.h"
#include "sys.h"
#include "systick.h"
#include "sys_config.h"



void delay_us(u32 n)
{
	u32 i;
	
	for (i = 0; i < n; i++) {
		systick_delay(48);
	}
}



void delay_ms(u32 n)
{
	u32 i;
	
	for (i = 0; i < n; i++) {
		systick_delay(48000); // 量程原因 
	}
}


void delay_s(u32 n)
{
	u32 i;
	u8 j;
	
	for (i = 0; i < n; i++) {
		for (j = 0; j < 5; j++) {
			systick_delay(MS_TO_TICKS(200)); // 量程原因 
		}
		feed_iwdg();
	}
}

/* +------------------------------------------+ *
 * |	  		     简单延时         	      | *
 * +------------------------------------------+ */
void simple_delay_us(u32 t)
{
	u32 i, j;
	
	for (i = 0; i < t; i++) {
		for (j = 0; j < SIMPLE_DELAY_TIME; j++);
	}
}



/* +------------------------------------------+ *
 * |	        数字字符串转数字              | *
 * +------------------------------------------+ */
	
u32 Str2Int(u8 *inputstr, int *intnum)
{
	u32 i = 0, res = 0;
	u32 val = 0;

	if (inputstr[0] == '0' && (inputstr[1] == 'x' || inputstr[1] == 'X')) {
		if (inputstr[2] == '\0') {
			return 0;
		}
		for (i = 2; i < 11; i++) {
			if (inputstr[i] == '\0') {
				*intnum = val;
				res = 1;
				break;
			}
			if (ISVALIDHEX(inputstr[i])) {
				val = (val << 4) + CONVERTHEX(inputstr[i]);
			} else {
				res = 0;
				break;
			}
		}
		/* over 8 digit hex --invalid */
		if (i >= 11) {
			res = 0;
		}
	} 
	else {/* max 10-digit decimal input */
	
		for (i = 0; i < 11; i++) {
			if (inputstr[i] == '\0') {
				*intnum = val;
				/* return 1 */
				res = 1;
				break;
			} else if ((inputstr[i] == 'k' || inputstr[i] == 'K') && (i > 0)) {
				val = val << 10;
				*intnum = val;
				res = 1;
				break;
			} else if ((inputstr[i] == 'm' || inputstr[i] == 'M') && (i > 0)) {
				val = val << 20;
				*intnum = val;
				res = 1;
				break;
			} else if (ISVALIDDEC(inputstr[i])) {
				val = val * 10 + CONVERTDEC(inputstr[i]);
			} else {
				/* return 0, Invalid input */
				res = 0;
				break;
			}
		}
		
		/* Over 10 digit decimal --invalid */
		if (i >= 11) {
			res = 0;
		}
	}

	return res;
}



u32 HEXStr2Int(u8 *inputstr, int *intnum)
{
	u32 i = 0, res = 0;
	u32 val = 0;

	for (i = 0; i < 11; i++) {
		if (inputstr[i] == '\0') {
			*intnum = val;
			res = 1;
			break;
		}
		if (ISVALIDHEX(inputstr[i])) {
			val = (val << 4) + CONVERTHEX(inputstr[i]);
		} else {
			res = 0;
			break;
		}
	}
	/* over 8 digit hex --invalid */
	if (i >= 11) {
		res = 0;
	}

	return res;
}


int check_str_is_valid_hex(const char *str, int len)
{
	int i;

	for (i = 0; i < len; i++) {
		if ( ! ISVALIDHEX(str[i]) ) {
			return 0;
		}
	}

	return 1;
}





static u8 leap(u16 year) 
{ 
    return ((( ! (year % 4) ) && (year % 100)) || ( ! (year % 400) )) ? 1 : 0;
} 


static const u8 day_tab[2][12] = {
	{31,28,31,30,31,30,31,31,30,31,30,31},
	{31,29,31,30,31,30,31,31,30,31,30,31}
}; 


#if 0
#define DT_DEBUG   printf
#else
#define DT_DEBUG   /\
/
#endif


struct tm *simple_localtime_r(const time_t t, struct tm *ret_tm, s8 t_zone)
{
    u32 days = t / ONE_DAY_SEC;
    u32 subsecs = t % ONE_DAY_SEC;
	u32 subsecs2;
    u32 years = 0;
    u32 i = 365;

	u32 subdays;
    u8 isleap;
    u32 nMonCounter = 0;
	u8 x = 0;
    u8 nMons = 0;
	
    while (i < days) {
        years++;
        i += 365;
		
		if (leap(1970 + years)) {
            i += 1;
        }
    }
	
    ret_tm->tm_year = years;
    ret_tm->tm_mon = 0;
	
    subdays = days - (i - 365);
	isleap = leap(1970 + years);

	for (nMonCounter = 0, nMons = 0; ; ) {
		if ((nMonCounter + day_tab[isleap][nMons]) >= subdays) {
			break;
		}
		nMonCounter += day_tab[isleap][nMons];
		nMons = (nMons + 1) % 12;		
		//ret_tm->tm_mon += 1;
		x += 1;
		ret_tm->tm_mon = x;
		//printf("ret_tm->tm_mon : %lu\r\n", ret_tm->tm_mon);
		//printf("ret_tm->tm_mon : %lu,%bx,%bx,%bx,%bx\r\n", x,(char)x,(char)(x>>8),(char)(x>>16),(char)(x>>24));
	}

	
	//DT_DEBUG("mon : %02lu\r\n", ret_tm->tm_mon + 1);
	
    ret_tm->tm_mday = subdays + 1 - nMonCounter;
	
	//DT_DEBUG("day : %02lu\r\n", ret_tm->tm_mday);


    ret_tm->tm_hour= (subsecs / 3600 + 24 + t_zone) % 24;
	//DT_DEBUG("hour : %02lu\r\n", ret_tm->tm_hour);
	
    subsecs2 = subsecs - ((s32)(subsecs / 3600)) * 3600;
    ret_tm->tm_min = subsecs2 / 60;
	//DT_DEBUG("min : %02lu\r\n", ret_tm->tm_min);

    ret_tm->tm_sec = subsecs2 - ((s32)(subsecs2 / 60)) * 60;
	//DT_DEBUG("sec : %02lu\r\n", ret_tm->tm_sec);

    //计算星期
    ret_tm->tm_wday=(days + 4) % 7;
	//DT_DEBUG("ret_tm->tm_wday : %ld\r\n", ret_tm->tm_wday);
	
    return ret_tm;
}




/* END OF FILE */

