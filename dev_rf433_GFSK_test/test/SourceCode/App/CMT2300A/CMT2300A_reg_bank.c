/****************************************Copyright (c)****************************************
                                                                                                                                             
                                           
                             Galaxywind Network System Co., Ltd.                                                    
                                                                                                                                
                                  http://www.galaxywind.com                                                             
                                                                                                                                             
                                   
 Author:        Y
 Date  :        2019-9-2
 *********************************************************************************************/
#include "stm32f0xx.h"
#include "CMT2300A_spi.h"
#include "CMT2300A_reg.h"
#include "CMT2300A.h"

#include "rfpdk_list_100K_20dB.c"
#include "rfpdk_list_100K_13dB.c"
#include "rfpdk_list_100K_10dB.c"
#include "rfpdk_list_64K_20dB.c"
#include "rfpdk_list_32K_20dB.c"
#include "rfpdk_list_16K_20dB.c"
#include "rfpdk_list_2400_20dB.c"
#include "rfpdk_list_GFSK_2_4k_5k_13db.c"
#include "rfpdk_list_GFSK_2400_20k_20db.c"
#include "rfpdk_list_GFSK_16k_32k_20db.c"
#include "rfpdk_list_GFSK_32k_64k_20db.c"
#include "rfpdk_list_GFSK_64k_128k_20db.c"
#include "rfpdk_list_GFSK_16k_32k_20db_gbt1.c"
#include "rfpdk_list_GFSK487_5_64_128_20db.c"
#include "rfpdk_list_GFSK487_5_16_32_20db.c"
#include "rfpdk_list_rssi_16k.c"
#include "rfpdk_list_rssi_64k.c"
//#include "rfpdk_list_16k_32k_20db.c"

// TODO: 64K

//[CMT Bank]
const u16 CMT_bank_64K[12] = {
0x0000,
0x0166,
0x02EC,
0x031D,//0x031D, 
0x0430,
0x0580,
0x0614,
0x0708,
0x0891,
0x0902,
0x0A02,
0x0BD0,
};
//[System Bank]
const u16 System_bank_64K[12] = {
0x0CAE,
0x0DE0,
0x0E35,
0x0F00,
0x1000,
0x11F4,
0x1210,
0x13E2,
0x1442,
0x1520,
0x1600,
0x1781,
};
//[Frequency Bank]
const u16 Frequency_bank_64K[8] = {
0x1842,
0x1971,
0x1ACE,
0x1B1C,
0x1C42,
0x1D5B,
0x1E1C,
0x1F1C,
};
//[Data Rate Bank]
const u16 DataRate_bank_64K[24] = {
0x2047,
0x2185,
0x2272,
0x2310,
0x24EC,
0x2536,
0x261F,
0x270A,
0x289F,
0x294B,
0x2A29,
0x2B29,
0x2CC0,
0x2D96,
0x2E01,
0x2F53,
0x3020,
0x3100,
0x32B4,
0x3300,
0x3400,
0x3501,
0x3600,
0x3700,
};

//[Baseband Bank]
const u16 Baseband_bank_64K[29] = {
0x3812,
0x3908,
0x3A00,
0x3BAA,
0x3C02,
0x3D00,
0x3E00,
0x3F00,
0x4000,
0x4100,
0x4200,
0x43D4,
0x442D,
0x4501,
0x4620,
0x4700,
0x4800,
0x4900,
0x4A00,
0x4B00,
0x4C01,
0x4DFF,
0x4E00,
0x4F64,
0x50FF,
0x5100,
0x5200,
0x531F,
0x5410,
};
//[TX Bank]
const u16 TX_bank_64K[11] = {
0x5570,
0x5607,
0x577E,
0x5800,
0x5901,
0x5A30,
0x5B00,
0x5C8A,
0x5D18,
0x5E3F,
0x5F7F,
};





static void __config_reg_bank(u16 *bank, u8 bank_len)
{
	u8 i, reg_addr, value;

	for (i = 0; i < bank_len; i++) {
		reg_addr = (u8)((bank[i] >> 8) & 0xFF);
		value = (u8)((bank[i] >> 0) & 0xFF);
		write_reg(reg_addr, value);
	}
}


