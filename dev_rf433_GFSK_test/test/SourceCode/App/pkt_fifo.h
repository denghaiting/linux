/****************************************Copyright (c)****************************************
 																							
                             Galaxywind Network System Co., Ltd.						    
                                      												
                                  http://www.galaxywind.com								
 																						
 Author:	Y
 Date  : 	2020-07-21
 *********************************************************************************************/ 
#ifndef _PKT_FIFO_H_
#define _PKT_FIFO_H_

#include "stm32f0xx.h"
#include "rf433.h"



#define MAX_PKT_LEN	(RF_PAYLOAD_LENGTH)



typedef struct pkt_s {
	u8 len;
	u8 buf[MAX_PKT_LEN];
	u8 rssi;
	u8 pad[3];
} pkt_t;

typedef struct pkt_fifo_s {
	pkt_t *pkt;
	u32 size;
	u32 in;
	u32 out;
} pkt_fifo_t;


void __memcpy(void *dst, const void *src, u32 count); 

u32 get_pkt_fifo_len(pkt_fifo_t *pkt_fifo);
u8 add_one_pkt_to_fifo(pkt_fifo_t *pkt_fifo, const u8 *data, u8 data_len, u8 rssi);
pkt_t *fetch_one_pkt_from_fifo(pkt_fifo_t *pkt_fifo);
void pkt_fifo_init(pkt_fifo_t *pkt_fifo, pkt_t *pkt, u32 size);



#endif /* #ifndef _BOLT_PKT_FIFO_H_ */


/* END OF FILE */

