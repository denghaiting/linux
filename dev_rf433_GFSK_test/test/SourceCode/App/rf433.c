/****************************************Copyright (c)****************************************
                                                                                                                                             
                                           
                             Galaxywind Network System Co., Ltd.                                                    
                                                                                                                                
                                  http://www.galaxywind.com                                                             
                                                                                                                                             
                                   
 Author:        Y
 Date  :        2020-7-16
 *********************************************************************************************/
#include "event.h"
#include "led.h"
#include "print_color.h"
#include "rf433.h"
#include "bsp_gpio.h"
#include "bsp_tim15.h"
#include "sys_tools.h"
#include "sys_nvic_config.h"
#include "pkt_fifo.h"
#include "debug_uart.h"
#include "freq_adjust.h"


#define EN_RF433_PRINTF

#if defined (EN_RF433_PRINTF)
#define RF433_NAME 	"rf433.c"
#define RF433_DEBUG(fmt, args...)		printf(PCLR_GRN_B"<%s:%03d> " fmt PCLR_DFT, RF433_NAME, __LINE__, ##args)
#define RF433_INFO(fmt, args...)		printf(PCLR_YEL_B"<%s:%03d> " fmt PCLR_DFT, RF433_NAME, __LINE__, ##args)
#define RF433_REMIND(fmt, args...)		printf(PCLR_LBL_B"<%s:%03d> " fmt PCLR_DFT, RF433_NAME, __LINE__, ##args)
#define RF433_WARN(fmt, args...)		printf(PCLR_PUR_B"<%s:%03d> " fmt PCLR_DFT, RF433_NAME, __LINE__, ##args)
#define RF433_ERROR(fmt, args...)		printf(PCLR_RED_B"<%s:%03d> " fmt PCLR_DFT, RF433_NAME, __LINE__, ##args)
#else
#define RF433_DEBUG(fmt, args...)
#define RF433_INFO(fmt, args...)
#define RF433_REMIND(fmt, args...)
#define RF433_WARN(fmt, args...)
#define RF433_ERROR(fmt, args...)
#endif



#if 0
#define U2_DBG(str) 		print_string(str)
#else
#define U2_DBG(str) 		 
#endif


#if 0 // 开发板用
#define CEI_GPIO_GROUP					GPIOB
#define CEI_GPIO_PIN					GPIO_Pin_15
#define CEI_GPIO_MODE					GPIO_Mode_OUT
#define CEI_GPIO_SPEED					GPIO_Speed_2MHz
#define CEI_GPIO_OTYPE					GPIO_OType_PP
#define CEI_GPIO_PUPD					GPIO_PuPd_NOPULL
#endif

#define CR_EN_GPIO_GROUP				GPIOB
#define CR_EN_GPIO_PIN					GPIO_Pin_3
#define CR_EN_GPIO_MODE					GPIO_Mode_IN
#define CR_EN_GPIO_SPEED				GPIO_Speed_2MHz
#define CR_EN_GPIO_OTYPE				GPIO_OType_PP
#define CR_EN_GPIO_PUPD					GPIO_PuPd_NOPULL

#define PA_EN_GPIO_GROUP				GPIOC
#define PA_EN_GPIO_PIN					GPIO_Pin_1
#define PA_EN_GPIO_MODE					GPIO_Mode_OUT
#define PA_EN_GPIO_SPEED				GPIO_Speed_2MHz
#define PA_EN_GPIO_OTYPE				GPIO_OType_PP
#define PA_EN_GPIO_PUPD					GPIO_PuPd_NOPULL

#define POWER_GPIO_GROUP				GPIOB
#define POWER_GPIO_PIN					GPIO_Pin_11
#define POWER_GPIO_MODE					GPIO_Mode_OUT
#define POWER_GPIO_SPEED				GPIO_Speed_2MHz
#define POWER_GPIO_OTYPE				GPIO_OType_PP
#define POWER_GPIO_PUPD					GPIO_PuPd_NOPULL

#define V1_EN_GPIO_GROUP				GPIOB
#define V1_EN_GPIO_PIN					GPIO_Pin_1
#define V1_EN_GPIO_MODE					GPIO_Mode_OUT
#define V1_EN_GPIO_SPEED				GPIO_Speed_2MHz
#define V1_EN_GPIO_OTYPE				GPIO_OType_PP
#define V1_EN_GPIO_PUPD					GPIO_PuPd_NOPULL

#define V2_EN_GPIO_GROUP				GPIOC
#define V2_EN_GPIO_PIN					GPIO_Pin_2
#define V2_EN_GPIO_MODE					GPIO_Mode_OUT
#define V2_EN_GPIO_SPEED				GPIO_Speed_2MHz
#define V2_EN_GPIO_OTYPE				GPIO_OType_PP
#define V2_EN_GPIO_PUPD					GPIO_PuPd_NOPULL

#define IRQ1_GPIO_GROUP					GPIOC
#define	IRQ1_GPIO_PIN 					GPIO_Pin_3
#define	IRQ1_GPIO_MODE					GPIO_Mode_IN
#define	IRQ1_GPIO_SPEED					GPIO_Speed_10MHz
#define	IRQ1_GPIO_OTYPE					GPIO_OType_PP
#define	IRQ1_GPIO_PUPD					GPIO_PuPd_NOPULL

#define IRQ1_GPIO_PORTSOURCE			EXTI_PortSourceGPIOC
#define IRQ1_GPIO_PINSOURCE				EXTI_PinSource3
#define IRQ1_GPIO_EXTI_LINE				EXTI_Line3						// 中断线
#define IRQ1_GPIO_EXTI_MODE				EXTI_Mode_Interrupt				// 中断模式
#define IRQ1_GPIO_EXTI_TRIGGER			EXTI_Trigger_Rising				// 上升沿
#define IRQ1_GPIO_NVIC_IRQCHANNEL 		EXTI2_3_IRQn					// 中断向量

#define IRQ2_GPIO_GROUP					GPIOA
#define	IRQ2_GPIO_PIN 					GPIO_Pin_1
#define	IRQ2_GPIO_MODE					GPIO_Mode_IN
#define	IRQ2_GPIO_SPEED					GPIO_Speed_10MHz
#define	IRQ2_GPIO_OTYPE					GPIO_OType_PP
#define	IRQ2_GPIO_PUPD					GPIO_PuPd_NOPULL

#define IRQ2_GPIO_PORTSOURCE			EXTI_PortSourceGPIOA
#define IRQ2_GPIO_PINSOURCE				EXTI_PinSource1
#define IRQ2_GPIO_EXTI_LINE				EXTI_Line1						// 中断线
#define IRQ2_GPIO_EXTI_MODE				EXTI_Mode_Interrupt				// 中断模式
#define IRQ2_GPIO_EXTI_TRIGGER			EXTI_Trigger_Rising				// 上升沿
#define IRQ2_GPIO_NVIC_IRQCHANNEL 		EXTI0_1_IRQn					// 中断向量

#define IRQ3_GPIO_GROUP					GPIOB
#define	IRQ3_GPIO_PIN 					GPIO_Pin_0
#define	IRQ3_GPIO_MODE					GPIO_Mode_IN
#define	IRQ3_GPIO_SPEED					GPIO_Speed_10MHz
#define	IRQ3_GPIO_OTYPE					GPIO_OType_PP
#define	IRQ3_GPIO_PUPD					GPIO_PuPd_NOPULL

#define IRQ3_GPIO_PORTSOURCE			EXTI_PortSourceGPIOB
#define IRQ3_GPIO_PINSOURCE				EXTI_PinSource0
#define IRQ3_GPIO_EXTI_LINE				EXTI_Line0						// 中断线
#define IRQ3_GPIO_EXTI_MODE				EXTI_Mode_Interrupt				// 中断模式
#define IRQ3_GPIO_EXTI_TRIGGER			EXTI_Trigger_Rising				// 上升沿
#define IRQ3_GPIO_NVIC_IRQCHANNEL 		EXTI0_1_IRQn					// 中断向量




enum {
	eCONFIG_REG_BANK_100K_20dB,
	eCONFIG_REG_BANK_100K_13dB,
	eCONFIG_REG_BANK_100K_10dB,
	eCONFIG_REG_BANK_64K_20dB,
	eCONFIG_REG_BANK_32K_20dB,
	eCONFIG_REG_BANK_16K_20dB,
	eCONFIG_REG_BANK_2400_20dB,
	eCONFIG_REG_BANK_GFSK_2_4K_5K_13dB,
	eCONFIG_REG_BANK_GFSK_2400_20k_20db,
	eCONFIG_REG_BANK_GFSK_16k_32k_20db,
	eCONFIG_REG_BANK_GFSK_32k_64k_20db,
	eCONFIG_REG_BANK_GFSK_64k_128k_20db,
	eCONFIG_REG_BANK_GFSK_16k_32k_20db_gbt1,
	eCONFIG_REG_BANK_16k_32k_20db,
	eCONFIG_REG_BANK_GFSK487_5_64_128_20db,
	eCONFIG_REG_BANK_GFSK487_5_16_32_20db,
	eCONFIG_REG_BANK_rssi_16k,
	eCONFIG_REG_BANK_rssi_64k,
};


//#define RF_CONFIG_DEFAULT 	eCONFIG_REG_BANK_100K_20dB
//#define RF_CONFIG_DEFAULT 	eCONFIG_REG_BANK_2400_20dB
#define RF_CONFIG_DEFAULT 	eCONFIG_REG_BANK_rssi_64k
//#define RF_CONFIG_DEFAULT 	eCONFIG_REG_BANK_32K_20dB
//#define RF_CONFIG_DEFAULT 	eCONFIG_REG_BANK_64K_20dB


int rf433_cfg_reg_bank = RF_CONFIG_DEFAULT;


#if 0
#define DISABLE_RF433M_IRQ								    \
	do { 													\
		if (!in_irq()) {                                    \
			disable_irq(SURFBOARDINT_GPIO);					\
		}	    										    \
	}while(0)

#define ENABLE_RF433M_IRQ							        \
	do { 													\
		if (!in_irq()) {                                    \
			enable_irq(SURFBOARDINT_GPIO);					\
		}                                                   \
	}while(0)
#else
#define DISABLE_RF433M_IRQ 
#define ENABLE_RF433M_IRQ 
#endif


#define MAX_RF_PKT_COUNT 	16	

pkt_fifo_t rf_pkt_fifo;
pkt_t rf_pkt[MAX_RF_PKT_COUNT];


typedef struct {
	u8 len;
	u8 buf[RF_PAYLOAD_LENGTH];
	u32 count;
	u32 space_ms;
	vu32 tx_timestamp;
	vu32 space_timestamp;
	event_t tmr_node;
	volatile BOOL tx_done_flag;
	volatile BOOL tx_timeout;
	volatile BOOL space_timeout;
	u8 pad[1];
} tx_mgr_t;


tx_mgr_t tx_mgr;

//u16 rf_dbg_len = 16;
u16 rf_dbg_len = 5;


