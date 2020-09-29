/****************************************Copyright (c)****************************************
 																							
                             Galaxywind Network System Co., Ltd.						    
                                      												
                                  http://www.galaxywind.com								
 																						
 Author:	Y
 Date  : 	2017-11-22
 *********************************************************************************************/ 
#include "bsp_flash.h"
#include "sys_config.h"


/* +------------------------------------------+ *
 * |  	       ÿ��д2�ֽڵĲ����ӿ�          | *
 * +------------------------------------------+ */
static u8 __flash_write(u32 address, u8 *p_data, u32 nbyte_want)
{
    u32 nbyte_write = 0;
	u32 addr = address;
	u8 *p = p_data;
	u16 temp;
    __IO FLASH_Status FS;

	FLASH_Unlock();
	
	while (nbyte_write < nbyte_want) {
		temp = p[0] & 0xFF;
		temp |= (p[1] << 8) & 0xFF00;
		if ( (FS = FLASH_ProgramHalfWord(addr, temp)) != FLASH_COMPLETE) {
			break;
		}
		
		nbyte_write += 2;
		p += 2;
		addr += 2;
	}

	FLASH_Lock();

	return (FLASH_COMPLETE == FS) ? 0 : 1;
}


/* +------------------------------------------+ *
 * |  	     ��ָ����ַ����ÿ��һ�ֽ�         | *
 * +------------------------------------------+ */
static void __flash_read(u32 address, u8 *p_data, u32 nbyte_want) 
{
    u32 nbyte_read = 0;

    while (nbyte_read < nbyte_want) {
        p_data[nbyte_read++] = *(vu8 *)address++;
    }
}


/* +------------------------------------------+ *
 * |  	       ���BUF�ǲ���ȫΪ0xFF          | *
 * +------------------------------------------+ */
static u8 __need_erase(u8 *buf, u32 size)
{
	u32 i = 0;
	
    for (i = 0; i < size; i++) { 
        if (buf[i] != 0xFF) {	// ��Ҫ���� 
			return 1; 
        }
    }

	return 0;
}


/* +------------------------------------------+ *
 * |  	     ���Ҫд���ַ�ĺϷ���           | *
 * +------------------------------------------+ */
static u8 __check_addr_ok(u32 addr)
{
	/* λ����ʼ��ַ֮�䣬���Ҳ���Ϊ���� */
	return ((addr >= USER_FLASH_START_ADDR) && (addr <= USER_FLASH_END_ADDR - 1) && ( ! (addr & 1) )) ? 1 : 0;
}


/* +------------------------------------------+ *
 * |  	        ������Ҫ������ҳ��            | *
 * +------------------------------------------+ */
static u32 __calc_flash_page_count(vu32 size)
{ 
	return (size % FLASH_PAGE_SIZE) ? ((size / FLASH_PAGE_SIZE) + 1) : (size / FLASH_PAGE_SIZE);
}


/* +------------------------------------------+ *
 * |  	            д�����ӿ�                | *
 * +------------------------------------------+ */
u8 flash_write(u32 addr, u8 *p_data, u32 size) 
{
    u32 sec_pos;		// �����׵�ַ
    u32 sec_offset;		// ƫ��
    u32 sec_remain; 	// ʣ��ռ�
    u32 nbyte_want;
    u32 nbyte_write = 0;
	u8 tmp_buf[FLASH_PAGE_SIZE];
	u8 *p = p_data;
	u32 i; 
	__IO FLASH_Status FS;

	/* ����ַ�Ϸ��� */
	if ( ! __check_addr_ok(addr) ) {
		return 3;
	}

	/* ÿ��д2���ֽڣ������ܴ�С����Ϊ���� */
	size = (size & 1) ? (size + 1) : size;

	/* ���д���С�Ƿ񳬹�FLASH���� */
	if (addr + size > USER_FLASH_END_ADDR) {
		return 2;
	}

    /* ͨ����ַ����������Ϣ */
    sec_pos = addr & (~(FLASH_PAGE_SIZE - 1));	// ������ַ 
    sec_offset = addr & (FLASH_PAGE_SIZE - 1); 	// �������ڵ�ƫ��
    sec_remain = FLASH_PAGE_SIZE - sec_offset; 	// ����ʣ��ռ��С 

	for ( ; ; ) {
		nbyte_want = (size < sec_remain) ? (size - nbyte_write) : sec_remain;
	    __flash_read(sec_pos, tmp_buf, FLASH_PAGE_SIZE); // ������������

		/* ����FLASH����������λ��Ϊ1ʱ����д������ҪУ�� */
	    if (__need_erase(&tmp_buf[sec_offset], sec_remain)) { //��Ҫ����
	    
			FLASH_Unlock();
	    	FS = FLASH_ErasePage(sec_pos); 
			FLASH_Lock();
			
			if (FS != FLASH_COMPLETE) {
				return 1;   
			}

			/* ��ƫ��λ�ø���Ҫд������� */
	        for (i = 0; i < nbyte_want; i++) {	 
	            tmp_buf[sec_offset + i] = p[i]; 
	        }
			
	        if (__flash_write(sec_pos, tmp_buf, FLASH_PAGE_SIZE)) {// д���������� 
				return 1;
	        }
	    } else {
	    	if (__flash_write(addr, p, nbyte_want)) { // д�Ѿ������˵�,ֱ��д������ʣ������
				return 1;
	    	}
	        
	    }

		nbyte_write += nbyte_want;

		if (nbyte_write == size) {
			return 0;
		}

		/* �����δд��,˵��Ҫд����һ������ */
		sec_pos += FLASH_PAGE_SIZE;		// ƫ�Ƶ���һ������
	    sec_offset = 0;					// ƫ��λ��Ϊ0 
	    sec_remain = FLASH_PAGE_SIZE;	// ʣ��ռ�Ϊһҳ��С
		addr += nbyte_write;			// д��ַƫ��
		p += nbyte_write;			// ����ָ��ƫ��

		/* ����ַ�Ϸ��� */
		if ( ! __check_addr_ok(addr) ) {
			return 1;
		}
	} 

}


/* +------------------------------------------+ *
 * |  	            �������ӿ�                | *
 * +------------------------------------------+ */
void flash_read(u32 addr, u8 *data_buf, u32 nbyte_want) 
{	
 	__flash_read(addr, data_buf, nbyte_want);
}



u8 flash_erase(u32 addr, u32 size) 
{
	u32 n_page = 0x0;
	u32 page_count = 0;

	if ((addr + size) > USER_FLASH_END_ADDR + 1) {
		return 2;
	}

	page_count = __calc_flash_page_count(size);

	FLASH_Unlock();
	
	for (n_page = 0; n_page < page_count; n_page++) {
	  	if (FLASH_COMPLETE != FLASH_ErasePage(addr + (FLASH_PAGE_SIZE * n_page))) {
			return 1;
	  	}
	}
	FLASH_Lock();

	return 0;
}





/* END OF FILE */

