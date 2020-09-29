/****************************************Copyright (c)****************************************
                                                                                                                                             
                                           
                             Galaxywind Network System Co., Ltd.                                                    
                                                                                                                                
                                  http://www.galaxywind.com                                                             
                                                                                                                                             
                                   
 Author:        Y
 Date  :        2020-8-5
 *********************************************************************************************/
#include "freq_adjust.h"
#include "rf433.h"
#include "print_color.h"
#include "debug_uart.h"

#define EN_FRQAD_PRINTF

#if defined (EN_FRQAD_PRINTF)
#define FRQAD_NAME 	"freq_adjust.c"
#define FRQAD_DEBUG(fmt, args...)		printf(PCLR_GRN_B"<%s:%03d> " fmt PCLR_DFT, FRQAD_NAME, __LINE__, ##args)
#define FRQAD_INFO(fmt, args...)		printf(PCLR_YEL_B"<%s:%03d> " fmt PCLR_DFT, FRQAD_NAME, __LINE__, ##args)
#define FRQAD_REMIND(fmt, args...)		printf(PCLR_LBL_B"<%s:%03d> " fmt PCLR_DFT, FRQAD_NAME, __LINE__, ##args)
#define FRQAD_WARN(fmt, args...)		printf(PCLR_PUR_B"<%s:%03d> " fmt PCLR_DFT, FRQAD_NAME, __LINE__, ##args)
#define FRQAD_ERROR(fmt, args...)		printf(PCLR_RED_B"<%s:%03d> " fmt PCLR_DFT, FRQAD_NAME, __LINE__, ##args)
#else
#define FRQAD_DEBUG(fmt, args...)
#define FRQAD_INFO(fmt, args...)
#define FRQAD_REMIND(fmt, args...)
#define FRQAD_WARN(fmt, args...)
#define FRQAD_ERROR(fmt, args...)
#endif


#if 0
/* 记录当前信道 */
u8 cur_channel = DEFAULT_CH;


int set_custom_ch(u8 ch)
{	
	u8 tmp;
	u32 base_freq = BASE_FREQ;
	
	if ((ch < eCUSTOM_CH1) || (ch > eCUSTOM_CH33)) {
		FRQAD_ERROR("Invalid ch!"NEW_LINE);
		return -1; 
	}

	write_reg(eCUS_FREQ_CHNL, ch);
	tmp = read_reg(eCUS_FREQ_CHNL);
	if (ch != tmp) {
		FRQAD_ERROR("set FREQ_CHNL failed!"NEW_LINE);
		return -2;
	}

	cur_channel = ch;

	FRQAD_REMIND("Cur CH = %u(Freq: %u.%03u MHz)"NEW_LINE, 
		cur_channel, (base_freq + 50 * cur_channel) / 1000, (base_freq + 50 * cur_channel) % 1000);

	return 0;
}


int custom_ch_init(void)
{
	u8 tmp;
	
	// TODO: 设置OFFSET
	write_reg(eCUS_FREQ_OFS, FH_OFFSET);
	tmp = read_reg(eCUS_FREQ_OFS);
	if (FH_OFFSET != tmp) {
		FRQAD_ERROR("set FREQ_OFS failed!"NEW_LINE);
		return -1;
	}

	// TODO: 设置默认信道
	if (0 != set_custom_ch(cur_channel)) {
		FRQAD_ERROR("set_custom_ch failed!"NEW_LINE);
		return -2;
	}

	return 0;
}
#else
/* 记录当前信道 */
u8 cur_channel = DEFAULT_CH;


int set_custom_ch(u8 ch)
{	
	u8 tmp;
	u32 base_freq = BASE_FREQ;
	
	if ((ch < eCUSTOM_CH1) || (ch > eCUSTOM_CH9)) {
		FRQAD_ERROR("Invalid ch!"NEW_LINE);
		return -1; 
	}

	write_reg(eCUS_FREQ_CHNL, ch);
	tmp = read_reg(eCUS_FREQ_CHNL);
	if (ch != tmp) {
		FRQAD_ERROR("set FREQ_CHNL failed!"NEW_LINE);
		return -2;
	}

	cur_channel = ch;

	FRQAD_REMIND("Cur CH = %u(Freq: %u.%03u MHz)"NEW_LINE, 
		cur_channel, (base_freq + (FH_OFFSET * 5 / 2) * cur_channel) / 1000, (base_freq + (FH_OFFSET * 5 / 2) * cur_channel) % 1000);

	return 0;
}


int custom_ch_init(void)
{
	u8 tmp;
	
	// TODO: 设置OFFSET
	write_reg(eCUS_FREQ_OFS, FH_OFFSET);
	tmp = read_reg(eCUS_FREQ_OFS);
	if (FH_OFFSET != tmp) {
		FRQAD_ERROR("set FREQ_OFS failed!"NEW_LINE);
		return -1;
	}

	// TODO: 设置默认信道
	if (0 != set_custom_ch(cur_channel)) {
		FRQAD_ERROR("set_custom_ch failed!"NEW_LINE);
		return -2;
	}

	return 0;
}

#endif

