#ifndef __KEY_H
#define __KEY_H
#include "stm32f4xx.h" 
#include "stm32f407zgt6.h"
#if configUSE_LED

#define KEY0 		        GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_4)                 /* PE4 */
#define KEY_UP 		        GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)	                /* PA0 */

#define KEY0_PRES 	        (1u)
#define KEYUP_PRES          (4u)

void vKeyInit(void);
uint8_t ucKeyScan(uint8_t ucScanMode);
#endif
#endif
