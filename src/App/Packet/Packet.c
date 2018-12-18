#include "Packet_type.h"
#include "Packet.h"
#include "usart.h"
#include "RAM.h"
#include "Uart_Comm.h"


//ͨѶ��ʽ����

#define NODE 	(0xb0)
#define REQUEST_SYMBOL	(0x00)    //�������ݱ����е�0x00�ֽ�
#define REQUEST_SUFFIX_SIZE (2)
#define REQUEST_PRIFIX_SIZE	(5)
#define RESPONSE_NODE_SIZE (1)
#define RCV_PRIFIX_SIZE  (4)
#define RCV_SUFFIX_SIZE (2)


#define PACKET_WAIT_TIME        6000        //�����շ�,Ӧ���ĵȴ��ĳ�ʱʱ��,��λ����
#define PACKET_GETHEAD_TIME     30          //��ȡ����ͷ�ĵȴ�ʱ��
#define PACK_GETHEAD_TIMES      10

//extern uint8_t BusyFlag;
extern UART_HandleTypeDef PWR_UartHandle;


//����ȫ�ֱ��������ڽ��ձ������ݣ�����ÿ�ζ�̬����
//AL_BYTE* RcvDataPtr = mRcvBuffer; //PWR_RX������һ������Ϊconst,�������ݽ��ջ�����	


/*Export Function-------------------------------------------*/
AL_STATUS PacSend(AL_BYTE reqCommand, const AL_BYTE* reqDataPtr, AL_WORD reqLength);
AL_STATUS PacRecv(AL_BYTE* reqCommand,uint8_t** ackData, AL_WORD* ackDataSize);
static AL_BOOL PacGenCheckXor(AL_BYTE *pTargetData, AL_DWORD TargetDataSize, AL_BYTE *pCheckXor);
static AL_BOOL PacGenCheckSum(AL_BYTE *pTargetData, AL_DWORD TargetDataSize, AL_BYTE *pCheckSum);


//����������е�XOR���ȡ��ֵ
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

//�����������SUM���ֵ
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
 * �����������Ӧ�����������������ģ���ȡ����Ӧ��״̬
 * ���:
        reqCommand ������
        reqData    �����͵�����ʵ��
        reqLength  �����͵����ݳ���
 * ����:
 *      ��
 * ����ֵ:
 *      AL_STS_OK ���ͳɹ�;  ���������쳣,������Ӧ������
**********************************/
AL_STATUS PacSend(AL_BYTE reqCommand, const AL_BYTE* reqDataPtr, AL_WORD reqLength)
{
    AL_WORD	successDataSize = 0;			/* send / receive success data size */
    AL_WORD totalCommandSize = 0;
    AL_BYTE cmdLength = 1;   /*CmdID length*/
	
		//IO_PWR_Init();   //ÿ�η���֮ǰ��ʼ������

    if (AL_NULL == reqCommand)
    {
        return AL_STS_ERROR_PARAMETER;
    }
		
		//�������ĳ���
    totalCommandSize = REQUEST_PRIFIX_SIZE + cmdLength + reqLength + REQUEST_SUFFIX_SIZE;
    if (totalCommandSize > MAX_SEND_LENGTH)
    {
        return AL_STS_ERROR_SEND_OVERLAOD;
    }
		
		//SendBufferPtr = (AL_BYTE*)reqDataPtr - (REQUEST_PRIFIX_SIZE + cmdLength); //ע����һ��������ָ��ָ��Ƿ��ڴ�

		AL_BYTE	tmpBuf[100];
    AL_BYTE  *sendData = tmpBuf;
		AL_BYTE  *SendBufferPtr = tmpBuf;  //Ŀ���Ǽ����������
		
    AL_WORD commandSize = totalCommandSize - REQUEST_SUFFIX_SIZE;   //��������Length�ĳ���
    // add sync
    *sendData = SYNC;
		sendData++;
    //add length
    *sendData = (AL_BYTE)(commandSize >> 8);  //��ȡ��λ
		sendData++;
    *sendData = (AL_BYTE)commandSize;         //��ȡ��λ
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

    if (totalCommandSize > AL_SIZE_PACK)  //�жϷ��͵ı��ĳ���
    {
      return AL_STS_ERROR_SEND;   //���ͱ��Ĺ�������
    }
		
    // send command
    if (PWR_TX(SendBufferPtr, totalCommandSize, &successDataSize) != AL_STS_OK)  //���ڷ���
    {
       return AL_STS_ERROR_SEND;
    }
		return AL_STS_OK;
}	 
		

/***********************************
 * ����Ӧ����,��������,��ȡ����ID��Data��������
 *
 * ����:
 *      reqCommand  ������յ�������ID
 *      ackData     ������յ�������ʵ��
 *      ackDataSize ������յ����ݶ��䳤���ֽ���
 * ����ֵ:
 *      AL_STS_OK ��������;  ���������쳣,����������
**********************************/
AL_STATUS PacRecv(AL_BYTE* reqCommand,uint8_t** ackData, AL_WORD* ackDataSize)
{

    AL_BYTE 	checkXor = 0;
    AL_BYTE 	checkSum = 0;
    AL_DWORD	commandLength = 0;
    volatile AL_STATUS ret;
		AL_BYTE* RcvDataPtr = mRcvBuffer; //PWR_RX������һ������Ϊconst,�������ݽ��ջ�����	
	
		ret = PWR_RX(RcvDataPtr, 1, PACKET_WAIT_TIME);  //ÿ�ν���1�ֽ�����
		if(ret != AL_STS_OK)
			return ret;
		commandLength = PWR_UartHandle.RxParameter; //����������ֽڳ��ȣ�7�ֽڣ�

    // check xor and sum
    PacGenCheckXor(RcvDataPtr + 4, commandLength - RCV_PRIFIX_SIZE + 1, &checkXor);  //�ɽ��յ���NODEλ��ʼ����
		//��Ҫ�������Բ��Ծ����Ӧ��ֵ
    if (checkXor != RcvDataPtr[commandLength + 1])
    {
        return AL_STS_ERROR_CHECKXOR;
    }

    PacGenCheckSum(RcvDataPtr + 4, commandLength - RCV_PRIFIX_SIZE + 2, &checkSum);
    if (checkSum != RcvDataPtr[commandLength + 2])
    {
        return AL_STS_ERROR_CHECKSUM;
    }

    //���ܵ�������ID
    *reqCommand = RcvDataPtr[RCV_PRIFIX_SIZE + 1];
		
		//���ܵ������ݺ����ݳ���
    *ackDataSize = commandLength - RCV_PRIFIX_SIZE - 1;
    if (ackDataSize > 0)
    {
			*ackData = &RcvDataPtr[RCV_PRIFIX_SIZE + 2];  //���յ�������
		}
		else
		{
			return AL_STS_ERROR;
		}
    return AL_STS_OK;
}





