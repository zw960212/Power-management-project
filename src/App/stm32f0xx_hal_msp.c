/**
  ******************************************************************************
  * File Name          : stm32f0xx_hal_msp.c
  * Description        : This file provides code for the MSP Initialization 
  *                      and de-Initialization codes.
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
#include "stm32f0xx_hal.h"
#include "timer.h"
#include "gpio.h"
#include "usart.h"

/* Extern Variables ---------------------------------------------------------*/
extern uint8_t BusyFlag;
volatile uint32_t key_press_time;
volatile uint32_t BattValue_time;

/* Extern Functions ---------------------------------------------------------*/
extern void Error_Handler(void);


/**
  * Initializes the Global MSP.
  */
void HAL_MspInit(void)
{
  /* USER CODE BEGIN MspInit 0 */

  /* USER CODE END MspInit 0 */

  __HAL_RCC_SYSCFG_CLK_ENABLE();

  /* System interrupt init*/
  /* SVC_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SVC_IRQn, 0, 0);
  /* PendSV_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(PendSV_IRQn, 0, 0);
  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);

  /* USER CODE BEGIN MspInit 1 */

  /* USER CODE END MspInit 1 */
}


void HAL_TIM_Base_MspInit(TIM_HandleTypeDef* tim_baseHandle)
{

  if(tim_baseHandle->Instance==TIM3)
  {
    /* TIM3 clock enable */
    __HAL_RCC_TIM3_CLK_ENABLE();

    /* TIM3 interrupt Init */
    HAL_NVIC_SetPriority(TIM3_IRQn, 0, 1);
    HAL_NVIC_EnableIRQ(TIM3_IRQn);
  }
	
	else if(tim_baseHandle->Instance==TIM14)
	{
		/* TIM14 clock enable */
    __HAL_RCC_TIM14_CLK_ENABLE();

    /* TIM14 interrupt Init */
    HAL_NVIC_SetPriority(TIM14_IRQn, 0, 2);
    HAL_NVIC_EnableIRQ(TIM14_IRQn);
	}
}

void HAL_TIM_Base_MspDeInit(TIM_HandleTypeDef* tim_baseHandle)
{

  if(tim_baseHandle->Instance==TIM3)
  {
    /* Peripheral clock disable */
    __HAL_RCC_TIM3_CLK_DISABLE();

    /* TIM3 interrupt Deinit */
    HAL_NVIC_DisableIRQ(TIM3_IRQn);
  }
	
	else if(tim_baseHandle->Instance==TIM14)
	{
		/* Peripheral clock disable */
    __HAL_RCC_TIM14_CLK_DISABLE();

    /* TIM14 interrupt Deinit */
    HAL_NVIC_DisableIRQ(TIM14_IRQn);	
	}
	
} 

//定时器回调函数
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)  
{
	 if(htim->Instance == TIM3)  //判断是否是TIM3定时器,1ms计时中断一次
    {
        key_press_time++;        //定时器计时	          
    }
		
		else if(htim->Instance == TIM14) //判断是否是TIM14定时器,1s计时中断一次
		{
				BattValue_time++;
		}
		
}

/* USER CODE BEGIN 1 */
/**
  * @brief UART MSP Initialization 
  *        This function configures the hardware resources used in this example: 
  *           - Peripheral's clock enable
  *           - Peripheral's GPIO Configuration  
  *           - DMA configuration for transmission request by peripheral 
  *           - NVIC configuration for DMA interrupt request enable
  * @param huart: UART handle pointer
  * @retval None
  */
