#ifndef  _IWDG_H
#define  _IWDG_H
#include "stm32f4xx.h" 
#include "stm32f407zgt6.h"

#if configUSE_IWDG
void vIWDGInit(void);
void vIWDGFeed(void);
#endif
#endif
