#include "iwdg.h"

#if configUSE_IWDG
/**@brief  Init iwdg
 */
void vIWDGInit(void)
{
    IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);                                   /* ȡ��д���� */
    IWDG_SetPrescaler(4);                                                           /* ��Ƶϵ��Ϊ4��64��Ƶ, Ƶ��0.5kHz, ����Ϊ2ms */
    IWDG_SetReload(500);                                                            /* ��װ��ֵ500, 500TimeoutΪ1s */
    IWDG_ReloadCounter();                                                           /* ����װ��һ�� */
    IWDG_Enable();                                                                  /* ʹ�ܿ��Ź� */
}

/**@brief  Watch counter reload value
 */ 
void vIWDGFeed(void)
{
	IWDG_ReloadCounter();
}
#endif
