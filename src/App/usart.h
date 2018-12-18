/**
  ******************************************************************************
  * File Name          : USART.h
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
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __usart_H
#define __usart_H
#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f0xx_hal.h"
#include "main.h"
#include "Packet_type.h"

/* Public macro -------------------------------------------------------------*/
#define NEW_LINE			0x0a		// '\n'
	
/* Exported variables --------------------------------------------------------*/
extern UART_HandleTypeDef PWR_UartHandle;

/* Exported Functions --------------------------------------------------------*/
extern void Error_Handler(void);

/* User can use this section to tailor USART_PWR/UARTx instance used and associated 
   resources */
/* Definition for USART_PWR clock resources */
#define USART_PWR                           USART1
#define USART_PWR_BAUDRATE										9600
#define USART_PWR_CLK_ENABLE()              __HAL_RCC_USART1_CLK_ENABLE()
#define USART_PWR_RX_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOA_CLK_ENABLE()
#define USART_PWR_TX_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOA_CLK_ENABLE()

#define USART_PWR_FORCE_RESET()             __HAL_RCC_USART1_FORCE_RESET()    //强制设备外围复位
#define USART_PWR_RELEASE_RESET()           __HAL_RCC_USART1_RELEASE_RESET()  //解除设备外围复位

/* Definition for USART_PWR Pins */
#define USART_PWR_TX_PIN                    GPIO_PIN_9
#define USART_PWR_TX_GPIO_PORT              GPIOA
#define USART_PWR_TX_AF                     GPIO_AF1_USART1
#define USART_PWR_RX_PIN                    GPIO_PIN_10
#define USART_PWR_RX_GPIO_PORT              GPIOA
#define USART_PWR_RX_AF                     GPIO_AF1_USART1

/* Definition for USART_PWR's NVIC */
#define USART_PWR_IRQn                      USART1_IRQn
#define USART_PWR_IRQHandler                USART1_IRQHandler

/* Exported macro ------------------------------------------------------------*/
#define COUNTOF(__BUFFER__)   (sizeof(__BUFFER__) / sizeof(*(__BUFFER__)))	 
	 

/* Exported functions ------------------------------------------------------- */	 
void IO_PWR_Init(void);
void IO_PWR_DeInit(void);
ITStatus PWR_GetUartStatus(void);
void PWR_ClearUartStatus(void);
void PWR_SetUartStatus(ITStatus status);
AL_STATUS PWR_TX(const uint8_t* sendPtr, uint16_t sendLen, uint16_t *actualSendLen);
AL_STATUS PWR_RX(const uint8_t* rsvPtr, uint16_t rsvLen, uint16_t timeout);

/* USER CODE BEGIN Prototypes */

/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif
#endif /*__ usart_H */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
