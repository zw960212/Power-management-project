#include "Battery.h"
#include "gpio.h"

#define	MAX_HEX_12BIT			4096
#define	MAX_VOLTAGE				330  //参考电压
#define MAX_BATTERY				270  //最大峰值电压  

/* Public variables ---------------------------------------------------------*/
/* ADC handle declaration */
extern ADC_HandleTypeDef             AdcHandle;

/* ADC channel configuration structure declaration */
extern ADC_ChannelConfTypeDef        sConfig;

/* Variable containing ADC conversions data */
extern uint16_t   aADCxConvertedData[];

/* Public Functions -------------------------------------------------------------*/
extern HAL_StatusTypeDef  ADCx_Init(void);
extern void               ADCx_DeInit(void);


/*********************** BATTERY POWER TEST **************************/

/**
  * @brief  Configures battery test available on
  *         managed through ADC to detect motion.
  * @retval Joystickstatus (0=> success, 1=> fail) 
  */
uint8_t BatteryInit(void)
{
	BSP_OUTPUT_Init(BATT_MONITOR_EN);
	BSP_OUTPUT_On(BATT_MONITOR_EN);
  if (ADCx_Init() != HAL_OK)
  {
    return (uint8_t) HAL_ERROR; 
  }
  
  /* Select Channel 8 to be converted */
  sConfig.Channel = ADC_CHANNEL_3;
  sConfig.Rank    = ADC_RANK_CHANNEL_NUMBER;

  /* Return Joystick initialization status */
  return (uint8_t)HAL_ADC_ConfigChannel(&AdcHandle, &sConfig);
}

/**
  * @brief  DeInit joystick GPIOs.
  * @note   battery test DeInit does not disable the Mfx, just set the Mfx pins in Off mode
  * @retval None.
  */
void BatteryDeInit(void)
{
	BSP_OUTPUT_Off(BATT_MONITOR_EN);
	BSP_OUTPUT_DeInit(BATT_MONITOR_EN);
	ADCx_DeInit();
}

/**
  * @brief  Returns power test result
  * @note   
  * @retval : get power voltage * 100
  */
uint16_t BatteryGetValue(void)
{
  uint16_t  hex = 0;
	uint16_t	v = 0;
  float factor;
 /* Start the conversion process */
  HAL_ADC_Start(&AdcHandle);
  
  /* Wait for the end of conversion */
  if (HAL_ADC_PollForConversion(&AdcHandle, 10) != HAL_TIMEOUT)
  {
    /* Get the converted value of regular channel */
    hex = HAL_ADC_GetValue(&AdcHandle);
  } 
	factor = (float)hex / MAX_HEX_12BIT;   //测量到的电压值
	v = factor * MAX_VOLTAGE;              //实际测出的电压值（经过分压电路分压的值）
  return v;
}

/**
  * @brief  battery capacity check
  * @param  None
  * @retval return percent of battery
  */
uint8_t BatteryCheck(void)
{
	uint16_t volt;
	uint16_t percent;
	float temp_volt;
	BatteryInit();
	volt = BatteryGetValue();  //实际电压值
	BatteryDeInit();
	
	//这里volt分为三段线性公式模拟计算
	if(170 < (int)volt && ((int)volt < 232 || (int)volt == 232))   //第一段
	{
		temp_volt = (float)((int)volt - 170) / (float)620;
		percent = temp_volt * 100;
	}
	
	else if((232 < (int)volt) && ((int)volt < 247 || (int)volt == 247))  //第二段
	{
		temp_volt = (float)((int)volt - 229) / (float)25;
		percent = temp_volt * 100;
	}
	
	else if((247 < (int)volt) && ((int)volt < MAX_BATTERY || (int)volt == 264))  //第三段
	{
		temp_volt = (float)((int)volt - 207) / (float)57; 
		percent = temp_volt * 100;
	}
	else
	{ 
		percent = -1;
	}
	
	return percent;
}

