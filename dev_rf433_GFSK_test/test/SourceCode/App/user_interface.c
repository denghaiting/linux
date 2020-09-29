/*###################################*
 *#                                 #*
 *#     Author: Yu                  #*
 *#     Email : 33076156@qq.com     #*
 *#     Date  : 2020.3.10           #*
 *#                                 #*
 *###################################*/
 
#include "sys_config.h"
#include "sys.h"
#include "sys_tools.h"
#include "version.h"
#include "print_color.h"
//#include "bsp_usart1.h"
//#include "bsp_usart2.h"
#include "bsp_usart1_dma_txrx.h"
#include "bsp_tim3.h"
#include "user_interface.h"
#include "event.h"
#include "list.h"
#include "led.h"
#include "rf433.h"
#include "freq_adjust.h"
//#include "pkt_fifo.h"

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


event_t usart_recv_node;
event_t timer_update_node;

event_t common_with_user_node;


static vu8 uart_recvd;


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



#if 0
static void start_usart_collect_data(int event, void *data, int data_len)
{
	if (uart_recvd) {
		return;
	}

	uart_recvd = 1;
	
	uart_rbyte = *(u8 *)data;
}


vu32 timer_counter;
//vu32 delay_timer;

static void timer_update_irq(int event, void *data, int data_len)
{
	timer_counter++;
	//delay_timer++;
}


void usart_recv_default_init(void)
{
	uart_recvd = 0;
	
	event_add(eEVENT_USART2_READ_DATA, &usart_recv_node, start_usart_collect_data);	

	event_add(eEVENT_TIM3_UPDATE_IRQ, &timer_update_node, timer_update_irq);

	bsp_tim3_init();
}

void usart_recv_default_fini(void)
{
	bsp_tim3_fini();
	
	event_del(eEVENT_USART2_READ_DATA, start_usart_collect_data);

	event_del(eEVENT_TIM3_UPDATE_IRQ, timer_update_irq);

	uart_recvd = 0;
}


#if 0
void print_start(u32 sec)
{
	char ch;
	
	printf(CLEAR_SCREEN ORIGINAL_POINT);
	ch = START_CMD_GOTO_USER;
	printf("<warning!!!> Will Go upmo After ");
	printf(PCLR_RED_B"%u"PCLR_DFT, sec);
	printf(" sec(s)..."NEW_LINE);
	printf("Please Input %c To User Contrl"NEW_LINE, ch);
}
#else
void print_start(u32 sec)
{
	printf("$");
}
#endif


void start_into(void)
{
	usart_recv_default_init();
	
	print_start(MAX_START_WAIT_CNT);

	DISABLE_IRQ();
	timer_counter = 0;
	ENABLE_IRQ();	
}


void start_loop(void)
{
	u32 counter;
	static u32 pre_counter = 0;
	static u8 timeout_cnt = MAX_START_WAIT_CNT;
	u8 rx_byte;

	counter = timer_counter;

	if ((counter - pre_counter) > MAX_WAIT_CMD_TIME) {
		pre_counter = counter;
		timeout_cnt--;
		print_start(timeout_cnt);
		if (0 == timeout_cnt) {
			goto _upmo;
		}
	}
	
	usart2_Rx_proc();
	
	/* USER 输入 CC */
	if ( ! uart_recvd ) {
		return;
	}	
	
	rx_byte = uart_rbyte;

	uart_recvd = 0;
	
	if (rx_byte == START_CMD_GOTO_USER) {
		LOOP_STAT_SHIFT(eLP_COMMON_TERMINAL);
		return; 
	} 

	return;


_upmo:

	LOOP_STAT_SHIFT(eLP_COMMON_UPMO);
	return;

}


void start_quit(void)
{
	usart_recv_default_fini();
}
#endif




#define COMMON_CMD_INPUT		"[User]$: "

#define BACKSPACE		0x8
#define TAB_KEY			0x9


