#ifndef  _FUNCTION_H
#define  _FUNCTION_H
#include "stdint.h"
#include "stdio.h"
#include "string.h"
#include "stdlib.h"
#include "stddef.h"
#include "stm32f4xx.h" 
#include "stm32f407zgt6.h"
#define  pdTRUE                                          ( 1u )
#define  pdFALSE                                         ( 0u )
#define  RESULT_SUCCESS                                  ( 1u )
#define  RESULT_ERROR                                    ( 0u )
typedef struct 
{
    uint8_t  *pucData;
    uint16_t usDlen;
}Data_t;

uint8_t ucTotalCheck(uint8_t *pucData, uint8_t ucDataLen, uint8_t ucType);
uint8_t ucIntToBCD(int8_t scRssi);
uint8_t vFindADStructure(uint8_t ucADStructureType, Data_t *pxAdvData, Data_t *pxStructureData);
#endif
/**************************************END OF FILE ****************************/
