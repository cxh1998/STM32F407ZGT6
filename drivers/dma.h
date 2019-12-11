#ifndef __DMA_H
#define __DMA_H
#include "stm32f4xx.h" 
#include "stm32f407zgt6.h"

#if configUSE_DMA
typedef struct
{
    DMA_Stream_TypeDef *DMA_Streamx;                                                /* DMA������,DMA1_Stream0~7/DMA2_Stream0~7 */
    uint32_t ulChannel;                                                             /* DMAͨ��ѡ��,@ref DMA_channel DMA_Channel_0~DMA_Channel_7 */
    uint32_t *pulPerAddr;                                                            /* �����ַ */    
    uint32_t *pulMemAddr;                                                            /* �洢����ַ */    
    uint16_t usTransLen;                                                            /* ���ݴ����� */    
}DMAInit_t;

void vDMAInit(DMAInit_t xDMAInit);
void vDMAEnable(DMA_Stream_TypeDef *DMA_Streamx, uint16_t usTransLen);
#endif
#endif
