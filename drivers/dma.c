#include "dma.h"

#if configUSE_DMA
/**@brief  DMA init
 * @param  xDMAInit  DMA初始话参数，参考 DMAInit_t 定义
 */
void vDMAInit(DMAInit_t xDMAInit)
{
    if((uint32_t)xDMAInit.DMA_Streamx > (uint32_t)DMA2)                               /* stream是属于DMA2还是DMA1 */
        RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2,ENABLE);
    else 
        RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1,ENABLE);
    
    DMA_DeInit(xDMAInit.DMA_Streamx);
    while(DMA_GetCmdStatus(xDMAInit.DMA_Streamx) != DISABLE){}                      /* 等待DMA可配置       */
	
    DMA_InitTypeDef DMA_InitStruct =                                                /* 配置 DMA Stream */
    {
        .DMA_Channel            = xDMAInit.ulChannel,                               /* 通道选择            */
        .DMA_PeripheralBaseAddr = (uint32_t)xDMAInit.pulPerAddr,                    /* DMA外设地址         */
        .DMA_Memory0BaseAddr    = (uint32_t)xDMAInit.pulMemAddr,                    /* DMA 存储器0地址     */
        .DMA_DIR                = DMA_DIR_MemoryToPeripheral,                       /* 存储器到外设模式    */
        .DMA_BufferSize         = xDMAInit.usTransLen,                              /* 数据传输量          */
        .DMA_PeripheralInc      = DMA_PeripheralInc_Disable,                        /* 外设非增量模式      */
        .DMA_MemoryInc          = DMA_MemoryInc_Enable,                             /* 存储器增量模式      */
        .DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte,                      /* 外设数据长度:8位    */
        .DMA_MemoryDataSize     = DMA_MemoryDataSize_Byte,                          /* 存储器数据长度:8位  */
        .DMA_Mode               = DMA_Mode_Normal,                                  /* 使用普通模式        */
        .DMA_Priority           = DMA_Priority_Medium,                              /* 中等优先级          */
        .DMA_FIFOMode           = DMA_FIFOMode_Disable,
        .DMA_FIFOThreshold      = DMA_FIFOThreshold_Full,
        .DMA_MemoryBurst        = DMA_MemoryBurst_Single,                           /* 存储器突发单次传输  */
        .DMA_PeripheralBurst    = DMA_PeripheralBurst_Single,                       /* 外设突发单次传输    */
    };
    
	DMA_Init(xDMAInit.DMA_Streamx, &DMA_InitStruct);                                /* 初始化DMA Stream    */
} 

/**@brief   开启一次DMA传输
 * @param   DMA_Streamx  DMA数据流,DMA1_Stream0~7/DMA2_Stream0~7 
 * @param   usTransLen   DMA数据传输量
 */ 
void vDMAEnable(DMA_Stream_TypeDef *DMA_Streamx, uint16_t usTransLen)
{
	DMA_Cmd(DMA_Streamx, DISABLE);                                                  /* 关闭DMA传输         */
	while (DMA_GetCmdStatus(DMA_Streamx) != DISABLE){}	                            /* 确保DMA可以被设置   */
	DMA_SetCurrDataCounter(DMA_Streamx,usTransLen);                                 /* 数据传输量          */
	DMA_Cmd(DMA_Streamx, ENABLE);                                                   /* 开启DMA传输         */
}	  
#endif

