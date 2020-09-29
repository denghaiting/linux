/****************************************Copyright (c)****************************************
 																							
                             Galaxywind Network System Co., Ltd.						    
                                      												
                                  http://www.galaxywind.com								
 																						
 Author:	Y
 Date  : 	2018-03-16
 *********************************************************************************************/ 
#include "user_interface.h"
#include "bsp_flash.h"
#include "version.h"

__align(4) const char *build_date = __DATE__;
__align(4) const char *build_time = __TIME__;
__align(4) const char *file_ptr = __FILE__;
__align(4) const int file_line = __LINE__;




typedef void (*ui_callback_t)(void);

typedef struct {
	loop_stat_e stat;
	ui_callback_t into;
	ui_callback_t doing;
	ui_callback_t quit;
} ui_t;


event_t usart_irq_node;
event_t timer_update_node;


static u8 uart_rbyte;
static vu8 uart_recvd;


app_image_info_t ai_info;


static ui_t ui[eLP_MAX];

__IO loop_stat_e loop_stat;


#define LOOP_STAT_SHIFT(new_stat) \
	do { \
		if (ui[loop_stat].quit) {\
			ui[loop_stat].quit();\
		}\
		loop_stat = new_stat; \
		if (ui[loop_stat].into) {\
			ui[loop_stat].into(); \
		}\
	} while (0)




static FunVoidType JumpToApplication;

static u32 m_JumpAddress;

static void jump_to_app(void)
{
	/* Jump to user application */
	m_JumpAddress = *(vu32*) (APP_IMAGE_ADDR + 4);
	JumpToApplication = (FunVoidType) m_JumpAddress;

	/* Initialize user application's Stack Pointer */
	__set_MSP(*(vu32*) APP_IMAGE_ADDR);
	
	JumpToApplication();
}



static void start_usart_irq(int event, void *data, int data_len)
{
	if (uart_recvd) {
		return;
	}

	uart_recvd = 1;
	
	uart_rbyte = *(u8 *)data;
}


vu32 timer_counter;
vu32 delay_timer;

static void timer_update_irq(int event, void *data, int data_len)
{
	timer_counter++;
	delay_timer++;
}





/* +------------------------------------------+ *
 * |	  		精确延时 单位 5ms        	  | *
 * +------------------------------------------+ */
void Delay_5ms(u32 t)
{
	delay_timer = 0;
	
	while (delay_timer < t);
}





void usart_recv_default_init(void)
{
	uart_recvd = 0;

#if defined (USE_USART_1)
	event_add(eEVENT_USART1_RXNE, &usart_irq_node, start_usart_irq);	
#elif defined (USE_USART_2)
	event_add(eEVENT_USART2_RXNE, &usart_irq_node, start_usart_irq);	
#endif
	
	event_add(eEVENT_TIM3_UPDATE_IRQ, &timer_update_node, timer_update_irq);

	bsp_tim3_init();
}

void usart_recv_default_fini(void)
{
	bsp_tim3_fini();

#if defined (USE_USART_1)
	event_del(eEVENT_USART1_RXNE, start_usart_irq);
#elif defined (USE_USART_2)
	event_del(eEVENT_USART2_RXNE, start_usart_irq);
#endif
	
	event_del(eEVENT_TIM3_UPDATE_IRQ, timer_update_irq);

	uart_recvd = 0;
}


#define BE_AZ(c)  				((c >= 'A') && (c <= 'Z'))
#define be_az(c)  				((c >= 'a') && (c <= 'z'))


char make_cmd(char ch)
{
	if (BE_AZ(ch)) {
		return (ch);
	}
	else if (be_az(ch)) {
		return (ch - ('a' - 'A'));
	}
	else {
		return 0;
	}
}


typedef enum {

	eCH_CMD_HELP,
	eCH_CMD_INFO,
	eCH_CMD_JUMP,
	eCH_CMD_UPGRADE,
	eCH_CMD_VERSION,
	eCH_CMD_MAX,

} ch_cmd_e;

typedef void (*ch_cmd_func_t)(void);

typedef struct {
	u8 idx;
	char ch_cmd;
	u16 pad;
	char *exegesis;
	ch_cmd_func_t func;
} ch_cmd_t;


static ch_cmd_t ch_cmd[eCH_CMD_MAX];




void print_version(void)
{
	int maj, min, rev;
	
	maj = APP_VERSION_MAJOR;
	min = APP_VERSION_MINOR;
	rev = APP_VERSION_REVISION;
	
	printf(NEW_LINE PCLR_RED_B"Version : %d.%d.%d"PCLR_DFT NEW_LINE, maj, min, rev);
	
}