void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
  GPIO_InitTypeDef  GPIO_InitStruct;
  if(huart->Instance == USART_PWR)
	{
		/*##-1- Enable peripherals and GPIO Clocks #################################*/
		/* Enable GPIO TX/RX clock */
		USART_PWR_TX_GPIO_CLK_ENABLE();
		USART_PWR_RX_GPIO_CLK_ENABLE();

		/* Enable USART_FVM clock */
		USART_PWR_CLK_ENABLE(); 
		
		/*##-2- Configure peripheral GPIO ##########################################*/  
		/* UART TX GPIO pin configuration  */
		GPIO_InitStruct.Pin       = USART_PWR_TX_PIN;
		GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Pull      = GPIO_PULLUP;
		GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_HIGH;
		GPIO_InitStruct.Alternate = USART_PWR_TX_AF;

		HAL_GPIO_Init(USART_PWR_TX_GPIO_PORT, &GPIO_InitStruct);

		/* UART RX GPIO pin configuration  */
		GPIO_InitStruct.Pin = USART_PWR_RX_PIN;
		GPIO_InitStruct.Alternate = USART_PWR_RX_AF;

		HAL_GPIO_Init(USART_PWR_RX_GPIO_PORT, &GPIO_InitStruct);

		/*##-3- Configure the NVIC for UART ########################################*/
		/* NVIC for USART */
		HAL_NVIC_SetPriority(USART_PWR_IRQn, 0, 1);  //设置中断优先级
		HAL_NVIC_EnableIRQ(USART_PWR_IRQn);
	}
	
}
/* USER CODE END 1 */


/* USER CODE BEGIN 2 */
/**
  * @brief UART MSP De-Initialization
  *        This function frees the hardware resources used in this example:
  *          - Disable the Peripheral's clock
  *          - Revert GPIO, DMA and NVIC configuration to their default state
  * @param huart: UART handle pointer
  * @retval None
  */
void HAL_UART_MspDeInit(UART_HandleTypeDef *huart)
{
	if(huart->Instance == USART_PWR)
	{
		/*##-1- Reset peripherals ##################################################*/
		USART_PWR_FORCE_RESET();
		USART_PWR_RELEASE_RESET();

		/*##-2- Disable peripherals and GPIO Clocks #################################*/
		/* Configure UART Tx as alternate function  */
		HAL_GPIO_DeInit(USART_PWR_TX_GPIO_PORT, USART_PWR_TX_PIN);
		/* Configure UART Rx as alternate function  */
		HAL_GPIO_DeInit(USART_PWR_RX_GPIO_PORT, USART_PWR_RX_PIN);
		
		/*##-3- Disable the NVIC for UART ##########################################*/
		HAL_NVIC_DisableIRQ(USART_PWR_IRQn);
	}
}
/* USER CODE END 2 */


/* USER CODE BEGIN 3 */
/**
  * @brief  Serial port interrupt receiving end handler
	* @param  PWR_UartHandle: UART handle.
  **/

uint16_t HAL_UART_RxEndProcess(UART_HandleTypeDef *huart)
{
	switch((huart->RxStopType))
	{
		case Stop_ByCount:
		{
			if(--huart->RxXferCount == 0U)
			{
				/* Disable the UART Parity Error Interrupt and RXNE interrupt*/
				CLEAR_BIT(huart->Instance->CR1, (USART_CR1_RXNEIE | USART_CR1_PEIE));

				/* Disable the UART Error Interrupt: (Frame error, noise error, overrun error) */
				CLEAR_BIT(huart->Instance->CR3, USART_CR3_EIE);

				/* Rx process is completed, restore huart->RxState to Ready */
				huart->RxState = HAL_UART_STATE_READY;

				HAL_UART_RxCpltCallback(huart);

				return 0;
			}
			else
			{
				return huart->RxXferCount;
			}
		}//break;
		default:break;
	}
	return 0;
}
/* USER CODE END 3 */


/* interrupt callback function ------------------------------------------------------- */

/* USER CODE BEGIN 4 */
/**
  * @brief  Tx Transfer completed callback
  * @param  NB_UartHandle: UART handle. 
  * @note   This example shows a simple way to report end of IT Tx transfer, and 
  *         you can add your own implementation. 
  * @retval None
  */
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *UartHandle)
{
	/* Set transmission flag: transfer complete */
	switch((uint32_t)(UartHandle->Instance))
	{
		case (uint32_t)USART_PWR:
		{
			UartHandle->UartReady = IT_SET;
		}break;
		default:break;
	}
	
}
/* USER CODE END 4 */


/* USER CODE BEGIN 5 */

#include "Packet.h"
#define	GET_HEAD_TIMES			3


