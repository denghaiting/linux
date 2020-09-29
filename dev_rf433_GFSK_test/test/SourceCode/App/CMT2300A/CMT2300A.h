/****************************************Copyright (c)****************************************
                                                                                                                                             
                                           
                             Galaxywind Network System Co., Ltd.                                                    
                                                                                                                                
                                  http://www.galaxywind.com                                                             
                                                                                                                                             
                                   
 Author:        Y
 Date  :        2019-9-2
 *********************************************************************************************/

#ifndef __CMT2300A_H__
#define __CMT2300A_H__


#define READ_FIFO(buf, len) 	 CMT2300A_spi_read_fifo(buf, len)	
#define WRITE_FIFO(buf, len) 	 CMT2300A_spi_write_fifo(buf, len)	

u8 read_reg(u8 reg_addr);
void write_reg(u8 reg_addr, u8 byte);

u8 CMT2300A_IntSrcFlagClr(void);
void CMT2300A_AfterCfg(void);
void CMT2300A_switch_status(u8 cmd);
u8 CMT2300A_get_status(void);
void CMT2300A_soft_reset(void);
void CMT2300A_LFOSC_set(BOOL en);
void CMT2300A_ConfigGpio(u8 nGpioSel);
void CMT2300A_ConfigInterrupt(u8 nInt1Sel, u8 nInt2Sel);
void CMT2300A_EnableInterrupt(u8 nEnable);
void CMT2300A_EnableAntennaSwitch(u8 nMode);
void CMT2300A_EnableReadFifo(void);
void CMT2300A_EnableWriteFifo(void);

void CMT2300A_fifo_merge_enable(void);
void CMT2300A_fifo_merge_disable(void);


u8 CMT2300A_ClearRxFifo(void);
u8 CMT2300A_ClearTxFifo(void);


u8 CMT2300A_clear_it_flags(void);

u8 CMT2300A_get_RSSI_DBM(void);
u8 CMT2300A_get_RSSI_CODE(void);

#endif
