#include "dma.h"

#if configUSE_DMA
/**@brief  DMA init
 * @param  xDMAInit  DMA��ʼ���������ο� DMAInit_t ����
 */
void vDMAInit(DMAInit_t xDMAInit)
{
    if((uint32_t)xDMAInit.DMA_Streamx > (uint32_t)DMA2)                               /* stream������DMA2����DMA1 */
        RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2,ENABLE);
    else 
        RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1,ENABLE);
    
    DMA_DeInit(xDMAInit.DMA_Streamx);
    while(DMA_GetCmdStatus(xDMAInit.DMA_Streamx) != DISABLE){}                      /* �ȴ�DMA������       */
	
    DMA_InitTypeDef DMA_InitStruct =                                                /* ���� DMA Stream */
    {
        .DMA_Channel            = xDMAInit.ulChannel,                               /* ͨ��ѡ��            */
        .DMA_PeripheralBaseAddr = (uint32_t)xDMAInit.pulPerAddr,                    /* DMA�����ַ         */
        .DMA_Memory0BaseAddr    = (uint32_t)xDMAInit.pulMemAddr,                    /* DMA �洢��0��ַ     */
        .DMA_DIR                = DMA_DIR_MemoryToPeripheral,                       /* �洢��������ģʽ    */
        .DMA_BufferSize         = xDMAInit.usTransLen,                              /* ���ݴ�����          */
        .DMA_PeripheralInc      = DMA_PeripheralInc_Disable,                        /* ���������ģʽ      */
        .DMA_MemoryInc          = DMA_MemoryInc_Enable,                             /* �洢������ģʽ      */
        .DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte,                      /* �������ݳ���:8λ    */
        .DMA_MemoryDataSize     = DMA_MemoryDataSize_Byte,                          /* �洢�����ݳ���:8λ  */
        .DMA_Mode               = DMA_Mode_Normal,                                  /* ʹ����ͨģʽ        */
        .DMA_Priority           = DMA_Priority_Medium,                              /* �е����ȼ�          */
        .DMA_FIFOMode           = DMA_FIFOMode_Disable,
        .DMA_FIFOThreshold      = DMA_FIFOThreshold_Full,
        .DMA_MemoryBurst        = DMA_MemoryBurst_Single,                           /* �洢��ͻ�����δ���  */
        .DMA_PeripheralBurst    = DMA_PeripheralBurst_Single,                       /* ����ͻ�����δ���    */
    };
    
	DMA_Init(xDMAInit.DMA_Streamx, &DMA_InitStruct);                                /* ��ʼ��DMA Stream    */
} 

/**@brief   ����һ��DMA����
 * @param   DMA_Streamx  DMA������,DMA1_Stream0~7/DMA2_Stream0~7 
 * @param   usTransLen   DMA���ݴ�����
 */ 
void vDMAEnable(DMA_Stream_TypeDef *DMA_Streamx, uint16_t usTransLen)
{
	DMA_Cmd(DMA_Streamx, DISABLE);                                                  /* �ر�DMA����         */
	while (DMA_GetCmdStatus(DMA_Streamx) != DISABLE){}	                            /* ȷ��DMA���Ա�����   */
	DMA_SetCurrDataCounter(DMA_Streamx,usTransLen);                                 /* ���ݴ�����          */
	DMA_Cmd(DMA_Streamx, ENABLE);                                                   /* ����DMA����         */
}	  
#endif

