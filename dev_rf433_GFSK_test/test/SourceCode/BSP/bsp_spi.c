 /****************************************Copyright (c)****************************************
 																							
                             Galaxywind Network System Co., Ltd.						    
                                      												
                                  http://www.galaxywind.com								
 																						
 Author:	Y
 Date  : 	2020-2-13
 *********************************************************************************************/ 
#include "bsp_spi.h"
#include "bsp_gpio.h"
#include "sys_nvic_config.h"
#include "ssv7241.h"


#define SPI_CMD_NOP				(0xFF)


// TODO: SPI1
#if 1
void spi1_init(void)
{
	SPI_InitTypeDef SPI_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
	
	SPI_Cmd(SPI1, DISABLE);
	SPI_I2S_DeInit(SPI1);

	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;              //主模式
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;          //数据帧8Bits
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;                 //空闲时钟状态低电平
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;               //第一个边缘改变数值
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8;
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;              
	SPI_InitStructure.SPI_CRCPolynomial = 7;

	SPI_Init(SPI1, &SPI_InitStructure);

	SPI_RxFIFOThresholdConfig(SPI1, SPI_RxFIFOThreshold_QF);
	SPI_NSSInternalSoftwareConfig(SPI1, SPI_NSSInternalSoft_Set);

	SPI_Cmd(SPI1, ENABLE);

	while (SET == SPI_I2S_GetFlagStatus(SPI1, SPI_SR_FTLVL))
		;
	
	while (SET == SPI_I2S_GetFlagStatus(SPI1, SPI_SR_BSY))
		;

	while (SET == SPI_I2S_GetFlagStatus(SPI1, SPI_SR_FRLVL)) {
		SPI_ReceiveData8(SPI1);
	}

	
}


void spi1_fini(void)
{
	SPI_I2S_DeInit(SPI1);
	SPI_Cmd(SPI1, DISABLE);

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, DISABLE);
}



static u8 spi1_exchange_data(u8 data)
{
	u8 retry = 0;
	
	/* Wait for transfer finished. */
	while (RESET == SPI_I2S_GetFlagStatus(SPI1, SPI_SR_TXE)) {
		retry++;
		if (retry > 100) {
			return 0;		
		}
	}

	/* Write data (8 or 16 bits, depending on DFF) into DR. */
	SPI_SendData8(SPI1, data);
	retry = 0;

	/* Wait for transfer finished. */
	while (RESET == SPI_I2S_GetFlagStatus(SPI1, SPI_SR_RXNE)) {
		retry++;
		if (retry > 100) {
			return 0;		
		}
	}

	/* Read the data (8 or 16 bits, depending on DFF bit) from DR. */
	return SPI_ReceiveData8(SPI1);
}



void spi1_write_reg(u8 reg, u8 value)
{
	u16 r = (u16)reg & 0xFF;
	u16 v = (u16)value & 0xFF;
		
	spi1_exchange_data(r);
	spi1_exchange_data(v);
}


u8 spi1_read_reg(u8 reg)
{
	u8 value = 0;

	/* Wait for transfer finished. */
	while (RESET == SPI_I2S_GetFlagStatus(SPI1, SPI_SR_TXE))
		;

	/* Write data (8 or 16 bits, depending on DFF) into DR. */
	SPI_SendData8(SPI1, reg);

	/* Wait for transfer finished. */
	while (RESET == SPI_I2S_GetFlagStatus(SPI1, SPI_SR_RXNE))
		;

	/* Read the data (8 or 16 bits, depending on DFF bit) from DR. */
	SPI_ReceiveData8(SPI1);

	/* Wait for transfer finished. */
	while (RESET == SPI_I2S_GetFlagStatus(SPI1, SPI_SR_TXE))
		;

	/* Write data (8 or 16 bits, depending on DFF) into DR. */
	SPI_SendData8(SPI1, SPI_CMD_NOP);

	/* Wait for transfer finished. */
	while (RESET == SPI_I2S_GetFlagStatus(SPI1, SPI_SR_RXNE))
		;

	/* Read the data (8 or 16 bits, depending on DFF bit) from DR. */
	value = SPI_ReceiveData8(SPI1);
	
	return value;
}


void spi1_write_buf(u8 reg, u8 *pBuf, u32 len)
{
	u32 idx;
	u16 r = (u16)reg & 0xFF;
	u16 v = 0;

	spi1_exchange_data(r);

	for (idx = 0; idx < len; idx++) {
		v = (u16)(pBuf[idx]) & 0xFF;
		spi1_exchange_data(v); 
	}
}


