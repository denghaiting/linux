/****************************************Copyright (c)****************************************
 																							
                             Galaxywind Network System Co., Ltd.						    
                                      												
                                  http://www.galaxywind.com								
 																						
 Author:	Y
 Date  : 	2017-11-22
 *********************************************************************************************/ 
#include "ymodem.h"
#include "bsp_flash.h"







ymodem_manager_t ym_mgr;



/* ��ȡ�ļ����ʹ�С�� */


image_file_t image_file;


#define	MAGIC1	"tAIl"
#define	MAGIC2	"TaiL"

typedef struct {
	u8 magic1[4];
	u8 size[2];
	u8 crc[2];
	u8 magic2[4];
} tail_t;


#define MIN(a, b)					((a) <= (b) ? (a) : (b))
#define IS_TXT_0_9(c)  				(((c) >= '0') && ((c) <= '9'))
#define IS_TXT_A_F(c)  				(((c) >= 'A') && ((c) <= 'F'))
#define CONVERTDEC(c)  				((c) - '0')




void my_delay(void)
{
	u32 i = 2000000;

	while (i--);
}



#define ADDRESSING_ONE_BYTE(addr)			(u8)(*((vu8 *)(addr)))


void page_cat(u32 addr)
{
	u16 x;
	u8 byte;

    printf(".\r\n");
    for (x = 0; x < FLASH_PAGE_SIZE; x++) {
		byte = ADDRESSING_ONE_BYTE(addr + x);
		
        if ((x & 15) == 0) {
			printf("0x%02X : ", x + addr);
        }
				
		printf("%02X ", byte);
		
        if ((x & 15) == 15) {
			if (x != (FLASH_PAGE_SIZE - 1)) {
            	printf("\r\n");
			}
        }
    }
	
   printf("\r\n");
}


void reboot(void)
{
	printf("\r\nWill reboot...\r\n");

	my_delay();

	//�����λ
    NVIC_SystemReset();
    while (1);
}



/* +------------------------------------------+ *
 * |	         ��ȡ�ļ����ʹ�С             | *
 * +------------------------------------------+ */
u32 extract_file_info(u8 *src, u16 src_len, u8 *pname)
{
	u8 file_size_buf[8];
	u8 *psize;
	u16 name_len, size_len;
	u16 name_idx, size_idx;
	u32 ret;
	u8 i;

	if ((NULL == src) || (NULL == pname)) {
		return 0;
	}

	name_len = MIN(128, src_len);

	for (name_idx = 0; name_idx < name_len; ) {
		pname[name_idx] = src[name_idx];
		
		++name_idx; 

		if ('\0' == pname[name_idx - 1]) { // Name��β��
			break;
		}

    }
	
	if ('\0' != pname[name_idx - 1]) { // û�ҵ� '\0'
		pname[name_idx - 1] = '\0'; // ����

		return 0; // ����û���ļ���С��Ϣ��
	}

	size_len = name_len - name_idx;
	if (size_len < 2) {
		return 0; // ����û���ļ���С��Ϣ�� (������1������ + 1���ո�)
	}


	size_len = MIN(sizeof(file_size_buf), size_len);

	psize = &src[name_idx];

	for (size_idx = 0; size_idx < size_len; ) {
		if (' ' == psize[size_idx]) {
			break;
		}
		
		file_size_buf[size_idx] = psize[size_idx];

		size_idx++;
	}

	ret = 0;

	for (i = 0; i < size_idx; i++) {
		if ( ! IS_TXT_0_9(file_size_buf[i]) ) {
			return 0;
		}

		//ret = (ret * 10) + CONVERTDEC(file_size_buf[i]); // ��������д���
		
		ret = (ret << 3) + (ret << 1) + CONVERTDEC(file_size_buf[i]);
	}
		
	return ret;
}



/* +------------------------------------------+ *
 * |	     	  У�龵��CRC�㷨    		  | *
 * +------------------------------------------+ */
static u16 __YModemCrc(u16 crc, u8 *pData, u16 sLen)
{  
   u16 i = 0;
   
   while (sLen--)  //len����Ҫ����ĳ���
   {
       crc = crc^(int)(*pData++) << 8; //    
       for (i=8; i!=0; i--) 
       {
           if (crc & 0x8000)   
               crc = crc << 1 ^ 0x1021;    
           else
               crc = crc << 1;
       }    
   }

   return crc;
}


