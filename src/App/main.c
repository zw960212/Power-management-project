/**
  ******************************************************************************
  * File Name          : main.c
  * Description        : Main program body
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
#include "main.h"
#include "stm32f0xx_hal.h"
#include "gpio.h"
#include "usart.h"
#include "key.h"
#include "timer.h"
#include "Packet.h"
#include "Uart_Comm.h"
#include "Task.h"


/* Private variables ---------------------------------------------------------*/
uint8_t BusyFlag;
uint8_t BattFlag;
//uint8_t cmd_id = 0;
//uint8_t* RcvMsg = {0};
//uint16_t MsgLen = 0;
//uint8_t Data[20];
uint8_t tmp_BattValue = 0;  //临时电量值



/* External variables --------------------------------------------------------*/
extern uint8_t RealTime_BattValue; 
extern volatile uint32_t BattValue_time;


/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);

/* Extern functions ---------------------------------------------------------*/



int main(void)
{
	
  /* MCU Configuration----------------------------------------------------------*/
  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* Configure the system clock */
  SystemClock_Config();

  /* Initialize all configured peripherals */
  Output_Init();      //GPIO初始化
  IO_PWR_Init();      //串口初始化
	KEY_GPIO_Init();    //开关机按键初始化
	TIM3_Init();  			//TIM3初始化
	TIM14_Init();       //TIM14初始化
	
	
  /* Infinite loop */
	while(1)
	{
		
		PWR_QueryUartRcv();  //串口查询函数
		PWR_OFF_EN();   //开关机按键检测
		Charging_Test();   //充电检测
		
		//无任务
		if(GetTask(TASK_All) == 0)
		{
			
		}
		
		//开机任务
		if(GetTask(TASK_PowerOn))
		{
			BSP_OUTPUT_On(PWR_ON_EN);   //保持高电平
			ClearTask(TASK_PowerOn);
		}
		
		//菜单任务
		if(GetTask(TASK_Menu))
		{
			Parse_Prod_Message(); 			//等待接收消息
			ClearTask(TASK_Menu);
		}
			
			
		//关机任务
		if(GetTask(TASK_PowerOff))
		{
			BSP_OUTPUT_Off(PWR_ON_EN);   //置为低电平
			ClearTask(TASK_PowerOff);
		}
		
		//充电检测任务
		if(GetTask(TASK_CHARGIN))
		{
			//比较两次电量值是否一致
			if(BattValue_time > 10 && RealTime_BattValue != tmp_BattValue)  //(定时器值大于10s)
			{
				tmp_BattValue = RealTime_BattValue;
				
				//上传当前剩余电量  
				if(PacSend(chargin_cmd,&RealTime_BattValue,1) != AL_STS_OK)
				{
					Error_Handler();
				}
				BattValue_time = 0;
			}
			
			ClearTask(TASK_CHARGIN);
		}
		
		
		//充电完成任务
		if(GetTask(TASK_CHARGDON))
		{
			//上传剩余电量
			if((int)RealTime_BattValue == 100)  //电量为100
			{
				if(PacSend(chargdon_cmd,&RealTime_BattValue,1) != AL_STS_OK)
				{
					Error_Handler();
				}
			}
			
			tmp_BattValue = 0;   //下次充电时，初始化
			ClearTask(TASK_CHARGDON);
		}
	
		
	}
	

}




/** System Clock Configuration
*/

void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_PeriphCLKInitTypeDef PeriphClkInit;

    //Initializes the CPU, AHB and APB busses clocks 
    
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = 16;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL4;
  RCC_OscInitStruct.PLL.PREDIV = RCC_PREDIV_DIV1;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

    //Initializes the CPU, AHB and APB busses clocks 
    
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }

  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART1;
  PeriphClkInit.Usart1ClockSelection = RCC_USART1CLKSOURCE_HSI;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }

    //Configure the Systick interrupt time 
    
  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

    //Configure the Systick 
  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  //SysTick_IRQn interrupt configuration 
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}





/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
void Error_Handler(void)
{
	BSP_OUTPUT_Init(PWR_LED);
  PWR_LED_On();
  while(1)
  {
    PWR_LED_Toggle(); 
		HAL_Delay(1000);
  }
}


#ifdef USE_FULL_ASSERT

/**
   * @brief Reports the name of the source file and the source line number
   * where the assert_param error has occurred.
   * @param file: pointer to the source file name
   * @param line: assert_param error line source number
   * @retval None
   */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
    ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* Infinite loop */
  while (1)
  {
  }

}

#endif

/**
  * @}
  */ 

/**
  * @}
*/ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
