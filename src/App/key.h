#ifndef __KEY_H
#define __KEY_H
#ifdef __cplusplus
 extern "C" {
#endif
	 
/* Includes ------------------------------------------------------------------*/
#include "stm32f0xx_hal.h"
#include "main.h"

	 
/*�������Ͷ���*/
typedef enum
{
	KEY_UP = 0,
	KEY_DOWN  = 1,	
}KEYState_Typedef;
	

/*�궨��*/
#define PWR_OFF_EN_PIN                        GPIO_PIN_1
#define PWR_OFF_EN_GPIO_PORT                  GPIOB
#define PWR_OFF_EN_GPIO_CLK_ENABLE()        	__HAL_RCC_GPIOB_CLK_ENABLE()  
#define PWR_OFF_EN_GPIO_CLK_DISABLE()       	__HAL_RCC_GPIOB_CLK_DISABLE()

#define PWR_OFF_EN_LEVEL                      0 /*��������ʱΪ�͵�ƽ*/

	 
/*Export Functions----------------------------------------------*/
void KEY_GPIO_Init(void);
KEYState_Typedef KEY_StateRead(void);
void PWR_OFF_EN(void);	 	  
	 
#ifdef __cplusplus
}
#endif
#endif /*__ KEY_H */