typedef enum {
	eCOMMON_CMD_CTRL_LED,
	eCOMMON_CMD_START_RELAY,

	//eCOMMON_CMD_TX_ONE_PKT,
	eCOMMON_CMD_START_TX,
	eCOMMON_CMD_RF_START_RX,
	eCOMMON_CMD_RF_END_RX,
	eCOMMON_CMD_RF_GW_TEST,
	eCOMMON_CMD_RF_DEBUG_LEN,
	eCOMMON_CMD_RF_SET_CH,
	eCOMMON_CMD_RF_TX_COUNT,
	eCOMMON_CMD_RF_TX_SPACE,
	eCOMMON_CMD_RF_TX_LEN,
	eCOMMON_CMD_RF_RATE,
	eCOMMON_CMD_CAT_INFO,
	eCOMMON_CMD_SHOW_HISCMD,
	eCOMMON_CMD_REBOOT,
	eCOMMON_CMD_CLEAR_SCREEN,
	eCOMMON_CMD_HELP,
    eCOMMON_CMD_MAX
} common_cmd_e;

typedef void (*cmd_func_t)(void *ptr, u16 len);

typedef struct {
	u8 cmd;
	u8 param;
	u16 resv;
	char *pcmd;
	cmd_func_t func;
} common_cmd_t;


//static const common_cmd_t _common_cmd[eCOMMON_CMD_MAX];
static common_cmd_t _common_cmd[eCOMMON_CMD_MAX];



#define RESERVE 	0

#define CMD_BUF_LEN 	160


static u8 cmd[CMD_BUF_LEN] = {0};
static u16 cmd_idx = 0;


#define MAX_HIS_CMD_CNT 8 // TODO: 必须设置为2的幂次方

typedef struct {
	s32 index;
	char buf[CMD_BUF_LEN];
} his_cmd_t;

his_cmd_t his_cmd[MAX_HIS_CMD_CNT] = {0};
u32 irr_index = 0; // irreversible 不可逆的，记录一条，index++，永无止境
u32 up_show_idx = 0;
u32 down_show_idx = 0;
u16 his_cmd_idx = 0;
u32 temp_idx = 0;



enum {
	eCMD_NO_PARAM = 0,
	eCMD_WITH_PARAM = ! eCMD_NO_PARAM
};



/* +------------------------------------------+ *
 * |	             打印界面                 | *
 * +------------------------------------------+ */
void print_welcome(void)
{
	printf(CLEAR_SCREEN);
	printf(ORIGINAL_POINT);
	
	printf(PCLR_DFT);

	printf("+-----------------------------------+"NEW_LINE);
    printf("|          RF433 Pre_study          |"NEW_LINE);
	printf("+-----------------------------------+"NEW_LINE);
	printf(PCLR_DFT);
}


static void do_clear_screen(void *ptr, u16 len)
{
	printf(CLEAR_SCREEN);
	printf(ORIGINAL_POINT);
}



void test_delay(void)
{
	u32 i = 2000000;

	while (i--);
}






static void do_sys_reboot(void *ptr, u16 len)
{
	printf(NEW_LINE"Will Reboot..."NEW_LINE);
	
	flush_tx_data();
	
	test_delay();

	//软件复位
    NVIC_SystemReset();
    while (1);
}







static u8 *ctrl_led_cmd[eCOLOR_MAX] = {
	"OFF",
	"R",
	"G",
	"Y",
	"B",
	"P",
	"LB",
	"W"
};


static void ctrl_led_usage(void)
{
	u8 i;
	
	printf("\r\n\033[36m\033[1mUsage:\r\n");

	for (i = 0; i < eCOLOR_MAX; i++) {
		printf("led_ctrl %s\r\n", ctrl_led_cmd[i]);
	}

	printf("\r\n\033[0m");	
}

static void do_ctrl_led(void *ptr, u16 len)
{
	int i;

	if ((NULL == ptr) || ( ! len )) {
		goto out;
	}

	//printf("%s\r\n", (char *)ptr);
	//printf("len = %u\r\n", len);

	for (i = 0; i < eCOLOR_MAX; i++) {
		if ( ! strncmp((const char *)ctrl_led_cmd[i], (const char *)ptr, len) ) {
			led_ctrl((color_e)i);
			return;
		}
	}

out:
	ctrl_led_usage();
}


