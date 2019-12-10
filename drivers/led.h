#ifndef __LED_H
#define __LED_H
#include "stm32f4xx.h" 
#include "stm32f407zgt6.h"
#if configUSE_LED
#define LED_RED                        (PFout(9))
#define LED_GREEN                      (PFout(10))
#define LED0 PFout(10)
#define LED1 PFout(9)
void vLedInit(void); 	
#endif
#endif
