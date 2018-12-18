#include "key.h"
#include "gpio.h"
#include "timer.h"
#include "Task.h"

extern volatile uint32_t key_press_time; //����ʱ��


void KEY_GPIO_Init(void)
{
	GPIO_InitTypeDef gpioinitstruct;
	
	/*Enable gpio colck*/
	PWR_OFF_EN_GPIO_CLK_ENABLE();

  gpioinitstruct.Pin = PWR_OFF_EN_PIN;
	gpioinitstruct.Mode = GPIO_MODE_INPUT;   
  gpioinitstruct.Pull = GPIO_PULLUP;       //����
	
	HAL_GPIO_Init(PWR_OFF_EN_GPIO_PORT,&gpioinitstruct);
	
}


//����ɨ����
KEYState_Typedef KEY_StateRead(void)
{
	 /*��ȡGPIO��ƽ*/
  if(HAL_GPIO_ReadPin(PWR_OFF_EN_GPIO_PORT,PWR_OFF_EN_PIN) == PWR_OFF_EN_LEVEL)
  {
    /*��ʱȥ����,��ʱ10ms*/
    HAL_Delay(10);
    
    if(HAL_GPIO_ReadPin(PWR_OFF_EN_GPIO_PORT,PWR_OFF_EN_PIN) == PWR_OFF_EN_LEVEL)
    {
			TIM3_Start(); //TIM3 START UPDATE 
      /*�ȴ���������ɨ�躯���˳�*/
      while(HAL_GPIO_ReadPin(PWR_OFF_EN_GPIO_PORT,PWR_OFF_EN_PIN) == PWR_OFF_EN_LEVEL);
			
      return KEY_DOWN;
    }
  }
	
  /*����δ������*/
  return KEY_UP;
	
}


//���ػ�������⺯��
void PWR_OFF_EN(void)
{
	KEYState_Typedef key_ret;
	key_ret = KEY_StateRead();   //�������
	
	if(key_ret == KEY_DOWN)   
	{
		if(key_press_time > 2000)  //��������3��ػ�
		{
			key_press_time = 0; 
			TIM3_Stop();  //�رն�ʱ���ж�
			ClearTask(TASK_All);
			AssignTask(TASK_PowerOff);
		}
		else
		{
			key_press_time = 0;
			TIM3_Stop();  //�رն�ʱ���ж�
			ClearTask(TASK_All);
			AssignTask(TASK_PowerOn);    //��������
		}
	}
	else                     //����δ������
	{
		return;
	}	
}