typedef struct {
	u8 len; 
	u8 buf[RF_PAYLOAD_LENGTH];
	u8 rssi;
	u8 pad[3];
} recv_pkt_t;


#define TMP_PRINT_CNT 	4

recv_pkt_t recv_pkt[TMP_PRINT_CNT];

enum {
	eMODE_SLEEP = 0,
	eMODE_STBY,
	eMODE_RX,
	eMODE_TX,
	eMODE_RELAY,
	eMODE_GW_TEST,
	eMODE_ERR
};

int rf_mode = eMODE_SLEEP;

static vs32 rx_count = 0;
static vs32 rssi_total = 0;



void power_init(void)
{
	bsp_gpio_init(POWER_GPIO_GROUP,
				  POWER_GPIO_PIN, 
				  POWER_GPIO_MODE, 
				  POWER_GPIO_SPEED,
				  POWER_GPIO_OTYPE,
				  POWER_GPIO_PUPD);

	// TODO: 拉低通电
	GPIO_ResetBits(POWER_GPIO_GROUP, POWER_GPIO_PIN);
}


void PA_to_Rx(void) // TODO: 这个是常态
{
	GPIO_ResetBits(PA_EN_GPIO_GROUP, PA_EN_GPIO_PIN);
}

void PA_to_Tx(void) // 
{
	GPIO_SetBits(PA_EN_GPIO_GROUP, PA_EN_GPIO_PIN);
}


// TODO: 直接写同一组寄存器
#if 0
void ANT_switch_to_Tx(void)
{
	u32 tmp = GPIOB->ODR;

	tmp |= TX_EN_GPIO_PIN;
	tmp &= ~(RX_EN_GPIO_PIN);

	GPIOB->ODR = tmp;
}

void ANT_switch_to_Rx(void) // TODO: 这个是常态
{
	u32 tmp = GPIOB->ODR;

	tmp |= RX_EN_GPIO_PIN;
	tmp &= ~(TX_EN_GPIO_PIN);

	GPIOB->ODR = tmp;
}

#else
void ANT_switch_to_Tx(void)
{
	GPIO_ResetBits(V1_EN_GPIO_GROUP, V1_EN_GPIO_PIN);
	GPIO_SetBits(V2_EN_GPIO_GROUP, V2_EN_GPIO_PIN);

	PA_to_Tx();

	udelay(5000); // 5ms
}

void ANT_switch_to_Rx(void) // TODO: 这个是常态
{
	GPIO_SetBits(V1_EN_GPIO_GROUP, V1_EN_GPIO_PIN);
	GPIO_ResetBits(V2_EN_GPIO_GROUP, V2_EN_GPIO_PIN);

	PA_to_Rx();
	
	udelay(5000); // 5ms
}
#endif

void TxRx_en_init(void)
{
	bsp_gpio_init(V1_EN_GPIO_GROUP,
				  V1_EN_GPIO_PIN, 
				  V1_EN_GPIO_MODE, 
				  V1_EN_GPIO_SPEED,
				  V1_EN_GPIO_OTYPE,
				  V1_EN_GPIO_PUPD);

	
	bsp_gpio_init(V2_EN_GPIO_GROUP,
				  V2_EN_GPIO_PIN, 
				  V2_EN_GPIO_MODE, 
				  V2_EN_GPIO_SPEED,
				  V2_EN_GPIO_OTYPE,
				  V2_EN_GPIO_PUPD);

	/* 平时就切换为RX */
	ANT_switch_to_Rx();
}


uint8 chose_rate_flag = 0;

void chose_rate(void)
{
	char *ddddf = NULL;

	bsp_gpio_init(CR_EN_GPIO_GROUP,
				  CR_EN_GPIO_PIN, 
				  CR_EN_GPIO_MODE, 
				  CR_EN_GPIO_SPEED,
				  CR_EN_GPIO_OTYPE,
				  CR_EN_GPIO_PUPD);

	udelay(20000); // 20ms(5);

	if(GPIO_ReadInputDataBit(CR_EN_GPIO_GROUP, CR_EN_GPIO_PIN)){
		chose_rate_flag = 1;
		ddddf = "eCONFIG_REG_BANK_rssi_64k";
	}else{
		chose_rate_flag = 0;
		ddddf = "eCONFIG_REG_BANK_rssi_16k";
	}

	printf(PCLR_GRN_B"gpio level = %d(%s)\r\n"PCLR_DFT, chose_rate_flag, ddddf);
}


static void dbg_len_usage(void)
{
	printf("\r\n\033[36m\033[1mUsage:\r\n");
	printf("dbg_len xx(xx can be 1 to 63)");
	printf("\r\n\033[0m");	
}

void dbg_len(void *ptr, u16 len)
{
	int num = 0;
	u8 num_buf[4] = {0};
	u8 dbg;
	u8 i;

	if ((NULL == ptr) || ( ! len )) {
		goto out;
	}
	
	if (len > 3) {
		goto out;
	}

	//printf("%s\r\n", (char *)ptr);
	//printf("len = %u\r\n", len);

	for (i = 0; i < len; ) {
		num_buf[i] = *((u8 *)ptr + i);
		i++;
	}

	num_buf[i] = '\0';

	if (Str2Int(num_buf, &num)) {
		dbg = (u8)num;
		if ((dbg > 63) || ( ! dbg )) {
			goto out;
		}

		rf_dbg_len = dbg;
		
		return;
	}

out:
	dbg_len_usage();
}

/*
(1) Deviation < 50K: 
	2, 5, 8, 11, 14, 17, 20, 23, 26, 29, 32
	<共11个CH, 间隔3个信道>
	
(2) 50 < Deviation < 100K:
	2, 7, 12, 17, 22, 27, 32
	<共7个CH, 间隔5个信道>

(3) 100 < Deviation < 150K:
	3, 10, 17, 24, 31
	<共5个CH, 间隔7个信道>
	
(4) 150 < Deviation <= 200K:
    6, 17, 28
	<共3个CH, 间隔11个信道>

 */
static void set_ch_usage(void)
{
	printf("\r\n\033[36m\033[1mUsage:\r\n");
	printf("ch xx(xx can be 1 to 33)");
	printf("\r\n\033[0m");	

	printf("You should follow this:"NEW_LINE);
	printf("Deviation[0,50),    use CH: 2,  5,  8, 11, 14, 17, 20, 23, 26, 29, 32"NEW_LINE);
	printf("Deviation[50,100),  use CH:         2,  7, 12, 17, 22, 27, 32"NEW_LINE);
	printf("Deviation[100,150), use CH:             3, 10, 17, 24, 31"NEW_LINE);
	printf("Deviation[150,200], use CH:                 6, 17, 28"NEW_LINE);
}

void set_ch(void *ptr, u16 len)
{
	int num = 0;
	u8 num_buf[4] = {0};
	u8 ch;
	u8 i;

	if ((rf_mode != eMODE_SLEEP) && (rf_mode != eMODE_STBY)) {
		printf(PCLR_RED_B"rf433 busy NOW!!!"PCLR_DFT NEW_LINE);
		return;
	}


	if ((NULL == ptr) || ( ! len )) {
		goto out;
	}
	
	if (len > 3) {
		goto out;
	}

	//printf("%s\r\n", (char *)ptr);
	//printf("len = %u\r\n", len);

	for (i = 0; i < len; ) {
		num_buf[i] = *((u8 *)ptr + i);
		i++;
	}

	num_buf[i] = '\0';

	if (Str2Int(num_buf, &num)) {
		ch = (u8)num;
		if ((ch > 33) || ( ! ch )) {
			goto out;
		}

		set_custom_ch(ch);
		
		return;
	}

out:
	set_ch_usage();
}


static void set_tx_count_usage(void)
{
	printf(NEW_LINE"Usage:"NEW_LINE);
	printf("count xxxxx(xxxxx can be 1 to 9999)"NEW_LINE);
}


static void __set_tx_count(u32 count)
{
	if (count > 9999) {
		printf("MAX count == 9999"NEW_LINE);
	} 
	else if (count < 1) {
		printf("MIN count == 1"NEW_LINE);
	}
	else {
		tx_mgr.count = count;
	}

	printf("tx_count[%u]"NEW_LINE, tx_mgr.count);
}


void set_tx_count(void *ptr, u16 len)
{
	int num = 0;
	u8 num_buf[6] = {0};
	u8 i;

	if ((NULL == ptr) || ( ! len ) || (len > 4)) {
		goto out;
	}

	for (i = 0; i < len; ) {
		num_buf[i] = *((u8 *)ptr + i);
		i++;
	}

	num_buf[i] = '\0';

	if (Str2Int(num_buf, &num)) {
		__set_tx_count((u32)num);
		return;
	}

out:
	set_tx_count_usage();
}


static void __set_space_time(u32 nms)
{
	if (nms > 999) {
		printf("MAX space_time == 999ms"NEW_LINE);
	} 
	else if (nms < 1) {
		printf("MIN space_time == 1ms"NEW_LINE);
	}
	else {
		tx_mgr.space_ms = nms;
	}

	printf("space_time[%ums]"NEW_LINE, tx_mgr.space_ms);
}

static void set_space_time_usage(void)
{
	printf("Usage:"NEW_LINE);
	printf("space xxx(xxx can be 1 to 999)"NEW_LINE);
}

void set_tx_space(void *ptr, u16 len)
{
	int num = 0;
	u8 num_buf[4] = {0};
	u8 i;

	if ((NULL == ptr) || ( ! len ) || (len > 3)) {
		goto out;
	}

	//printf("%s\r\n", (char *)ptr);
	//printf("len = %u\r\n", len);

	for (i = 0; i < len; ) {
		num_buf[i] = *((u8 *)ptr + i);
		i++;
	}

	num_buf[i] = '\0';

	if (Str2Int(num_buf, &num)) {
		__set_space_time((u32)num);
		return;
	}

out:
	set_space_time_usage();
}



static void set_tx_len_usage(void)
{
	printf("Usage:"NEW_LINE);
	printf("len xx(xx can be 1 to 63)"NEW_LINE);
}

void set_tx_len(void *ptr, u16 len)
{
	int num = 0;
	u8 num_buf[4] = {0};
	u8 i;

	if ((NULL == ptr) || ( ! len ) || (len > 2)) {
		goto out;
	}

	//printf("%s\r\n", (char *)ptr);
	//printf("len = %u\r\n", len);

	for (i = 0; i < len; ) {
		num_buf[i] = *((u8 *)ptr + i);
		i++;
	}

	num_buf[i] = '\0';


	if (Str2Int(num_buf, &num)) {
		if ((num < 1) || (num > 63)) {
			goto out;
		}
		
		tx_mgr.len = num;
		return;
	}

out:
	set_tx_len_usage();
}

