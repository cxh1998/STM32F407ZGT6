#include "stm32f4xx.h" 
#include "stm32f407zgt6.h"
#include "led.h"
#include "delay.h"
#include "usart.h"
#include "key.h"
#include "iwdg.h"
#include "wwdg.h"
#include "dma.h"
#include "string.h"

uint8_t pucBuffer[8028];	
uint8_t pucString[] = "UART_DMA_TEST.\r\n";	

/* DMA��ʼ������ */
DMAInit_t xDMAInitConfig = 
{
    .DMA_Streamx = DMA2_Stream7,                                                    /* DMA������,DMA1_Stream0~7/DMA2_Stream0~7 */
    .ulChannel   = DMA_Channel_4,                                                   /* DMAͨ��ѡ��,@ref DMA_channel DMA_Channel_0~DMA_Channel_7 */
    .pulPerAddr  = (uint32_t *)&(USART1->DR),                                       /* �����ַ */    
    .pulMemAddr  = (uint32_t *)pucBuffer,                                           /* �洢����ַ */    
    .usTransLen  = 8028
};

/**@brief Application main function.
 */
int main(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    vURATInit(115200);
	vLedInit();
    vKeyInit();
    vDMAInit(xDMAInitConfig);
    for(;;)
	{
        
    }
}
/**
 * @}
 */
