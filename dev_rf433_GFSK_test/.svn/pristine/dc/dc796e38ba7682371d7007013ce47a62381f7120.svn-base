/****************************************Copyright (c)****************************************
 																							
                             Galaxywind Network System Co., Ltd.						    
                                      												
                                  http://www.galaxywind.com								
 																						
 Author:	Y
 Date  : 	2018-03-16
 *********************************************************************************************/ 
#include "fifo.h"
#include <string.h>


#if 0
#define NULL 	0ul

static void __memcpy(void *dst, const void *src, u32 count)  
{    
	u8 *pdst = (u8 *)dst;
	const u8 *psrc = (const u8 *)src;

	assert(dst != NULL);       
	assert(src != NULL);       
	
	assert(!(psrc <= pdst && pdst<psrc+count));
	assert(!(pdst <= psrc && psrc<pdst+count)); 
	
	while (count--) {
		*pdst = *psrc;          
		pdst++;          
		psrc++;      
	}  
}
#endif


/* +------------------------------------------+ *
 * |				清空缓冲区          	  | *
 * +------------------------------------------+ */
void clear_fifo(fifo_t *fifo)
{
	fifo->in = fifo->out = 0;
}



/* +------------------------------------------+ *
 * |		向缓冲区里放入数据的接口          | *
 * +------------------------------------------+ */
u32 fifo_put(fifo_t *fifo, const u8 *buffer, u32 len)
{
	u32 l;
	  
	len = __MIN(len, fifo->size - fifo->in + fifo->out);
	 
	/* first put the data starting from fifo->in to buffer end */
	l = __MIN(len, fifo->size - (fifo->in & (fifo->size - 1)));
	memcpy(fifo->buffer + (fifo->in & (fifo->size - 1)), buffer, l);
	 
	/* then put the rest (if any) at the beginning of the buffer */
	memcpy(fifo->buffer, buffer + l, len - l);

	fifo->in += len;

	return len;
}

/*
	len = min(len, fifo->size - fifo->in + fifo->out); 
	在 len 和 (fifo->size - fifo->in + fifo->out) 之间取一个较小的值赋给len。
	注意，当 (fifo->in == fifo->out+fifo->size) 时，表示缓冲区已满，此时得到的较小值一定是0，
	后面实际写入的字节数也全为0。 
	另一种边界情况是当 len 很大时（因为len是无符号的，负数对它来说也是一个很大的正数），
	这一句也能保证len取到一个较小的值，因为 fifo->in 总是大于等于 fifo->out ，
	所以后面的那个表达式 
	l = min(len, fifo->size - (fifo->in & (fifo->size - 1)));的值不会超过fifo->size的大小。 
	l = min(len, fifo->size - (fifo->in & (fifo->size - 1)));
	是把上一步决定的要写入的字节数 len “切开”，这里又使用了一个技巧。
	注意：实际分配给 fifo->buffer 的字节数 fifo->size ，必须是2的幂，否则这里就会出错。
	既然 fifo->size 是2的幂，那么 (fifo->size-1) 也就是一个后面几位全为1的数，
	也就能保证 (fifo->in & (fifo->size - 1)) 总为不超过 (fifo->size - 1) 的那一部分，
	和 (fifo->in)% (fifo->size - 1) 的效果一样。 这样后面的代码就不难理解了，
	它先向  fifo->in  到缓冲区末端这一块写数据，如果还没写完，在从缓冲区头开始写入剩下的，
	从而实现了循环缓冲。最后，把写指针后移 len 个字节，并返回len。 
	从上面可以看出，fifo->in的值可以从0变化到超过fifo->size的数值，
	fifo->out也如此，但它们的差不会超过fifo->size。
*/


/* +------------------------------------------+ *
 * |		 从fifo向外读数据的函数           | *
 * +------------------------------------------+ */
u32 fifo_get(fifo_t *fifo, u8 *buffer, u32 len)
{
	u32 l;

	len = __MIN(len, fifo->in - fifo->out);

	/* first get the data from fifo->out until the end of the buffer */
	l = __MIN(len, fifo->size - (fifo->out & (fifo->size - 1)));
	memcpy(buffer, fifo->buffer + (fifo->out & (fifo->size - 1)), l);

	/* then get the rest (if any) from the beginning of the buffer */
	memcpy(buffer + l, fifo->buffer, len - l);
	 
	fifo->out += len;
	 
	return len;
}


/* +------------------------------------------+ *
 * |		 返回缓冲区中实际的字节数         | *
 * +------------------------------------------+ */
u32 get_fifo_len(fifo_t *fifo)
{
	return fifo->in - fifo->out;
}



/* +------------------------------------------+ *
 * |		     缓冲区初始化函数         	  | *
 * +------------------------------------------+ */
void fifo_init(fifo_t *fifo, u8 *buffer, u32 size)
{
	fifo->buffer = buffer;
	fifo->size = size;
	fifo->in = 0;
	fifo->out = 0;
}



/* END OF FILE */

