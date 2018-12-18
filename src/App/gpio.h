/**
  ******************************************************************************
  * File Name          : gpio.h
  * Description        : This file contains all the functions prototypes for 
  *                      the gpio  
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
#ifndef __gpio_H
#define __gpio_H
#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f0xx_hal.h"
#include "main.h"

/* Export function-------------------------------------------------*/
extern void Output_Init(void);
extern void PWR_LED_On(void);
extern void PWR_LED_Off(void);
extern void PWR_LED_Toggle(void);	 
	 

/** @defgroup STM32F0XX_NUCLEO_Exported_Types Exported Types
  * @{
  */ 
typedef enum 
{
  PWR_VM = 0,
	PWR_LED = 1,
	PWR_CAMERA = 2,
	PWR_IDCARD = 3,
	BATT_MONITOR_EN = 4,
	PWR_ON_EN  = 5
	
} Output_TypeDef;
 
typedef enum 
{ 
  CHARGIN_EN = 0,
	CHARGDON_EN = 1
	
} Input_TypeDef;

typedef enum 
{  
  INPUT_MODE_GPIO = 0,
  INPUT_MODE_EXTI = 1
} InputMode_TypeDef; 


/** @defgroup STM32F0XX_NUCLEO_OUTPIN OUTPIN Constants
  * @{
  */
#define OUTPUTn                               7

#define PWR_VM_PIN                            GPIO_PIN_15
#define PWR_VM_GPIO_PORT                      GPIOA
#define PWR_VM_GPIO_CLK_ENABLE()            	__HAL_RCC_GPIOA_CLK_ENABLE()  
#define PWR_VM_GPIO_CLK_DISABLE()           	__HAL_RCC_GPIOA_CLK_DISABLE()	 
	 
#define PWR_LED_PIN                           GPIO_PIN_5
#define PWR_LED_GPIO_PORT                     GPIOB
#define PWR_LED_GPIO_CLK_ENABLE()           	__HAL_RCC_GPIOB_CLK_ENABLE()  
#define PWR_LED_GPIO_CLK_DISABLE()          	__HAL_RCC_GPIOB_CLK_DISABLE()	

#define PWR_CAMERA_PIN                        GPIO_PIN_3
#define PWR_CAMERA_GPIO_PORT                  GPIOB
#define PWR_CAMERA_GPIO_CLK_ENABLE()        	__HAL_RCC_GPIOB_CLK_ENABLE()  
#define PWR_CAMERA_GPIO_CLK_DISABLE()      	 	__HAL_RCC_GPIOB_CLK_DISABLE()	

#define PWR_IDCARD_PIN                        GPIO_PIN_4
#define PWR_IDCARD_GPIO_PORT                  GPIOB
#define PWR_IDCARD_GPIO_CLK_ENABLE()        	__HAL_RCC_GPIOB_CLK_ENABLE()  
#define PWR_IDCARD_GPIO_CLK_DISABLE()       	__HAL_RCC_GPIOB_CLK_DISABLE()	

#define BATT_MONITOR_PIN                      GPIO_PIN_4
#define BATT_MONITOR_GPIO_PORT                GPIOA
#define BATT_MONITOR_GPIO_CLK_ENABLE()        __HAL_RCC_GPIOA_CLK_ENABLE()  
#define BATT_MONITOR_GPIO_CLK_DISABLE()       __HAL_RCC_GPIOA_CLK_DISABLE()

#define PWR_ON_EN_PIN                         GPIO_PIN_0
#define PWR_ON_EN_GPIO_PORT                   GPIOB
#define PWR_ON_EN_GPIO_CLK_ENABLE()        	  __HAL_RCC_GPIOB_CLK_ENABLE()  
#define PWR_ON_EN_GPIO_CLK_DISABLE()       	  __HAL_RCC_GPIOB_CLK_DISABLE()	


