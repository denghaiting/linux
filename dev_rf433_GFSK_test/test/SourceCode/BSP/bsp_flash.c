/****************************************Copyright (c)****************************************
 																							
                             Galaxywind Network System Co., Ltd.						    
                                      												
                                  http://www.galaxywind.com								
 																						
 Author:	Y
 Date  : 	2017-11-22
 *********************************************************************************************/ 
#include "bsp_flash.h"
#include "sys_config.h"


/* +------------------------------------------+ *
 * |  	       每次写2字节的操作接口          | *
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
 * |  	     从指定地址读，每次一字节         | *
 * +------------------------------------------+ */
static void __flash_read(u32 address, u8 *p_data, u32 nbyte_want) 
{
    u32 nbyte_read = 0;

    while (nbyte_read < nbyte_want) {
        p_data[nbyte_read++] = *(vu8 *)address++;
    }
}


/* +------------------------------------------+ *
 * |  	       检查BUF是不是全为0xFF          | *
 * +------------------------------------------+ */
static u8 __need_erase(u8 *buf, u32 size)
{
	u32 i = 0;
	
    for (i = 0; i < size; i++) { 
        if (buf[i] != 0xFF) {	// 需要擦除 
			return 1; 
        }
    }

	return 0;
}


/* +------------------------------------------+ *
 * |  	     检查要写入地址的合法性           | *
 * +------------------------------------------+ */
static u8 __check_addr_ok(u32 addr)
{
	/* 位于起始地址之间，并且不能为奇数 */
	return ((addr >= USER_FLASH_START_ADDR) && (addr <= USER_FLASH_END_ADDR - 1) && ( ! (addr & 1) )) ? 1 : 0;
}


/* +------------------------------------------+ *
 * |  	        计算所要擦除的页数            | *
 * +------------------------------------------+ */
static u32 __calc_flash_page_count(vu32 size)
{ 
	return (size % FLASH_PAGE_SIZE) ? ((size / FLASH_PAGE_SIZE) + 1) : (size / FLASH_PAGE_SIZE);
}


/* +------------------------------------------+ *
 * |  	            写操作接口                | *
 * +------------------------------------------+ */
u8 flash_write(u32 addr, u8 *p_data, u32 size) 
{
    u32 sec_pos;		// 扇区首地址
    u32 sec_offset;		// 偏移
    u32 sec_remain; 	// 剩余空间
    u32 nbyte_want;
    u32 nbyte_write = 0;
	u8 tmp_buf[FLASH_PAGE_SIZE];
	u8 *p = p_data;
	u32 i; 
	__IO FLASH_Status FS;

	/* 检查地址合法性 */
	if ( ! __check_addr_ok(addr) ) {
		return 3;
	}

	/* 每次写2个字节，所以总大小不能为奇数 */
	size = (size & 1) ? (size + 1) : size;

	/* 检查写入大小是否超过FLASH上限 */
	if (addr + size > USER_FLASH_END_ADDR) {
		return 2;
	}

    /* 通过地址计算扇区信息 */
    sec_pos = addr & (~(FLASH_PAGE_SIZE - 1));	// 扇区地址 
    sec_offset = addr & (FLASH_PAGE_SIZE - 1); 	// 在扇区内的偏移
    sec_remain = FLASH_PAGE_SIZE - sec_offset; 	// 扇区剩余空间大小 

	for ( ; ; ) {
		nbyte_want = (size < sec_remain) ? (size - nbyte_write) : sec_remain;
	    __flash_read(sec_pos, tmp_buf, FLASH_PAGE_SIZE); // 读出整个扇区

		/* 仅当FLASH的扇区所有位都为1时才能写，所以要校验 */
	    if (__need_erase(&tmp_buf[sec_offset], sec_remain)) { //需要擦除
	    
			FLASH_Unlock();
	    	FS = FLASH_ErasePage(sec_pos); 
			FLASH_Lock();
			
			if (FS != FLASH_COMPLETE) {
				return 1;   
			}

			/* 从偏移位置复制要写入的数据 */
	        for (i = 0; i < nbyte_want; i++) {	 
	            tmp_buf[sec_offset + i] = p[i]; 
	        }
			
	        if (__flash_write(sec_pos, tmp_buf, FLASH_PAGE_SIZE)) {// 写入整个扇区 
				return 1;
	        }
	    } else {
	    	if (__flash_write(addr, p, nbyte_want)) { // 写已经擦除了的,直接写入扇区剩余区间
				return 1;
	    	}
	        
	    }

		nbyte_write += nbyte_want;

		if (nbyte_write == size) {
			return 0;
		}

		/* 如果还未写完,说明要写到下一个扇区 */
		sec_pos += FLASH_PAGE_SIZE;		// 偏移到下一个扇区
	    sec_offset = 0;					// 偏移位置为0 
	    sec_remain = FLASH_PAGE_SIZE;	// 剩余空间为一页大小
		addr += nbyte_write;			// 写地址偏移
		p += nbyte_write;			// 数据指针偏移

		/* 检查地址合法性 */
		if ( ! __check_addr_ok(addr) ) {
			return 1;
		}
	} 

}


/* +------------------------------------------+ *
 * |  	            读操作接口                | *
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

