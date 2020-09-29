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
 * |				��ջ�����          	  | *
 * +------------------------------------------+ */
void clear_fifo(fifo_t *fifo)
{
	fifo->in = fifo->out = 0;
}



/* +------------------------------------------+ *
 * |		�򻺳�����������ݵĽӿ�          | *
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
	�� len �� (fifo->size - fifo->in + fifo->out) ֮��ȡһ����С��ֵ����len��
	ע�⣬�� (fifo->in == fifo->out+fifo->size) ʱ����ʾ��������������ʱ�õ��Ľ�Сֵһ����0��
	����ʵ��д����ֽ���ҲȫΪ0�� 
	��һ�ֱ߽�����ǵ� len �ܴ�ʱ����Ϊlen���޷��ŵģ�����������˵Ҳ��һ���ܴ����������
	��һ��Ҳ�ܱ�֤lenȡ��һ����С��ֵ����Ϊ fifo->in ���Ǵ��ڵ��� fifo->out ��
	���Ժ�����Ǹ����ʽ 
	l = min(len, fifo->size - (fifo->in & (fifo->size - 1)));��ֵ���ᳬ��fifo->size�Ĵ�С�� 
	l = min(len, fifo->size - (fifo->in & (fifo->size - 1)));
	�ǰ���һ��������Ҫд����ֽ��� len ���п�����������ʹ����һ�����ɡ�
	ע�⣺ʵ�ʷ���� fifo->buffer ���ֽ��� fifo->size ��������2���ݣ���������ͻ����
	��Ȼ fifo->size ��2���ݣ���ô (fifo->size-1) Ҳ����һ�����漸λȫΪ1������
	Ҳ���ܱ�֤ (fifo->in & (fifo->size - 1)) ��Ϊ������ (fifo->size - 1) ����һ���֣�
	�� (fifo->in)% (fifo->size - 1) ��Ч��һ���� ��������Ĵ���Ͳ�������ˣ�
	������  fifo->in  ��������ĩ����һ��д���ݣ������ûд�꣬�ڴӻ�����ͷ��ʼд��ʣ�µģ�
	�Ӷ�ʵ����ѭ�����塣��󣬰�дָ����� len ���ֽڣ�������len�� 
	��������Կ�����fifo->in��ֵ���Դ�0�仯������fifo->size����ֵ��
	fifo->outҲ��ˣ������ǵĲ�ᳬ��fifo->size��
*/


/* +------------------------------------------+ *
 * |		 ��fifo��������ݵĺ���           | *
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
 * |		 ���ػ�������ʵ�ʵ��ֽ���         | *
 * +------------------------------------------+ */
u32 get_fifo_len(fifo_t *fifo)
{
	return fifo->in - fifo->out;
}



/* +------------------------------------------+ *
 * |		     ��������ʼ������         	  | *
 * +------------------------------------------+ */
void fifo_init(fifo_t *fifo, u8 *buffer, u32 size)
{
	fifo->buffer = buffer;
	fifo->size = size;
	fifo->in = 0;
	fifo->out = 0;
}



/* END OF FILE */