enum {
	eRF_CFG_100K_20DB = 0,
	eRF_CFG_100K_13DB,
	eRF_CFG_100K_10DB,
	eRF_CFG_64K_20DB,
	eRF_CFG_32K_20DB,
	eRF_CFG_16K_20DB,
	eRF_CFG_2400_20DB,
	eRF_CFG_GFSK_2_4K_5K_13dB,
	eRF_CFG_GFSK_2400_20k_20db,
	eRF_CFG_GFSK_16k_32k_20db,
	eRF_CFG_GFSK_32k_64k_20db,
	eRF_CFG_GFSK_64k_128k_20db,
	eRF_CFG_GFSK_16k_32k_20db_gbt1,
	eRF_CFG_16k_32k_20db,
	eRF_CFG_GFSK487_5_64_128_20db,
	eRF_CFG_GFSK487_5_16_32_20db,
	eRF_CFG_rssi_16k,
	eRF_CFG_rssi_64k,
	eRATE_MAX
};


static u8 *set_rf_cfg_cmd[eRATE_MAX] = {
	"100K_20DB",
	"100K_13DB",
	"100K_10DB",
	"64K_20DB",
	"32K_20DB",
	"16K_20DB",
	"2400_20DB",
	"GFSK_2_4K_5K_13dB",
	"GFSK_2400_20k_20db",
	"GFSK_16k_32k_20db",
	"GFSK_32k_64k_20db",
	"GFSK_64k_128k_20db",
	"GFSK_16k_32k_20db_gbt1",
	"16k_32k_20db",
	"GFSK487_5_64_128_20db",
	"GFSK487_5_16_32_20db",
	"rssi_16k",
	"rssi_64k",
};

static void set_rf_cfg_usage(void)
{
	u8 i;
	
	printf("Usage:"NEW_LINE);

	for (i = 0; i < eRATE_MAX; i++) {
		printf("rf_cfg %s"NEW_LINE, set_rf_cfg_cmd[i]);
	}
}

static void __set_rf_cfg(int rate)
{
	if (rate == eRF_CFG_100K_20DB) {
		rf433_cfg_reg_bank = eCONFIG_REG_BANK_100K_20dB;
	}
	else if (rate == eRF_CFG_100K_13DB) {
		rf433_cfg_reg_bank = eCONFIG_REG_BANK_100K_13dB;
	}
	else if (rate == eRF_CFG_100K_10DB) {
		rf433_cfg_reg_bank = eCONFIG_REG_BANK_100K_10dB;
	}
	else if (rate == eRF_CFG_64K_20DB) {
		rf433_cfg_reg_bank = eCONFIG_REG_BANK_64K_20dB;
	}
	else if (rate == eRF_CFG_32K_20DB) {
		rf433_cfg_reg_bank = eCONFIG_REG_BANK_32K_20dB;
	}
	else if (rate == eRF_CFG_16K_20DB) {
		rf433_cfg_reg_bank = eCONFIG_REG_BANK_16K_20dB;
	}
	else if (rate == eRF_CFG_2400_20DB) {
		rf433_cfg_reg_bank = eCONFIG_REG_BANK_2400_20dB;
	}
	else if (rate == eRF_CFG_GFSK_2_4K_5K_13dB) {
		rf433_cfg_reg_bank = eCONFIG_REG_BANK_GFSK_2_4K_5K_13dB;
	}
	else if (rate == eRF_CFG_GFSK_2400_20k_20db) {
		rf433_cfg_reg_bank = eCONFIG_REG_BANK_GFSK_2400_20k_20db;
	}
	else if (rate == eRF_CFG_GFSK_16k_32k_20db) {
		rf433_cfg_reg_bank = eCONFIG_REG_BANK_GFSK_16k_32k_20db;
	}
	else if (rate == eRF_CFG_GFSK_32k_64k_20db) {
		rf433_cfg_reg_bank = eCONFIG_REG_BANK_GFSK_32k_64k_20db;
	}
	else if (rate == eRF_CFG_GFSK_64k_128k_20db) {
		rf433_cfg_reg_bank = eCONFIG_REG_BANK_GFSK_64k_128k_20db;
	}
	else if (rate == eRF_CFG_GFSK_16k_32k_20db_gbt1) {
		rf433_cfg_reg_bank = eCONFIG_REG_BANK_GFSK_16k_32k_20db_gbt1;
	}
	else if (rate == eRF_CFG_16k_32k_20db) {
		rf433_cfg_reg_bank = eCONFIG_REG_BANK_16k_32k_20db;
	}
	else if (rate == eRF_CFG_GFSK487_5_64_128_20db) {
		rf433_cfg_reg_bank = eCONFIG_REG_BANK_GFSK487_5_64_128_20db;
	}
	else if (rate == eRF_CFG_GFSK487_5_16_32_20db) {
		rf433_cfg_reg_bank = eCONFIG_REG_BANK_GFSK487_5_16_32_20db;
	}
	else if (rate == eRF_CFG_rssi_16k) {
		rf433_cfg_reg_bank = eCONFIG_REG_BANK_rssi_16k;
	}
	else if (rate == eRF_CFG_rssi_64k) {
		rf433_cfg_reg_bank = eCONFIG_REG_BANK_rssi_64k;
	}

	rf433m_set_reg_bank(rf433_cfg_reg_bank);
}


void do_rf_cfg(void *ptr, u16 len)
{
	int i;

	if ((rf_mode != eMODE_SLEEP) && (rf_mode != eMODE_STBY)) {
		printf(PCLR_RED_B"rf433 busy NOW!!!"PCLR_DFT NEW_LINE);
		return;
	}

	if ((NULL == ptr) || ( ! len )) {
		goto out;
	}

	//printf("%s\r\n", (char *)ptr);
	//printf("len = %u\r\n", len);

	for (i = 0; i < eRATE_MAX; i++) {
		if ( ! strncmp((const char *)set_rf_cfg_cmd[i], (const char *)ptr, len) ) {
			__set_rf_cfg(i);
			return;
		}
	}

out:
	set_rf_cfg_usage();
}


void cat_info(void *ptr, u16 len)
{
	u8 i;


	if (rf_mode == eMODE_SLEEP) {
		printf(PCLR_PUR_B"rf mode is[SLEEP]"PCLR_DFT NEW_LINE);
	}
	else if (rf_mode == eMODE_STBY) {
		printf(PCLR_PUR_B"rf mode is[STBY]"PCLR_DFT NEW_LINE);
	}
	else if (rf_mode == eMODE_RX) {
		printf(PCLR_PUR_B"rf mode is[RX]"PCLR_DFT NEW_LINE);
	}
	else if (rf_mode == eMODE_TX) {
		printf(PCLR_PUR_B"rf mode is[TX]"PCLR_DFT NEW_LINE);
	}
	else if (rf_mode == eMODE_ERR) {
		printf(PCLR_PUR_B"rf mode is[ERR]"PCLR_DFT NEW_LINE);
	}


	if (rf433_cfg_reg_bank == eCONFIG_REG_BANK_100K_20dB) {
		printf(PCLR_RED_B"100K 20dBm"PCLR_DFT NEW_LINE);
	}
	else if (rf433_cfg_reg_bank == eCONFIG_REG_BANK_100K_13dB) {
		printf(PCLR_RED_B"100K 13dBm"PCLR_DFT NEW_LINE);
	}
	else if (rf433_cfg_reg_bank == eCONFIG_REG_BANK_100K_10dB) {
		printf(PCLR_RED_B"100K 10dBm"PCLR_DFT NEW_LINE);
	}
	else if (rf433_cfg_reg_bank == eCONFIG_REG_BANK_64K_20dB) {
		printf(PCLR_RED_B"64K 20dBm"PCLR_DFT NEW_LINE);
	}
	else if (rf433_cfg_reg_bank == eCONFIG_REG_BANK_32K_20dB) {
		printf(PCLR_RED_B"32K 20dBm"PCLR_DFT NEW_LINE);
	}
	else if (rf433_cfg_reg_bank == eCONFIG_REG_BANK_16K_20dB) {
		printf(PCLR_RED_B"16K 20dBm"PCLR_DFT NEW_LINE);
	}
	else if (rf433_cfg_reg_bank == eCONFIG_REG_BANK_2400_20dB) {
		printf(PCLR_RED_B"2400 20dBm"PCLR_DFT NEW_LINE);
	}
	else if (rf433_cfg_reg_bank == eCONFIG_REG_BANK_GFSK_2_4K_5K_13dB) {
		printf(PCLR_RED_B"GFSK_2_4K_5K_13dB"PCLR_DFT NEW_LINE);
	}
	else if (rf433_cfg_reg_bank == eCONFIG_REG_BANK_GFSK_2400_20k_20db) {
		printf(PCLR_RED_B"GFSK_2400_20k_20db"PCLR_DFT NEW_LINE);
	}
	else if (rf433_cfg_reg_bank == eCONFIG_REG_BANK_GFSK_16k_32k_20db) {
		printf(PCLR_RED_B"GFSK_16k_32k_20db"PCLR_DFT NEW_LINE);
	}
	else if (rf433_cfg_reg_bank == eCONFIG_REG_BANK_GFSK_32k_64k_20db) {
		printf(PCLR_RED_B"GFSK_32k_64k_20db"PCLR_DFT NEW_LINE);
	}
	else if (rf433_cfg_reg_bank == eCONFIG_REG_BANK_GFSK_64k_128k_20db) {
		printf(PCLR_RED_B"GFSK_64k_128k_20db"PCLR_DFT NEW_LINE);
	}
	else if (rf433_cfg_reg_bank == eCONFIG_REG_BANK_GFSK_16k_32k_20db_gbt1) {
		printf(PCLR_RED_B"GFSK_16k_32k_20db_gbt1"PCLR_DFT NEW_LINE);
	}
	else if (rf433_cfg_reg_bank == eCONFIG_REG_BANK_16k_32k_20db) {
		printf(PCLR_RED_B"16k_32k_20db"PCLR_DFT NEW_LINE);
	}
	else if (rf433_cfg_reg_bank == eCONFIG_REG_BANK_GFSK487_5_64_128_20db) {
		printf(PCLR_RED_B"GFSK487_5_64_128_20db"PCLR_DFT NEW_LINE);
	}
	else if (rf433_cfg_reg_bank == eCONFIG_REG_BANK_GFSK487_5_16_32_20db) {
		printf(PCLR_RED_B"GFSK487_5_16_32_20db"PCLR_DFT NEW_LINE);
	}
	else if (rf433_cfg_reg_bank == eCONFIG_REG_BANK_rssi_16k) {
		printf(PCLR_RED_B"rssi_16k"PCLR_DFT NEW_LINE);
	}
	else if (rf433_cfg_reg_bank == eCONFIG_REG_BANK_rssi_64k) {
		printf(PCLR_RED_B"rssi_64k"PCLR_DFT NEW_LINE);
	}

	printf(PCLR_YEL_B"(PS: R = Rate, D = Deviation)"PCLR_DFT NEW_LINE);


	printf(PCLR_WHI_B"CH = %u(Freq: %u.%u MHz)"PCLR_DFT NEW_LINE, 
		cur_channel, (BASE_FREQ + (FH_OFFSET * 5 / 2) * cur_channel) / 1000, (BASE_FREQ + (FH_OFFSET * 5 / 2) * cur_channel) % 1000);

	printf(PCLR_YEL_B"dbg_len = %u"PCLR_DFT NEW_LINE, rf_dbg_len);

	printf(PCLR_LBL_B"count = %u, space = %u ms"PCLR_DFT NEW_LINE, tx_mgr.count, tx_mgr.space_ms);

	if (1) {
		
		printf(PCLR_WHI_B"Tx pkt(len = %02u): "NEW_LINE, tx_mgr.len);

		for (i = 0; i < tx_mgr.len; i++) {
			printf("%02X ", tx_mgr.buf[i]);
			if ((i % 16) == 15) {
				printf(NEW_LINE);
			}
		}

		printf(PCLR_DFT NEW_LINE);
	}


}



