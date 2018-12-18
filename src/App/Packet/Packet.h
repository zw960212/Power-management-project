/**
 *  通用保文收发处理头文件
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

#define REQUEST_SUFFIX_SIZE (2)     //发送报文后缀长度（即异或位和求和位）
#define REQUEST_PRIFIX_SIZE	(5)     //发送报文前缀长度（即CmdID之前的字节长度）
#define RCV_SUFFIX_SIZE (2)	        //接收报文后缀长度（即异或位和求和位）
#define RCV_PRIFIX_SIZE  (4)        //接收报文前缀长度（即CmdID之前的字节长度） 
#define RESPONSE_NODE_SIZE (1)      //NODE字节的长度
#define MAX_SEND_LENGTH     1024    //单个命令，最大报文发送长度
#define MAX_RECV_LENGTH				1024
#define MAX_RECV_MESS_LENGTH     30        //接收的信令最大长度


/*Extern Varibles-------------------*/
extern AL_BYTE* RcvDataPtr;
	
	
/* Exported functions ------------------------------------------------------- */		
AL_STATUS PacSend(AL_BYTE reqCommand, const AL_BYTE* reqDataPtr, AL_WORD reqLength);
AL_STATUS PacRecv(AL_BYTE* reqCommand,uint8_t** ackData, AL_WORD* ackDataSize);	
	
																											
											

#ifdef __cplusplus
}
#endif

#endif  /* __PACKET_H__ */
