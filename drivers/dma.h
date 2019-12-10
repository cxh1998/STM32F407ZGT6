#ifndef __DMA_H
#define __DMA_H
#include "stm32f4xx.h" 
#include "stm32f407zgt6.h"

#if configUSE_DMA
typedef struct
{
    DMA_Stream_TypeDef *DMA_Streamx;                                                /* DMA数据流,DMA1_Stream0~7/DMA2_Stream0~7 */
    uint32_t ulChannel;                                                             /* DMA通道选择,@ref DMA_channel DMA_Channel_0~DMA_Channel_7 */
    uint32_t *pulPerAddr;                                                            /* 外设地址 */    
    uint32_t *pulMemAddr;                                                            /* 存储器地址 */    
    uint16_t usTransLen;                                                            /* 数据传输量 */    
}DMAInit_t;

void vDMAInit(DMAInit_t xDMAInit);
void vDMAEnable(DMA_Stream_TypeDef *DMA_Streamx, uint16_t usTransLen);
#endif
#endif
