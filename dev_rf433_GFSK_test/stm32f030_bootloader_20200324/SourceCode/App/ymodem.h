/****************************************Copyright (c)****************************************
 																							
                             Galaxywind Network System Co., Ltd.						    
                                      												
                                  http://www.galaxywind.com								
 																						
 Author:	Y
 Date  : 	2017-12-09
 *********************************************************************************************/ 

/*
	http://blog.csdn.net/IDOshi201109/article/details/50901615

	SENDER�����ͷ������ļ�����fileName.bin�� 
	RECEIVER:���շ��� 
	�������ֵĲ������£� 
	1�����շ�����һ���ַ���C��,Ҳ����ʮ�����ơ�43����������շ��Ѿ����ڽ������ݵ�״̬�� 
	2�����ͷ����յ���C��֮�󣬷���ͷ֡���ݰ����������£� 
	SOH 00 FF fileName.bin NULL[116] CRC CRC 
	���ݰ����ݽ��ͣ� 
	2-1 SOH����1�ֽڣ�����ʾ����������С��128�ֽڡ���STX��ʾ�����ݰ���������С1024�ֽڣ��� 
	2-2 00 ����2�ֽڣ������ݿ��š� ��һ��Ϊ00���ڶ���Ϊ01���˺������ۼӡ�FF�󣬼�����00ѭ���� 
	2-3 FF ����3�ֽڣ������ݿ��ŵķ��롣 ���00��FF��01��FE���˺��������ơ� 
	2-4 fileName.bin NULL[116]����������128�ֽڡ� fileName.bin���ļ����������ն��£����ļ������滹���ļ���С�� ����������128�ֽڵģ���0x00���롣 
	2-5 CRCУ�飨���2���ֽڣ���16λCRCУ�飬��λ�ֽ���ǰ����λ�ֽ��ں󡣣�ע�⣺ֻ��������������CRCУ�飬������ͷ�����롢���뷴�룩�� 
	3�����շ��յ����ݰ��󣬷���ACK��ȷӦ��Ȼ����һ���ַ���C���� 
	4�����ͷ��յ���C���󣬿�ʼ���͵ڶ�֡���ݡ��ڶ�֡���ݴ�ŵ��ǵ�һ�����ݡ� 
	5�����շ��պ����ݰ��󣬷���ACK��ȷӦ��Ȼ��ȴ���һ�����ݴ�����ϣ�����ACKӦ�𡣣�ѭ���� 
	6�����ݴ�����Ϻ󣬷��ͷ���һ�η�EOT����һ�ν��շ���NAKӦ�𣬽��ж���ȷ�ϡ� 
	7�����ͷ��յ�NAK�󣬵ڶ��η�EOT�����շ��ڶ����յ���������������ACK��C��Ӧ�� 
	8�����ͷ��յ�ACK��C֮�󣬷��ͽ�������>SOH 00 FF 00��00[128��00] CRCH CRCL�� 
	9�����շ��յ�������֮����ACK��Ӧ��Ȼ��ͨ����ʽ������
 */


#ifndef _YMODEM_H_
#define _YMODEM_H_

#include "sys.h"
#include "usart_comm.h"



#define INDEX_HEAD 				0
#define INDEX_SN 				1
#define INDEX_SN_COMP 			2
#define INDEX_DATA_START 		3


#define PACKET_TRAILER          (2)
#define PACKET_OVERHEAD         (INDEX_DATA_START + PACKET_TRAILER)
#define PACKET_SIZE             (128)
#define PACKET_1K_SIZE          (1024)

#define FILE_NAME_LENGTH        (128)
#define FILE_SIZE_LENGTH        (8)


/* YMODEM USART RX FIFO ��С */
#define YMODEM_BUFF_SIZE			1032


#define NUL                     (0x00)  /* NULL \0 */
#define SOH                     (0x01)  /* start of 128-byte data packet */
#define STX                     (0x02)  /* start of 1024-byte data packet */
#define ETX                     (0x03)  /* end of Text */
#define EOT                     (0x04)  /* end of transmission */
#define ACK                     (0x06)  /* acknowledge */
#define NAK                     (0x15)  /* negative acknowledge */
#define CA                      (0x18)  /* two of these in succession aborts transfer */
#define CRC16                   (0x43)  /* 'C' == 0x43, request 16-bit CRC */

#define ABORT1                  (0x41)  /* 'A' == 0x41, abort by user */
#define ABORT2                  (0x61)  /* 'a' == 0x61, abort by user */

#define NAK_TIMEOUT             (200)   // 1s
#define MAX_ERRORS              (5)

typedef enum {
    eYM_PROC_INIT = 0,   
    eYM_PROC_RECV_HEAD,
    eYM_PROC_RECV_DATA,
} ymodem_proc_stat_e;


typedef enum {
	eYM_RECV_HEAD = 0,	
	eYM_RECV_SN,
	eYM_RECV_SN_COMPLEMENT,
	eYM_RECV_DATA
} ymodem_recv_stat_e;



typedef struct {
	u32 start_addr;
	u32 m_ProgramAddr;
	u32 max_size;

	vu8 proc_stat;
	u8 confirm; 	//ȷ�ϴ���    
	u8 frame_sn; 	//���ݰ����
	u8 error_cnt; 	//���������

	vu8 recv_state;
	volatile bool need_analysis; // ��Ҫ��ѭ������
	u16 rlen;
	
	u16 idx;
	u8 buf[YMODEM_BUFF_SIZE];	
} ymodem_manager_t;


enum {
	eIMAGE_CHECK_OK = 0,
	eIMAGE_SIZE_ERR1 = 1,
	eIMAGE_SIZE_ERR2 = 2,
	eIMAGE_SIZE_ERR3 = 3,
	eIMAGE_SIZE_ERR100 = 100,
	eIMAGE_NOT_CHECK = 101,
};


#define	FILE_MAGIC	"FiLe"

#define	FILE_MAGIC_LEN 4

typedef struct {
	u8 name[FILE_NAME_LENGTH];
	u32 size;
	u8 magic[FILE_MAGIC_LEN];
} image_file_t;

extern image_file_t image_file;
extern ymodem_manager_t ym_mgr;

void Ymodem_uart_isr(int event, void *data, int data_len);

void Ymodem_proc(u32 now_time);
void ymodem_init(u32 start_time);

u8 check_app(u8 *app, u32 size, u8 crc[2], u16 *psize);

#endif /* #ifndef _YMODEM_H_ */


/* END OF FILE */

