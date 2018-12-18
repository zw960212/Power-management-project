#include "Uart_Comm.h"
#include "usart.h"
#include "gpio.h"
#include "Packet.h"
#include "Cmd_Comm.h"
#include "Battery.h"
#include "Task.h"
#include "timer.h"

/* Extern	variables ------------------------------------------------------------------*/
extern UART_HandleTypeDef PWR_UartHandle;

/* Private	variables ------------------------------------------------------------------*/
Uart_MesgID_TypeDef cmd;      //通信协议命令枚举体
uint8_t RealTime_BattValue;   //全局变量实时电量值


/* Private Functions ------------------------------------------------------------------*/


/* Extern Functions ------------------------------------------------------------------*/
extern AL_STATUS PacSend(AL_BYTE reqCommand, const AL_BYTE* reqDataPtr, AL_WORD reqLength);
extern void BSP_OUTPUT_On(Output_TypeDef OutPin);



/***串口查询接收函数
	* @brief
	* @param
	* @retval
***/
void PWR_QueryUartRcv(void)
{
	if(__HAL_UART_GET_FLAG(&PWR_UartHandle,UART_FLAG_RXNE))
	{
		AssignTask(TASK_Menu);   //注册接收任务
	}
}


/***串口信息解析
	* @brief
	* @param
	* @retval
***/
void Parse_Prod_Message(void)
{
	
	AL_WORD Uart_MsgLen = 0;       //接收数据长度
	uint8_t* Uart_RcvMsg = NULL;   //接收数据
	
		do
		{
			
			switch(PacRecv(&cmd,&Uart_RcvMsg,&Uart_MsgLen))
			{
				case AL_STS_OK:
				{
					switch(cmd)
					{
						//PC command, Query Devic batterypercent Information need response -------------------
						case Msg_Rcv_QueryBattery:
						{
							Query_Batt_SS(Uart_RcvMsg,Uart_MsgLen);	
							
						}break;
						//PC command, Turn on the equipment power Information need response -------------------
						case Msg_Rcv_OpenBattery:
						{
							Open_Batt_result(Uart_RcvMsg,Uart_MsgLen);
							
						}break;
						//PC command, Turn off the equipment power Information need response -------------------
						case Msg_Rcv_CloseBattery:
						{
							Close_Batt_result(Uart_RcvMsg,Uart_MsgLen);	
							
						}break;
						//PC command, Response results at charging begin -------------------
						case Msg_Rcv_StartCharge:
						{
							//接收上位机回复的数据，是否收到命令
							
						}break;
						//PC command, Response results at charging end -------------------
						case Msg_Rcv_EndCharge:
						{
							
						}break;
					}
				}break;
				case AL_STS_ERROR_PARAMETER:
				case AL_STS_ERROR_CHECKXOR:
				case AL_STS_ERROR_CHECKSUM:
				default:
				{
				}break;			
			}		
		}
		while(PWR_UartHandle.UartReady != IT_RESET);
 }

