/****************************************Copyright (c)****************************************
 																							
                             Galaxywind Network System Co., Ltd.						    
                                      												
                                  http://www.galaxywind.com								
 																						
 Author:	Y
 Date  : 	2020-07-21
 *********************************************************************************************/ 
#include "pkt_fifo.h"



void __memcpy(void *dst, const void *src, u32 count)  
{    
	u8 *pdst = (u8 *)dst;
	const u8 *psrc = (const u8 *)src;

#if 0
	assert(dst != NULL);       
	assert(src != NULL);       
	
	assert(!(psrc <= pdst && pdst<psrc+count));
	assert(!(pdst <= psrc && psrc<pdst+count)); 
#endif

	while (count--) {
		*pdst = *psrc;          
		pdst++;          
		psrc++;      
	}  
}


u32 get_pkt_fifo_len(pkt_fifo_t *pkt_fifo)
{
	return pkt_fifo->in - pkt_fifo->out;
}

u8 add_one_pkt_to_fifo(pkt_fifo_t *pkt_fifo, const u8 *data, u8 data_len, u8 rssi)
{
	pkt_t *pkt;

	if ( ! (pkt_fifo->size - pkt_fifo->in + pkt_fifo->out) ) { //满了
		return 0;
	}

	pkt = pkt_fifo->pkt + (pkt_fifo->in & (pkt_fifo->size - 1));

	pkt->len = (data_len <= sizeof(pkt->buf)) ? data_len : sizeof(pkt->buf);

	__memcpy(pkt->buf, data, pkt->len);
	pkt->rssi = rssi;

	pkt_fifo->in += 1;

	return 1;
}


pkt_t *fetch_one_pkt_from_fifo(pkt_fifo_t *pkt_fifo)
{
	pkt_t *pkt;

	if (pkt_fifo->in == pkt_fifo->out) { //空的
		return NULL;
	}

	pkt = pkt_fifo->pkt + (pkt_fifo->out & (pkt_fifo->size - 1));

	pkt_fifo->out += 1;

	return pkt;
}


void pkt_fifo_init(pkt_fifo_t *pkt_fifo, pkt_t *pkt, u32 size)
{
	pkt_fifo->pkt = pkt;
	pkt_fifo->size = size;
	pkt_fifo->in = 0;
	pkt_fifo->out = 0;
}

/* +------------------------------------------+ *
 * |				清空缓冲区          	  | *
 * +------------------------------------------+ */
void clear_pkt_fifo(pkt_fifo_t *pkt_fifo)
{
	pkt_fifo->in = pkt_fifo->out = 0;
}




/* END OF FILE */