void spi1_read_buf(u8 reg, u8 *pBuf, u32 len)
{
	u32 idx = 0;
	u16 r = (u16)reg & 0xFF;
	
	spi1_exchange_data(r);
	
	for (idx = 0; idx < len; idx++) {
		pBuf[idx] = (u8)(spi1_exchange_data(SPI_CMD_NOP) & 0xFF);
	}
}



// TODO: SPI2
#else

void spi2_init(void)
{
	SPI_InitTypeDef SPI_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);
	
	SPI_Cmd(SPI2, DISABLE);
	SPI_I2S_DeInit(SPI2);

	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;              //主模式
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;          //数据帧8Bits
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;                 //空闲时钟状态低电平
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;               //第一个边缘改变数值
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8;
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;              
	SPI_InitStructure.SPI_CRCPolynomial = 7;

	SPI_Init(SPI2, &SPI_InitStructure);

	SPI_RxFIFOThresholdConfig(SPI2, SPI_RxFIFOThreshold_QF);
	SPI_NSSInternalSoftwareConfig(SPI2, SPI_NSSInternalSoft_Set);

	SPI_Cmd(SPI2, ENABLE);

	while (SET == SPI_I2S_GetFlagStatus(SPI2, SPI_SR_FTLVL))
		;
	
	while (SET == SPI_I2S_GetFlagStatus(SPI2, SPI_SR_BSY))
		;

	while (SET == SPI_I2S_GetFlagStatus(SPI2, SPI_SR_FRLVL)) {
		SPI_ReceiveData8(SPI2);
	}

	
}

void spi2_fini(void)
{
	SPI_I2S_DeInit(SPI2);
	SPI_Cmd(SPI2, DISABLE);

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, DISABLE);
}


static u8 spi2_exchange_data(u8 data)
{
	u8 retry = 0;
	
	/* Wait for transfer finished. */
	while (RESET == SPI_I2S_GetFlagStatus(SPI2, SPI_SR_TXE)) {
		retry++;
		if (retry > 100) {
			return 0;		
		}
	}

	/* Write data (8 or 16 bits, depending on DFF) into DR. */
	SPI_SendData8(SPI2, data);
	retry = 0;

	/* Wait for transfer finished. */
	while (RESET == SPI_I2S_GetFlagStatus(SPI2, SPI_SR_RXNE)) {
		retry++;
		if (retry > 100) {
			return 0;		
		}
	}

	/* Read the data (8 or 16 bits, depending on DFF bit) from DR. */
	return SPI_ReceiveData8(SPI2);
}



void spi2_write_reg(u8 reg, u8 value)
{
	u16 r = (u16)reg & 0xFF;
	u16 v = (u16)value & 0xFF;
		
	spi2_exchange_data(r);
	spi2_exchange_data(v);
}


u8 spi2_read_reg(u8 reg)
{
	u8 value = 0;

	/* Wait for transfer finished. */
	while (RESET == SPI_I2S_GetFlagStatus(SPI2, SPI_SR_TXE))
		;

	/* Write data (8 or 16 bits, depending on DFF) into DR. */
	SPI_SendData8(SPI2, reg);

	/* Wait for transfer finished. */
	while (RESET == SPI_I2S_GetFlagStatus(SPI2, SPI_SR_RXNE))
		;

	/* Read the data (8 or 16 bits, depending on DFF bit) from DR. */
	SPI_ReceiveData8(SPI2);

	/* Wait for transfer finished. */
	while (RESET == SPI_I2S_GetFlagStatus(SPI2, SPI_SR_TXE))
		;

	/* Write data (8 or 16 bits, depending on DFF) into DR. */
	SPI_SendData8(SPI2, SPI_CMD_NOP);

	/* Wait for transfer finished. */
	while (RESET == SPI_I2S_GetFlagStatus(SPI2, SPI_SR_RXNE))
		;

	/* Read the data (8 or 16 bits, depending on DFF bit) from DR. */
	value = SPI_ReceiveData8(SPI2);
	
	return value;
}


void spi2_write_buf(u8 reg, u8 *pBuf, u32 len)
{
	u32 idx;
	u16 r = (u16)reg & 0xFF;
	u16 v = 0;

	spi2_exchange_data(r);

	for (idx = 0; idx < len; idx++) {
		v = (u16)(pBuf[idx]) & 0xFF;
		spi2_exchange_data(v); 
	}
}


void spi2_read_buf(u8 reg, u8 *pBuf, u32 len)
{
	u32 idx = 0;
	u16 r = (u16)reg & 0xFF;
	
	spi2_exchange_data(r);
	
	for (idx = 0; idx < len; idx++) {
		pBuf[idx] = (u8)(spi2_exchange_data(SPI_CMD_NOP) & 0xFF);
	}
}

#endif





/* END OF FILE */