/*
 * return:
 * 	0: ok
 * 	other: failed
 */
u8 check_app(u8 *app, u32 size, u8 crc[2], u16 *psize)
{
	u16 c = 0;
	u32 i;
	tail_t *t;

	if (size < sizeof(tail_t))	
		return eIMAGE_SIZE_ERR1;
	
	size = size - sizeof(tail_t) + 1;
	
	for (i = 0; i < size; i++, app++) {
		if (*app != 't') {
			c = __YModemCrc(c, app, 1);
			continue;
		}

		t = (tail_t *)app;
		if ( ! memcmp(t->magic1, MAGIC1, 4) && ! memcmp(t->magic2, MAGIC2, 4)) {
			if (((t->size[0] << 8) | t->size[1]) != i) {
				return eIMAGE_SIZE_ERR2;
			}
			
			if (t->crc[0] != ((c >> 8) & 0xFF) || t->crc[1] != (c & 0xFF)) {
				return eIMAGE_SIZE_ERR3;
			}
			
			/* ��ȡAPP��Ϣ */
			if (NULL != crc) {
				crc[0] = t->crc[0];
				crc[1] = t->crc[1];
			}
			
			if (NULL != psize) {
				*psize = (u16)(t->size[0] << 8) | t->size[1];
			}
			
			return eIMAGE_CHECK_OK;
		}
		
		c = __YModemCrc(c, app, 1);
	}

	return eIMAGE_SIZE_ERR100;
}





/* +------------------------------------------+ *
 * |	   Y-Modem �����жϴ���ص�����       | *
 * +------------------------------------------+ */
void Ymodem_uart_isr(int event, void *data, int data_len)
{
	u8 rbyte;
	
	/* ������ڴ����������ʱ���������� */
	if (ym_mgr.need_analysis) {
		return;
	}

	rbyte = *(u8 *)data;

	switch (ym_mgr.recv_state)  {
	case eYM_RECV_HEAD:
		if (CA == rbyte) {
			ym_mgr.buf[ym_mgr.idx++] = rbyte;
			if (ym_mgr.idx >= 2) {
				goto loop_analysis;
			}
			return;
		}
		
		if ((ETX == rbyte) || (EOT == rbyte)) {
			ym_mgr.buf[ym_mgr.idx++] = rbyte;
			goto loop_analysis;
		}

		if ((SOH != rbyte) && (STX != rbyte)) {
			return;
		}

		ym_mgr.rlen = (SOH == rbyte) ? PACKET_SIZE : PACKET_1K_SIZE;
		
		ym_mgr.buf[INDEX_HEAD] = rbyte;
		ym_mgr.recv_state = eYM_RECV_SN;
		break;

	case eYM_RECV_SN:
		ym_mgr.buf[INDEX_SN] = rbyte;
		ym_mgr.recv_state = eYM_RECV_SN_COMPLEMENT;	
		break;

	case eYM_RECV_SN_COMPLEMENT:
		ym_mgr.buf[INDEX_SN_COMP] = rbyte;
		ym_mgr.idx = INDEX_DATA_START;
		ym_mgr.recv_state = eYM_RECV_DATA;
		break;

	case eYM_RECV_DATA:
		ym_mgr.buf[ym_mgr.idx++] = rbyte;
		if (ym_mgr.idx >= (ym_mgr.rlen + 5)) {	// head, sn, sn_comp, crc1 crc2
			ym_mgr.recv_state = eYM_RECV_HEAD;
			goto loop_analysis;
		}
		
		break;

	default:
		break;
	}

	return;

loop_analysis:

	ym_mgr.need_analysis = TRUE;

}


u32 pre_time;

void ymodem_init(u32 start_time)
{		
	ym_mgr.proc_stat = eYM_PROC_RECV_HEAD; 
	ym_mgr.m_ProgramAddr = ym_mgr.start_addr;

	ym_mgr.need_analysis = FALSE;
		
	pre_time = start_time;
}



//���index��Ӧ��ʱ�䳬ʱ�����㲢����1��δ��ʱ����0
//#define is_timeout(count)    		((SYS_MS >= (count)) ? ((SYS_MS = 0) == 0) : 0)


bool is_timeout(u32 now_count, u32 count)
{	
	u32 res;
	
	res = now_count - pre_time;
	
	if (res >= count) {
		pre_time = now_count;
		return TRUE;
	}
	
	return FALSE;
}