static void do_help(void *ptr, u16 len)
{
	int i;
	
	printf(NEW_LINE"---------cmd---------"NEW_LINE);
	for (i = 0; i < eCOMMON_CMD_MAX; i++) {
		if (_common_cmd[i].param == eCMD_WITH_PARAM) {
			printf(PCLR_PUR_B);
		}
		else if (_common_cmd[i].param == eCMD_NO_PARAM) {
			printf(PCLR_WHI_B);
		}
		//printf("<%02u> %s"NEW_LINE, i, _common_cmd[i].pcmd);
		printf("%s"NEW_LINE, _common_cmd[i].pcmd);
	}
	printf(PCLR_DFT"---------------------"NEW_LINE);
}



static void do_show_his_cmd(void *ptr, u16 len)
{
	u32 start;
	u32 i;

	//u32 max_cnt = __MIN(irr_index, MAX_HIS_CMD_CNT);

	//printf("irr_index = %u\r\n", irr_index);

	if (irr_index > MAX_HIS_CMD_CNT) {
		start = irr_index - MAX_HIS_CMD_CNT;
		//max_cnt = MAX_HIS_CMD_CNT;
	}
	else {
		start = 0;
		//max_cnt = irr_index;
	}

	for (i = start ; i < irr_index; i++) {
		printf("%3u %s"NEW_LINE, i - start + 1, his_cmd[i & (MAX_HIS_CMD_CNT - 1)].buf);
	}

}

//static const common_cmd_t _common_cmd[eCOMMON_CMD_MAX] = {
static common_cmd_t _common_cmd[eCOMMON_CMD_MAX] = {
	{eCOMMON_CMD_CTRL_LED, eCMD_WITH_PARAM, RESERVE, "led_ctrl", do_ctrl_led},
	{eCOMMON_CMD_START_RELAY, eCMD_WITH_PARAM, RESERVE, "start_relay", do_rf_start_relay},

	//{eCOMMON_CMD_TX_ONE_PKT, eCMD_NO_PARAM, RESERVE, "tx_one_pkt", do_tx_one_pkt},
	{eCOMMON_CMD_START_TX, eCMD_NO_PARAM, RESERVE, "tx_start", do_rf_start_tx},
	{eCOMMON_CMD_RF_START_RX, eCMD_NO_PARAM, RESERVE, "rx_start", do_rf_start_rx},
	{eCOMMON_CMD_RF_END_RX, eCMD_NO_PARAM, RESERVE, "end_rx", do_rf_end_rx},	
	{eCOMMON_CMD_RF_GW_TEST, eCMD_NO_PARAM, RESERVE, "gw_test", do_rf_start_gw_test},	
	{eCOMMON_CMD_RF_DEBUG_LEN, eCMD_WITH_PARAM, RESERVE, "dbg_len", dbg_len},
	{eCOMMON_CMD_RF_SET_CH, eCMD_WITH_PARAM, RESERVE, "set_ch", set_ch},
	{eCOMMON_CMD_RF_TX_COUNT, eCMD_WITH_PARAM, RESERVE, "set_count", set_tx_count},
	{eCOMMON_CMD_RF_TX_SPACE, eCMD_WITH_PARAM, RESERVE, "set_space", set_tx_space},
	{eCOMMON_CMD_RF_TX_LEN, eCMD_WITH_PARAM, RESERVE, "set_tx_len", set_tx_len},
	{eCOMMON_CMD_RF_RATE, eCMD_WITH_PARAM, RESERVE, "rf_cfg", do_rf_cfg},
	{eCOMMON_CMD_CAT_INFO, eCMD_NO_PARAM, RESERVE, "cat_info", cat_info},
	{eCOMMON_CMD_SHOW_HISCMD, eCMD_NO_PARAM, RESERVE, "hiscmd", do_show_his_cmd},
	{eCOMMON_CMD_CLEAR_SCREEN, eCMD_NO_PARAM, RESERVE, "CLEAR_Screen", do_clear_screen},
	{eCOMMON_CMD_REBOOT, eCMD_NO_PARAM, RESERVE, "sys_reboot", do_sys_reboot},
	{eCOMMON_CMD_HELP, eCMD_NO_PARAM, RESERVE, "help", do_help},
};

