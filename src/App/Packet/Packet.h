/**
 *  ͨ�ñ����շ�����ͷ�ļ�
 *  zw 20181030
*/
#ifndef __PACKET_H__
#define __PACKET_H__

#include "RAM.h"
#include "Packet_type.h"

#ifdef __cplusplus
extern "C" {
#endif

#define SYNC	(0x6d)

#define REQUEST_SUFFIX_SIZE (2)     //���ͱ��ĺ�׺���ȣ������λ�����λ��
#define REQUEST_PRIFIX_SIZE	(5)     //���ͱ���ǰ׺���ȣ���CmdID֮ǰ���ֽڳ��ȣ�
#define RCV_SUFFIX_SIZE (2)	        //���ձ��ĺ�׺���ȣ������λ�����λ��
#define RCV_PRIFIX_SIZE  (4)        //���ձ���ǰ׺���ȣ���CmdID֮ǰ���ֽڳ��ȣ� 
#define RESPONSE_NODE_SIZE (1)      //NODE�ֽڵĳ���
#define MAX_SEND_LENGTH     1024    //�����������ķ��ͳ���
#define MAX_RECV_LENGTH				1024
#define MAX_RECV_MESS_LENGTH     30        //���յ�������󳤶�


/*Extern Varibles-------------------*/
extern AL_BYTE* RcvDataPtr;
	
	
/* Exported functions ------------------------------------------------------- */		
AL_STATUS PacSend(AL_BYTE reqCommand, const AL_BYTE* reqDataPtr, AL_WORD reqLength);
AL_STATUS PacRecv(AL_BYTE* reqCommand,uint8_t** ackData, AL_WORD* ackDataSize);	
	
																											
											

#ifdef __cplusplus
}
#endif

#endif  /* __PACKET_H__ */