BOOL wait_status_result(u8 stat)
{
	u32 wait_cnt;
	u8 cur_stat;
	
	for (wait_cnt = 0; wait_cnt < 300; wait_cnt++) {
		delay_ms(1);
		cur_stat = CMT2300A_get_status();
		//printk("cur_stat = %u"NEW_LINE, cur_stat);
		if (stat == cur_stat) {
			return TRUE;
		}
	}
	
	return FALSE;
}



void CMT2300A_SetPayloadLength(u8 payload_len)
{
	u8 tmp;	

	tmp = read_reg(eCUS_PKT14);
	
	tmp &= 0x8F;				//length<256
	tmp &= (~0x01);				//Packet Mode
	tmp |= CMT2300A_PKT_TYPE_VARIABLE;//设置为可变长包格式

	write_reg(eCUS_PKT14, tmp);
	write_reg(eCUS_PKT15, payload_len);
}


BOOL rf433_tx_prepare(u8 *buf, u8 len)
{
	BOOL ret = FALSE;

	if (0 == len) {
		RF433_WARN("tx len == 0!"NEW_LINE);
		goto out;
	}
		
	CMT2300A_switch_status(CMT2300A_GO_STBY);

	if ( ! wait_status_result(CMT2300A_STA_STBY) ) {
		RF433_ERROR("switch STBY timout!"NEW_LINE);
		goto out;
	}

	CMT2300A_clear_it_flags();
	
    CMT2300A_ConfigInterrupt(
        CMT2300A_INT_SEL_TX_DONE, /* Config INT1 */
        CMT2300A_INT_SEL_CRC_OK   /* Config INT2 */
        );

    /* Enable interrupt */
    CMT2300A_EnableInterrupt(CMT2300A_MASK_TX_DONE_EN);

    CMT2300A_ClearTxFifo();
	CMT2300A_EnableWriteFifo();

	CMT2300A_fifo_merge_enable();

	tx_mgr.len = len;
	memcpy(tx_mgr.buf, buf, len);

	//CMT2300A_SetPayloadLength(len);

	ret = TRUE;

	//RF433_REMIND("-> Tx prepare"NEW_LINE);
	
out:
	return ret;
}



void rf433_start_tx(void)
{	
	
	//DISABLE_IRQ();

	tx_mgr.tx_done_flag = FALSE;

	CMT2300A_ClearTxFifo();

	WRITE_FIFO(tx_mgr.buf, tx_mgr.len);

	CMT2300A_SetPayloadLength(tx_mgr.len);
	
	CMT2300A_switch_status(CMT2300A_GO_TX);

	#if 0 // TODO: 这里可以不等 因为有发包中断 
	if ( ! wait_status_result(CMT2300A_STA_TX) ) {
		KERNEL_ERROR("switch TX timout!"NEW_LINE);
		ret = FALSE;
	}
	#endif

	//ENABLE_IRQ();

}



BOOL rf433_rx_mode(void)
{
	BOOL ret = FALSE;
		
	CMT2300A_switch_status(CMT2300A_GO_STBY);

	if ( ! wait_status_result(CMT2300A_STA_STBY) ) {
		RF433_ERROR("switch STBY timout!"NEW_LINE);
		goto out;
	}

	CMT2300A_clear_it_flags();

    /* Enable interrupt */
    CMT2300A_EnableInterrupt(CMT2300A_MASK_CRC_OK_EN);

    CMT2300A_ClearRxFifo();
	CMT2300A_EnableReadFifo();


#if 1 // TODO: 测试切换时间 
	CMT2300A_switch_status(CMT2300A_GO_SLEEP);

	if ( ! wait_status_result(CMT2300A_STA_SLEEP) ) {
		RF433_ERROR("###switch SLEEP timout!"NEW_LINE);
		goto out;
	}
#endif

	delay_ms(1);

	//led_ctrl(eCOLOR_OFF);

	CMT2300A_switch_status(CMT2300A_GO_RX);

	if ( ! wait_status_result(CMT2300A_STA_RX) ) {
		RF433_ERROR("switch RX timout!"NEW_LINE);
		goto out;
	}
	
	//led_ctrl(eCOLOR_BLUE);

	ret = TRUE;

	//RF433_REMIND("-> Rx mode"NEW_LINE);
	
out:
	return ret;
}


void tx_done_isr(void)
{
	u8 it_flag;
	u8 clear = 0;
	
	U2_DBG("Tx isr."NEW_LINE);

	it_flag = read_reg(eCUS_INT_CLR1);
    if (CMT2300A_MASK_TX_DONE_FLG != it_flag) {
		CMT2300A_clear_it_flags();
		U2_DBG("isr NOT match!"NEW_LINE);
    }

	clear |= CMT2300A_MASK_TX_DONE_CLR;    /* Clear CRC_OK_FLG */
    write_reg(eCUS_INT_CLR1, clear);

	tx_mgr.tx_done_flag = TRUE;
	
	U2_DBG("Tx done."NEW_LINE);

}

u8 rssi_code = 0;
u8 rssi_dbm = 0;
void rx_pkt_isr(void)
{
	u8 buf[RF_PAYLOAD_LENGTH];
	u8 len;
	u8 it_flag;
	u8 clear = 0;
	u8 rssi;

	led_ctrl(eCOLOR_RED);
	
	U2_DBG("Rx isr."NEW_LINE);

	it_flag = read_reg(eCUS_INT_FLAG);

	if (CMT2300A_MASK_CRC_OK_FLG != it_flag) {
		CMT2300A_clear_it_flags();
		U2_DBG("isr NOT match!"NEW_LINE);
		return;
	}

	rssi = CMT2300A_get_RSSI_DBM();

	CMT2300A_spi_read_fifo(&len, 1);

	len = __MIN(len, RF_PAYLOAD_LENGTH); 
	if (len) {
		CMT2300A_spi_read_fifo(buf, len);
	}
	
	clear |= CMT2300A_MASK_CRC_OK_CLR;    /* Clear CRC_OK_FLG */
    write_reg(eCUS_INT_CLR2, clear);

	CMT2300A_ClearRxFifo();

	U2_DBG("crc OK"NEW_LINE);

	U2_DBG("len = ");

	//print_02X(TRUE, len);
	
	U2_DBG(NEW_LINE);

	if ( ! add_one_pkt_to_fifo(&rf_pkt_fifo, buf, len, rssi) ) {
		U2_DBG("Full!"NEW_LINE);
		RF433_WARN("FIFO Full!"NEW_LINE);
	}

	led_ctrl(eCOLOR_OFF);

	#if 0
	if (len) {
		kernel_pkt_dump(buf, len);
	}
	#endif
}








BOOL rf433m_set_reg_bank(int set)
{
	switch (set) {		
	case eCONFIG_REG_BANK_100K_20dB:
		CMT2300A_config_reg_bank_100K_20dB();
		break;
				
	case eCONFIG_REG_BANK_100K_13dB:
		CMT2300A_config_reg_bank_100K_13dB();
		break;
				
	case eCONFIG_REG_BANK_100K_10dB:
		CMT2300A_config_reg_bank_100K_10dB();
		break;
		
	case eCONFIG_REG_BANK_64K_20dB:
		CMT2300A_config_reg_bank_64K_20dB();
		break;
		
	case eCONFIG_REG_BANK_32K_20dB:
		CMT2300A_config_reg_bank_32K_20dB();
		break;
		
	case eCONFIG_REG_BANK_16K_20dB:
		CMT2300A_config_reg_bank_16K_20dB();
		break;
				
	case eCONFIG_REG_BANK_2400_20dB:
		CMT2300A_config_reg_bank_2400_20dB();
		break;

	case eCONFIG_REG_BANK_GFSK_2_4K_5K_13dB:
		CMT2300A_config_reg_bank_GFSK_2_4K_5K_13dB();
		break;

	case eCONFIG_REG_BANK_GFSK_2400_20k_20db:
		CMT2300A_config_reg_bank_GFSK_2400_20k_20db();
		break;

	case eCONFIG_REG_BANK_GFSK_16k_32k_20db:
		CMT2300A_config_reg_bank_GFSK_16k_32k_20db();
		break;

	case eCONFIG_REG_BANK_GFSK_32k_64k_20db:
		CMT2300A_config_reg_bank_GFSK_32k_64k_20db();
		break;

	case eCONFIG_REG_BANK_GFSK_64k_128k_20db:
		CMT2300A_config_reg_bank_GFSK_64k_128k_20db();
		break;

	case eCONFIG_REG_BANK_GFSK_16k_32k_20db_gbt1:
		CMT2300A_config_reg_bank_GFSK_16k_32k_20db_gbt1();
		break;

	case eCONFIG_REG_BANK_16k_32k_20db:
		CMT2300A_config_reg_bank_16k_32k_20db();
		break;

	case eCONFIG_REG_BANK_GFSK487_5_64_128_20db:
		CMT2300A_config_reg_bank_GFSK487_5_64_128_20db();
		break;

	case eCONFIG_REG_BANK_GFSK487_5_16_32_20db:
		CMT2300A_config_reg_bank_GFSK487_5_16_32_20db();
		break;

	case eCONFIG_REG_BANK_rssi_16k:
		CMT2300A_config_reg_bank_rssi_16k();
		break;

	case eCONFIG_REG_BANK_rssi_64k:
		CMT2300A_config_reg_bank_rssi_64k();
		break;

	default:
		return FALSE;
	}

	return TRUE;
}



BOOL is_txing(void)
{
	return (rf_mode == eMODE_TX) ? TRUE : FALSE;
}

BOOL is_relaying(void)
{
	return (rf_mode == eMODE_RELAY) ? TRUE : FALSE;
}

#if 0
void do_tx_one_pkt(void *ptr, u16 len)
{
	u8 testbuf[8] = {0x12, 0x34, 0x56, 0x78, 0x90, 0xAB, 0xCD, 0xEF};
	
	rf433_tx_prepare(testbuf, 8);

	rf433_start_tx();
}
#endif


#define TX_TIME_OUT 				20000 // 2000ms TX超时时间


