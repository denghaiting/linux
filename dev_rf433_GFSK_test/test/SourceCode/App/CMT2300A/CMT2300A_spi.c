/****************************************Copyright (c)****************************************
                                                                                                                                             
                                           
                             Galaxywind Network System Co., Ltd.                                                    
                                                                                                                                
                                  http://www.galaxywind.com                                                             
                                                                                                                                             
                                   
 Author:        Y
 Date  :        2019-9-2
 *********************************************************************************************/
#include "stm32f0xx.h"
#include "bsp_gpio.h"


#if 0 // 开发板
#define FCSB_GPIO_GROUP					GPIOB
#define FCSB_GPIO_PIN					GPIO_Pin_14
#define FCSB_GPIO_MODE					GPIO_Mode_OUT
#define FCSB_GPIO_SPEED					GPIO_Speed_50MHz
#define FCSB_GPIO_OTYPE					GPIO_OType_PP
#define FCSB_GPIO_PUPD					GPIO_PuPd_UP

#define CSB_GPIO_GROUP					GPIOB
#define CSB_GPIO_PIN					GPIO_Pin_13
#define CSB_GPIO_MODE					GPIO_Mode_OUT
#define CSB_GPIO_SPEED					GPIO_Speed_50MHz
#define CSB_GPIO_OTYPE					GPIO_OType_PP
#define CSB_GPIO_PUPD					GPIO_PuPd_UP

#define SDIO_GPIO_GROUP					GPIOB
#define SDIO_GPIO_PIN					GPIO_Pin_12
#define SDIO_GPIO_MODE					GPIO_Mode_OUT
#define SDIO_GPIO_SPEED					GPIO_Speed_2MHz
#define SDIO_GPIO_OTYPE					GPIO_OType_PP
#define SDIO_GPIO_PUPD					GPIO_PuPd_UP

#define SCLK_GPIO_GROUP					GPIOB
#define SCLK_GPIO_PIN					GPIO_Pin_11
#define SCLK_GPIO_MODE					GPIO_Mode_OUT
#define SCLK_GPIO_SPEED					GPIO_Speed_50MHz
#define SCLK_GPIO_OTYPE					GPIO_OType_PP
#define SCLK_GPIO_PUPD					GPIO_PuPd_UP

#else
#define FCSB_GPIO_GROUP					GPIOA
#define FCSB_GPIO_PIN					GPIO_Pin_0
#define FCSB_GPIO_MODE					GPIO_Mode_OUT
#define FCSB_GPIO_SPEED					GPIO_Speed_50MHz
#define FCSB_GPIO_OTYPE					GPIO_OType_PP
#define FCSB_GPIO_PUPD					GPIO_PuPd_UP

#define CSB_GPIO_GROUP					GPIOA
#define CSB_GPIO_PIN					GPIO_Pin_4
#define CSB_GPIO_MODE					GPIO_Mode_OUT
#define CSB_GPIO_SPEED					GPIO_Speed_50MHz
#define CSB_GPIO_OTYPE					GPIO_OType_PP
#define CSB_GPIO_PUPD					GPIO_PuPd_UP

#define SCLK_GPIO_GROUP					GPIOA
#define SCLK_GPIO_PIN					GPIO_Pin_5
#define SCLK_GPIO_MODE					GPIO_Mode_OUT
#define SCLK_GPIO_SPEED					GPIO_Speed_50MHz
#define SCLK_GPIO_OTYPE					GPIO_OType_PP
#define SCLK_GPIO_PUPD					GPIO_PuPd_UP

// MOSI - 软件
#define SDIO_GPIO_GROUP					GPIOA
#define SDIO_GPIO_PIN					GPIO_Pin_7
#define SDIO_GPIO_MODE					GPIO_Mode_OUT
#define SDIO_GPIO_SPEED					GPIO_Speed_2MHz
#define SDIO_GPIO_OTYPE					GPIO_OType_PP
#define SDIO_GPIO_PUPD					GPIO_PuPd_UP
#define SDIO_AF							GPIO_AF_0

// MISO - 软件
#define MISO_GPIO_GROUP					GPIOA
#define MISO_GPIO_PIN					GPIO_Pin_6
#define MISO_GPIO_PIN_SOURCE			GPIO_PinSource6
#define MISO_GPIO_MODE					GPIO_Mode_IN
#define MISO_GPIO_SPEED					GPIO_Speed_10MHz
#define MISO_GPIO_OTYPE					GPIO_OType_PP
#define MISO_GPIO_PUPD					GPIO_PuPd_NOPULL
#define MISO_AF							GPIO_AF_0
#endif



/* ================ SPI 细节设置 ================ */

// TODO: 配置 SPI脉宽(即0.5周期) 单位微秒
#define SCLK_PULSE 	1


// TODO: 配置 CSB 有效电平
#if 1
#define CSB_VALID_LEV_LOW
#else
#define CSB_VALID_LEV_HIGH
#endif