void print_all_ch_cmd(void)
{
	int i;
	
	printf(NEW_LINE);
	printf(PCLR_WHI_B"-------- CH CMD --------"PCLR_DFT NEW_LINE);

	printf(PCLR_YEL_B);
	
	for (i = 0; i < eCH_CMD_MAX; i++) {

		printf("<%02u> %c : %s\r\n", i, ch_cmd[i].ch_cmd, ch_cmd[i].exegesis);
	}

	printf(PCLR_WHI_B"------------------------"PCLR_DFT NEW_LINE);
}


void do_ch_cmd_help(void)
{
	print_all_ch_cmd();
}


#if 0
void do_ch_cmd_info(void)
{
	printf(NEW_LINE);
	printf(PCLR_WHI_B"------------------ INFO ------------------"PCLR_DFT NEW_LINE);

	printf(PCLR_RED_B);
	printf("MCU TYPE   : STM32F030C8\r\n"NEW_LINE);
	printf("MCU ID     : 0x%08X\r\n", DBGMCU->IDCODE);

	printf(PCLR_LBL_B);
	printf("FLASH SIZE : 0x%X (%uK) (%u Byte)\r\n", FLASH_TOTAL_SIZE, (FLASH_TOTAL_SIZE >> 10), FLASH_TOTAL_SIZE);
	printf("RAM SIZE   : 0x%X (%uK) (%u Byte)\r\n", RAM_TOTAL_SIZE, (RAM_TOTAL_SIZE >> 10), RAM_TOTAL_SIZE);
	printf("PAGE SIZE  : 0x%X (%uK) (%u Byte)\r\n", FLASH_PAGE_SIZE, (FLASH_PAGE_SIZE >> 10), FLASH_PAGE_SIZE);
	
	printf(PCLR_PUR_B);
	printf("Build date : %s\r\n", build_date);
	printf("Build time : %s\r\n", build_time);

	printf(PCLR_YEL_B);
	printf("Image start addr: 0x%08X\r\n", APP_IMAGE_START);
	printf("Image Max Size  : 0x%X (%u Byte)\r\n", APP_IMAGE_SIZE, APP_IMAGE_SIZE);
	printf("Image check ret : %u\r\n", ai_info.check_ret);
	printf("Image crc       : crc[0]=0x%02x, crc[1]=0x%02x\r\n", ai_info.crc[0], ai_info.crc[1]);
	printf("Image pure size : %u Bytes\r\n", ai_info.size);
	if (eIMAGE_CHECK_OK == ai_info.check_ret) {

		if ( ! memcmp(ai_info.imfl.magic, FILE_MAGIC, FILE_MAGIC_LEN) ) {
			printf("Image file size : %u Bytes\r\n", ai_info.imfl.size);
			ai_info.imfl.name[FILE_NAME_LENGTH - 1] = '\0';
			printf("Image file name : %s\r\n", ai_info.imfl.name);
		}
	}
	
	
	printf(PCLR_WHI_B"------------------------------------------"PCLR_DFT NEW_LINE);
}
#else

