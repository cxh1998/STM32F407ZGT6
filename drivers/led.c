#include "led.h" 
#if configUSE_LED
/**@brief   Init led gpio
 */ 
void vLedInit(void)
{
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF,ENABLE);
    
    GPIO_InitTypeDef  GPIO_InitStruct =
    {
        .GPIO_Pin   = GPIO_Pin_9 | GPIO_Pin_10,                                     /* PA9->LED_RED, PA10->LED_GREEN */
        .GPIO_Mode  = GPIO_Mode_OUT,                                                /* ���ģʽ */            
        .GPIO_OType = GPIO_OType_PP,                                                /* ���� */
        .GPIO_PuPd  = GPIO_PuPd_UP,                                                 /* ���� */
        .GPIO_Speed = GPIO_Speed_100MHz
    };
    GPIO_Init(GPIOF, &GPIO_InitStruct);                                             /* ��ʼ��GPIO */
    GPIO_SetBits(GPIOF, GPIO_Pin_9|GPIO_Pin_10);                                    /* ����ߣ�LEDĬ�Ϲر� */
}
#endif
