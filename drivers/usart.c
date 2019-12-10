#include "usart.h"	
#if configUSE_UART
/**@brief   Init uart 
 * @param   ulBaudRate   波特率
 */ 
void vURATInit(uint32_t ulBaudRate)
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
    
    /* 开启GPIO复用,不能写成(GPIOA,GPIO_PinSource9|GPIO_PinSource10)这种方式,
     * GPIO_PinSource9 和 GPIO_Pin_9 是不一样的 
     */
    GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_USART1);
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_USART1);
    
	/* 初始化GPIO, PA9 Tx, PA10 Rx */
    GPIO_InitTypeDef GPIO_InitStruct = 
    {
        .GPIO_Pin   = GPIO_Pin_9 | GPIO_Pin_10,
        .GPIO_Mode  = GPIO_Mode_AF,                                                 /* 复用功能 */
        .GPIO_Speed = GPIO_Speed_50MHz,
        .GPIO_OType = GPIO_OType_PP,                                                /* 推挽 */
        .GPIO_PuPd  = GPIO_PuPd_UP                                                  /* 上拉 */
    };
	GPIO_Init(GPIOA, &GPIO_InitStruct);
    
    /* USART1 配置 */
    USART_InitTypeDef USART_InitStruct = 
    {
        .USART_BaudRate   = ulBaudRate,                                             /* 波特率 */
        .USART_WordLength = USART_WordLength_8b,                                    /* 8位数位 */
        .USART_StopBits   = USART_StopBits_1,                                       /* 1位停止位 */
        .USART_Parity     = USART_Parity_No,                                        /* 无奇偶校验 */ 
        .USART_HardwareFlowControl = USART_HardwareFlowControl_None,                /* 无流控制 */
        .USART_Mode       = USART_Mode_Rx | USART_Mode_Tx
    };
    USART_Init(USART1, &USART_InitStruct); 
    
#if configUSE_USART1_RX_IRQ	
	/* 中断配置 */
    NVIC_InitTypeDef NVIC_InitStruct = 
    {
        .NVIC_IRQChannel = USART1_IRQn,                                             /* USART1中断通道 */
        .NVIC_IRQChannelPreemptionPriority=3,                                       /* 抢占优先级3 */
        .NVIC_IRQChannelSubPriority =3,		                                        /* 子优先级3 */
        .NVIC_IRQChannelCmd = ENABLE 			                                    /* IRQ通道使能 */
    };
    NVIC_Init(&NVIC_InitStruct);
    /* 开启接收中断 */
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
#endif
    USART_ClearFlag(USART1, USART_FLAG_TC);                                         /* 清除USARTx的待处理标志位 */
    /* 使能串口 */
    USART_Cmd(USART1, ENABLE);
}

/**@brief  USART1 interrupt handler
 */ 
void USART1_IRQHandler(void)
{
#if configUSE_USART1_RX_IRQ	
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
	{
		uint16_t usRxVlaue = USART_ReceiveData(USART1);
        USART_SendData(USART1,usRxVlaue); 		 
    }
#endif
}



/* 加入以下代码,支持printf函数,而不需要选择use MicroLIB */
#pragma import(__use_no_semihosting)             
//标准库需要的支持函数                 
struct __FILE 
{ 
	int handle; 
}; 

FILE __stdout;       
//定义_sys_exit()以避免使用半主机模式    
void _sys_exit(int x) 
{ 
	x = x; 
} 
//重定义fputc函数 
int fputc(int ch, FILE *f)
{
    while((USART1->SR & 0x40) == 0) {}                                              /* 循环发送,直到上一次发送完毕 */
    USART1->DR = (uint8_t)ch;
    while((USART1->SR & 0x40) == 0) {}                                              /* 循环发送,直到这一次发送完毕，防止最后一个字节发不出去*/
    return ch;
}
#endif
