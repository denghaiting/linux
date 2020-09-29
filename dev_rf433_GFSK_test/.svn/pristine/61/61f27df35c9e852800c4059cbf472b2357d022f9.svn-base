/****************************************Copyright (c)****************************************
                                                                                                                                             
                                           
                             Galaxywind Network System Co., Ltd.                                                    
                                                                                                                                
                                  http://www.galaxywind.com                                                             
                                                                                                                                             
                                   
 Author:        Y
 Date  :        2020-7-16
 *********************************************************************************************/
#ifndef _RF433_H_
#define _RF433_H_

#include "stm32f0xx.h"
#include "CMT2300A_spi.h"
#include "CMT2300A_reg.h"
#include "CMT2300A.h"
#include "CMT2300A_reg_bank.h"



#if 0 // TODO: 中继模式
#define IS_DELAY
#endif




#define RF_PAYLOAD_LENGTH 	63

void power_init(void);
void ANT_switch_to_Tx(void);
void ANT_switch_to_Rx(void); // TODO: 这个是常态
void TxRx_en_init(void);

BOOL is_txing(void);
BOOL is_relaying(void);

void dbg_len(void *ptr, u16 len);
void cat_info(void *ptr, u16 len);
void set_tx_count(void *ptr, u16 len);
void set_tx_space(void *ptr, u16 len);
void set_tx_len(void *ptr, u16 len);
void do_rf_cfg(void *ptr, u16 len);

BOOL rf433m_set_reg_bank(int set);

void tx_done_isr(void);
void rx_pkt_isr(void);


BOOL rf433_rx_mode(void);

//void do_tx_one_pkt(void *ptr, u16 len);

void do_rf_start_tx(void *ptr, u16 len);
void do_rf_start_rx(void *ptr, u16 len);
void do_rf_end_rx(void *ptr, u16 len);
void do_rf_start_gw_test(void *ptr, u16 len);
void do_rf_start_relay(void *ptr, u16 len);

void start_relay(void);

void rf_loop_proc(void);


void rf433_init(void);

void set_ch(void *ptr, u16 len);



#endif	// #ifndef _RF433_H_
