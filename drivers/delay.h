#ifndef __DELAY_H
#define __DELAY_H 
#include "stm32f4xx.h" 
#include "stm32f407zgt6.h"
#if configUSE_DELAY
void vDelayInit(void);
void vDelayus(uint32_t ulDelayTime);
void vDelayXms(uint16_t usDelayTime);
void vDelayms(uint16_t usDelayTime);
#endif
#endif





