//void tmr_handle(int event, void *data, int data_len)
//{
//	if (tx_mgr.tx_timestamp != 0xFFFFFFFF) {
//		tx_mgr.tx_timestamp++;

//		if (tx_mgr.tx_timestamp >= TX_TIME_OUT) {
//			tx_mgr.tx_timeout = TRUE;
//		}
//	}
//	
//	if (tx_mgr.space_timestamp != 0xFFFFFFFF) { 
//		tx_mgr.space_timestamp++;
//	#if defined (IS_DELAY)
//		if (tx_mgr.space_timestamp >= 100) { // 10ms后回包 TIM15是100us周期 
//	#else
//		if (tx_mgr.space_timestamp >= tx_mgr.space_ms * 10) {// TIM15是100us周期 
//	#endif
//			tx_mgr.space_timeout = TRUE;
//		}
//	}
//	
//}

void tmr_handle(int event, void *data, int data_len)
{
	if (tx_mgr.tx_timestamp != 0xFFFFFFFF) {
		tx_mgr.tx_timestamp++;

		if (tx_mgr.tx_timestamp >= TX_TIME_OUT) {
			tx_mgr.tx_timeout = TRUE;
		}
	}
	
	if (tx_mgr.space_timestamp != 0xFFFFFFFF) { 
		tx_mgr.space_timestamp++;
		if(rf_mode == eMODE_RELAY){
			if (tx_mgr.space_timestamp >= 100) { // 10ms后回包 TIM15是100us周期 
				tx_mgr.space_timeout = TRUE;
			}
		}else{
			if (tx_mgr.space_timestamp >= tx_mgr.space_ms * 10) {// TIM15是100us周期 
				tx_mgr.space_timeout = TRUE;
			}
		}
	}
}

void do_rf_start_tx(void *ptr, u16 len)
{		
	u8 i;

	if ((rf_mode != eMODE_SLEEP) && (rf_mode != eMODE_STBY)) {
		printf(PCLR_RED_B"rf433 busy NOW!!!"PCLR_DFT NEW_LINE);
		return;
	}
	
	if ( ! rf433_tx_prepare(tx_mgr.buf, tx_mgr.len) ) {
		RF433_ERROR("rf433_tx_prepare Failed!"NEW_LINE);
		return;
	}

	printf(ORIGINAL_POINT);
	printf(CLEAR_SCREEN);

	RF433_WARN("rf433_start_tx. count = %u, space = %u ms"NEW_LINE, tx_mgr.count, tx_mgr.space_ms);

	if (rf433_cfg_reg_bank == eCONFIG_REG_BANK_100K_20dB) {
		printf(PCLR_RED_B"100K, 20dBm"PCLR_DFT NEW_LINE);
	}
	else if (rf433_cfg_reg_bank == eCONFIG_REG_BANK_100K_13dB) {
		printf(PCLR_RED_B"100K, 13dBm"PCLR_DFT NEW_LINE);
	}
	else if (rf433_cfg_reg_bank == eCONFIG_REG_BANK_100K_10dB) {
		printf(PCLR_RED_B"100K, 10dBm"PCLR_DFT NEW_LINE);
	}
	else if (rf433_cfg_reg_bank == eCONFIG_REG_BANK_64K_20dB) {
		printf(PCLR_RED_B"64K, 20dBm"PCLR_DFT NEW_LINE);
	}
	else if (rf433_cfg_reg_bank == eCONFIG_REG_BANK_32K_20dB) {
		printf(PCLR_RED_B"32K, 20dBm"PCLR_DFT NEW_LINE);
	}
	else if (rf433_cfg_reg_bank == eCONFIG_REG_BANK_16K_20dB) {
		printf(PCLR_RED_B"16K, 20dBm"PCLR_DFT NEW_LINE);
	}
	else if (rf433_cfg_reg_bank == eCONFIG_REG_BANK_2400_20dB) {
		printf(PCLR_RED_B"2400, 20dBm"PCLR_DFT NEW_LINE);
	}
	else if (rf433_cfg_reg_bank == eCONFIG_REG_BANK_GFSK_2_4K_5K_13dB) {
		printf(PCLR_RED_B"GFSK_2_4K_5K_13dB"PCLR_DFT NEW_LINE);
	}
	else if (rf433_cfg_reg_bank == eCONFIG_REG_BANK_GFSK_2400_20k_20db) {
		printf(PCLR_RED_B"GFSK_2400_20k_20db"PCLR_DFT NEW_LINE);
	}	
	else if (rf433_cfg_reg_bank == eCONFIG_REG_BANK_GFSK_16k_32k_20db) {
		printf(PCLR_RED_B"GFSK_16k_32k_20db"PCLR_DFT NEW_LINE);
	}
	else if (rf433_cfg_reg_bank == eCONFIG_REG_BANK_GFSK_32k_64k_20db) {
		printf(PCLR_RED_B"GFSK_32k_64k_20db"PCLR_DFT NEW_LINE);
	}
	else if (rf433_cfg_reg_bank == eCONFIG_REG_BANK_GFSK_64k_128k_20db) {
		printf(PCLR_RED_B"GFSK_64k_128k_20db"PCLR_DFT NEW_LINE);
	}
	else if (rf433_cfg_reg_bank == eCONFIG_REG_BANK_GFSK_16k_32k_20db_gbt1) {
		printf(PCLR_RED_B"GFSK_16k_32k_20db_gbt1"PCLR_DFT NEW_LINE);
	}
	else if (rf433_cfg_reg_bank == eCONFIG_REG_BANK_16k_32k_20db) {
		printf(PCLR_RED_B"16k_32k_20db"PCLR_DFT NEW_LINE);
	}
	else if (rf433_cfg_reg_bank == eCONFIG_REG_BANK_GFSK487_5_64_128_20db) {
		printf(PCLR_RED_B"GFSK487_5_64_128_20db"PCLR_DFT NEW_LINE);
	}
	else if (rf433_cfg_reg_bank == eCONFIG_REG_BANK_GFSK487_5_16_32_20db) {
		printf(PCLR_RED_B"GFSK487_5_16_32_20db"PCLR_DFT NEW_LINE);
	}
	else if (rf433_cfg_reg_bank == eCONFIG_REG_BANK_rssi_16k) {
		printf(PCLR_RED_B"rssi_16k"PCLR_DFT NEW_LINE);
	}
	else if (rf433_cfg_reg_bank == eCONFIG_REG_BANK_rssi_64k) {
		printf(PCLR_RED_B"rssi_64k"PCLR_DFT NEW_LINE);
	}

	printf(PCLR_YEL_B"(PS: R = Rate, D = Deviation)"PCLR_DFT NEW_LINE);


	printf(PCLR_WHI_B"CH = %u(Freq: %u.%u MHz)"PCLR_DFT NEW_LINE, 
		cur_channel, (BASE_FREQ + (FH_OFFSET * 5 / 2) * cur_channel) / 1000, (BASE_FREQ + (FH_OFFSET * 5 / 2) * cur_channel) % 1000);

	printf(PCLR_YEL_B"dbg_len = %u"PCLR_DFT NEW_LINE, rf_dbg_len);

	printf(PCLR_LBL_B"count = %u, space = %u ms"PCLR_DFT NEW_LINE, tx_mgr.count, tx_mgr.space_ms);
	
	if (1) {
		
		printf(PCLR_WHI_B"Tx pkt(%02u): "NEW_LINE, tx_mgr.len);

		for (i = 0; i < tx_mgr.len; i++) {
			printf("%02X ", tx_mgr.buf[i]);
			if ((i % 16) == 15) {
				printf(NEW_LINE);
			}
		}

		printf(PCLR_DFT NEW_LINE);
	}

	tx_mgr.tx_timeout = FALSE;
	tx_mgr.tx_timestamp = 0xFFFFFFFF;

	tx_mgr.space_timeout = FALSE;
	tx_mgr.space_timestamp = 0xFFFFFFFF;
	
	event_add(eEVENT_TIM15_UPDATE_IRQ, &tx_mgr.tmr_node, tmr_handle);
	bsp_tim15_init();

	ANT_switch_to_Tx();

	rf_mode = eMODE_TX;
}

void do_rf_start_rx(void *ptr, u16 len)
{		
	memset(&recv_pkt, 0, sizeof(recv_pkt));

	rx_count = 0;
	rssi_total = 0;

	rf_mode = eMODE_RX;
	//led_ctrl(eCOLOR_OFF);

	rf433_rx_mode();
	
	CMT2300A_fifo_merge_enable();
		
	RF433_INFO("rx start..."NEW_LINE);
}

void do_rf_end_rx(void *ptr, u16 len)
{
	RF433_INFO("rx abort!"NEW_LINE);

	CMT2300A_fifo_merge_disable();

	CMT2300A_switch_status(CMT2300A_GO_STBY);

	if ( ! wait_status_result(CMT2300A_GO_STBY) ) {
		RF433_ERROR("switch STBY timout!"NEW_LINE);
	}

#if 0
	CMT2300A_switch_status(CMT2300A_GO_SLEEP);

	if ( ! wait_status_result(CMT2300A_GO_SLEEP) ) {
		RF433_ERROR("switch SLEEP timout!"NEW_LINE);
	}
#endif

	rf_mode = eMODE_STBY;

	led_ctrl(eCOLOR_BLUE);
}




u32 gw_test_id = 0;

