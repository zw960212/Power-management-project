#include "Packet_type.h"
#include "Packet.h"
#include "usart.h"
#include "RAM.h"
#include "Uart_Comm.h"


//通讯格式定义

#define NODE 	(0xb0)
#define REQUEST_SYMBOL	(0x00)    //发送数据报文中的0x00字节
#define REQUEST_SUFFIX_SIZE (2)
#define REQUEST_PRIFIX_SIZE	(5)
#define RESPONSE_NODE_SIZE (1)
#define RCV_PRIFIX_SIZE  (4)
#define RCV_SUFFIX_SIZE (2)


#define PACKET_WAIT_TIME        6000        //单次收发,应答报文等待的超时时长,单位毫秒
#define PACKET_GETHEAD_TIME     30          //获取报文头的等待时长
#define PACK_GETHEAD_TIMES      10

//extern uint8_t BusyFlag;
extern UART_HandleTypeDef PWR_UartHandle;


//定义全局变量，用于接收报文内容，避免每次动态申请
//AL_BYTE* RcvDataPtr = mRcvBuffer; //PWR_RX函数第一个参数为const,分配数据接收缓冲区	


/*Export Function-------------------------------------------*/
AL_STATUS PacSend(AL_BYTE reqCommand, const AL_BYTE* reqDataPtr, AL_WORD reqLength);
AL_STATUS PacRecv(AL_BYTE* reqCommand,uint8_t** ackData, AL_WORD* ackDataSize);
static AL_BOOL PacGenCheckXor(AL_BYTE *pTargetData, AL_DWORD TargetDataSize, AL_BYTE *pCheckXor);
static AL_BOOL PacGenCheckSum(AL_BYTE *pTargetData, AL_DWORD TargetDataSize, AL_BYTE *pCheckSum);


//计算命令报文中的XOR异或取反值
static AL_BOOL PacGenCheckXor(AL_BYTE *pTargetData, AL_DWORD TargetDataSize, AL_BYTE *pCheckXor)
{
    AL_BYTE		tmpCheckXor = 0;			/* temporary check xor */
    AL_DWORD	LoopCounter = 0;			/* loop counter */
    AL_BYTE		*pCurTargetData = AL_NULL;	/* current target data pointer */

    /* Parameter check */
    if (AL_NULL == pTargetData || AL_NULL == pCheckXor || 0 == TargetDataSize) {
        return AL_FALSE;
    }

    /* Calculate checksum */
    pCurTargetData = (AL_BYTE *) pTargetData;
    for (LoopCounter = 0; LoopCounter < TargetDataSize; LoopCounter++) {
        tmpCheckXor = tmpCheckXor ^ (AL_BYTE) *pCurTargetData;
        ++pCurTargetData;
    }

    /* Output calculated checkxor */
    *pCheckXor = ~tmpCheckXor;

    return AL_TRUE;
}

//计算命令报文中SUM求和值
static AL_BOOL PacGenCheckSum(AL_BYTE *pTargetData, AL_DWORD TargetDataSize, AL_BYTE *pCheckSum)
{
    AL_BYTE		tmpCheckSum = 0;			/* temporary check sum */
    AL_DWORD	LoopCounter = 0;			/* loop counter */
    AL_BYTE		*pCurTargetData = AL_NULL;	/* current target data pointer */

    /* Parameter check */
    if (AL_NULL == pTargetData || AL_NULL == pCheckSum || 0 == TargetDataSize) {
        return AL_FALSE;
    }

    /* Calculate checksum */
    pCurTargetData = (AL_BYTE *) pTargetData;
    for (LoopCounter = 0; LoopCounter < TargetDataSize; LoopCounter++) {
        tmpCheckSum = tmpCheckSum + (AL_BYTE) *pCurTargetData;
        ++pCurTargetData;
    }

    /* Output calculated checksum */
    *pCheckSum = tmpCheckSum;

    return AL_TRUE;
}