/***查询剩余电量
	* @brief
	* @param
	* @retval
***/
 void Query_Batt_SS(uint8_t* frame, uint16_t frameLen)
 {
	 uint8_t data[20] = {0};   //接收到的数据
	 DataCompress(frame,frameLen,data);
	 
	 if(data[0] == 0x52)   //解析数据
	 {
		 uint8_t BattValue;
		 BattValue = BatteryCheck();   //获取电量函数
		 
		 //上传剩余电量
		 if(PacSend(cmd,&BattValue,1) != AL_STS_OK)
		 {
			 Error_Handler();
		 }
	 } 
		
 }
 
 
 /***开启电源操作
	* @brief
	* @param
	* @retval
***/
void Open_Batt_result(uint8_t* frame, uint16_t frameLen)
{
	uint8_t data[20] = {0};   //接收到的数据
	DataCompress(frame,frameLen,data);
	
	switch(data[0]) 
	{
		case 0x01:     //开启指静脉模块电源
		{
			AL_BYTE Data[2] = {0x01,0x01};
			BSP_OUTPUT_On(PWR_VM);
			
			//上传应答电源开启结果   
			if(PacSend(cmd,Data,2) != AL_STS_OK)
			{
				Error_Handler();
  		}	
		}break;
		
		case 0x02:     //开启身份证模块电源
		{
			AL_BYTE Data[2] = {0x02,0x01};
			BSP_OUTPUT_On(PWR_IDCARD);
			
			//上传应答电源开启结果   
			if(PacSend(cmd,Data,2) != AL_STS_OK)
			{
				Error_Handler();
			}
		}break;
		
		case 0x03:     //开启摄像头电源
		{
			AL_BYTE Data[2] = {0x03,0x01};
			BSP_OUTPUT_On(PWR_CAMERA);
			
			//上传应答电源开启结果   
			if(PacSend(cmd,Data,2) != AL_STS_OK)
			{
				Error_Handler();
			}
		}break;
		
		case 0x04:     //开启指示灯电源
		{
			AL_BYTE Data[2] = {0x04,0x01};
			BSP_OUTPUT_On(PWR_LED);
			
			//上传应答电源开启结果   
			if(PacSend(cmd,Data,2) != AL_STS_OK)
			{
				Error_Handler();
			}
		}break;
		default:break;
	}
	
}


/***关闭电源操作
	* @brief
	* @param
	* @retval
***/
void Close_Batt_result(uint8_t* frame, uint16_t frameLen)
{
	
	uint8_t data[20] = {0};   //接收到的数据
	DataCompress(frame,frameLen,data);
	
	switch(data[0]) 
	{
		case 0x01:     //关闭指静脉模块电源
		{
			AL_BYTE Data[2] = {0x01,0x01};
			BSP_OUTPUT_Off(PWR_VM);
			
			//上传应答电源关闭结果   
			if(PacSend(cmd,Data,2) != AL_STS_OK)
			{
				Error_Handler();
			}
		}break;
		
		case 0x02:     //关闭身份证模块电源
		{
			AL_BYTE Data[2] = {0x02,0x01};
			BSP_OUTPUT_Off(PWR_IDCARD);
			
			//上传应答电源关闭结果   
			if(PacSend(cmd,Data,2) != AL_STS_OK)
			{
				Error_Handler();
			}
		}break;
		
		case 0x03:     //关闭摄像头电源
		{
			AL_BYTE Data[2] = {0x03,0x01};
			BSP_OUTPUT_Off(PWR_CAMERA);
			
			//上传应答电源关闭结果   
			if(PacSend(cmd,Data,2) != AL_STS_OK)
			{
				Error_Handler();
			}
		}break;
		
		case 0x04:     //关闭指示灯电源
		{
			AL_BYTE Data[2] = {0x04,0x01};
			BSP_OUTPUT_Off(PWR_LED);
			
			//上传应答电源关闭结果   
			if(PacSend(cmd,Data,2) != AL_STS_OK)
			{
				Error_Handler();
			}
		}break;
		default:break;
	}		
	
}


/***编写一个检测电源充电开始和充电结束的函数
	* @brief
	* @param
	* @retval
***/
void Charging_Test(void)
{
	BSP_INPUT_Init(CHARGIN_EN,INPUT_MODE_GPIO);
	BSP_INPUT_Init(CHARGDON_EN,INPUT_MODE_GPIO);
	
	//检测充电开始
	if(BSP_INPUT_GetState(CHARGIN_EN) == 0 && BSP_INPUT_GetState(CHARGDON_EN) == 1)
	{
		
		RealTime_BattValue = BatteryCheck();   //获取当前实时电量函数
		AssignTask(TASK_CHARGIN);   //注册充电检测任务
		TIM14_Start();  //开启计时功能
	}
	
	//检测充电完成
	else if(BSP_INPUT_GetState(CHARGIN_EN) == 1 && BSP_INPUT_GetState(CHARGDON_EN) == 0)
	{
		RealTime_BattValue = BatteryCheck();   //获取电量函数
		AssignTask(TASK_CHARGDON);  //注册充电完成任务
		TIM14_Stop();  //停止计时功能
	}
	
	else
	{
		return;
	}
	
}




 
 
