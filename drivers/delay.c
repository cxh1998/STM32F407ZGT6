#include "delay.h"

#if configUSE_DELAY
/*******************************************************************************
* ��ʱ����ʱ��Ϊ��168000 000/8 = 21000 000
* 1�����21000 000��,1ms����21000��, Ҫ��ʱN ms, �����N*21000
*******************************************************************************/

/**@brief Systick��ʱ����ʼ��,����ʱ��ԴλHCLK/8,��168000000/8=21000000,21MHz
 */ 
void vDelayInit(void)
{
    SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);
}

/**@brief  ΢����ʱ
 * @param  ulDelayTime ��ʱʱ��, ulDelayTime <= 798915(2^24/21)
 */
void vDelayus(uint32_t ulDelayTime)   
{   
    uint32_t ulRegValue;   
    SysTick->LOAD = ulDelayTime*(SystemCoreClock/8000000);                          /* 1us���� 168000000/8000000 = 21�� */
    SysTick->VAL  = 0;                                                              /* ��ռ�����*/
    SysTick->CTRL |= 0x01;                                                          /* ��ʼ������ */
    do  
    {   
        ulRegValue = SysTick->CTRL; 
    }
    while((ulRegValue&0x01) &&! (ulRegValue&(1<<16)));                              /* �ȴ�ʱ�䵽�� */	
    SysTick->CTRL &= 0;                                                             /* �رռ����� */
    SysTick->VAL  = 0;                                                              /* ��ռ����� */
}   

/**@brief   ������ʱ
 * @param   usDelayTime  ��ʱʱ��, ulDelayTime <= 798(2^24/21000)
 */ 
void vDelayXms(uint16_t usDelayTime)                                                /* ms<=1864  SysTick��24λ��ʱ����(2^24)/21000=1864.135111111111 */
{   
    uint32_t ulRegValue;   
    SysTick->LOAD = usDelayTime*(SystemCoreClock/8000);                             /* ʱ�����SystemCoreClock = SYSCLK_FREQ_72MHz = 72000000 */
    SysTick->VAL = 0;                                                               /* ��ռ�����*/
    SysTick->CTRL |= 0x01;                                                          /* ��ʼ������ */
    do  
    {   
        ulRegValue = SysTick->CTRL; 
    }
    while((ulRegValue&0x01) &&! (ulRegValue&(1<<16)));                              /* �ȴ�ʱ�䵽�� */
    SysTick->CTRL &= 0;                                                             /* �رռ����� */
    SysTick->VAL  = 0;                                                              /* ��ռ����� */
}						    

/**@brief   ������ʱ
 * @param   usDelayTime  ��ʱʱ��, ulDelayTime <= 65535
 */ 
void vDelayms(uint16_t usDelayTime)
{   
    /* ������540,�ǿ��ǳ�Ƶ��248M��ʱ��,delay_xms���ֻ����ʱ541ms������ */
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
