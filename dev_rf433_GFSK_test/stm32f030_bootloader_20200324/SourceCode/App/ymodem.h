/****************************************Copyright (c)****************************************
 																							
                             Galaxywind Network System Co., Ltd.						    
                                      												
                                  http://www.galaxywind.com								
 																						
 Author:	Y
 Date  : 	2017-12-09
 *********************************************************************************************/ 

/*
	http://blog.csdn.net/IDOshi201109/article/details/50901615

	SENDER：发送方。（文件名：fileName.bin） 
	RECEIVER:接收方。 
	具体握手的步骤如下： 
	1、接收方发送一个字符‘C’,也就是十六进制‘43’。代表接收方已经处于接收数据的状态。 
	2、发送方接收到‘C’之后，发送头帧数据包，内容如下： 
	SOH 00 FF fileName.bin NULL[116] CRC CRC 
	数据包内容解释： 
	2-1 SOH（第1字节）：表示本数据区大小有128字节。（STX表示本数据包数据区大小1024字节）。 
	2-2 00 （第2字节）：数据块编号。 第一包为00，第二包为01，此后依次累加。FF后，继续从00循环。 
	2-3 FF （第3字节）：数据块编号的反码。 编号00—FF，01—FE，此后依次类推。 
	2-4 fileName.bin NULL[116]：数据区。128字节。 fileName.bin是文件名，超级终端下，在文件名后面还有文件大小。 数据区不足128字节的，用0x00补齐。 
	2-5 CRC校验（最后2个字节）：16位CRC校验，高位字节在前，地位字节在后。（注意：只有数据区参与了CRC校验，不包含头、编码、编码反码）。 
	3、接收方收到数据包后，发送ACK正确应答，然后发送一个字符‘C’。 
	4、发送方收到‘C’后，开始发送第二帧数据。第二帧数据存放的是第一包数据。 
	5、接收方收好数据包后，发送ACK正确应答，然后等待下一包数据传送完毕，继续ACK应答。（循环） 
	6、数据传输完毕后，发送方第一次发EOT，第一次接收方以NAK应答，进行二次确认。 
	7、发送方收到NAK后，第二次发EOT。接收方第二次收到结束符，依次以ACK和C做应答。 
	8、发送方收到ACK和C之后，发送结束符—>SOH 00 FF 00…00[128个00] CRCH CRCL。 
	9、接收方收到结束符之后，以ACK做应答，然后通信正式结束。
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


/* YMODEM USART RX FIFO 大小 */
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
	u8 confirm; 	//确认次数    
	u8 frame_sn; 	//数据包编号
	u8 error_cnt; 	//错误包次数

	vu8 recv_state;
	volatile bool need_analysis; // 需要外循环解析
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

