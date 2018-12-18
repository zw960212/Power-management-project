#ifndef __UART_COMM__H
#define	__UART_COMM__H

#ifdef __cplusplus
extern "C" {
#endif
/* Includes ---------------------------------------------------------------*/
#include "stdint.h"


	
/* Public Macro -------------------------------------------------------------*/

	
/* Private variables ---------------------------------------------------------*/
static uint8_t chargin_cmd = 0x80;
static uint8_t chargdon_cmd = 0x81;

/* Extern  variable---------------------------------------------*/



typedef enum
{
	Msg_QueryDevic_None								= 0xFFU	,
	
	/* 上位机命令 需要响应 ------------------*/ 
	Msg_Rcv_QueryBattery							=	0x01U	,
	Msg_Rcv_OpenBattery							  =	0x02U	,
	Msg_Rcv_CloseBattery						  =	0x03U	,
	Msg_Rcv_StartCharge							  =	0x80U	,
	Msg_Rcv_EndCharge                 = 0x81U
	
}Uart_MesgID_TypeDef;



/*Export Function-------------------------*/
void PWR_QueryUartRcv(void);
void Parse_Prod_Message(void);
void Query_Batt_SS(uint8_t* frame, uint16_t frameLen);
void Open_Batt_result(uint8_t* frame, uint16_t frameLen);
void Close_Batt_result(uint8_t* frame, uint16_t frameLen);
void Charging_Test(void);

#ifdef __cplusplus
}
#endif

#endif		// #ifndef __UART_COMM__H

