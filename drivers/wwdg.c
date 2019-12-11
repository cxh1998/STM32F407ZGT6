#include "wwdg.h"
#include "led.h"

#if configUSE_WWDG
/**@brief Init wwdg
 */ 
void vWWDGInit(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_WWDG,ENABLE);                             /* ʹ�ܴ��ڿ��Ź�ʱ�� */

    /* ��Ƶֵ */
	WWDG_SetPrescaler(WWDG_Prescaler_8); 
    /* �ϴ���ֵ,�´���ֵĬ��0x3F */    
	WWDG_SetWindowValue(0x5F); 
    /* �������Ź�,�����ü�����ֵ */    
	WWDG_Enable(0x7F);                                                              
	
    NVIC_InitTypeDef NVIC_InitStruct =
    {
        .NVIC_IRQChannel = WWDG_IRQn,
        .NVIC_IRQChannelPreemptionPriority = 2,
        .NVIC_IRQChannelSubPriority = 3,	
        .NVIC_IRQChannelCmd = ENABLE 
    };
	NVIC_Init(&NVIC_InitStruct);
	
	WWDG_ClearFlag();                                                               /* �����ǰ�����жϱ�־λ */
    WWDG_EnableIT();                                                                /* ������ǰ�����ж� */
}

/**@brief This function handles WWDG exception.
 */ 
void WWDG_IRQHandler(void)
{
    /* ���贰�ڿ��Ź�ֵ */
	WWDG_SetCounter(0x7F);
    /* �����ǰ�����жϱ�־λ */
	WWDG_ClearFlag();
    
	LED_GREEN =! LED_GREEN;
}
#endif