void record_cmd(u8 *cmd)
{
	strcpy(his_cmd[irr_index & (MAX_HIS_CMD_CNT - 1)].buf, (const char *)cmd);
	down_show_idx = irr_index;
	irr_index++;
	up_show_idx = irr_index;
}


/* +------------------------------------------+ *
 * |	     	 与用户交互界面      		  | *
 * +------------------------------------------+ */
void common_with_user(int event, void *data, int data_len)
{	
	u8 *rbuf = (u8 *)data;
	
	u16 read_cnt = 0;
	u16 read_idx = 0;
	u16 x = 0, y = 0;
	u16 n_cmd = 0;
	u8 j = 0, k = 0;
	u8 n_space = 0;
	u8 flag = 0;
	u16 which_cmd = eCOMMON_CMD_MAX;
	u8 const_cmd_len; // 固有CMD长度
	u8 input_cmd_len; // 输入CMD长度(不含前面的空格)
	u8 *ptr = NULL;
	u16 len;
	u8 find;
	u16 idx;
	u16 space_cnt;
	u16 cmd_l;
	u16 ll;

	//printf("rbuf[0] = %c, data_len = %d\r\n", rbuf[0], data_len);

	read_cnt = data_len;

	if (NULL == rbuf) {
		return;
	}

	if (is_txing()) {
		return;
	}

	if (is_relaying()) {
		//return;
	}
	
	switch (data_len) {
	case 0:
		return;

	case 1:
		if (BACKSPACE == rbuf[0]) {
			if (cmd_idx) {
				printf("\b \b");
				cmd_idx--;
			}		
			return;
			
		}
		else if ((TAB_KEY == rbuf[0]) ) {
			while (cmd[n_space] == ' ') {
				n_space++;
			}
			
			for (j = 0; j < eCOMMON_CMD_MAX; j++) {
				if ( ! strncmp((const char *)(cmd + n_space), (const char *)_common_cmd[j].pcmd, cmd_idx - n_space)) {
					n_cmd++;
					which_cmd = j;
				}
			}

			if (n_cmd) {
				if (1 == n_cmd) {
					if (which_cmd != eCOMMON_CMD_MAX) {
						strcpy((char *)&cmd[n_space], (const char *)_common_cmd[which_cmd].pcmd);
						printf("%s", _common_cmd[which_cmd].pcmd + cmd_idx - n_space);
						cmd_idx += strlen(_common_cmd[which_cmd].pcmd) - cmd_idx + n_space;
					}
				}
				else {
					for (j = 0; j < eCOMMON_CMD_MAX; j++) {
						if ( ! strncmp((const char *)(cmd + n_space), (const char *)_common_cmd[j].pcmd, cmd_idx - n_space)) {
							if ( ! flag ) {
								printf(NEW_LINE);
								flag = 1;
							}
							
							cmd_l = strlen(_common_cmd[j].pcmd);
							
							if (cmd_l < 16) {
								space_cnt = 16 - cmd_l;
							} 
							else if (cmd_l < 32) {
								space_cnt = 32 - cmd_l;
							} 
							else if (cmd_l < 64) {
								space_cnt = 64 - cmd_l;
							}
							
							printf("%s", _common_cmd[j].pcmd);
							for (ll = 0; ll < space_cnt; ll++) {
								printf(" ");
							}
							
							if (y % 5 == 4)
								printf(NEW_LINE);
							y++;
						}
					}

					cmd[cmd_idx] = '\0';
					printf(NEW_LINE COMMON_CMD_INPUT"%s", cmd);
				}
			}
		
			return;	
		} 
		break;

	case 3:
		if (0x1b == rbuf[0]) {
			if (0x5b == rbuf[1]) {
				switch(rbuf[2]) {
					/* 上下键 */
					// TODO: 有兴趣再写历史命令
					case 0x41:
					case 0x42:
						if (0 == irr_index) { // 没有历史命令
							return;
						}

						if (rbuf[2] == 0x41) { // 上键(查询上一条历史记录)
							/* 先判断是否写满了 */
							if (irr_index < MAX_HIS_CMD_CNT) { // 没写满
								if (0 == up_show_idx) { // 且当时已经指向最初那条了
									up_show_idx = irr_index;
								}
							}
							
							up_show_idx--;
							down_show_idx = up_show_idx; // 保持同步	
							idx = up_show_idx;		
						}
						else if (rbuf[2] == 0x42) { // 下键(查询下一条历史记录)
							/* 先判断是否写满了 */
							if (irr_index < MAX_HIS_CMD_CNT) { // 没写满
								if ((down_show_idx == (irr_index - 1))) { 
									down_show_idx = 0;
								}
								else {
									down_show_idx++;
								}
							}
							else {
								down_show_idx++;
							}

							up_show_idx = down_show_idx; // 保持同步	
							idx = down_show_idx;		
						}

						idx &= (MAX_HIS_CMD_CNT - 1);
	
						strcpy((char *)cmd, (const char *)his_cmd[idx].buf);
						printf("\r\033[K");
						printf(COMMON_CMD_INPUT"%s", cmd);
						cmd_idx = strlen(his_cmd[idx].buf);
						return;

					/* 左右键 */
					// TODO: 这个难度有点大，暂时不实现了
					case 0x43:
					case 0x44:
						return;
						
					default:
						return;
				}
			}
		}
		break;

	default:
		break;
	}

	for (read_idx = 0; read_idx < read_cnt; read_idx++) {
		if (cmd_idx < (sizeof(cmd) - 1)) {
			if (BACKSPACE == rbuf[read_idx]) {
				if (cmd_idx) {
					printf("\b \b");
					cmd_idx--;
				}
				continue;
			}
			else {
				//回显
				if ((rbuf[read_idx] == '\r') || 
					(rbuf[read_idx] == '\n') ||
					((rbuf[read_idx] >= 32) && (rbuf[read_idx] <= 127))) { // 去掉不可见字符
					printf("%c", rbuf[read_idx]);
					if ('\r' == rbuf[read_idx]) {
						printf("\n");
					}
				}
			}
			
			if ((rbuf[read_idx] == '\r') || (rbuf[read_idx] == '\n')) {
				cmd[cmd_idx] = '\0';
				n_space = 0;
				while (cmd[n_space] == ' ') {
					n_space++;
				}
				
				if (cmd_idx - n_space) {
					record_cmd(cmd + n_space);
					#if 0
					if (1 == (cmd_idx - n_space)) {
						switch (cmd[n_space]) {
						case '1':
							printf("$[%s]\r\n", (char *)_common_cmd[0].pcmd);
							cmd_idx = 0;
							LED_PIN_R = 1;
							return;
						case '2':
							printf("$[%s]\r\n", (char *)_common_cmd[1].pcmd);
							cmd_idx = 0;
							LED_PIN_R = 0;
							return;

						default:
							
							break;
						}
					}
					#endif
										
					for (k = 0; k < eCOMMON_CMD_MAX; k++) {
						const_cmd_len = strlen((const char *)_common_cmd[k].pcmd);
						if ( ! strncmp((const char *)(cmd + n_space), (const char *)_common_cmd[k].pcmd, const_cmd_len) ) {
							input_cmd_len = strlen((const char *)(cmd + n_space));
							
							if (const_cmd_len == input_cmd_len) { // (除去前面的空格)刚好和命令一样长
								break;
							} 
							else if (const_cmd_len < input_cmd_len) {
								x = 0;
								while (cmd[cmd_idx - x - 1] == ' ') {
									x++;
								}

								if (x == (input_cmd_len - const_cmd_len)) { // 命令后全是空格
									if (_common_cmd[k].param == eCMD_NO_PARAM) { // 不带参,可以执行
										break;
									}
									else if (_common_cmd[k].param == eCMD_WITH_PARAM) { //带参数
										ptr = NULL;
										len = 0;
										break;
									}
								}

								/* 以下为 命令后不全是空格 */
								if (_common_cmd[k].param == eCMD_NO_PARAM) { // 不带参
									continue; // 说明不匹配
								}
								else if (_common_cmd[k].param == eCMD_WITH_PARAM) { //带参数
									if (cmd[n_space + const_cmd_len] != ' ') { // 命令后面不是空格
										continue; // 说明不匹配
									}
										
									for (x = n_space + const_cmd_len; x < cmd_idx; x++) {
										ptr = &cmd[x];
										if (*ptr != ' ') { // 找到参数位置
											len = cmd_idx - x;
											find = 1;
											break;
										}
									}

									if (find) {
										break;
									}
								}
								
							}
							else {
								printf("!!!!!!!!!!!!NO Possible Reach Here!!!!!!!!!!!!!"NEW_LINE);
								cmd_idx = 0;	
								return;
							}
						}
					}

					if (k >= eCOMMON_CMD_MAX) {
						//printf("#######%s#######\r\n", (char *)&cmd[n_space]);
						printf("<!>Error: %s: command not found"NEW_LINE, (char *)&cmd[n_space]);
					}
					else {
						
						if (_common_cmd[k].param == eCMD_NO_PARAM) {
							_common_cmd[k].func(NULL, 0);
						}
						else if (_common_cmd[k].param == eCMD_WITH_PARAM) {
							_common_cmd[k].func(ptr, len);
						}
					}
	
					cmd_idx = 0;				
					
					printf(COMMON_CMD_INPUT);
					
				}
				else {
				
					printf(COMMON_CMD_INPUT);
				}
				
			} 
			else {
				cmd[cmd_idx++] = rbuf[read_idx];
			}
		}
		else {
			printf(NEW_LINE COMMON_CMD_INPUT);
			cmd_idx = 0;
		}

	}

}