void do_rf_start_gw_test(void *ptr, u16 len)
{		
	u8 i;

	if ((rf_mode != eMODE_SLEEP) && (rf_mode != eMODE_STBY)) {
		printf(PCLR_RED_B"rf433 busy NOW!!!"PCLR_DFT NEW_LINE);
		return;
	}
	
	if ( ! rf433_tx_prepare(tx_mgr.buf, tx_mgr.len) ) {
		RF433_ERROR("rf433_tx_prepare Failed!"NEW_LINE);
		return;
	}

	printf(ORIGINAL_POINT);
	printf(CLEAR_SCREEN);

	RF433_WARN("rf433_start_tx. count = %u"NEW_LINE, tx_mgr.count);

	if (rf433_cfg_reg_bank == eCONFIG_REG_BANK_100K_20dB) {
		printf(PCLR_RED_B"100K, 20dBm"PCLR_DFT NEW_LINE);
	}
	else if (rf433_cfg_reg_bank == eCONFIG_REG_BANK_100K_13dB) {
		printf(PCLR_RED_B"100K, 13dBm"PCLR_DFT NEW_LINE);
	}
	else if (rf433_cfg_reg_bank == eCONFIG_REG_BANK_100K_10dB) {
		printf(PCLR_RED_B"100K, 10dBm"PCLR_DFT NEW_LINE);
	}
	else if (rf433_cfg_reg_bank == eCONFIG_REG_BANK_64K_20dB) {
		printf(PCLR_RED_B"64K, 20dBm"PCLR_DFT NEW_LINE);
	}
	else if (rf433_cfg_reg_bank == eCONFIG_REG_BANK_32K_20dB) {
		printf(PCLR_RED_B"32K, 20dBm"PCLR_DFT NEW_LINE);
	}
	else if (rf433_cfg_reg_bank == eCONFIG_REG_BANK_16K_20dB) {
		printf(PCLR_RED_B"16K, 20dBm"PCLR_DFT NEW_LINE);
	}
	else if (rf433_cfg_reg_bank == eCONFIG_REG_BANK_2400_20dB) {
		printf(PCLR_RED_B"2400, 20dBm"PCLR_DFT NEW_LINE);
	}

	printf(PCLR_YEL_B"(PS: R = Rate, D = Deviation)"PCLR_DFT NEW_LINE);


	printf(PCLR_WHI_B"CH = %u(Freq: %u.%u MHz)"PCLR_DFT NEW_LINE, 
		cur_channel, (BASE_FREQ + (FH_OFFSET * 5 / 2) * cur_channel) / 1000, (BASE_FREQ + (FH_OFFSET * 5 / 2) * cur_channel) % 1000);

	printf(PCLR_YEL_B"dbg_len = %u"PCLR_DFT NEW_LINE, rf_dbg_len);
	
	if (1) {
		
		printf("Tx pkt(%02u): "NEW_LINE, tx_mgr.len);

		for (i = 0; i < tx_mgr.len; i++) {
			printf("%02X ", tx_mgr.buf[i]);
			if ((i % 16) == 15) {
				printf(NEW_LINE);
			}
		}

		printf(NEW_LINE);
	}

	tx_mgr.tx_timeout = FALSE;
	tx_mgr.tx_timestamp = 0xFFFFFFFF;

	tx_mgr.space_timeout = FALSE;
	tx_mgr.space_timestamp = 0xFFFFFFFF;
	
	event_add(eEVENT_TIM15_UPDATE_IRQ, &tx_mgr.tmr_node, tmr_handle);
	bsp_tim15_init();

	ANT_switch_to_Tx();

	rf_mode = eMODE_GW_TEST;
}



void start_relay(void)
{
	memset(&recv_pkt, 0, sizeof(recv_pkt));

	rf_mode = eMODE_RELAY;
	
	led_ctrl(eCOLOR_BLUE);

	rf433_rx_mode();
	
	CMT2300A_fifo_merge_enable();
		
	RF433_INFO("-> relay"NEW_LINE);
}

void do_rf_start_relay(void *ptr, u16 len)
{	

	if ((rf_mode != eMODE_SLEEP) && (rf_mode != eMODE_STBY)) {
		printf(PCLR_RED_B"rf433 busy NOW!!!"PCLR_DFT NEW_LINE);
		return;
	}

	printf(ORIGINAL_POINT);
	printf(CLEAR_SCREEN);

	RF433_WARN("rf433_start_relay..."NEW_LINE);

	if (rf433_cfg_reg_bank == eCONFIG_REG_BANK_100K_20dB) {
		printf(PCLR_RED_B"100K, 20dBm"PCLR_DFT NEW_LINE);
	}
	else if (rf433_cfg_reg_bank == eCONFIG_REG_BANK_100K_13dB) {
		printf(PCLR_RED_B"100K, 13dBm"PCLR_DFT NEW_LINE);
	}
	else if (rf433_cfg_reg_bank == eCONFIG_REG_BANK_100K_10dB) {
		printf(PCLR_RED_B"100K, 10dBm"PCLR_DFT NEW_LINE);
	}
	else if (rf433_cfg_reg_bank == eCONFIG_REG_BANK_64K_20dB) {
		printf(PCLR_RED_B"64K, 20dBm"PCLR_DFT NEW_LINE);
	}
	else if (rf433_cfg_reg_bank == eCONFIG_REG_BANK_32K_20dB) {
		printf(PCLR_RED_B"32K, 20dBm"PCLR_DFT NEW_LINE);
	}
	else if (rf433_cfg_reg_bank == eCONFIG_REG_BANK_16K_20dB) {
		printf(PCLR_RED_B"16K, 20dBm"PCLR_DFT NEW_LINE);
	}
	else if (rf433_cfg_reg_bank == eCONFIG_REG_BANK_2400_20dB) {
		printf(PCLR_RED_B"2400, 20dBm"PCLR_DFT NEW_LINE);
	}
	else if (rf433_cfg_reg_bank == eCONFIG_REG_BANK_GFSK_2_4K_5K_13dB) {
		printf(PCLR_RED_B"GFSK_2_4K_5K_13dB"PCLR_DFT NEW_LINE);
	}		
	else if (rf433_cfg_reg_bank == eCONFIG_REG_BANK_GFSK_2400_20k_20db) {
		printf(PCLR_RED_B"GFSK_2400_20k_20db"PCLR_DFT NEW_LINE);
	}	
	else if (rf433_cfg_reg_bank == eCONFIG_REG_BANK_GFSK_16k_32k_20db) {
		printf(PCLR_RED_B"GFSK_16k_32k_20db"PCLR_DFT NEW_LINE);
	}
	else if (rf433_cfg_reg_bank == eCONFIG_REG_BANK_GFSK_32k_64k_20db) {
		printf(PCLR_RED_B"GFSK_32k_64k_20db"PCLR_DFT NEW_LINE);
	}
	else if (rf433_cfg_reg_bank == eCONFIG_REG_BANK_GFSK_64k_128k_20db) {
		printf(PCLR_RED_B"GFSK_64k_128k_20db"PCLR_DFT NEW_LINE);
	}
	else if (rf433_cfg_reg_bank == eCONFIG_REG_BANK_GFSK_16k_32k_20db_gbt1) {
		printf(PCLR_RED_B"GFSK_16k_32k_20db_gbt1"PCLR_DFT NEW_LINE);
	}
	else if (rf433_cfg_reg_bank == eCONFIG_REG_BANK_16k_32k_20db) {
		printf(PCLR_RED_B"16k_32k_20db"PCLR_DFT NEW_LINE);
	}
	else if (rf433_cfg_reg_bank == eCONFIG_REG_BANK_GFSK487_5_64_128_20db) {
		printf(PCLR_RED_B"GFSK487_5_64_128_20db"PCLR_DFT NEW_LINE);
	}
	else if (rf433_cfg_reg_bank == eCONFIG_REG_BANK_GFSK487_5_16_32_20db) {
		printf(PCLR_RED_B"GFSK487_5_16_32_20db"PCLR_DFT NEW_LINE);
	}
	else if (rf433_cfg_reg_bank == eCONFIG_REG_BANK_rssi_16k) {
		printf(PCLR_RED_B"rssi_16k"PCLR_DFT NEW_LINE);
	}
	else if (rf433_cfg_reg_bank == eCONFIG_REG_BANK_rssi_64k) {
		printf(PCLR_RED_B"rssi_64k"PCLR_DFT NEW_LINE);
	}
	
	start_relay();
}

static void do_rx(void)
{
	pkt_t *pkt_ptr = NULL;
	u8 cnt; // 记录本次取了多少
	u8 i, j;
	u8 rssi;
	u8 value;
	u8 sign;
	int aver_rssi;

	
	DISABLE_IRQ();

	cnt = __MIN(get_pkt_fifo_len(&rf_pkt_fifo), TMP_PRINT_CNT);
	if ( ! cnt ) {
		ENABLE_IRQ();
		//udelay(7);
		return;
	}
	
	//printf("cnt = %u"NEW_LINE, cnt);

	for (j = 0; j < cnt; j++) {
		pkt_ptr = fetch_one_pkt_from_fifo(&rf_pkt_fifo);
		//if (NULL != pkt_ptr) {
			__memcpy(recv_pkt[j].buf, pkt_ptr->buf, pkt_ptr->len);
			recv_pkt[j].len = pkt_ptr->len;
			recv_pkt[j].rssi = pkt_ptr->rssi;

		//}
	}
			

	ENABLE_IRQ();

	for (j = 0; j < cnt; j++) {

		if ( ! recv_pkt[j].len ) {
			continue;
		}

		
		rx_count++;

	
		//printf("R(%02u): ", recv_pkt[j].len);
		u1_print_string("Rx(");

		u1_print_DEC(rx_count);
		u1_print_string(") ");

		rssi = recv_pkt[j].rssi;
		
		if (rssi >= 128) {
			value = rssi - 128;
			sign = 0;
			rssi_total += value;
		}
		else {
			value = 128 - rssi;
			sign = 1;
			rssi_total -= value;
		}

		u1_print_string("RSSI: ");
		if (sign) {
			u1_print_string("-");
		}
		else {
			u1_print_string("+");
		}
		u1_print_DEC(value);

		aver_rssi = rssi_total / rx_count;

//		printf("  %d / %d = %d\r\n", rssi_total, rx_count, aver_rssi);
		if (aver_rssi >= 0) {
			value = aver_rssi;
			sign = 0;
		}
		else {
			aver_rssi *= -1;
			value = aver_rssi;
			sign = 1;
		}
		
		u1_print_string("(aver = ");
		if (sign) {
			u1_print_string("-");
		}
		else {
			u1_print_string("+");
		}
		u1_print_DEC(value);

		u1_print_string(") [Len: ");
		u1_print_DEC(recv_pkt[j].len);
		u1_print_string("] ");


		//for (i = 0; i < recv_pkt.len; i++) {
		for (i = 0; i < __MIN(recv_pkt[j].len, rf_dbg_len); i++) {
			//printf("%02X ", recv_pkt[j].buf[i]);
			u1_print_02X(FALSE, recv_pkt[j].buf[i]);
			u1_print_string(" ");
		}

		if (rf_dbg_len < recv_pkt[j].len) {
			//printf("...");
			u1_print_string("...");
		}

		//printf(NEW_LINE);
		u1_print_string(NEW_LINE);

		recv_pkt[j].len = 0;

	}
}


