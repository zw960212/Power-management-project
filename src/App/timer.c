#include "timer.h"
#include "stm32f0xx_hal_tim.h"

TIM_HandleTypeDef htim3;
TIM_HandleTypeDef htim14;

/* TIM3 init function */
void TIM3_Init(void)
{
  TIM_ClockConfigTypeDef sClockSourceConfig;
  TIM_MasterConfigTypeDef sMasterConfig;

  htim3.Instance = TIM3;
  htim3.Init.Prescaler = 16;   //分频系数
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.Period = 999;      //自动装载值
  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }

  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;  //内部时钟源
  if (HAL_TIM_ConfigClockSource(&htim3, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }

  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }

}


/* TIM3 Deinit function */
void TIM3_DeInit(void)
{
	if (HAL_TIM_Base_DeInit(&htim3) != HAL_OK)
  {
    Error_Handler();
  }
}

void TIM3_Start(void)
{
	/*##-2- Start the TIM Base generation in interrupt mode ####################*/
  /* Start Channel1 */ 
	if (HAL_TIM_Base_Start_IT(&htim3) != HAL_OK)
  {
    // Starting Error
    Error_Handler();
  }
	//TIM3_Clear();
}


void TIM3_Clear(void)
{
	htim3.Instance->CNT = 0x00000000;
	__HAL_TIM_CLEAR_IT(&htim3,TIM_IT_UPDATE);
}


void TIM3_Stop(void)
{
	if (HAL_TIM_Base_Stop_IT(&htim3) != HAL_OK)
  {
    // Starting Error 
    Error_Handler();
  }
}



/* TIM14 init function */
void TIM14_Init(void)
{
	htim14.Instance = TIM14;
  htim14.Init.Prescaler = 15999;
  htim14.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim14.Init.Period = 999;
  htim14.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim14.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim14) != HAL_OK)
  {
    // Starting Error
    Error_Handler();
  }
}


void TIM14_Start(void)
{
	/*##-2- Start the TIM Base generation in interrupt mode ####################*/
  /* Start Channel1 */ 
	if (HAL_TIM_Base_Start_IT(&htim14) != HAL_OK)
  {
    // Starting Error
    Error_Handler();
  }
	//TIM3_Clear();
}

void TIM14_Stop(void)
{
	if (HAL_TIM_Base_Stop_IT(&htim14) != HAL_OK)
  {
    // Starting Error 
    Error_Handler();
  }
}


