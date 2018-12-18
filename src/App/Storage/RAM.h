#ifndef __RAM__H
#define __RAM__H
#ifdef __cplusplus
extern "C" {
#endif
/* Includes ------------------------------------------------------------------*/
#include "stdint.h"


/* Export Definition ------------------------------------------------------------------*/
/* Size of Trasmission buffer */
#define PWR_TXBUFFERSIZE                      1024
/* Size of Reception buffer */
#define PWR_RXBUFFERSIZE                      1024
	
/* Export Typedef ------------------------------------------------------------------*/

	
	
/* Exprot various ------------------------------------------------------------------*/
extern uint8_t mSendDataBuffer[PWR_TXBUFFERSIZE];   /*没有作用，因为每次发送和接收报文都要验证数据*/
extern uint8_t mSendCmdBuffer[2];
extern uint8_t mRcvBuffer[PWR_RXBUFFERSIZE];

#ifdef __cplusplus
}
#endif
#endif		//#ifndef __RAM__H
