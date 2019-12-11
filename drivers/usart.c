#include "usart.h"	
#if configUSE_UART
/**@brief   Init uart 
 * @param   ulBaudRate   ������
 */ 
void vURATInit(uint32_t ulBaudRate)
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
    
    /* ����GPIO����,����д��(GPIOA,GPIO_PinSource9|GPIO_PinSource10)���ַ�ʽ,
     * GPIO_PinSource9 �� GPIO_Pin_9 �ǲ�һ���� 
     */
    GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_USART1);
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_USART1);
    
	/* ��ʼ��GPIO, PA9 Tx, PA10 Rx */
    GPIO_InitTypeDef GPIO_InitStruct = 
    {
        .GPIO_Pin   = GPIO_Pin_9 | GPIO_Pin_10,
        .GPIO_Mode  = GPIO_Mode_AF,                                                 /* ���ù��� */
        .GPIO_Speed = GPIO_Speed_50MHz,
        .GPIO_OType = GPIO_OType_PP,                                                /* ���� */
        .GPIO_PuPd  = GPIO_PuPd_UP                                                  /* ���� */
    };
	GPIO_Init(GPIOA, &GPIO_InitStruct);
    
    /* USART1 ���� */
    USART_InitTypeDef USART_InitStruct = 
    {
        .USART_BaudRate   = ulBaudRate,                                             /* ������ */
        .USART_WordLength = USART_WordLength_8b,                                    /* 8λ��λ */
        .USART_StopBits   = USART_StopBits_1,                                       /* 1λֹͣλ */
        .USART_Parity     = USART_Parity_No,                                        /* ����żУ�� */ 
        .USART_HardwareFlowControl = USART_HardwareFlowControl_None,                /* �������� */
        .USART_Mode       = USART_Mode_Rx | USART_Mode_Tx
    };
    USART_Init(USART1, &USART_InitStruct); 
    
#if configUSE_USART1_RX_IRQ	
	/* �ж����� */
    NVIC_InitTypeDef NVIC_InitStruct = 
    {
        .NVIC_IRQChannel = USART1_IRQn,                                             /* USART1�ж�ͨ�� */
        .NVIC_IRQChannelPreemptionPriority=3,                                       /* ��ռ���ȼ�3 */
        .NVIC_IRQChannelSubPriority =3,		                                        /* �����ȼ�3 */
        .NVIC_IRQChannelCmd = ENABLE 			                                    /* IRQͨ��ʹ�� */
    };
    NVIC_Init(&NVIC_InitStruct);
    /* ���������ж� */
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
#endif
    USART_ClearFlag(USART1, USART_FLAG_TC);                                         /* ���USARTx�Ĵ������־λ */
    /* ʹ�ܴ��� */
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



/* �������´���,֧��printf����,������Ҫѡ��use MicroLIB */
#pragma import(__use_no_semihosting)             
//��׼����Ҫ��֧�ֺ���                 
struct __FILE 
{ 
	int handle; 
}; 

FILE __stdout;       
//����_sys_exit()�Ա���ʹ�ð�����ģʽ    
void _sys_exit(int x) 
{ 
	x = x; 
} 
//�ض���fputc���� 
int fputc(int ch, FILE *f)
{
    while((USART1->SR & 0x40) == 0) {}                                              /* ѭ������,ֱ����һ�η������ */
    USART1->DR = (uint8_t)ch;
    while((USART1->SR & 0x40) == 0) {}                                              /* ѭ������,ֱ����һ�η�����ϣ���ֹ���һ���ֽڷ�����ȥ*/
    return ch;
}
#endif
