#include "iwdg.h"

#if configUSE_IWDG
/**@brief  Init iwdg
 */
void vIWDGInit(void)
{
    IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);                                   /* 取消写保护 */
    IWDG_SetPrescaler(4);                                                           /* 分频系数为4，64分频, 频率0.5kHz, 周期为2ms */
    IWDG_SetReload(500);                                                            /* 重装载值500, 500Timeout为1s */
    IWDG_ReloadCounter();                                                           /* 先重装载一次 */
    IWDG_Enable();                                                                  /* 使能看门狗 */
}

/**@brief  Watch counter reload value
 */ 
void vIWDGFeed(void)
{
	IWDG_ReloadCounter();
}
#endif
