/**
  ******************************************************************************
  * File Name          : gpio.c
  * Description        : This file provides code for the configuration
  *                      of all used GPIO pins.
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
#include "gpio.h"
#include "stm32f0xx_hal_gpio.h"


/**
  * @}
  */ 

/** @defgroup STM32F0XX_NUCLEO_Private_Variables Private Variables
  * @{
  */ 
const GPIO_TypeDef*  OUTPUT_PORT[] = {PWR_VM_GPIO_PORT\
																			,PWR_LED_GPIO_PORT\
																			,PWR_CAMERA_GPIO_PORT\
																			,PWR_IDCARD_GPIO_PORT\
																			,BATT_MONITOR_GPIO_PORT\
																			,PWR_ON_EN_GPIO_PORT
	
																			};

const uint16_t OUTPUT_PIN[] = {PWR_VM_PIN\
																			,PWR_LED_PIN\
																			,PWR_CAMERA_PIN\
																			,PWR_IDCARD_PIN\
																			,BATT_MONITOR_PIN\
																			,PWR_ON_EN_PIN
																		
																			};																			

																			
const GPIO_PinState  OUTPUT_ON[] = {GPIO_PIN_SET\
																		,GPIO_PIN_SET\
																		,GPIO_PIN_SET\
																		,GPIO_PIN_SET\
																		,GPIO_PIN_SET\
																		,GPIO_PIN_SET
																		
																		};

																		
const GPIO_PinState  OUTPUT_OFF[] = {GPIO_PIN_RESET\
																		,GPIO_PIN_RESET\
																		,GPIO_PIN_RESET\
																		,GPIO_PIN_RESET\
																		,GPIO_PIN_RESET\
																		,GPIO_PIN_RESET
																		
																		};																		

const GPIO_TypeDef*  INPUT_PORT[] = { CHARGIN_EN_GPIO_PORT,\
																		  CHARGDON_EN_GPIO_PORT
	
																		};			

const uint16_t       INPUT_PIN[]  = { CHARGIN_EN_PIN,\
																		  CHARGDON_EN_PIN
																
																		};																		
																																																																					
/*----------------------------------------------------------------------------*/
/* Configure GPIO                                                             */
/*----------------------------------------------------------------------------*/																		
																		
/**
  * @brief  gpio all reset
  * @param  None
  * @retval None
  */
void GPIO_All_Reset(void)
{
  GPIO_InitTypeDef GPIO_InitStruct;

  /* Allow access to Backup */
  //HAL_PWR_EnableBkUpAccess();
	
  /* Configure all GPIO as analog to reduce current consumption on non used IOs */
  /* Warning : Reconfiguring all GPIO will close the connexion with the debugger */
  /* Enable GPIOs clock */

  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

	
  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
  GPIO_InitStruct.Pull = GPIO_NOPULL;

  GPIO_InitStruct.Pin = (GPIO_PIN_All);
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = GPIO_PIN_All;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /* Disable GPIOs clock */
  __HAL_RCC_GPIOA_CLK_DISABLE();
  __HAL_RCC_GPIOB_CLK_DISABLE();
	
}																		


/**
  * @brief  Configures LED GPIO.
  * @param  OutPin OutPin to be configured. 
  *         This parameter can be one of the following values:
  * @arg LED2
  * @retval None
  */
void BSP_OUTPUT_Init(Output_TypeDef OutPin)
{
  GPIO_InitTypeDef  gpioinitstruct;
  
  /* Enable the GPIO_LED Clock */
  OUTPUT_CLK_ENABLE(OutPin);

  /* Configure the GPIO_LED pin */
  gpioinitstruct.Pin = OUTPUT_PIN[OutPin];
  gpioinitstruct.Mode = GPIO_MODE_OUTPUT_PP; //推挽输出模式
  gpioinitstruct.Pull = GPIO_NOPULL;
  gpioinitstruct.Speed = GPIO_SPEED_FREQ_HIGH;
  
  HAL_GPIO_Init((GPIO_TypeDef*)OUTPUT_PORT[OutPin], &gpioinitstruct);
	BSP_OUTPUT_Off(OutPin);
}


/**
  * @brief  DeInit LEDs.
  * @param  OutPin LED to be de-init. 
  *   This parameter can be one of the following values:
  *     @arg  LED2
  * @note OutPin DeInit does not disable the GPIO clock nor disable the Mfx 
  * @retval None
  */
void BSP_OUTPUT_DeInit(Output_TypeDef OutPin)
{
  GPIO_InitTypeDef  gpio_init_structure;

  /* Turn off LED */
  HAL_GPIO_WritePin((GPIO_TypeDef*)OUTPUT_PORT[OutPin], OUTPUT_PIN[OutPin], GPIO_PIN_RESET);
  /* DeInit the GPIO_LED pin */
  gpio_init_structure.Pin = OUTPUT_PIN[OutPin];
  HAL_GPIO_DeInit((GPIO_TypeDef*)OUTPUT_PORT[OutPin], gpio_init_structure.Pin);
}