static void do_tx(void)
{
	static int stat = 0;
	static u32 sent = 0;
	static u32 failed = 0;
	
	if (sent >= tx_mgr.count) {
	//if (sent >= 0xFFFFFFF8) { // TODO: 上电一直发送
		printf(NEW_LINE"Tx done."NEW_LINE);

		CMT2300A_fifo_merge_disable();

		CMT2300A_switch_status(CMT2300A_GO_STBY);

		if ( ! wait_status_result(CMT2300A_GO_STBY) ) {
			RF433_ERROR("switch STBY timout!"NEW_LINE);
		}

#if 0
		CMT2300A_switch_status(CMT2300A_GO_SLEEP);

		if ( ! wait_status_result(CMT2300A_GO_SLEEP) ) {
			RF433_ERROR("switch SLEEP timout!"NEW_LINE);
		}
#endif
		tx_mgr.tx_timestamp = 0xFFFFFFFF;
		tx_mgr.space_timestamp = 0xFFFFFFFF;
		tx_mgr.tx_timeout = FALSE;
		tx_mgr.space_timeout = FALSE;

		ANT_switch_to_Rx();

		rf_mode = eMODE_STBY;

		bsp_tim15_fini();

		event_del(eEVENT_TIM15_UPDATE_IRQ, tmr_handle);
		
		led_ctrl(eCOLOR_BLUE);

		sent = 0;
		failed = 0;
		stat = 0;
		return;
	}

	if (0 == stat) { // 开始发包
		led_ctrl(eCOLOR_GREEN);
		rf433_start_tx();
		DISABLE_IRQ();
		tx_mgr.tx_timestamp = 0;
		tx_mgr.tx_timeout = FALSE;
		ENABLE_IRQ();

		stat = 1;
	}
	else if (1 == stat) { // 等待发包中断
		if (tx_mgr.tx_timeout) {
			RF433_ERROR("TO!"NEW_LINE);
			failed++;
			sent++;
			stat = 2;
			
			DISABLE_IRQ();
			tx_mgr.space_timestamp = 0;
			tx_mgr.space_timeout = FALSE;
			ENABLE_IRQ();
			led_ctrl(eCOLOR_OFF);

			return;
		}
	
		if (tx_mgr.tx_done_flag) {
			sent++;
			//printf("\033[10;0H" CURSOR_CLEAR_LINE);
			u1_print_string("\033[12;0H" CURSOR_CLEAR_LINE);
			//printf("%u/%u Finished"NEW_LINE, sent, tx_mgr.count);
			u1_print_DEC(sent);
			u1_print_string("/");
			u1_print_DEC(tx_mgr.count);
			u1_print_string(" Finished");
			u1_print_string("(");
			u1_print_DEC(failed);
			u1_print_string(" Errors)"NEW_LINE);
			
			stat = 2;
			
			DISABLE_IRQ();
			tx_mgr.space_timestamp = 0;
			tx_mgr.space_timeout = FALSE;
			ENABLE_IRQ();
			led_ctrl(eCOLOR_OFF);

			return;
		}
	}
	else if (2 == stat) { // 发包间隙

		if (tx_mgr.space_timeout) {
			stat = 0;
		}
	}
}


#define CMD_RELAY 0x88




static void do_relay(void)
{
	pkt_t tmp_pkt ;
	pkt_t *pkt_ptr = NULL;
	u8 cnt; // 记录本次取了多少
//	u8 i;
	
	DISABLE_IRQ();

	cnt = get_pkt_fifo_len(&rf_pkt_fifo);
	if ( ! cnt ) {
		ENABLE_IRQ();
		return;
	}

	pkt_ptr = fetch_one_pkt_from_fifo(&rf_pkt_fifo);
	__memcpy(tmp_pkt.buf, pkt_ptr->buf, pkt_ptr->len);
	tmp_pkt.len = pkt_ptr->len;
			
	ENABLE_IRQ();

	if ( ! tmp_pkt.len ) {
		return;
	}

#if 0
	u1_print_string("Rx(");
	u1_print_DEC(tmp_pkt.len);
	u1_print_string("): ");

	for (i = 0; i < __MIN(tmp_pkt.len, rf_dbg_len); i++) {
		u1_print_02X(FALSE, tmp_pkt.buf[i]);
		u1_print_string(" ");
	}

	if (rf_dbg_len < tmp_pkt.len) {
		u1_print_string("...");
	}

	u1_print_string(NEW_LINE);
#endif

	if (tmp_pkt.buf[0] != CMD_RELAY) {
		goto out;
	}

// TODO: 开始转发 

	rf433_tx_prepare(tmp_pkt.buf, tmp_pkt.len);

	tx_mgr.tx_timeout = FALSE;
	tx_mgr.tx_timestamp = 0xFFFFFFFF;

	tx_mgr.space_timeout = FALSE;
	tx_mgr.space_timestamp = 0xFFFFFFFF;
	
	event_add(eEVENT_TIM15_UPDATE_IRQ, &tx_mgr.tmr_node, tmr_handle);
	bsp_tim15_init();

	DISABLE_IRQ();
	tx_mgr.space_timeout = FALSE;
	tx_mgr.space_timestamp = 0;
	ENABLE_IRQ();

	while (1) { // 等待GW切换至接收状态
		if (tx_mgr.space_timeout) {
			break;
		}
	}

	ANT_switch_to_Tx();

	led_ctrl(eCOLOR_GREEN);
	rf433_start_tx();
	
	DISABLE_IRQ();
	tx_mgr.tx_timestamp = 0;
	tx_mgr.tx_timeout = FALSE;
	ENABLE_IRQ();

	while (1) { // 等待发包中断
		if (tx_mgr.tx_timeout) {
			RF433_ERROR("TO!"NEW_LINE);
			break;
		}
		
		if (tx_mgr.tx_done_flag) {
			//u1_print_string("Tx done"NEW_LINE);
			break;
		}
	}

	led_ctrl(eCOLOR_BLUE);

#if 0
	// TODO: 切成SLEEP先 测试稳定性 
	CMT2300A_switch_status(CMT2300A_GO_STBY);

	if ( ! wait_status_result(CMT2300A_GO_STBY) ) {
		RF433_ERROR("switch STBY timout!"NEW_LINE);
	}
		
	CMT2300A_switch_status(CMT2300A_GO_SLEEP);

	if ( ! wait_status_result(CMT2300A_GO_SLEEP) ) {
		RF433_ERROR("switch SLEEP timout!"NEW_LINE);
	}
	
	delay_ms(1);
#endif

// TODO: 切换回RX

	ANT_switch_to_Rx();

	rf433_rx_mode();

	bsp_tim15_fini();
	
	event_del(eEVENT_TIM15_UPDATE_IRQ, tmr_handle);
	
out:
	tmp_pkt.len = 0;

}


static void do_gw_test(void)
{
	static int stat = 0;
	static vu32 sent = 0;
	static u32 recv = 0;
	static u32 failed = 0;
	
	pkt_t tmp_pkt ;
	pkt_t *pkt_ptr = NULL;
	u8 cnt; // 记录本次取了多少
	u8 i;
	

	if (0 == stat) { // 准备报文
		if (sent >= tx_mgr.count) {
//		if (sent >= 0xFFFFFFF8) {
			u1_print_string(NEW_LINE"Tx Finished."NEW_LINE);

			CMT2300A_fifo_merge_disable();

			CMT2300A_switch_status(CMT2300A_GO_STBY);

			if ( ! wait_status_result(CMT2300A_GO_STBY) ) {
				RF433_ERROR("switch STBY timout!"NEW_LINE);
			}
			
			tx_mgr.tx_timestamp = 0xFFFFFFFF;
			tx_mgr.space_timestamp = 0xFFFFFFFF;
			tx_mgr.tx_timeout = FALSE;
			tx_mgr.space_timeout = FALSE;

			ANT_switch_to_Rx();
			rf_mode = eMODE_STBY;

			bsp_tim15_fini();

			event_del(eEVENT_TIM15_UPDATE_IRQ, tmr_handle);
			
			led_ctrl(eCOLOR_BLUE);

			sent = 0;
			failed = 0;
			stat = 0;
			recv = 0;
			return;
		}

		gw_test_id++;
		tx_mgr.buf[1] = (u8)((gw_test_id >> 24) & 0xFF);
		tx_mgr.buf[2] = (u8)((gw_test_id >> 16) & 0xFF);
		tx_mgr.buf[3] = (u8)((gw_test_id >> 8) & 0xFF);
		tx_mgr.buf[4] = (u8)((gw_test_id >> 0) & 0xFF);
		stat = 1;
	}
	else if (1 == stat) { // 开始发包
		led_ctrl(eCOLOR_GREEN);
		rf433_start_tx();
		DISABLE_IRQ();
		tx_mgr.tx_timestamp = 0;
		tx_mgr.tx_timeout = FALSE;
		ENABLE_IRQ();

		stat = 2;
	}
	else if (2 == stat) { // 等待发包中断
		if (tx_mgr.tx_timeout) {
			u1_print_string(PCLR_RED_B "TO!"PCLR_DFT NEW_LINE);
			failed++;
			sent++;
			
			DISABLE_IRQ();
			tx_mgr.space_timestamp = 0;
			tx_mgr.space_timeout = FALSE;
			ENABLE_IRQ();
			led_ctrl(eCOLOR_OFF);

			stat = 3;
			return;
		}
	
		if (tx_mgr.tx_done_flag) {
			led_ctrl(eCOLOR_OFF);
			sent++;
			#if 0
			u1_print_string("\033[10;0H" CURSOR_CLEAR_LINE);
			u1_print_DEC(sent);
			u1_print_string("/");
			u1_print_DEC(tx_mgr.count);
			u1_print_string(" Finished");
			u1_print_string("(");
			u1_print_DEC(failed);
			u1_print_string(" Errors)"NEW_LINE);
			#endif
			
			stat = 3;			

			return;
		}
	}
	else if (3 == stat) { // 切换收包
		ANT_switch_to_Rx();
		rf433_rx_mode();

		// TODO: 收包倒计时	
		DISABLE_IRQ();
		tx_mgr.space_timestamp = 0;
		tx_mgr.space_timeout = FALSE;
		ENABLE_IRQ();

		stat = 4;
	}
	else if (4 == stat) { // 等待收包

		if (tx_mgr.space_timeout) {
			stat = 5;
			u1_print_string(PCLR_RED_B"R-TO!"PCLR_DFT NEW_LINE);
			return;
		}

		DISABLE_IRQ();

		cnt = get_pkt_fifo_len(&rf_pkt_fifo);
		if ( ! cnt ) {
			ENABLE_IRQ();
			return;
		}

		pkt_ptr = fetch_one_pkt_from_fifo(&rf_pkt_fifo);
		__memcpy(tmp_pkt.buf, pkt_ptr->buf, pkt_ptr->len);
		tmp_pkt.len = pkt_ptr->len;
				
		ENABLE_IRQ();

		if ( ! tmp_pkt.len ) {
			return;
		}
		
		u1_print_string("Rx(");
		u1_print_DEC(tmp_pkt.len);
		u1_print_string("): ");

		for (i = 0; i < __MIN(tmp_pkt.len, rf_dbg_len); i++) {
			u1_print_02X(FALSE, tmp_pkt.buf[i]);
			u1_print_string(" ");
		}

		if (rf_dbg_len < tmp_pkt.len) {
			u1_print_string("...");
		}

		u1_print_string(NEW_LINE);

		if (tmp_pkt.buf[0] != CMD_RELAY) {
			return;
		}

		if (memcmp(tx_mgr.buf, tmp_pkt.buf, tx_mgr.len)) {
			return;
		}
		else {
			//u1_print_string("R OK."NEW_LINE);
			recv++;
			stat = 5;
		}
	}
	else if (5 == stat) { // 统计
		printf("Tx: %u/%u (%u Success, %u Errors) Rx(%u), ", 
//		printf("Tx: %u/** (%u Success, %u Errors) Rx(%u), ", 
			sent, tx_mgr.count, (sent - failed), failed, recv);

		printf("success-rate = %.2f"NEW_LINE, 
			(float)recv * 100 / (float)(sent - failed));
		
		stat = 6;
	}
	else if (6 == stat) { // 切回TX

		rf433_tx_prepare(tx_mgr.buf, tx_mgr.len);

		DISABLE_IRQ();
		tx_mgr.tx_timeout = FALSE;
		tx_mgr.tx_timestamp = 0xFFFFFFFF;

		tx_mgr.space_timeout = FALSE;
		tx_mgr.space_timestamp = 0xFFFFFFFF;
		ENABLE_IRQ();

		stat = 0;

		ANT_switch_to_Tx();
	}

}


