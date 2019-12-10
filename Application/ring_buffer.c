#include "ring_buffer.h"

uint8_t comm_btSppData_buf[BT_SPP_DATA_BUF_LEN];

tRING_BUF tCommBtSppRingBuf;

/**
 * @brief  初始化ring buffer的结构，同时指定buffer
 * @param
 * @return
 */
void vFBuffInit(tRING_BUF *pRingBuf, uint8_t *pBuf, uint16_t buflen)
{
    memset((char *)pRingBuf, 0, sizeof(tRING_BUF)); 

    pRingBuf->size = buflen;
    pRingBuf->pBuf = pBuf; 
}

/**
 * @brief  初始化通讯buf
 * @param
 * @return
 */
void comm_bufInit(void)
{
    //RingBuf_init(&tCommUartRingBuf, comm_uartData_buf, UART_DATA_BUF_LEN);
    RingBuf_init(&tCommBtSppRingBuf, comm_btSppData_buf, BT_SPP_DATA_BUF_LEN);
}

/**
 * @brief  向指定ringbuf写入指定长度的数据
 * @param
 * @return
 */
uint16_t RingBuf_Write(tRING_BUF *pRingBuf, uint8_t *pWrite, uint16_t writeLen)
{
    uint16_t  left,actWr=0;
    uint16_t  rd,wr,size,len;
    uint8_t	  *pBuf;

    rd  = pRingBuf->rd;
    wr  = pRingBuf->wr;
    size= pRingBuf->size;
    len = pRingBuf->havelen;
    pBuf= pRingBuf->pBuf;
    
    if(len + writeLen > size)    //不能写入全部时就一点也不写
    {
        return 0;
    }
    
    left = size-len-1;

    actWr = (left >= writeLen)? writeLen:left;

    if (actWr < writeLen)
    {
        return 0;
    }

    if(wr >= rd)
    {
        left = size - wr;
        if(left >= actWr)
        {
            memcpy(&pBuf[wr], pWrite, actWr);
        }
        else
        {
            memcpy(&pBuf[wr], pWrite, left);
            memcpy(&pBuf[0], &pWrite[left], actWr-left);
        }
    }
    else// if(wr < rd)
    {
        memcpy(&pBuf[wr], pWrite, actWr);
    }
    
    wr += actWr;
    if(wr >= size) wr -= size;
    pRingBuf->wr = wr;
    pRingBuf->havelen += actWr;
      
    return actWr;
}

/**
 * @brief  从指定ringbuf读取指定长度的数据，只读取，不改变ring的指针
 * @param
 * @return
 */
uint16_t RingBuf_PreRead(tRING_BUF *pRingBuf, uint8_t *pRead, uint16_t readLen)
{
    uint16_t		left,actRd=0;
    uint16_t		rd,wr,size,len;
    uint8_t		*pBuf;

    rd  = pRingBuf->rd;
    wr  = pRingBuf->wr;
    size= pRingBuf->size;
    len = pRingBuf->havelen;
    pBuf= pRingBuf->pBuf;

    if(len == 0)
    {
        return 0;
    }
    actRd = (len >= readLen)?readLen:len;
    if(rd > wr)
    {
        left = size - rd;
        if(left >= actRd)
        {
            memcpy(&pRead[0], &pBuf[rd], actRd);
        }
        else
        {
            memcpy(&pRead[0], &pBuf[rd], left);
            memcpy(&pRead[left], &pBuf[0], actRd-left);
        }
        rd += actRd;
        if(rd >= size) rd -= size;
        //pRingBuf->rd = rd;
        //pRingBuf->havelen -= actRd;
    }
    else if(rd < wr)
    {
        memcpy(&pRead[0], &pBuf[rd], actRd);
        rd += actRd;
        if(rd >= size) rd -= size;
        //pRingBuf->rd = rd;
        //pRingBuf->havelen -= actRd;
    }
    return actRd;
}

/**
 * @brief  从指定ringbuf读取指定长度的数据，只改变指针，不读取内容，
 *		   要与RingBuf_PreRead配对连续使用，否则会导致读取数据内容错误
 * @param
 * @return 实际读取的长度
 */
