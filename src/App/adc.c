/**
  ******************************************************************************
  * File Name          : ADC.c
  * Description        : This file provides code for the configuration
  *                      of the ADC instances.
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
#include "adc.h"

#include "gpio.h"


/* Definition of ADCx conversions data table size */
#define ADC_CONVERTED_DATA_BUFFER_SIZE   ((uint32_t)  1)   /* Size of array aADCxConvertedData[] */

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* ADC handle declaration */
ADC_HandleTypeDef             AdcHandle;

/* ADC channel configuration structure declaration */
ADC_ChannelConfTypeDef        sConfig;

/* Definition of ADCx conversions data table size*/
static uint16_t   aADCxConvertedData[ADC_CONVERTED_DATA_BUFFER_SIZE];

/* Private Functions -------------------------------------------------------------*/
HAL_StatusTypeDef  ADCx_Init(void);
void               ADCx_DeInit(void);
static void        ADCx_MspInit(ADC_HandleTypeDef *hadc);
static void        ADCx_MspDeInit(ADC_HandleTypeDef *hadc);



/**
  * @brief  Initialize ADC MSP.
  * @retval None
  */
static void ADCx_MspInit(ADC_HandleTypeDef *hadc)
{
  GPIO_InitTypeDef  gpioinitstruct;
  
  /*** Configure the GPIOs ***/  
  /* Enable GPIO clock */
  MC20PWR_ADCx_GPIO_CLK_ENABLE();
  
  /* Configure ADC1 Channel3 as analog input */
  gpioinitstruct.Pin = MC20PWR_ADCx_GPIO_PIN ;
  gpioinitstruct.Mode = GPIO_MODE_ANALOG;
  HAL_GPIO_Init(MC20PWR_ADCx_GPIO_PORT, &gpioinitstruct);

  /* Enable ADC clock */
  MC20PWR_ADCx_CLK_ENABLE();
}


/**
  * @brief  DeInitializes ADC MSP.
  * @note ADC DeInit does not disable the GPIO clock
  * @retval None
  */
static void ADCx_MspDeInit(ADC_HandleTypeDef *hadc)
{
  GPIO_InitTypeDef  gpioinitstruct;

  /* Disable ADC clock */
  MC20PWR_ADCx_CLK_DISABLE(); 

  /* Configure the selected ADC Channel as analog input */
  gpioinitstruct.Pin = MC20PWR_ADCx_GPIO_PIN ;
  HAL_GPIO_DeInit(MC20PWR_ADCx_GPIO_PORT, gpioinitstruct.Pin);

}


/**
  * @brief  Initializes ADC HAL.
  * @retval None
  */
HAL_StatusTypeDef ADCx_Init(void)
{
  /* Set ADC instance */
  AdcHandle.Instance                     = MC20PWR_ADCx;
  
  if(HAL_ADC_GetState(&AdcHandle) == HAL_ADC_STATE_RESET)
  {
    /* ADC Config */
    AdcHandle.Init.ClockPrescaler        = ADC_CLOCK_SYNC_PCLK_DIV2; //(8MHz)     /* ADC clock of STM32F0 must not exceed 14MHz (PLLCK=16MHz)*/
    AdcHandle.Init.Resolution            = ADC_RESOLUTION_12B;
    AdcHandle.Init.DataAlign             = ADC_DATAALIGN_RIGHT;           /*Align the results right*/
    AdcHandle.Init.ScanConvMode          = ADC_SCAN_DIRECTION_FORWARD;    /* Sequencer will convert the number of channels configured below, successively from the lowest to the highest channel number */
    AdcHandle.Init.EOCSelection          = ADC_EOC_SINGLE_CONV;           /*EOC做轮询或中断标记*/
    AdcHandle.Init.LowPowerAutoWait      = DISABLE;
    AdcHandle.Init.LowPowerAutoPowerOff  = DISABLE;
    AdcHandle.Init.ContinuousConvMode    = DISABLE;                       /* Continuous mode disabled to have only 1 conversion at each conversion trig */
    AdcHandle.Init.DiscontinuousConvMode = DISABLE;                       /* Parameter discarded because sequencer is disabled */
    AdcHandle.Init.ExternalTrigConv      = ADC_SOFTWARE_START;            /* Software start to trig the 1st conversion manually, without external event */
    AdcHandle.Init.ExternalTrigConvEdge  = ADC_EXTERNALTRIGCONVEDGE_NONE; /* Parameter discarded because trig by software start */
    AdcHandle.Init.DMAContinuousRequests = DISABLE;
    AdcHandle.Init.Overrun               = ADC_OVR_DATA_OVERWRITTEN;
    AdcHandle.Init.SamplingTimeCommon    = ADC_SAMPLETIME_41CYCLES_5;     /*采样时间*/
    
    /* Initialize MSP related to ADC */
    ADCx_MspInit(&AdcHandle);
    
    /* Initialize ADC */
    if (HAL_ADC_Init(&AdcHandle) != HAL_OK)
    {
      return HAL_ERROR;
    }

    /* Run ADC calibration */
    if (HAL_ADCEx_Calibration_Start(&AdcHandle) != HAL_OK)
    {
      return HAL_ERROR;
    }
  }
  
  return HAL_OK;
}


/**
  * @brief  Initializes ADC HAL.
  * @retval None
  */
void ADCx_DeInit(void)
{
    AdcHandle.Instance   = MC20PWR_ADCx;
    
    HAL_ADC_DeInit(&AdcHandle);
    ADCx_MspDeInit(&AdcHandle);
}



void HAL_ADC_MspInit(ADC_HandleTypeDef* adcHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct;
  if(adcHandle->Instance==ADC1)
  {
    /* ADC1 clock enable */
    __HAL_RCC_ADC1_CLK_ENABLE();
  
    /**ADC GPIO Configuration    
    PA3     ------> ADC_IN3 
    */
    GPIO_InitStruct.Pin = GPIO_PIN_3;
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* ADC1 interrupt Init */
    HAL_NVIC_SetPriority(ADC1_IRQn, 0, 1);
    HAL_NVIC_EnableIRQ(ADC1_IRQn);
		
  }
}

void HAL_ADC_MspDeInit(ADC_HandleTypeDef* adcHandle)
{

  if(adcHandle->Instance==ADC1)
  {
    /* Peripheral clock disable */
    __HAL_RCC_ADC1_CLK_DISABLE();
  
    /**ADC GPIO Configuration    
    PA3     ------> ADC_IN3 
    */
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_3);

    /* ADC1 interrupt Deinit */
    HAL_NVIC_DisableIRQ(ADC1_IRQn);
  
  }
} 


/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
