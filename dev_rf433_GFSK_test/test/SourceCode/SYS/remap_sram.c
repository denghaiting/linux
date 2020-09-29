/****************************************Copyright (c)****************************************
 																							
                             Galaxywind Network System Co., Ltd.						    
                                      												
                                  http://www.galaxywind.com								
 																						
 Author:	ZhangYu
 Date  : 	2015-06-01
 Email :	33076156@qq.com
 *********************************************************************************************/ 

#include "stm32f0xx.h"




/*M0没有向量表偏移寄存器，在APP的开头main（）里面加了一段代码，
把中断向量表复制到SRAM的起始地址0x20000000去了。
*/
#define APPLICATION_ADDRESS     ((uint32_t)0x08004000)
#if   (defined ( __CC_ARM ))
  __IO uint32_t VectorTable[48] __attribute__((at(0x20000000)));
#elif (defined (__ICCARM__))
#pragma location = 0x20000000
  __no_init __IO uint32_t VectorTable[48];
#elif defined   (  __GNUC__  )
  __IO uint32_t VectorTable[48] __attribute__((section(".RAMVectorTable")));
#elif defined ( __TASKING__ )
  __IO uint32_t VectorTable[48] __at(0x20000000);
#endif




void sys_remap_sram(void)
{
	uint32_t i = 0;

	/* Relocate by software the vector table to the internal SRAM at 0x20000000 ***/  

	/* Copy the vector table from the Flash (mapped at the base of the application
	load address 0x08003000) to the base address of the SRAM at 0x20000000. */

	for (i = 0; i < 48; i++) {
		VectorTable[i] = *(__IO uint32_t*)(APPLICATION_ADDRESS + (i << 2));
	}

	/* Enable the SYSCFG peripheral clock*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE); 

	/* Remap SRAM at 0x00000000 */
	SYSCFG_MemoryRemapConfig(SYSCFG_MemoryRemap_SRAM);
}







/* END OF FILE */

