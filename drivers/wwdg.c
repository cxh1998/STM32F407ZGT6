#include "wwdg.h"
#include "led.h"

#if configUSE_WWDG
/**@brief Init wwdg
 */ 
void vWWDGInit(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_WWDG,ENABLE);                             /* 使能窗口看门狗时钟 */

    /* 分频值 */
	WWDG_SetPrescaler(WWDG_Prescaler_8); 
    /* 上窗口值,下窗口值默认0x3F */    
	WWDG_SetWindowValue(0x5F); 
    /* 开启看门狗,并设置计数器值 */    
	WWDG_Enable(0x7F);                                                              
	
    NVIC_InitTypeDef NVIC_InitStruct =
    {
        .NVIC_IRQChannel = WWDG_IRQn,
        .NVIC_IRQChannelPreemptionPriority = 2,
        .NVIC_IRQChannelSubPriority = 3,	
        .NVIC_IRQChannelCmd = ENABLE 
    };
	NVIC_Init(&NVIC_InitStruct);
	
	WWDG_ClearFlag();                                                               /* 清除提前唤醒中断标志位 */
    WWDG_EnableIT();                                                                /* 开启提前唤醒中断 */
}

/**@brief This function handles WWDG exception.
 */ 
void WWDG_IRQHandler(void)
{
    /* 重设窗口看门狗值 */
	WWDG_SetCounter(0x7F);
    /* 清除提前唤醒中断标志位 */
	WWDG_ClearFlag();
    
	LED_GREEN =! LED_GREEN;
}
#endif


