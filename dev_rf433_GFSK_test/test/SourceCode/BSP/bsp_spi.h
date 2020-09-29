 /****************************************Copyright (c)****************************************
 																							
                             Galaxywind Network System Co., Ltd.						    
                                      												
                                  http://www.galaxywind.com								
 																						
 Author:	Y
 Date  : 	2020-2-13
 *********************************************************************************************/ 
#ifndef _BSP_SPI_H_
#define _BSP_SPI_H_

#include "stm32f0xx.h"



void spi1_init(void);
void spi1_fini(void);
void spi1_write_reg(u8 reg, u8 value);
u8 spi1_read_reg(u8 reg);
void spi1_write_buf(u8 reg, u8 *pBuf, u32 len);
void spi1_read_buf(u8 reg, u8 *pBuf, u32 len);

#if 0
void spi2_init(void);
void spi2_fini(void);
void spi2_write_reg(u8 reg, u8 value);
u8 spi2_read_reg(u8 reg);
void spi2_write_buf(u8 reg, u8 *pBuf, u32 len);
void spi2_read_buf(u8 reg, u8 *pBuf, u32 len);
#endif



#endif	// #ifndef _BSP_SPI_H_



