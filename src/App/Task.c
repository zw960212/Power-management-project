#include "Task.h"

/* Private macro -------------------------------------------------------------*/


/* Extern functions ---------------------------------------------------------*/


/* Private variables ---------------------------------------------------------*/


/* task bit flag */
volatile static uint32_t TaskFlag = 0;
volatile static uint32_t TaskFlagBackup = 0;


/**
  * @brief  task program
  * @param  None
  * @retval None
  */
void AssignTask(uint32_t taskMask)
{
	TaskFlag |= taskMask;
}
void ClearTask(uint32_t taskMask)
{
	TaskFlag &= ~taskMask;
}
uint32_t GetTask(uint32_t taskMask)
{
	return (TaskFlag & taskMask);
}

void TaskPush(void)
{
	TaskFlagBackup = TaskFlag;
}

void TaskPop(void)
{
	TaskFlag = TaskFlagBackup;
}