void rf_loop_proc(void)
{		
	if (rf_mode == eMODE_ERR) {
		return;
	}

	if ((rf_mode == eMODE_SLEEP) || (rf_mode == eMODE_STBY)) {
		return;
	}
	
	if (rf_mode == eMODE_RX) {
//		{
//			static u32 loop_cnt = 0;
//			u8 res, value, sign;
//		
//			loop_cnt++;

//			if ((loop_cnt & 0x7FFF) == 0) {

//				DISABLE_IRQ();
//				res = CMT2300A_get_RSSI_DBM();
//				ENABLE_IRQ();

//				if (res >= 128) {
//					value = res - 128;
//					sign = 0;
//				}
//				else {
//					value = 128 - res;
//					sign = 1;
//				}

//				u1_print_string("RSSI: ");
//				if (sign) {
//					u1_print_string("-");
//				}
//				else {
//					u1_print_string("+");
//				}
//				u1_print_DEC(value);
//				u1_print_string(" dBm"NEW_LINE);

//			}
//		}
		do_rx();
	}
	else if (rf_mode == eMODE_TX) {
		do_tx();
	}
	else if (rf_mode == eMODE_RELAY) {
		do_relay();
	}
	else if (rf_mode == eMODE_GW_TEST) {
		do_gw_test();
	}

	
}



static void tx_mgr_init(void)
{
	u8 i;
	//u8 testbuf[8] = {0x12, 0x34, 0x56, 0x78, 0x90, 0xAB, 0xCD, 0xEF};

	//memcpy(tx_mgr.buf, testbuf, 8);


	tx_mgr.buf[0] = CMD_RELAY;
	tx_mgr.buf[1] = 0;
	tx_mgr.buf[2] = 0;
	tx_mgr.buf[3] = 0;
	tx_mgr.buf[4] = 0;

	for (i = 5; i < RF_PAYLOAD_LENGTH; i++) {
	//for (i = 0; i < RF_PAYLOAD_LENGTH; i++) {
		tx_mgr.buf[i] = i + 1;
		//tx_mgr.buf[i] = 0xFF;
	}
	
	//tx_mgr.len = RF_PAYLOAD_LENGTH;
	tx_mgr.len = 63;
	//tx_mgr.len = 2;
	
	tx_mgr.count = 500;
	tx_mgr.space_ms = 5; // TX时作为包间隔，GW_TEST时作为收包超时时间
//	tx_mgr.space_ms = 2; // TX时作为包间隔，GW_TEST时作为收包超时时间
	tx_mgr.tx_done_flag = FALSE;

	tx_mgr.tx_timeout = FALSE;
	tx_mgr.space_timeout = FALSE;

	tx_mgr.tx_timestamp = 0xFFFFFFFF;
	tx_mgr.space_timestamp = 0xFFFFFFFF;

}


/*
	1. 在 MCU 准备好工作后，发送一次软复位，等待 20ms。
	2. 确认芯片完成了复位并停留在 SLEEP 状态。
	3. 发送 go_stby 命令并确认芯片进入了 STBY 状态。
	4. 将 RFPDK 生成的寄存器内容写入配置区，地址是 0x00 – 0x5F。
	5. 项将地址为 0x09 寄存器低 3 比特[2:0]配置为 0x02，这个为内部优化配置项；
	6. 如果不需要使用 SLEEP TIMER，将 RECAL_LFOSC_EN， LFSOC_CAL1_EN 和 LFOSC_CAL2_EN
	   配置成 0。
	7. 按照实际需要，设置好控制区 1（0x60 – 0x6A）的寄存器，将 RSTN_IN_EN 设置为 0。
	8. 将 CUS_EN_CTL（0x62） 寄存器的第 5 个比特 ERROR_STOP_EN 配置成为 1，这个比特会让芯片在
	   遇到异常干扰情况下停留在 Error 状态，让 MCU 能通过状态读取来判断（即异常诊断机制）； 将
	   CUS_MODE_STA（0x61）寄存器的第 4 个比特 CFG_RETAIN 配置成 1，这个比特会让 0x00 – 0x5F
	   整个配置区的值不会被软复位擦除掉。
	9. 发送 go_sleep 命令，这个动作让寄存器配置生效。

 */
static BOOL rf_init(void)
{	
	u8 tmp;	

	CMT2300A_soft_reset();

	udelay(20000); // 20ms

	CMT2300A_switch_status(CMT2300A_GO_STBY);

	if ( ! wait_status_result(CMT2300A_STA_STBY) ) {
		RF433_ERROR("switch STBY timout!"NEW_LINE);
		return FALSE;
	}

	if(chose_rate_flag){
		rf433_cfg_reg_bank = eCONFIG_REG_BANK_rssi_64k;
	}else{
		rf433_cfg_reg_bank = eCONFIG_REG_BANK_rssi_16k;
	}

	rf433m_set_reg_bank(rf433_cfg_reg_bank);

    /* 内部优化配置 */
    tmp = (~0x07) & read_reg(eCUS_CMT10);
    write_reg(eCUS_CMT10, tmp | 0x02);
	
    /* Disable low frequency OSC calibration */
    CMT2300A_LFOSC_set(FALSE);

	/* Enable LOCKING_EN */
    tmp = read_reg(eCUS_EN_CTL);
    tmp |= CMT2300A_MASK_LOCKING_EN;         
    write_reg(eCUS_EN_CTL, tmp);

	/* Disable RstPin */
	tmp = read_reg(eCUS_MODE_STA);
	tmp |= CMT2300A_MASK_CFG_RETAIN;
	tmp &= (~CMT2300A_MASK_RSTN_IN_EN);			//Disable RstPin	
	write_reg(eCUS_MODE_STA, tmp);

	/* 清除中断 */
	CMT2300A_IntSrcFlagClr();

	/* 配置中断 */
    /* Config GPIOs */
    CMT2300A_ConfigGpio(
        CMT2300A_GPIO1_SEL_DOUT | 
        CMT2300A_GPIO2_SEL_INT1 | 	/* INT1 > GPIO2 */ // PB2
        CMT2300A_GPIO3_SEL_INT2 	/* INT2 > GPIO3 */ // PA0
        );

    /* Config interrupt */
    CMT2300A_ConfigInterrupt(
        CMT2300A_INT_SEL_TX_DONE, /* Config INT1 */
        CMT2300A_INT_SEL_CRC_OK   /* Config INT2 */
        );
	
#if 0
    /* Enable interrupt */
    CMT2300A_EnableInterrupt(
        //CMT2300A_MASK_TX_DONE_EN  |
        //CMT2300A_MASK_PREAM_OK_EN |
        //CMT2300A_MASK_SYNC_OK_EN  |
        //CMT2300A_MASK_NODE_OK_EN  |
        //CMT2300A_MASK_CRC_OK_EN   |
        //CMT2300A_MASK_PKT_DONE_EN
        CMT2300A_MASK_CRC_OK_EN
        );
#endif

#if 1
	/* 中心频率初始化 */
	if (0 != custom_ch_init()) {

		RF433_ERROR("custom_ch_init failed!"NEW_LINE);
		return FALSE;
	}
#endif

	CMT2300A_switch_status(CMT2300A_GO_SLEEP);

	if ( ! wait_status_result(CMT2300A_STA_SLEEP) ) {
		RF433_ERROR("switch SLEEP timout!"NEW_LINE);
		return FALSE;
	}

	return TRUE;
}




void irq_init(void)
{
	EXTI_InitTypeDef EXTI_InitStructure;
	
	bsp_gpio_init(IRQ2_GPIO_GROUP,
				  IRQ2_GPIO_PIN, 
				  IRQ2_GPIO_MODE,
				  IRQ2_GPIO_SPEED,
				  IRQ2_GPIO_OTYPE,
				  IRQ2_GPIO_PUPD);

	bsp_gpio_init(IRQ3_GPIO_GROUP,
				  IRQ3_GPIO_PIN, 
				  IRQ3_GPIO_MODE,
				  IRQ3_GPIO_SPEED,
				  IRQ3_GPIO_OTYPE,
				  IRQ3_GPIO_PUPD);

	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);			//使能系统时钟配置								

	SYSCFG_EXTILineConfig(IRQ2_GPIO_PORTSOURCE, IRQ2_GPIO_PINSOURCE); 
	
	EXTI_InitStructure.EXTI_Line = IRQ2_GPIO_EXTI_LINE;
	EXTI_InitStructure.EXTI_Mode = IRQ2_GPIO_EXTI_MODE;
	EXTI_InitStructure.EXTI_Trigger = IRQ2_GPIO_EXTI_TRIGGER; 
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure); 
	nvic_config(IRQ2_GPIO_NVIC_IRQCHANNEL, RF_IRQ_PRIOR, ENABLE);

	SYSCFG_EXTILineConfig(IRQ3_GPIO_PORTSOURCE, IRQ3_GPIO_PINSOURCE); 
	
	EXTI_InitStructure.EXTI_Line = IRQ3_GPIO_EXTI_LINE;
	EXTI_InitStructure.EXTI_Mode = IRQ3_GPIO_EXTI_MODE;
	EXTI_InitStructure.EXTI_Trigger = IRQ3_GPIO_EXTI_TRIGGER; 
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure); 
	nvic_config(IRQ3_GPIO_NVIC_IRQCHANNEL, RF_IRQ_PRIOR, ENABLE);

}

void PA_en_init(void)
{
	bsp_gpio_init(PA_EN_GPIO_GROUP,
				  PA_EN_GPIO_PIN, 
				  PA_EN_GPIO_MODE, 
				  PA_EN_GPIO_SPEED,
				  PA_EN_GPIO_OTYPE,
				  PA_EN_GPIO_PUPD);


	/* 平时就切换为RX */
	PA_to_Rx();
}

void rf433_init(void)
{
	CMT2300A_spi_init();
	
//	power_init();

	PA_en_init();

	TxRx_en_init();

	pkt_fifo_init(&rf_pkt_fifo, rf_pkt, MAX_RF_PKT_COUNT);

	tx_mgr_init();

	chose_rate();
	
	if ( ! rf_init() ) {
		rf_mode = eMODE_ERR;
		RF433_ERROR("rf433_init Failed!"NEW_LINE);
		return;
	}
	
	rf_mode = eMODE_SLEEP;

	irq_init();

	//RF433_INFO("rf433_init Success."NEW_LINE);
}


