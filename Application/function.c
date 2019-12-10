/*******************************************************************************
 * 1、常用函数
 * 2、
 *******************************************************************************
 */

#include "function.h"

/**@brief  数据校验
 * @param  ucType 0：累加检验 1：异或校验
 * @return 校验结果
 */
uint8_t ucTotalCheck(uint8_t *pucData, uint8_t ucDataLen, uint8_t ucType)
{
    uint8_t i;
    uint8_t ucCheckValue = 0;
    if(ucType)
    {
        for(i = 0; i < ucDataLen; i++)
            ucCheckValue ^= pucData[i];
    }
    else
    {
        for(i = 0; i < ucDataLen; i++) 
            ucCheckValue += pucData[i];
    } 
    return ucCheckValue;
}

/**@brief  int8类型的rssi信号强度转为BCD码格式
 * @param
 * @return
 */
uint8_t ucIntToBCD(int8_t scRssi)
{
    uint8_t ucValue;

    ucValue = ((-scRssi) / 10) << 4;
    ucValue = ucValue | ((-scRssi) % 10);
    return ucValue;
}

/**@brief   查找指定类型的AD_Structure
 * @param
 * @return  1 - success; 2 - failed;
 */
uint8_t vFindADStructure(uint8_t ucADStructureType, Data_t *pxAdvData, Data_t *pxStructureData)
{
    uint32_t  ulIndex = 0;
    uint8_t   *pucTemData;

    pucTemData = pxAdvData->pucData;

    while (ulIndex < pxAdvData->usDlen)
    {
        uint8_t ucFieldLen = pucTemData[ulIndex];
        uint8_t ucFielType = pucTemData[ulIndex + 1];

        if (ucFielType == ucADStructureType)
        {
        #if 0
            pxStructureData->pucData = &pucTemData[ulIndex + 2];
            pxStructureData->usDlen = ucFieldLen - 1;
        #else
            pxStructureData->pucData = &pucTemData[ulIndex];
            pxStructureData->usDlen = ucFieldLen + 1;
        #endif
            return 1;
        }

        ulIndex += (ucFieldLen + 1);
    }
    return 0;
}

#define RING_BUFF_LENGTH  200
typedef struct 
{
    uint32_t pulBuffer[RING_BUFF_LENGTH];
    uint16_t usReIndex;
    uint16_t usWrIndex;
    uint16_t usHavelen;
}Buffer_t;
Buffer_t xRingBuff;

/**@brief   RingBuffer init
 * @param   
 * @return
 */
void vRingBufInit(void)
{
    memset(&xRingBuff, 0, sizeof(Buffer_t));
}

/**@brief   Ring buffer read byte
 * @param   
 * @return  0xFFFF   没有数据
 */
uint32_t ucRingBufReadByte(void)
{
    uint32_t ulValue;
    if(xRingBuff.usHavelen == 0) return 0xFFFF;
    ulValue = xRingBuff.pulBuffer[xRingBuff.usReIndex++];
    if(xRingBuff.usReIndex >= RING_BUFF_LENGTH)
    {
        xRingBuff.usReIndex = 0;
    }
    xRingBuff.usHavelen--;
    return ulValue;
}

/**@brief   Ring buffer write byte
 * @param   
 * @return
 */
void vRingBufWriteByte(uint32_t ulData)
{
    if(xRingBuff.usHavelen == RING_BUFF_LENGTH) return;
    xRingBuff.pulBuffer[xRingBuff.usWrIndex++] = ulData;
    if(xRingBuff.usWrIndex >= RING_BUFF_LENGTH)
    {
        xRingBuff.usWrIndex = 0;
    }
    xRingBuff.usHavelen++;
}

uint8_t pucSendBuffer[150];
/**@brief   Read data from ringbuffer
 */
void vReadDataFromRingBuff(void)
{
    /* 没有数据退出 */
    if(xRingBuff.usHavelen == 0) return;

    uint8_t ucIndex = 0;
    /* 一次最多传输8个数据（32字节)*/
    uint8_t ucDatalen = (xRingBuff.usHavelen > 32 ? 32 : xRingBuff.usHavelen);
    /* 首字节为数据个数 */
    pucSendBuffer[0] = ucDatalen;
    ucIndex += 1;
    for(uint8_t i = 0; i < ucDatalen; i++)
    {
        uint32_t ulData  = ucRingBufReadByte();
        memcpy(&pucSendBuffer[ucIndex], &ulData, 4);
        ucIndex += 4;
    }
    pucSendBuffer[ucIndex] = ucTotalCheck(pucSendBuffer, (4*ucDatalen + 1), 0);
}
/**************************************END OF FILE ****************************/