static void config_reg_bank_whole_array(u8 *bank)
{
	u8 i, value;

	for (i = 0; i < 0x60; i++) {
		value = bank[i];
		write_reg(i, value);
	}
}


void CMT2300A_config_reg_bank_100K_20dB(void)
{
	config_reg_bank_whole_array((u8 *)CMTBank_100K_20dB);
}

void CMT2300A_config_reg_bank_100K_13dB(void)
{
	config_reg_bank_whole_array((u8 *)CMTBank_100K_13dB);
}

void CMT2300A_config_reg_bank_100K_10dB(void)
{
	config_reg_bank_whole_array((u8 *)CMTBank_100K_10dB);
}

void CMT2300A_config_reg_bank_64K_20dB(void)
{
	config_reg_bank_whole_array((u8 *)CMTBank_64K_20dB);
}

void CMT2300A_config_reg_bank_32K_20dB(void)
{
	config_reg_bank_whole_array((u8 *)CMTBank_32K_20dB);
}

void CMT2300A_config_reg_bank_16K_20dB(void)
{
	config_reg_bank_whole_array((u8 *)CMTBank_16K_20dB);
}


void CMT2300A_config_reg_bank_2400_20dB(void)
{
	config_reg_bank_whole_array((u8 *)CMTBank_2400_20dB);
}

void CMT2300A_config_reg_bank_GFSK_2_4K_5K_13dB(void)
{
	config_reg_bank_whole_array((u8 *)CMTBank_GFSK_2_4K_5k_13dB);
}

void CMT2300A_config_reg_bank_GFSK_2400_20k_20db(void)
{
	config_reg_bank_whole_array((u8 *)CMTBank_GFSK_2400_20k_20db);
}

void CMT2300A_config_reg_bank_GFSK_16k_32k_20db(void)
{
	config_reg_bank_whole_array((u8 *)CMTBank_GFSK_16k_32k_20db);
}

void CMT2300A_config_reg_bank_GFSK_32k_64k_20db(void)
{
	config_reg_bank_whole_array((u8 *)CMTBank_GFSK_32k_64k_20db);
}

void CMT2300A_config_reg_bank_GFSK_64k_128k_20db(void)
{
	config_reg_bank_whole_array((u8 *)CMTBank_GFSK_64k_128k_20db);
}

void CMT2300A_config_reg_bank_GFSK_16k_32k_20db_gbt1(void)
{
	config_reg_bank_whole_array((u8 *)CMTBank_GFSK_16k_32k_20db_gbt1);
}

void CMT2300A_config_reg_bank_16k_32k_20db(void)
{
//	config_reg_bank_whole_array((u8 *)CMTBank_16k_32k_20db);
}

void CMT2300A_config_reg_bank_GFSK487_5_64_128_20db(void)
{
	config_reg_bank_whole_array((u8 *)rfpdk_list_GFSK487_5_64_128_20db);
}

void CMT2300A_config_reg_bank_GFSK487_5_16_32_20db(void)
{
	config_reg_bank_whole_array((u8 *)rfpdk_list_GFSK487_5_16_32_20db);
}

void CMT2300A_config_reg_bank_rssi_16k(void)
{
	config_reg_bank_whole_array((u8 *)rssi_16k);
}

void CMT2300A_config_reg_bank_rssi_64k(void)
{
	config_reg_bank_whole_array((u8 *)rssi_64k);
}

void CMT2300A_config_reg_bank_64K(void)
{
    __config_reg_bank((u16 *)CMT_bank_64K, sizeof(CMT_bank_64K) / sizeof(u16));
    __config_reg_bank((u16 *)System_bank_64K, sizeof(System_bank_64K) / sizeof(u16));
    __config_reg_bank((u16 *)Frequency_bank_64K, sizeof(Frequency_bank_64K) / sizeof(u16));
    __config_reg_bank((u16 *)DataRate_bank_64K, sizeof(DataRate_bank_64K) / sizeof(u16));
    __config_reg_bank((u16 *)Baseband_bank_64K, sizeof(Baseband_bank_64K) / sizeof(u16));
    __config_reg_bank((u16 *)TX_bank_64K, sizeof(TX_bank_64K) / sizeof(u16));
}