/**
  * @brief  Rx Transfer completed callback
  * @param  NB_UartHandle: UART handle
  * @note   This example shows a simple way to report end of DMA Rx transfer, and 
  *         you can add your own implementation.
  * @retval None
  */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *UartHandle)
{
  /* Set transmission flag: transfer complete */
	if(UartHandle->Instance == USART_PWR)
	{
			uint8_t getHead = 0;
			HAL_StatusTypeDef ret;
			switch(UartHandle->RxContent)
			{
				case Rx_Head:
				{
					do
					{
						if (*(UartHandle->pRxBuffPtr - 1) == SYNC)  //每次中断接收1个数据
						{
							getHead = 1;
							break;
						}
						else
						{
							UartHandle->RxTryTimes++;
							ret = HAL_UART_Receive_IT(UartHandle, (uint8_t*)UartHandle->pRxBuffPtr, 1);
							return;
						}
					}while(UartHandle->RxTryTimes < GET_HEAD_TIMES);
					
					
					if(UartHandle->RxTryTimes < GET_HEAD_TIMES)
					{
						UartHandle->RxContent = Rx_Length;
						ret = HAL_UART_Receive_IT(UartHandle, UartHandle->pRxBuffPtr+1, 2); //每次中断接收数据的长度Length的值
						return;
					}
					else
					{
						UartHandle->UartReady = IT_ERROR;
						BusyFlag = 0;
						return;
					}
					
				}break;
				case Rx_Length:
				{
					volatile AL_WORD commandLength = (AL_WORD)*(UartHandle->pRxBuffPtr - 2);  //此时pRxBuffPtr指向没有接收到数据的空位置
					commandLength <<= 8;
					commandLength += *(UartHandle->pRxBuffPtr - 1);
					UartHandle->RxParameter = commandLength;     //整个报文的长度Length的值为7
					commandLength += (RCV_SUFFIX_SIZE - RCV_PRIFIX_SIZE + 1);   //计算出从Node位开始到结束的数据字节数
					if(commandLength > (uint16_t)MAX_RECV_LENGTH)   //注意比较长度的问题,导致串口不能正常接收数据
					{
						UartHandle->UartReady = IT_ERROR;
						BusyFlag = 0;
						return;
					}
					else
					{
						ret = HAL_UART_Receive_IT(UartHandle, UartHandle->pRxBuffPtr, commandLength); //接收从Node位开始到后面的内容
						UartHandle->RxContent = Rx_Body;  //接收的内容
						return;
					}
				}break;
				case Rx_Body:
				{
					UartHandle->UartReady = IT_SET;
					BusyFlag = 0;
					return;
				}break;
			}
	}
}

/* USER CODE END 5 */


/* USER CODE BEGIN 6 */
/**
  * @brief  UART error callbacks
  * @param  FVM_UartHandle: UART handle
  * @note   This example shows a simple way to report transfer error, and you can
  *         add your own implementation.
  * @retval None
  */
void HAL_UART_ErrorCallback(UART_HandleTypeDef *UartHandle)
{
  switch((uint32_t)(UartHandle->Instance))
	{
		case (uint32_t)USART_PWR:
		{
			switch(UartHandle->ErrorCode)
			{
				case HAL_UART_ERROR_NONE:   							/*!< No error            */
				{
					return;
				}//break;
				case HAL_UART_ERROR_PE:          	 				/*!< Parity error        */
				{}break;
				case HAL_UART_ERROR_NE:           				 /*!< Noise error         */
				{}break;
				case HAL_UART_ERROR_FE:         				   /*!< frame error         */
				{}break;
				case HAL_UART_ERROR_ORE:        				   /*!< Overrun error       */
				{
				//	if(HAL_UART_Transmit_IT(UartHandle, (uint8_t*)UartHandle->Init.pTxBuffPtr, UartHandle->Init.TxXferSize)== HAL_BUSY)
				//	{
			//		}
				//	HAL_UART_Receive_IT(UartHandle, (uint8_t *)UartHandle->Init.pRxBuffPtr, 1);
					//while(UartHandle->UartReady == IT_RESET){}
					//UartHandle->UartReady = IT_RESET;
				}break;
				case HAL_UART_ERROR_DMA:        				   /*!< DMA transfer error  */
				{}break;
				case HAL_UART_ERROR_BUSY: 
				{}break;
				default:break;
			}
			BusyFlag = 0;
		}break;
	}
}
/* USER CODE END 6 */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

		
