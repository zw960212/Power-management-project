#include "key.h"
#include "gpio.h"
#include "timer.h"
#include "Task.h"

extern volatile uint32_t key_press_time; //按键时间


void KEY_GPIO_Init(void)
{
	GPIO_InitTypeDef gpioinitstruct;
	
	/*Enable gpio colck*/
	PWR_OFF_EN_GPIO_CLK_ENABLE();

  gpioinitstruct.Pin = PWR_OFF_EN_PIN;
	gpioinitstruct.Mode = GPIO_MODE_INPUT;   
  gpioinitstruct.Pull = GPIO_PULLUP;       //上拉
	
	HAL_GPIO_Init(PWR_OFF_EN_GPIO_PORT,&gpioinitstruct);
	
}


//按键扫描检测
KEYState_Typedef KEY_StateRead(void)
{
	 /*读取GPIO电平*/
  if(HAL_GPIO_ReadPin(PWR_OFF_EN_GPIO_PORT,PWR_OFF_EN_PIN) == PWR_OFF_EN_LEVEL)
  {
    /*延时去抖动,延时10ms*/
    HAL_Delay(10);
    
    if(HAL_GPIO_ReadPin(PWR_OFF_EN_GPIO_PORT,PWR_OFF_EN_PIN) == PWR_OFF_EN_LEVEL)
    {
			TIM3_Start(); //TIM3 START UPDATE 
      /*等待按键弹开扫描函数退出*/
      while(HAL_GPIO_ReadPin(PWR_OFF_EN_GPIO_PORT,PWR_OFF_EN_PIN) == PWR_OFF_EN_LEVEL);
			
      return KEY_DOWN;
    }
  }
	
  /*按键未被按下*/
  return KEY_UP;
	
}


//开关机按键检测函数
void PWR_OFF_EN(void)
{
	KEYState_Typedef key_ret;
	key_ret = KEY_StateRead();   //按键检测
	
	if(key_ret == KEY_DOWN)   
	{
		if(key_press_time > 2000)  //按键按下3秒关机
		{
			key_press_time = 0; 
			TIM3_Stop();  //关闭定时器中断
			ClearTask(TASK_All);
			AssignTask(TASK_PowerOff);
		}
		else
		{
			key_press_time = 0;
			TIM3_Stop();  //关闭定时器中断
			ClearTask(TASK_All);
			AssignTask(TASK_PowerOn);    //开机任务
		}
	}
	else                     //按键未被按下
	{
		return;
	}	
}