/**
  * @brief  Turns selected LED On, this function took 7 us
  * @param  OutPin Specifies the OutPin to be set on. 
  *   This parameter can be one of following parameters:
  *     @arg LED2
  * @retval None
  */
void BSP_OUTPUT_On(Output_TypeDef OutPin)
{
  HAL_GPIO_WritePin((GPIO_TypeDef*)OUTPUT_PORT[OutPin], OUTPUT_PIN[OutPin], OUTPUT_ON[OutPin]); 
}

/**
  * @brief  Turns selected LED Off.
  * @param  OutPin Specifies the OutPin to be set off. 
  *   This parameter can be one of following parameters:
  *     @arg LED2
  * @retval None
  */
void BSP_OUTPUT_Off(Output_TypeDef OutPin)
{
  HAL_GPIO_WritePin((GPIO_TypeDef*)OUTPUT_PORT[OutPin], OUTPUT_PIN[OutPin], OUTPUT_OFF[OutPin]); 
}

/**
  * @brief  Toggles the selected LED.
  * @param  OutPin Specifies the OutPin to be toggled. 
  *   This parameter can be one of following parameters:
  *     @arg LED2
  * @retval None
  */
void BSP_OUTPUT_Toggle(Output_TypeDef OutPin)
{
  HAL_GPIO_TogglePin((GPIO_TypeDef*)OUTPUT_PORT[OutPin], OUTPUT_PIN[OutPin]);
}


/**
  * @brief  Configures Button GPIO and EXTI Line.
  * @param  Button Specifies the Button to be configured.
  *   This parameter should be: RTC_WAKEUP
  * @param  ButtonMode Specifies Button mode.
  *   This parameter can be one of following parameters:   
  *     @arg INPUT_MODE_GPIO: Button will be used as simple IO
  *     @arg INPUT_MODE_EXTI: Button will be connected to EXTI line with interrupt
  *                            generation capability  
  * @retval None
  */
void BSP_INPUT_Init(Input_TypeDef Button, InputMode_TypeDef InputMode)
{
  GPIO_InitTypeDef gpioinitstruct;

	INPUT_CLK_ENABLE(Button);

  gpioinitstruct.Pin = INPUT_PIN[Button];
  gpioinitstruct.Pull = GPIO_NOPULL;
  gpioinitstruct.Speed = GPIO_SPEED_FREQ_HIGH;

  if(InputMode == INPUT_MODE_GPIO)
  {
    /* Configure Button pin as input */
    gpioinitstruct.Mode = GPIO_MODE_INPUT;

    HAL_GPIO_Init((GPIO_TypeDef*)INPUT_PORT[Button], &gpioinitstruct);
  }
	
}

/**
  * @brief  Push Button DeInit.
  * @param  Button Button to be configured
  *   This parameter should be: RTC_WAKEUP
  * @note PB DeInit does not disable the GPIO clock
  * @retval None
  */
void BSP_INPUT_DeInit(Input_TypeDef Button)
{
  GPIO_InitTypeDef gpio_init_structure;

  gpio_init_structure.Pin = INPUT_PIN[Button];
  HAL_GPIO_DeInit((GPIO_TypeDef*)INPUT_PORT[Button], gpio_init_structure.Pin);  
}

/**
  * @brief  Returns the selected Button state.
  * @param  Button Specifies the Button to be checked.
  *   This parameter should be: RTC_WAKEUP
  * @retval Button state.
  */
uint32_t BSP_INPUT_GetState(Input_TypeDef input)
{
  return HAL_GPIO_ReadPin((GPIO_TypeDef*)INPUT_PORT[input], INPUT_PIN[input]);
}



/**init all output
  * @}
  */ 
void Output_Init(void)
{
	BSP_OUTPUT_Init(PWR_VM);
	BSP_OUTPUT_Init(PWR_LED);
	BSP_OUTPUT_Init(PWR_CAMERA);
	BSP_OUTPUT_Init(PWR_IDCARD);
	BSP_OUTPUT_Init(BATT_MONITOR_EN);   //输出adc检测的电量
	BSP_OUTPUT_Init(PWR_ON_EN);
}

/**
  * @brief  Configures pwr led on
  * @param  none
  * @retval None
  */
void PWR_LED_On(void)
{
	BSP_OUTPUT_On(PWR_LED);
}

/**
  * @brief  Configures pwr led off
  * @param  none
  * @retval None
  */
void PWR_LED_Off(void)
{
	BSP_OUTPUT_Off(PWR_LED);
}

/**
  * @brief  Configures pwr led toggle
  * @param  none
  * @retval None
  */
void PWR_LED_Toggle(void)
{
	BSP_OUTPUT_Toggle(PWR_LED);
}



/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