/* +------------------------------------------+ *
 * | Ymodem ����һ���ַ� ���ж� �����������  | *
 * +------------------------------------------+ */
static void YmodemSendChar(unsigned char ch)
{
    PUT_CHAR(ch);
  
    if (ACK == ch) {         //�������������
        ym_mgr.error_cnt = 0;
    }
    else if (ch == NAK) {     //����������ۼ�
        ++ym_mgr.error_cnt;
    }
}



/* +------------------------------------------+ *
 * |  		   Ymodem crc���ֽ�Ч��   	      | *
 * +------------------------------------------+ */
static u16 Ymodem_crc(u8 *pData, u16 sLen)
{  
   u16 _CRC = 0;    
   u8 i;
   
   while (sLen--) {  // sLen����Ҫ����ĳ���
       _CRC = _CRC ^ (int)(*pData++) << 8;   
       for (i = 8; i; i--) {
           if (_CRC & 0x8000) {  
               _CRC = _CRC << 1 ^ 0x1021;  
           } else {
               _CRC = _CRC << 1;
           }
       }    
   }
   return _CRC;
}


#if 0
bool write_image(u32 write_addr, u8 *pdat, u16 total_len)
{
	u32 remain;
	u32 can_wrlen;
	u32 wrlen;
	
	for (remain = total_len; remain; ) {
		can_wrlen = FLASH_PAGE_SIZE - (u16)(write_addr & (FLASH_PAGE_SIZE - 1));
		wrlen = MIN(remain, can_wrlen);
		flash_wrte(write_addr, pdat, wrlen);
		pdat += wrlen;
		write_addr += wrlen;
		remain = total_len - wrlen;
	}
	
	return true;
}
#endif




/* +------------------------------------------+ *
 * |	           Ymodem ��ѭ��              | *
 * +------------------------------------------+ */