void do_ch_cmd_info(void)
{
	int maj, min, rev;
	
	printf(NEW_LINE);
	printf(PCLR_WHI_B"------------------ INFO ------------------"PCLR_DFT NEW_LINE);

	maj = APP_VERSION_MAJOR;
	min = APP_VERSION_MINOR;
	rev = APP_VERSION_REVISION;
	
	printf(PCLR_RED_B"Version : %d.%d.%d"PCLR_DFT NEW_LINE, maj, min, rev);

	printf(PCLR_BLU_B NEW_LINE);
	//printf("MCU TYPE   : STM32F030C8\r\n"NEW_LINE);
	//printf("=============================================================="NEW_LINE);
	printf("STM32F030C8"NEW_LINE);
	printf("      |  ||"NEW_LINE);
	printf("      |  |+------------ FLASH=64K,RAM=8K"NEW_LINE);
	printf("      |  +------- 48PIN"NEW_LINE);
	printf("      +--M0 core"NEW_LINE NEW_LINE);
	//printf("=============================================================="NEW_LINE);
	
	printf("MCU ID     : 0x%08X"NEW_LINE, DBGMCU->IDCODE);

	printf(PCLR_LBL_B);
	printf("FLASH SIZE : 0x%x (%uK) (%u Byte)"NEW_LINE, FLASH_TOTAL_SIZE, (FLASH_TOTAL_SIZE >> 10), FLASH_TOTAL_SIZE);
	printf("RAM SIZE   : 0x%x (%uK) (%u Byte)"NEW_LINE, RAM_TOTAL_SIZE, (RAM_TOTAL_SIZE >> 10), RAM_TOTAL_SIZE);
	printf("PAGE SIZE  : 0x%x (%uK) (%u Byte)"NEW_LINE, FLASH_PAGE_SIZE, (FLASH_PAGE_SIZE >> 10), FLASH_PAGE_SIZE);
	
	printf(PCLR_PUR_B);
	printf("Build date : %s"NEW_LINE, build_date);
	printf("Build time : %s"NEW_LINE, build_time);

	printf(PCLR_YEL_B);
	printf("Image start addr: 0x%08X"NEW_LINE, APP_IMAGE_START);
	printf("Image Max Size  : 0x%x (%u Byte)"NEW_LINE, APP_IMAGE_SIZE, APP_IMAGE_SIZE);
	printf("Image check ret : %u"NEW_LINE, ai_info.check_ret);
	printf("Image crc       : crc[0]=0x%02x, crc[1]=0x%02x"NEW_LINE, ai_info.crc[0], ai_info.crc[1]);
	printf("Image pure size : %u Bytes"NEW_LINE, ai_info.size);
	if (eIMAGE_CHECK_OK == ai_info.check_ret) {

		if ( ! memcmp(ai_info.imfl.magic, FILE_MAGIC, FILE_MAGIC_LEN) ) {
			printf("Image file size : %u Bytes"NEW_LINE, ai_info.imfl.size);
			ai_info.imfl.name[FILE_NAME_LENGTH - 1] = '\0';
			printf("Image file name : %s"NEW_LINE, ai_info.imfl.name);
		}
	}
	
	
	printf(PCLR_WHI_B"------------------------------------------"PCLR_DFT NEW_LINE);
}
#endif


void do_ch_cmd_jump(void)
{
	printf(NEW_LINE PCLR_BLU_B);
	printf("Jump"PCLR_DFT NEW_LINE);
	LOOP_STAT_SHIFT(eLP_JUMP_APP); 
}


void do_ch_cmd_upgrade(void)
{
	ym_mgr.start_addr = APP_IMAGE_START;
	ym_mgr.max_size = APP_IMAGE_SIZE;
	printf(NEW_LINE PCLR_PUR_B);
	printf("Upgrade..."PCLR_DFT NEW_LINE);
	LOOP_STAT_SHIFT(eLP_YMODEM); 
}


void do_ch_cmd_version(void)
{
	print_version();
}


static ch_cmd_t ch_cmd[eCH_CMD_MAX] = {
	{eCH_CMD_HELP, CH_CMD_HELP, PAD, EXEG_HELP, do_ch_cmd_help},
	{eCH_CMD_INFO, CH_CMD_INFO, PAD, EXEG_INFO, do_ch_cmd_info},
	{eCH_CMD_JUMP, CH_CMD_JUMP, PAD, EXEG_JUMP, do_ch_cmd_jump},
	{eCH_CMD_UPGRADE, CH_CMD_UPGRADE, PAD, EXEG_UPGRADE, do_ch_cmd_upgrade},
	{eCH_CMD_VERSION, CH_CMD_VERSION, PAD, EXEG_VERSION, do_ch_cmd_version},

};



image_file_t temp_image_file;


void start_into(void)
{
	usart_recv_default_init();

	/* 镜像校验 */
	ai_info.check_ret = check_app((u8 *)APP_IMAGE_START, APP_IMAGE_SIZE, ai_info.crc, &ai_info.size);
	if (eIMAGE_CHECK_OK == ai_info.check_ret) {
		flash_read(BOOT_USE_PAGE0, (u8 *)&ai_info.imfl, sizeof(ai_info.imfl));
	}
	
	PUT_CHAR(REBOOT_BYTE);

	__DISABLE_IRQ();
	timer_counter = 0;
	__ENABLE_IRQ();	
}

void start_loop(void)
{
	u32 counter;
	u8 rx_byte;
	static u8 c_count = 0;

	counter = timer_counter;

	if (counter > MAX_WAIT_CMD_TIME) {
		if (eIMAGE_CHECK_OK != ai_info.check_ret) {
			printf("\r\nimage_check_ret = %u\r\n", ai_info.check_ret);
			ym_mgr.start_addr = APP_IMAGE_START;
			ym_mgr.max_size = APP_IMAGE_SIZE;
			goto y_modem;
		}

		LOOP_STAT_SHIFT(eLP_JUMP_APP);
		return;
	}

	/* USER 输入 CC */
	if ( ! uart_recvd ) {
		return;
	}	
	
	rx_byte = uart_rbyte;

	uart_recvd = 0;
	
	if ((rx_byte == USER_START_CMD || rx_byte == CAP2SMA(USER_START_CMD))) {
		if (++c_count >= USER_START_CMD_CNT) {
						
			PUT_CHAR(BACK_TO_USER_CMD);

			LOOP_STAT_SHIFT(eLP_COMMON_TERMINAL);
			return; 
		}
	} 
	else {
		c_count = 0;
	}

	return;
	
y_modem:
	LOOP_STAT_SHIFT(eLP_YMODEM); // 进入Y-MODEM	
}



