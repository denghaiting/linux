/****************************************Copyright (c)****************************************
                                                                                                                                             
                                           
                             Galaxywind Network System Co., Ltd.                                                    
                                                                                                                                
                                  http://www.galaxywind.com                                                             
                                                                                                                                             
                                   
 Author:        Y
 Date  :        2019-9-2
 *********************************************************************************************/

#ifndef __CMT2300A_SPI_H__
#define __CMT2300A_SPI_H__



void udelay(u32 i);

void CMT2300A_spi_read_reg(u8 reg_addr, u8 *read_buf, u8 read_len);
void CMT2300A_spi_write_reg(u8 reg_addr, u8 *write_buf, u8 write_len);

void CMT2300A_spi_read_fifo(u8 *read_buf, u8 read_len);
void CMT2300A_spi_write_fifo(u8 *write_buf, u8 write_len);

void CMT2300A_spi_init(void);


#endif