// TODO: 配置 FCSB 有效电平
#if 1
#define FCSB_VALID_LEV_LOW
#else
#define FCSB_VALID_LEV_HIGH
#endif

/* ================ ============ ================ */


// TODO: CSB有效无效宏定义
#if defined (CSB_VALID_LEV_LOW)
#define CSB_VALID() 	GPIO_ResetBits(CSB_GPIO_GROUP, CSB_GPIO_PIN)
#define CSB_INVALID() 	GPIO_SetBits(CSB_GPIO_GROUP, CSB_GPIO_PIN)
#else
#define CSB_VALID() 	GPIO_SetBits(CSB_GPIO_GROUP, CSB_GPIO_PIN)
#define CSB_INVALID() 	GPIO_ResetBits(CSB_GPIO_GROUP, CSB_GPIO_PIN)
#endif

// TODO: FCSB有效无效宏定义
#if defined (FCSB_VALID_LEV_LOW)
#define FCSB_VALID() 	GPIO_ResetBits(FCSB_GPIO_GROUP, FCSB_GPIO_PIN)
#define FCSB_INVALID() 	GPIO_SetBits(FCSB_GPIO_GROUP, FCSB_GPIO_PIN)
#else
#define FCSB_VALID() 	GPIO_SetBits(FCSB_GPIO_GROUP, FCSB_GPIO_PIN)
#define FCSB_INVALID() 	GPIO_ResetBits(FCSB_GPIO_GROUP, FCSB_GPIO_PIN)
#endif


// TODO: 切换至寄存器模式
#define MODE_CTRL_REG() \
	do {\
		FCSB_INVALID();\
	 	CSB_VALID();\
	} while (0);

// TODO: 切换至FIFO模式
#define MODE_CTRL_FIFO() \
	do {\
		CSB_INVALID();\
	 	FCSB_VALID();\
	} while (0);

// TODO: SCLK 操作
#define SCLK_HIGH() 		GPIO_SetBits(SCLK_GPIO_GROUP, SCLK_GPIO_PIN)
#define SCLK_LOW() 			GPIO_ResetBits(SCLK_GPIO_GROUP, SCLK_GPIO_PIN)

// TODO: SDIO INPUT 操作
#define SET_SDIO_IN() 		set_SDIO_input()
#define GET_SDIO() 			GPIO_ReadInputDataBit(SDIO_GPIO_GROUP, SDIO_GPIO_PIN)

// TODO: SDIO OUTPUT 操作
#define SET_SDIO_OUT() 		set_SDIO_output()
#define SDIO_HIGH() 		GPIO_SetBits(SDIO_GPIO_GROUP, SDIO_GPIO_PIN)
#define SDIO_LOW() 			GPIO_ResetBits(SDIO_GPIO_GROUP, SDIO_GPIO_PIN)


// TODO: 周期
#if defined (SCLK_PULSE)
#define SCLK_CYCLE 	(SCLK_PULSE * 2)
#else
#error "NOT DEFINED SCLK PULSE!"
#endif



static void set_SDIO_input(void)
{
	bsp_gpio_init(SDIO_GPIO_GROUP,
				  SDIO_GPIO_PIN, 
				  GPIO_Mode_IN, 
				  GPIO_Speed_2MHz,
				  GPIO_OType_PP,
				  GPIO_PuPd_NOPULL);
}

static void set_SDIO_output(void)
{
	bsp_gpio_init(SDIO_GPIO_GROUP,
				  SDIO_GPIO_PIN, 
				  GPIO_Mode_OUT, 
				  GPIO_Speed_2MHz,
				  GPIO_OType_PP,
				  GPIO_PuPd_NOPULL);
}



#define ONE_US_CNT 	1

void udelay(u32 i)
{	
	u32 x;
	u32 n;
	
	for (n = 0; n < i; n++) {
		for (x = 0; x < ONE_US_CNT; x++) {

		}
	}

}

#define US_DELAY(n) 				udelay(n)


static u8 __spi_recv_byte(void)
{
	u8 byte = 0;
	u8 bitcnt;
	
	for (bitcnt = 0; bitcnt < 8; bitcnt++) {
 		
		US_DELAY(SCLK_PULSE);
		
 		SCLK_HIGH();
		
 		if (GET_SDIO()) {
 			byte |= 1 << (7 - bitcnt);
 		}
		
		US_DELAY(SCLK_PULSE);

		SCLK_LOW();
 	}
	
	return byte;
}


static void __spi_send_byte(u8 byte)
{
	u8 bitcnt;
	
	for (bitcnt = 0; bitcnt < 8; bitcnt++) {

		if ((byte >> (7 - bitcnt)) & 0x1) {
			SDIO_HIGH();
		}
		else {
			SDIO_LOW();
		}

		US_DELAY(SCLK_PULSE);

		SCLK_HIGH();
		US_DELAY(SCLK_PULSE);

		SCLK_LOW();
	}
}