void Ymodem_proc(u32 now_time)
{
	u8 frame_data[1024];
 	u8 name_buf[FILE_NAME_LENGTH];
 	u32 fsize;
	u16 frame_len;            
    u16 ym_crc;
	u8 ret;

	/* �����Ƿ�ʱ�� */
    if (is_timeout(now_time, NAK_TIMEOUT)) {  
		
		if (eYM_PROC_RECV_HEAD == ym_mgr.proc_stat) { 
            YmodemSendChar(CRC16); 		//���� 'C'
			ym_mgr.confirm = 0;             
			ym_mgr.frame_sn = 0;            
			ym_mgr.error_cnt = 0;               
		}
		else if (eYM_PROC_RECV_DATA == ym_mgr.proc_stat) {
	        goto end;
		}
		/*
		else if (eYM_PROC_ERR == ym_mgr.proc_stat) {

			uart0_send_byte(NAK);
			uart0_send_byte(ABORT1);
			uart0_send_byte(ABORT2);
			uart0_send_byte(EOT);
		}
		*/
		
		return;
    }

		
	if ( ! ym_mgr.need_analysis ) {	
		return;
	}


	if ((EOT == ym_mgr.buf[INDEX_HEAD]) || 
		(ETX == ym_mgr.buf[INDEX_HEAD]) ||
		((CA == ym_mgr.buf[INDEX_HEAD]) && (CA == ym_mgr.buf[INDEX_HEAD + 1]))
	   ) {
	   
		if (ym_mgr.confirm) {
			ym_mgr.frame_sn = 0;  
			YmodemSendChar(ACK);       //��ȷӦ��
			if (eYM_PROC_RECV_DATA == ym_mgr.proc_stat) {
				ym_mgr.proc_stat = eYM_PROC_RECV_HEAD;
			}
			
		} else {       
			YmodemSendChar(NAK);       //����Ӧ�� ����ȷ��
		}
		
 		ym_mgr.confirm++;

		if (ym_mgr.error_cnt > MAX_ERRORS) {  //�������������
			goto end;
		}
		
		goto out2;
	}

	if ((SOH != ym_mgr.buf[INDEX_HEAD]) && (STX != ym_mgr.buf[INDEX_HEAD])) {
        YmodemSendChar(NAK);       //����Ӧ��
		if (ym_mgr.error_cnt > MAX_ERRORS) {  //�������������
			goto end;
		}
		
		goto out1;
	}

	frame_len = (SOH == ym_mgr.buf[INDEX_HEAD]) ? PACKET_SIZE : PACKET_1K_SIZE;

    if (ym_mgr.buf[INDEX_SN] != ym_mgr.frame_sn) {	//���ݱ�Ŵ���
		goto out1;
    }


    if ((ym_mgr.buf[INDEX_SN_COMP] + ym_mgr.frame_sn) != 0xFF) { //���ݱ�Ų������
		goto out1;
    }

    if (ym_mgr.idx != (frame_len + PACKET_OVERHEAD)) { //�����ܳ��ȴ���
		goto out1;
    }

    /* YModemֻЧ�����ݲ��� */
    ym_crc = ((u16)ym_mgr.buf[ym_mgr.idx - 2] << 8) | ym_mgr.buf[ym_mgr.idx - 1];
   
    if (ym_crc != Ymodem_crc(&ym_mgr.buf[INDEX_DATA_START], frame_len)) { //crcЧ�����
		goto out1;
    }

	/* ��֡���������� */
	memcpy(frame_data, &ym_mgr.buf[INDEX_DATA_START], frame_len);

	if (eYM_PROC_RECV_HEAD == ym_mgr.proc_stat) { 
        if (NUL == frame_data[0]) { // ����
        	goto end;
        }

		/* ��ʼ��׼������ */
		if (0 == ym_mgr.frame_sn) {			
			fsize = extract_file_info(frame_data, frame_len, name_buf);

			printf("fsize = %u\r\n", fsize);

			
			if ((0 == fsize) || (fsize > ym_mgr.max_size)) {
				goto end;
			}
			else {
				memcpy(image_file.name, name_buf, sizeof(image_file.name));
				image_file.size = fsize;
			}

			if (APP_IMAGE_START == ym_mgr.start_addr) {

				if (flash_erase(APP_IMAGE_START, APP_IMAGE_SIZE)) {
					goto end;
				}
			/*
			else if (LICENSE_ADDR == ym_mgr.start_addr) {
				ret = erase_one_page(LICENSE_ADDR);
				if ( ! ret ) {
					goto end;
				}
			*/
			}
			
			
    	}
		
        YmodemSendChar(ACK);               //��ȷӦ��
        YmodemSendChar(CRC16);             //���� 'C' 
        ym_mgr.proc_stat = eYM_PROC_RECV_DATA;

	}
	else if (eYM_PROC_RECV_DATA == ym_mgr.proc_stat) {

		flash_write(ym_mgr.m_ProgramAddr, frame_data, frame_len);
		
		ym_mgr.m_ProgramAddr += frame_len;

        YmodemSendChar(ACK);        //��ȷӦ��
	}

	ym_mgr.frame_sn++;                


out1:
		
	ym_mgr.confirm = 0;
	
out2:
	is_timeout(now_time, 0);                      //�峬ʱ������        
	ym_mgr.need_analysis = FALSE;
	ym_mgr.idx = 0;

	return;

end:
    PUT_CHAR(ACK);           //��ֹ
    PUT_CHAR(CA);            //��ֹ
    PUT_CHAR(CA);            //��ֹ

	image_file.name[FILE_NAME_LENGTH - 1] = '\0';
	printf("\r\nName: %s\r\n", image_file.name);
	
	printf("Size: %u\r\n", image_file.size);

	printf("program addr = 0x%02X\r\n", ym_mgr.m_ProgramAddr);

#if 0
	if (APP_IMAGE_START == ym_mgr.start_addr) {
		u32 i;
		u32 n = (image_file.size % FLASH_PAGE_SIZE) ? (image_file.size / FLASH_PAGE_SIZE + 1) : (image_file.size / FLASH_PAGE_SIZE);

		for (i = 0; i < n; i++) {
			page_cat(APP_IMAGE_START + (i << 10));
			FEED_DOG();
		}
	}

#endif

	ret = check_app((u8 *)APP_IMAGE_START, APP_IMAGE_SIZE, NULL, NULL);
	if (eIMAGE_CHECK_OK == ret) {

		printf("OK\r\n");

		flash_erase(BOOT_USE_PAGE0, FLASH_PAGE_SIZE);

		memcpy(image_file.magic, FILE_MAGIC, FILE_MAGIC_LEN);
		
		flash_write(BOOT_USE_PAGE0, (u8 *)&image_file, sizeof(image_file));
		
		//my_delay();

		//page_cat(BOOT_USE_PAGE0);
	}

	reboot();
}

/* END OF FILE */