uint16_t RingBuf_ConfirmRead(tRING_BUF *pRingBuf, uint16_t readLen)
{
    uint16_t		actRd=0;
    uint16_t		rd,size,len;
    rd  = pRingBuf->rd;
    size= pRingBuf->size;
    len = pRingBuf->havelen;
#if 1
    if(readLen > len)
    {
        pRingBuf->havelen = 0;
        pRingBuf->rd = pRingBuf->wr;
    }
    else
    {
        len -= readLen;
        rd += readLen;
        if(rd >= size) rd -= size;
        pRingBuf->havelen = len;
        pRingBuf->rd = rd;
    }
#else
    //只调整rd值
    rd += readLen;
    if(rd >= size) rd -= size;
    //intLev = InterruptDisable();
    pRingBuf->rd = rd;
    //InterruptRestore(intLev);
#endif	
    return actRd;	
}


/**
 * @brief  发送
 * @param 
 * @return HY_OK------发送成功
 *         HY_ERROR---发送失败
 */
int comm_send(uint8_t transType, uint8_t cmd, uint8_t *pData, int len)/* 封包*/
{
    uint8_t i, temp[128], sum=0;
    
    if (pData==NULL) return (int)HY_ERROR;
#if 0
    if (!flag)
    {
        HalUARTInit();
        SerialApp_Init(0);
        flag = 1;
    }
#endif
    temp[0] =  COMM_PAKET_START_BYTE;
    temp[1] = len+2;
    temp[2] = transType;
    temp[3] = cmd;
    memcpy(&temp[4], pData, len);
    temp[len+5] = COMM_PAKET_END_BYTE;
    for(i=0; i<len+3; i++)
    {
        sum += temp[i+1];
    }
    temp[len+4] = sum;
    //sbpSerialAppWrite(temp, len+6);

    return HY_OK;
}


/**
 * @brief  通讯数据解析
 * @param 
 * @return HY_OK------成功接收到数据
 *         HY_ERROR---无有效数据
 */
int comm_dataParse(void)
{
    //BaseType_t rtosApiResult= pdFALSE;
    //cy_en_ble_api_result_t  bleApiResult;
    //cy_en_em_eeprom_status_t eepromReturnValue;
    uint8_t btRecvBuf[BT_SPP_DATA_BUF_LEN];
    int ret, recvLen,i;
    uint8_t findPacket,sum;
    tCOMM_PAKET_HEADER *pHeader;
    static uint32_t cnt=0;
    ret = (int)HY_ERROR;
    //尝试解析数据
    while(1)
    {
        findPacket = 0;
        memset(btRecvBuf,0,BT_SPP_DATA_BUF_LEN);
        recvLen = RingBuf_PreRead(&tCommBtSppRingBuf, &btRecvBuf[0], BT_SPP_DATA_BUF_LEN);
        //有效字节数最少要大于6个字节，否则就不是一个完整的数据包
        if ((uint8_t)recvLen > COMM_PAKET_LEN_MIN)
        {
            if((btRecvBuf[0] == COMM_PAKET_START_BYTE)&& (btRecvBuf[1] > 8))
            {
                //找到疑似paket头，继续判断数据
                pHeader = (tCOMM_PAKET_HEADER *)btRecvBuf;
                //去掉包起始、数据长度、校验字节、包结束4个字节
                if (pHeader->packetLen > (BT_SPP_DATA_BUF_LEN-4))
                {
                    //封包长度大于最大接收长度，认为是错误封包
                    RingBuf_ConfirmRead(&tCommBtSppRingBuf, 1);
                    //消耗掉一个字节，从下一个字节继续判断
                    continue;
                }
                if (pHeader->packetLen > (recvLen-4))
                {
                    //封包长度大于现有数据，退出等待接收更多数据
                    break;
                }
                //找到封包结束字节
                if (btRecvBuf[pHeader->packetLen+3] == COMM_PAKET_END_BYTE)
                {
                    //找到packet尾，判断校验和
                    sum = 0;
                    for (i=0; i< (pHeader->packetLen+1); i++)
                    {
                        sum += btRecvBuf[i+1];
                    }
                    //找到完整的packet，解析并执行
                    if (sum == btRecvBuf[pHeader->packetLen+2])
                    {
                        
                    }
                    else
                    {
                        /* 校验和错误 */
                    }
                }
                else
                {
                    /* 封包错误 */
                }
            }//判断packet头
            else
            {
                /* 没有找到packet头 */
            }
            if (findPacket == 0)/* 没有找到数据帧，消耗一个字节，继续判断 */
            {
                cnt++;
                RingBuf_ConfirmRead(&tCommBtSppRingBuf, 1);
            }
        }
        else
        {
            break;/* 数据不足，退出 */
        }
    }//while(1)
    return ret;
}