void CMT2300A_spi_read_reg(u8 reg_addr, u8 *read_buf, u8 read_len)
{
	u8 i;

	/* 发送准备 */
	SET_SDIO_OUT(); // SDIO 平时为高
	SDIO_HIGH();

	SCLK_LOW(); // 保证SCLK平时为低

	MODE_CTRL_REG();	// FCSB失能 CSB使能

	US_DELAY(SCLK_CYCLE); // > 0.5 CYCLE

	__spi_send_byte(reg_addr | 0x80); // 发送寄存器地址

	/* 切换至接收 */
	SDIO_HIGH(); // SDIO 平时为高
	SET_SDIO_IN();

	for (i = 0; i < read_len; i++) {
		read_buf[i] = __spi_recv_byte();
	}

	US_DELAY(SCLK_CYCLE); // > 0.5 CYCLE

	CSB_INVALID(); // CSB失能
	
	SET_SDIO_OUT(); // SDIO 平时为高
	SDIO_HIGH();
}


void CMT2300A_spi_write_reg(u8 reg_addr, u8 *write_buf, u8 write_len)
{
	u8 i;

	/* 发送准备 */
	SET_SDIO_OUT(); // SDIO 平时为高
	SDIO_HIGH();

	SCLK_LOW(); // 保证SCLK平时为低

	MODE_CTRL_REG();	// FCSB失能 CSB使能

	US_DELAY(SCLK_CYCLE); // > 0.5 CYCLE

	__spi_send_byte(reg_addr & 0x7F); // 发送寄存器地址

	for (i = 0; i < write_len; i++) {
		__spi_send_byte(write_buf[i]);
	}

	US_DELAY(SCLK_CYCLE); // > 0.5 CYCLE

	CSB_INVALID(); // CSB失能
	
	SDIO_HIGH(); // SDIO 平时为高
}


void CMT2300A_spi_read_fifo(u8 *read_buf, u8 read_len)
{
	u8 i;

	/* 准备工作 */
	SET_SDIO_IN();

	SCLK_LOW(); // 保证SCLK平时为低

	for (i = 0; i < read_len; i++) {
		US_DELAY(2 * SCLK_CYCLE); // > 1 CYCLE
		FCSB_VALID();
		
		read_buf[i] = __spi_recv_byte();
		
		US_DELAY(3); // > 2us

		FCSB_INVALID();
		
		US_DELAY(5); // > 4us
	}
	
	SET_SDIO_OUT(); // SDIO 平时为高
	SDIO_HIGH();
}


void CMT2300A_spi_write_fifo(u8 *write_buf, u8 write_len)
{
	u8 i;

	SCLK_LOW(); // 保证SCLK平时为低

	for (i = 0; i < write_len; i++) {
		US_DELAY(2 * SCLK_CYCLE); // > 1 CYCLE
		FCSB_VALID();
		
		__spi_send_byte(write_buf[i]);
		
		US_DELAY(3); // > 2us

		FCSB_INVALID();
		
		US_DELAY(5); // > 4us
	}
	
	SDIO_HIGH(); // SDIO 平时为高
}


void CMT2300A_spi_init(void)
{

	/* CSB */
	bsp_gpio_init(CSB_GPIO_GROUP,
				  CSB_GPIO_PIN, 
				  CSB_GPIO_MODE, 
				  CSB_GPIO_SPEED,
				  CSB_GPIO_OTYPE,
				  CSB_GPIO_PUPD);
	CSB_INVALID();
	
	/* FCSB */
	bsp_gpio_init(FCSB_GPIO_GROUP,
				  FCSB_GPIO_PIN, 
				  FCSB_GPIO_MODE, 
				  FCSB_GPIO_SPEED,
				  FCSB_GPIO_OTYPE,
				  FCSB_GPIO_PUPD);
	FCSB_INVALID();
	
	/* SCLK */
	bsp_gpio_init(SCLK_GPIO_GROUP,
				  SCLK_GPIO_PIN, 
				  SCLK_GPIO_MODE, 
				  SCLK_GPIO_SPEED,
				  SCLK_GPIO_OTYPE,
				  SCLK_GPIO_PUPD);
	SCLK_LOW(); // SCLK 平时为低
	
	/* SDIO */
	bsp_gpio_init(SDIO_GPIO_GROUP,
				  SDIO_GPIO_PIN, 
				  SDIO_GPIO_MODE, 
				  SDIO_GPIO_SPEED,
				  SDIO_GPIO_OTYPE,
				  SDIO_GPIO_PUPD);
	SDIO_HIGH(); // SDIO 平时为高

	/* MISO */
	bsp_gpio_init(MISO_GPIO_GROUP,
				  MISO_GPIO_PIN, 
				  MISO_GPIO_MODE, 
				  MISO_GPIO_SPEED,
				  MISO_GPIO_OTYPE,
				  MISO_GPIO_PUPD);


	udelay(1000);
}


