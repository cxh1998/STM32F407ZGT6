#ifndef RING_BUFFER_H
#define RING_BUFFER_H
#include "stdio.h"
#include "string.h"
#include <stdint.h>
/*-------------------宏定义--------------------------------------------------*/
#define SUCCESS                         (0x00u)
#define FAILURE                         (0x01u)
#define HY_OK                           (0x00u)
#define HY_ERROR                        (0xFFFFFFFFu)
#define UART_DATA_BUF_LEN               (256u)
#define BT_SPP_DATA_BUF_LEN             (256u)
/* 封包起始和结尾字节 */
#define COMM_PAKET_START_BYTE           (0x55u)              /*< 封包开始字节*/
#define COMM_PAKET_END_BYTE             (0x2Au)              /*< 封包结束字节*/
/* 收发类型 */
#define	R11_TRANS_TYPE_SEND				(0x53)	             /* 'S'---send */
#define	UART_TRANS_TYPE_RESP			(0x52)	             /* 'R'---response */
#define	CWATCH_TRANS_TYPE_SEND          (0x73u)              /* 's'---send */
#define	CWATCH_TRANS_TYPE_RESP          (0x72u)              /* 'r'---response */
/* 命令类型 */
#define	COMM_CMD_TYPE_DATA				(0x01u)
#define BT_CMD_TYPE_VIBARTE             (0x02u)
#define BT_CMD_TYPE_WRITE_SERIALNUM		(0x08u)
#define BT_CMD_TYPE_VERSION				(0x09u)
#define	COMM_CMD_TYPE_STATUS		    (0x0Au)
#define BT_CMD_TYPE_OK					(0x0Bu)
#define BT_CMD_TYPE_RSSI_REMOTE			(0x0Cu)
/* 封包最小长度 */
#define COMM_PAKET_LEN_MIN              (6u)
#define UART_PAKET_LEN_MIN              (6u)
/*-------------------数据类型定义--------------------------------------------*/
typedef struct _ringbuf_t
{
    uint8_t	    *pBuf;
    uint16_t	size;
    uint16_t	havelen; /*有效数据长度*/
    uint16_t	rd;
    uint16_t	wr;
}tRING_BUF;
typedef struct _comm_packet_header
{
    uint8_t startByte;
    uint8_t packetLen;
    uint8_t transType;
    uint8_t cmdType;
} tCOMM_PAKET_HEADER;
extern uint8_t comm_btSppData_buf[BT_SPP_DATA_BUF_LEN];
extern tRING_BUF tCommBtSppRingBuf;
/*-------------------通用函数声明--------------------------------------------*/
uint8_t ucCalculateChecksum(uint8_t *pucData,uint8_t uclength);
uint8_t fun_verify(uint8_t *alueGroup,uint8_t length);   
void RingBuf_init(tRING_BUF *pRingBuf, uint8_t *pBuf, uint16_t buflen);
void comm_bufInit(void);
uint16_t RingBuf_Write(tRING_BUF *pRingBuf, uint8_t *pWrite, uint16_t writeLen);
uint16_t RingBuf_Read(tRING_BUF *pRingBuf, uint8_t *pRead, uint16_t readLen);
uint16_t RingBuf_PreRead(tRING_BUF *pRingBuf, uint8_t *pRead, uint16_t readLen);
uint16_t RingBuf_ConfirmRead(tRING_BUF *pRingBuf, uint16_t readLen);
uint16_t RingBuf_getValidNum(tRING_BUF *pRingBuf);
int comm_dataParse(void);
#endif