/***********************************
 * 发送命令，接收应答数据流，解析报文，获取报文应答状态
 * 入参:
        reqCommand 命令码
        reqData    待发送的数据实体
        reqLength  待发送的数据长度
 * 出参:
 *      无
 * 返回值:
 *      AL_STS_OK 发送成功;  其他发送异常,反馈对应错误码
**********************************/
AL_STATUS PacSend(AL_BYTE reqCommand, const AL_BYTE* reqDataPtr, AL_WORD reqLength)
{
    AL_WORD	successDataSize = 0;			/* send / receive success data size */
    AL_WORD totalCommandSize = 0;
    AL_BYTE cmdLength = 1;   /*CmdID length*/
	
		//IO_PWR_Init();   //每次发送之前初始化串口

    if (AL_NULL == reqCommand)
    {
        return AL_STS_ERROR_PARAMETER;
    }
		
		//整条报文长度
    totalCommandSize = REQUEST_PRIFIX_SIZE + cmdLength + reqLength + REQUEST_SUFFIX_SIZE;
    if (totalCommandSize > MAX_SEND_LENGTH)
    {
        return AL_STS_ERROR_SEND_OVERLAOD;
    }
		
		//SendBufferPtr = (AL_BYTE*)reqDataPtr - (REQUEST_PRIFIX_SIZE + cmdLength); //注意这一句可能造成指针指向非法内存

		AL_BYTE	tmpBuf[100];
    AL_BYTE  *sendData = tmpBuf;
		AL_BYTE  *SendBufferPtr = tmpBuf;  //目的是检验异或和求和
		
    AL_WORD commandSize = totalCommandSize - REQUEST_SUFFIX_SIZE;   //发送命令Length的长度
    // add sync
    *sendData = SYNC;
		sendData++;
    //add length
    *sendData = (AL_BYTE)(commandSize >> 8);  //获取高位
		sendData++;
    *sendData = (AL_BYTE)commandSize;         //获取低位
		sendData++;
		//add request_symbol
		*sendData = REQUEST_SYMBOL;
		sendData++;
    //add node
    *sendData = NODE;
		sendData++;
    //add command
    *sendData = reqCommand;
		sendData++;
    //add data
    if(reqLength > 0)
    {
			StringCpy(sendData, (AL_BYTE*)reqDataPtr, reqLength);
      sendData += reqLength;
    }

    AL_BYTE CheckXor = 0;
    AL_BYTE CheckSum = 0;
    //add xor
    PacGenCheckXor(SendBufferPtr + REQUEST_PRIFIX_SIZE - 1, totalCommandSize - REQUEST_SUFFIX_SIZE- REQUEST_PRIFIX_SIZE + 1, &CheckXor);
    *sendData = CheckXor;
		sendData++;
    //add sum
    PacGenCheckSum(SendBufferPtr + REQUEST_PRIFIX_SIZE - 1, totalCommandSize - REQUEST_SUFFIX_SIZE - REQUEST_PRIFIX_SIZE + 2, &CheckSum);
    *sendData = CheckSum;
		sendData++;

    if (totalCommandSize > AL_SIZE_PACK)  //判断发送的报文长度
    {
      return AL_STS_ERROR_SEND;   //发送报文过长错误
    }
		
    // send command
    if (PWR_TX(SendBufferPtr, totalCommandSize, &successDataSize) != AL_STS_OK)  //串口发送
    {
       return AL_STS_ERROR_SEND;
    }
		return AL_STS_OK;
}	 
		

/***********************************
 * 接收应答报文,解析报文,获取命令ID及Data数据内容
 *
 * 出参:
 *      reqCommand  保存接收到的命令ID
 *      ackData     保存接收到的数据实体
 *      ackDataSize 保存接收到数据段落长度字节数
 * 返回值:
 *      AL_STS_OK 接收正常;  其他接收异常,反馈错误码
**********************************/
AL_STATUS PacRecv(AL_BYTE* reqCommand,uint8_t** ackData, AL_WORD* ackDataSize)
{

    AL_BYTE 	checkXor = 0;
    AL_BYTE 	checkSum = 0;
    AL_DWORD	commandLength = 0;
    volatile AL_STATUS ret;
		AL_BYTE* RcvDataPtr = mRcvBuffer; //PWR_RX函数第一个参数为const,分配数据接收缓冲区	
	
		ret = PWR_RX(RcvDataPtr, 1, PACKET_WAIT_TIME);  //每次接收1字节数据
		if(ret != AL_STS_OK)
			return ret;
		commandLength = PWR_UartHandle.RxParameter; //接收命令的字节长度（7字节）

    // check xor and sum
    PacGenCheckXor(RcvDataPtr + 4, commandLength - RCV_PRIFIX_SIZE + 1, &checkXor);  //由接收到的NODE位开始计算
		//需要单步调试测试具体对应的值
    if (checkXor != RcvDataPtr[commandLength + 1])
    {
        return AL_STS_ERROR_CHECKXOR;
    }

    PacGenCheckSum(RcvDataPtr + 4, commandLength - RCV_PRIFIX_SIZE + 2, &checkSum);
    if (checkSum != RcvDataPtr[commandLength + 2])
    {
        return AL_STS_ERROR_CHECKSUM;
    }

    //接受到的命令ID
    *reqCommand = RcvDataPtr[RCV_PRIFIX_SIZE + 1];
		
		//接受到的数据和数据长度
    *ackDataSize = commandLength - RCV_PRIFIX_SIZE - 1;
    if (ackDataSize > 0)
    {
			*ackData = &RcvDataPtr[RCV_PRIFIX_SIZE + 2];  //接收到的数据
		}
		else
		{
			return AL_STS_ERROR;
		}
    return AL_STS_OK;
}