void common_with_user_init(void)
{
	event_add(eEVENT_USART1_READ_DATA, &common_with_user_node, common_with_user);
	
	print_welcome();
	memset(his_cmd, 0, sizeof(his_cmd));
	printf(COMMON_CMD_INPUT);
	
}

void common_with_user_fini(void)
{
	event_del(eEVENT_USART2_READ_DATA, common_with_user);
}

void common_tmn_into(void)
{
	led_ctrl(eCOLOR_BLUE);
	
	common_with_user_init();
}

void common_tmn_loop(void)
{
	usart1_Rx_proc();

	//rx_proc();
}

void common_tmn_quit(void)
{
	common_with_user_fini();
	usart1_Rx_disable();

	led_ctrl(eCOLOR_OFF);
}


/* -------------------------------- 分割线 -------------------------------- */


void pkt_dump(char *color, u8 *buf, u16 len)
{
    u16 x;

	printf(color);	

    for (x = 0; x < len; x++) {
        //if (x % 32 == 0)
        //    printf("[0x%02X]: ", x);
		
        printf("%02X ", buf[x]);
		
        if (x % 32 == 31) {
			if (x != (len - 1)) {
            	printf(NEW_LINE);
			}
        }
    }
    printf(PCLR_DFT NEW_LINE);
}


#define PRT_COLOR_RED 			31
#define PRT_COLOR_GREEN 		32
#define PRT_COLOR_YELLOW 		33
#define PRT_COLOR_BLUE	 		34
#define PRT_COLOR_PURPLE 		35
#define PRT_COLOR_LIGHT_GREEN 	36
#define PRT_COLOR_WHITE 		37

/* 
 * 	     	    UI 状态机     		    
 */
static ui_t ui[eLP_MAX] = {
	//{eLP_IDLE, start_into, start_loop, start_quit},
	{eLP_IDLE, NULL, NULL, NULL},
	{eLP_COMMON_TERMINAL, common_tmn_into, common_tmn_loop, common_tmn_quit},	
};



void ui_init(void)
{
	LOOP_STAT_SHIFT(eLP_COMMON_TERMINAL);
}


void ui_loop(void)
{
	if (ui[loop_stat].doing) {
		ui[loop_stat].doing();
	}
}





/* END OF FILE */