#define OUTPUT_CLK_ENABLE(__INDEX__)   do { switch(__INDEX__){case 0:{PWR_VM_GPIO_CLK_ENABLE();}break;\
																															case 1:{PWR_LED_GPIO_CLK_ENABLE();}break;\
																															case 2:{PWR_CAMERA_GPIO_CLK_ENABLE();}break;\
																															case 3:{PWR_IDCARD_GPIO_CLK_ENABLE();}break;\
																															case 4:{BATT_MONITOR_GPIO_CLK_ENABLE();}break;\
																															case 5:{PWR_ON_EN_GPIO_CLK_ENABLE();}break;\
																															default:break;}} while(0)
																														
#define OUTPUT_CLK_DISABLE(__INDEX__)  do { switch(__INDEX__){case 0:{PWR_VM_GPIO_CLK_DISABLE();}break;\
																															case 1:{PWR_LED_GPIO_CLK_DISABLE();}break;\
																															case 2:{PWR_CAMERA_GPIO_CLK_DISABLE();}break;\
																															case 3:{PWR_IDCARD_GPIO_CLK_DISABLE();}break;\
																															case 4:{BATT_MONITOR_GPIO_CLK_DISABLE();}break;\
																															case 5:{PWR_ON_EN_GPIO_CLK_DISABLE();}break;\
																															default:break;}} while(0)		

/**
  * @}
  */ 
  
/** @defgroup STM32F0XX_NUCLEO_BUTTON BUTTON Constants
  * @{
  */ 

#define INPUTn    2

/**
  * @brief Charge In Wakeup pin
  */		
																															
#define CHARGIN_EN_PIN                       GPIO_PIN_7
#define CHARGIN_EN_GPIO_PORT                 GPIOB
#define CHARGIN_EN_GPIO_CLK_ENABLE()         __HAL_RCC_GPIOB_CLK_ENABLE()   
#define CHARGIN_EN_GPIO_CLK_DISABLE()        __HAL_RCC_GPIOB_CLK_DISABLE()  
																															
#define CHARGDON_EN_PIN                       GPIO_PIN_6
#define CHARGDON_EN_GPIO_PORT                 GPIOB
#define CHARGDON_EN_GPIO_CLK_ENABLE()         __HAL_RCC_GPIOB_CLK_ENABLE()   
#define CHARGDON_EN_GPIO_CLK_DISABLE()        __HAL_RCC_GPIOB_CLK_DISABLE()		

																														
#define INPUT_CLK_ENABLE(__INDEX__)    do { switch(__INDEX__){case 0:{CHARGIN_EN_GPIO_CLK_ENABLE();}break;\
																															case 1:{CHARGDON_EN_GPIO_CLK_ENABLE();}break;\
																															default:break;}} while(0)

#define INPUT_CLK_DISABLE(__INDEX__)   do { switch(__INDEX__){case 0:{CHARGIN_EN_GPIO_CLK_DISABLE();}break;\
																															case 1:{CHARGDON_EN_GPIO_CLK_DISABLE();}break;\
																															default:break;}} while(0)																															
																																																														

/** @defgroup STM32F0XX_NUCLEO_LED_Functions  LED Functions
* @{
  */ 
extern void BSP_OUTPUT_Init(Output_TypeDef OutPin);
extern void BSP_OUTPUT_DeInit(Output_TypeDef OutPin);
extern void BSP_OUTPUT_On(Output_TypeDef OutPin);
extern void BSP_OUTPUT_Off(Output_TypeDef OutPin);
extern void BSP_OUTPUT_Toggle(Output_TypeDef OutPin);

																															
/** @addtogroup STM32F0XX_NUCLEO_INPUT_Functions
  * @{
  */
extern void      BSP_INPUT_Init(Input_TypeDef Button, InputMode_TypeDef ButtonMode);
extern void      BSP_INPUT_DeInit(Input_TypeDef Button);
extern uint32_t  BSP_INPUT_GetState(Input_TypeDef Button);																														
extern void GPIO_All_Reset(void);																															
																															
																															
#ifdef __cplusplus
}
#endif
#endif /*__ pinoutConfig_H */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
