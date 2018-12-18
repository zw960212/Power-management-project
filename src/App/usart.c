/**
  ******************************************************************************
  * File Name          : USART.c
  * Description        : This file provides code for the configuration
  *                      of the USART instances.
  ******************************************************************************
  ** This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * COPYRIGHT(c) 2018 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "usart.h"
#include "gpio.h"




/* UART handler declaration */
UART_HandleTypeDef PWR_UartHandle;


/* USART1 init function */
void IO_PWR_Init(void)
{
	 /*##-1- Configure the UART peripheral ######################################*/
  /* Put the USART peripheral in the Asynchronous mode (UART Mode) */
  /* UART configured as follows:
      - Word Length = 8 Bits   
      - Stop Bit = One Stop bit
      - Parity = None
      - BaudRate = 9600 baud
      - Hardware flow control disabled (RTS and CTS signals) */
  PWR_UartHandle.Instance        = USART_PWR;   
  PWR_UartHandle.Init.BaudRate   = USART_PWR_BAUDRATE;
  PWR_UartHandle.Init.WordLength = UART_WORDLENGTH_8B;
  PWR_UartHandle.Init.StopBits   = UART_STOPBITS_1;
  PWR_UartHandle.Init.Parity     = UART_PARITY_NONE;
  PWR_UartHandle.Init.HwFlowCtl  = UART_HWCONTROL_NONE;
  PWR_UartHandle.Init.Mode       = UART_MODE_TX_RX;
  PWR_UartHandle.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT; 
  if(HAL_UART_DeInit(&PWR_UartHandle) != HAL_OK)
  {
    Error_Handler();
  }
  if(HAL_UART_Init(&PWR_UartHandle) != HAL_OK)
  {
    Error_Handler();
  }
	
	PWR_ClearUartStatus();
}


void IO_PWR_DeInit(void)
{
	if(HAL_UART_DeInit(&PWR_UartHandle) != HAL_OK)
  {
    Error_Handler();
  }
	PWR_ClearUartStatus();
}

ITStatus PWR_GetUartStatus(void)
{
	return PWR_UartHandle.UartReady;
 }

void PWR_SetUartStatus(ITStatus status)
{
	PWR_UartHandle.UartReady = status;
}

void PWR_ClearUartStatus(void)
{
	PWR_UartHandle.UartReady = IT_RESET;
}

/*USART1_Tx Function-------------------*/
AL_STATUS PWR_TX(const uint8_t* sendPtr, uint16_t sendLen, uint16_t *actualSendLen)
{
	PWR_UartHandle.pTxBuffPtr	= (uint8_t*)sendPtr;
	PWR_UartHandle.TxXferSize	=	sendLen;
  if(HAL_UART_Transmit_IT(&PWR_UartHandle, (uint8_t*)sendPtr, sendLen)!= HAL_OK)
  {
    return AL_STS_ERROR;
  }
	while(PWR_UartHandle.UartReady == IT_RESET){}
	PWR_ClearUartStatus();
	*actualSendLen = sendLen - PWR_UartHandle.RxXferCount;
	return AL_STS_OK;
}

/*USART1_Rx Function-------------------*/  //(ÓÐ¸Ä¶¯)
AL_STATUS PWR_RX(const uint8_t* rsvPtr, uint16_t rsvLen, uint16_t timeout)
{
	HAL_StatusTypeDef ret;
	PWR_UartHandle.RxStopType = Stop_ByCount;
	PWR_UartHandle.RxTryTimes = 0;
	PWR_UartHandle.RxContent = Rx_Head;
	PWR_UartHandle.pRxBuffPtr = (uint8_t*)rsvPtr;
	
	do{
		ret = HAL_UART_Receive_IT(&PWR_UartHandle, (uint8_t *)rsvPtr, 1);
	}while(ret == HAL_BUSY);

	if(ret != HAL_OK)
  {
    return AL_STS_ERROR;
  }
	while((PWR_UartHandle.UartReady == IT_BUSY || PWR_UartHandle.UartReady == IT_RESET)&& timeout--)
	{
		HAL_Delay(1);
	}

	switch(PWR_UartHandle.UartReady)
	{
		case IT_RESET:
		{
			PWR_ClearUartStatus();
			HAL_UART_Abort_IT(&PWR_UartHandle);
			return AL_STS_TIMEOUT;
		}break;
		case IT_SET:
		{
			PWR_ClearUartStatus();
			return AL_STS_OK;
		}break;
		case IT_ERROR:
		{
			PWR_ClearUartStatus();
			return AL_STS_ERROR;
		}
		default:
		{
			PWR_ClearUartStatus();
			return AL_STS_ERROR;
		}break;
	}
}





/* USER CODE BEGIN 1 */

/* USER CODE END 1 */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
