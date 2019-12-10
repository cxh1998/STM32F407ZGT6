#include "delay.h"

#if configUSE_DELAY
/*******************************************************************************
* 定时器的时钟为：168000 000/8 = 21000 000
* 1秒计数21000 000次,1ms计数21000次, 要延时N ms, 则计数N*21000
*******************************************************************************/

/**@brief Systick定时器初始化,设置时钟源位HCLK/8,即168000000/8=21000000,21MHz
 */ 
void vDelayInit(void)
{
    SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);
}

/**@brief  微秒延时
 * @param  ulDelayTime 延时时间, ulDelayTime <= 798915(2^24/21)
 */
void vDelayus(uint32_t ulDelayTime)   
{   
    uint32_t ulRegValue;   
    SysTick->LOAD = ulDelayTime*(SystemCoreClock/8000000);                          /* 1us计数 168000000/8000000 = 21次 */
    SysTick->VAL  = 0;                                                              /* 清空计数器*/
    SysTick->CTRL |= 0x01;                                                          /* 开始倒计数 */
    do  
    {   
        ulRegValue = SysTick->CTRL; 
    }
    while((ulRegValue&0x01) &&! (ulRegValue&(1<<16)));                              /* 等待时间到达 */	
    SysTick->CTRL &= 0;                                                             /* 关闭计数器 */
    SysTick->VAL  = 0;                                                              /* 清空计数器 */
}   

/**@brief   毫秒延时
 * @param   usDelayTime  延时时间, ulDelayTime <= 798(2^24/21000)
 */ 
void vDelayXms(uint16_t usDelayTime)                                                /* ms<=1864  SysTick是24位定时器，(2^24)/21000=1864.135111111111 */
{   
    uint32_t ulRegValue;   
    SysTick->LOAD = usDelayTime*(SystemCoreClock/8000);                             /* 时间加载SystemCoreClock = SYSCLK_FREQ_72MHz = 72000000 */
    SysTick->VAL = 0;                                                               /* 清空计数器*/
    SysTick->CTRL |= 0x01;                                                          /* 开始倒计数 */
    do  
    {   
        ulRegValue = SysTick->CTRL; 
    }
    while((ulRegValue&0x01) &&! (ulRegValue&(1<<16)));                              /* 等待时间到达 */
    SysTick->CTRL &= 0;                                                             /* 关闭计数器 */
    SysTick->VAL  = 0;                                                              /* 清空计数器 */
}						    

/**@brief   毫秒延时
 * @param   usDelayTime  延时时间, ulDelayTime <= 65535
 */ 
void vDelayms(uint16_t usDelayTime)
{   
    /* 这里用540,是考虑超频到248M的时候,delay_xms最大只能延时541ms左右了 */
	uint8_t  ucRepeat = usDelayTime / 540;
	uint16_t usRemain = usDelayTime % 540;
	while(ucRepeat)
	{
		vDelayXms(540);
		ucRepeat--;
	}
	if(usRemain)vDelayXms(usRemain);
}	
#endif