void start_quit(void)
{
	usart_recv_default_fini();
}



void common_tmn_into(void)
{	
	usart_recv_default_init();	
}

void common_tmn_loop(void)
{
	int i;
	u8 rx_byte, cmd;

	if ( ! uart_recvd ) {
		return;
	}	
	
	rx_byte = uart_rbyte;

	uart_recvd = 0;

	cmd = make_cmd(rx_byte);
	if ( ! cmd ) {
		return;
	}

	for (i = 0; i < eCH_CMD_MAX; i++) {
		if (ch_cmd[i].ch_cmd == cmd) {
			if (NULL != ch_cmd[i].func) {
				ch_cmd[i].func();
				break;
			}
		}
	}
}

void common_tmn_quit(void)
{
	usart_recv_default_fini();	
}



static void loop_jump(void)
{
	int ret;
	u8 app_crc[2];

	if (((*(vu32*)APP_IMAGE_ADDR) & 0x2FFE0000 ) == 0x20000000) {
		ret = check_app((u8 *)APP_IMAGE_ADDR, APP_IMAGE_SIZE, app_crc, NULL);
		if (ret) {
			printf("\r\ncheck_app failed!!! ret is %d\r\n", ret);
			printf("Please Transmit The image By Y-Modem\r\n");
			goto y_modem;
		}
		
		//printf("JUMP TO 0x%08X\r\n", APP_IMAGE_ADDR + 4);

		// TODO: 这里关了全局中断后 APP第一句话应该打开 否则会跑飞
		__DISABLE_IRQ();
		 
		usart_common_fini();

	    jump_to_app();
	} 
	else {
		printf("NO Image or Invalid Image! Please Transmit The image By Y-Modem\r\n");
		goto y_modem;
	}

y_modem:
	ym_mgr.start_addr = APP_IMAGE_START;
	ym_mgr.max_size = APP_IMAGE_SIZE;
	LOOP_STAT_SHIFT(eLP_YMODEM);
}


void ymodem_into(void)
{
	ymodem_init(0);

#if defined (USE_USART_1)
	event_add(eEVENT_USART1_RXNE, &usart_irq_node, Ymodem_uart_isr);	
#elif defined (USE_USART_2)
	event_add(eEVENT_USART2_RXNE, &usart_irq_node, Ymodem_uart_isr);	
#endif

	event_add(eEVENT_TIM3_UPDATE_IRQ, &timer_update_node, timer_update_irq);

	bsp_tim3_init();

	__DISABLE_IRQ();
	timer_counter = 0;
	__ENABLE_IRQ();
}

void ymodem_loop(void)
{
	u32 now_time = timer_counter;
	
	Ymodem_proc(now_time);
}

void ymodem_quit(void)
{	
	bsp_tim3_fini();

#if defined (USE_USART_1)
	event_del(eEVENT_USART1_RXNE, Ymodem_uart_isr);
#elif defined (USE_USART_2)
	event_del(eEVENT_USART2_RXNE, Ymodem_uart_isr);
#endif

	event_del(eEVENT_TIM3_UPDATE_IRQ, timer_update_irq);
}



/* 
 * 	     	    UI 状态机     		    
 */
static ui_t ui[eLP_MAX] = {
	{eLP_START, start_into, start_loop, start_quit},
	{eLP_COMMON_TERMINAL, common_tmn_into, common_tmn_loop, common_tmn_quit},	
	{eLP_YMODEM, ymodem_into, ymodem_loop, ymodem_quit}, 
	{eLP_JUMP_APP, NULL, loop_jump, NULL},
};



void ui_init(void)
{
	memset((u8 *)&ym_mgr, 0, sizeof(ym_mgr));
	memset((u8 *)&ai_info, 0, sizeof(ai_info));
	ai_info.check_ret = eIMAGE_NOT_CHECK;

	LOOP_STAT_SHIFT(eLP_START);
}


void ui_loop(void)
{
	if (ui[loop_stat].doing) {
		ui[loop_stat].doing();
	}
}





/* END OF FILE */

