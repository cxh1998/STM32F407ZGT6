#include "key.h"
#include "delay.h" 
#if configUSE_KEY
/**@brief  Init key gpio
 */ 
void vKeyInit(void)
{
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA|RCC_AHB1Periph_GPIOE, ENABLE);
    /* key 0 */
    GPIO_InitTypeDef  GPIO_InitStruct =
    {
        .GPIO_Pin   = GPIO_Pin_4 | GPIO_Pin_0,
        .GPIO_Mode  = GPIO_Mode_IN,                                                 /* 输入模式 */
        .GPIO_Speed = GPIO_Speed_100MHz,
        .GPIO_PuPd  = GPIO_PuPd_DOWN                                                /* 下拉 */
    };
    GPIO_Init(GPIOE, &GPIO_InitStruct);
    /* key up */
    GPIO_InitStruct.GPIO_Pin  = GPIO_Pin_0;
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_DOWN ;                                    /* 下拉 */
    GPIO_Init(GPIOA, &GPIO_InitStruct);
 
}

/**@brief  按键扫描，注意此函数有响应优先级,KEY0>WK_UP
 * 
 * @param  ucScanMode 0 不支持连按
 * @param  ucScanMode 1 支持连按
 * 
 * @return 0  没有按键按下
 * @return 1  key0 按下
 * @return 4  key_up 按下
 */ 
uint8_t ucKeyScan(uint8_t ucScanMode)
{
    /* 按键松开标志 */
    static u8 ucKeyRelease = 1 ;
    /* 连按模式下每次扫描默认按键已松开 */
    if(ucScanMode) ucKeyRelease= 1; 
    
    if(ucKeyRelease && (KEY0==1 || KEY_UP==1))
    {
        /* 消抖 */
        vDelayms(10);
        ucKeyRelease = 0;
        if(KEY0 == 1)    return KEY0_PRES;
        if(KEY_UP == 1)  return KEYUP_PRES;
    }
    else if(KEY0==0 && KEY_UP==0)
    {
        ucKeyRelease = 1; 
    }	    
    return 0;
}
#endif
