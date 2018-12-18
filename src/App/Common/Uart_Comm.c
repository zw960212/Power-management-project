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
Uart_MesgID_TypeDef cmd;      //ͨ��Э������ö����
uint8_t RealTime_BattValue;   //ȫ�ֱ���ʵʱ����ֵ


/* Private Functions ------------------------------------------------------------------*/


/* Extern Functions ------------------------------------------------------------------*/
extern AL_STATUS PacSend(AL_BYTE reqCommand, const AL_BYTE* reqDataPtr, AL_WORD reqLength);
extern void BSP_OUTPUT_On(Output_TypeDef OutPin);



/***���ڲ�ѯ���պ���
	* @brief
	* @param
	* @retval
***/
void PWR_QueryUartRcv(void)
{
	if(__HAL_UART_GET_FLAG(&PWR_UartHandle,UART_FLAG_RXNE))
	{
		AssignTask(TASK_Menu);   //ע���������
	}
}


/***������Ϣ����
	* @brief
	* @param
	* @retval
***/
void Parse_Prod_Message(void)
{
	
	AL_WORD Uart_MsgLen = 0;       //�������ݳ���
	uint8_t* Uart_RcvMsg = NULL;   //��������
	
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
							//������λ���ظ������ݣ��Ƿ��յ�����
							
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

/***��ѯʣ�����
	* @brief
	* @param
	* @retval
***/
 void Query_Batt_SS(uint8_t* frame, uint16_t frameLen)
 {
	 uint8_t data[20] = {0};   //���յ�������
	 DataCompress(frame,frameLen,data);
	 
	 if(data[0] == 0x52)   //��������
	 {
		 uint8_t BattValue;
		 BattValue = BatteryCheck();   //��ȡ��������
		 
		 //�ϴ�ʣ�����
		 if(PacSend(cmd,&BattValue,1) != AL_STS_OK)
		 {
			 Error_Handler();
		 }
	 } 
		
 }
 
 
 /***������Դ����
	* @brief
	* @param
	* @retval
***/
void Open_Batt_result(uint8_t* frame, uint16_t frameLen)
{
	uint8_t data[20] = {0};   //���յ�������
	DataCompress(frame,frameLen,data);
	
	switch(data[0]) 
	{
		case 0x01:     //����ָ����ģ���Դ
		{
			AL_BYTE Data[2] = {0x01,0x01};
			BSP_OUTPUT_On(PWR_VM);
			
			//�ϴ�Ӧ���Դ�������   
			if(PacSend(cmd,Data,2) != AL_STS_OK)
			{
				Error_Handler();
  		}	
		}break;
		
		case 0x02:     //�������֤ģ���Դ
		{
			AL_BYTE Data[2] = {0x02,0x01};
			BSP_OUTPUT_On(PWR_IDCARD);
			
			//�ϴ�Ӧ���Դ�������   
			if(PacSend(cmd,Data,2) != AL_STS_OK)
			{
				Error_Handler();
			}
		}break;
		
		case 0x03:     //��������ͷ��Դ
		{
			AL_BYTE Data[2] = {0x03,0x01};
			BSP_OUTPUT_On(PWR_CAMERA);
			
			//�ϴ�Ӧ���Դ�������   
			if(PacSend(cmd,Data,2) != AL_STS_OK)
			{
				Error_Handler();
			}
		}break;
		
		case 0x04:     //����ָʾ�Ƶ�Դ
		{
			AL_BYTE Data[2] = {0x04,0x01};
			BSP_OUTPUT_On(PWR_LED);
			
			//�ϴ�Ӧ���Դ�������   
			if(PacSend(cmd,Data,2) != AL_STS_OK)
			{
				Error_Handler();
			}
		}break;
		default:break;
	}
	
}


/***�رյ�Դ����
	* @brief
	* @param
	* @retval
***/
void Close_Batt_result(uint8_t* frame, uint16_t frameLen)
{
	
	uint8_t data[20] = {0};   //���յ�������
	DataCompress(frame,frameLen,data);
	
	switch(data[0]) 
	{
		case 0x01:     //�ر�ָ����ģ���Դ
		{
			AL_BYTE Data[2] = {0x01,0x01};
			BSP_OUTPUT_Off(PWR_VM);
			
			//�ϴ�Ӧ���Դ�رս��   
			if(PacSend(cmd,Data,2) != AL_STS_OK)
			{
				Error_Handler();
			}
		}break;
		
		case 0x02:     //�ر����֤ģ���Դ
		{
			AL_BYTE Data[2] = {0x02,0x01};
			BSP_OUTPUT_Off(PWR_IDCARD);
			
			//�ϴ�Ӧ���Դ�رս��   
			if(PacSend(cmd,Data,2) != AL_STS_OK)
			{
				Error_Handler();
			}
		}break;
		
		case 0x03:     //�ر�����ͷ��Դ
		{
			AL_BYTE Data[2] = {0x03,0x01};
			BSP_OUTPUT_Off(PWR_CAMERA);
			
			//�ϴ�Ӧ���Դ�رս��   
			if(PacSend(cmd,Data,2) != AL_STS_OK)
			{
				Error_Handler();
			}
		}break;
		
		case 0x04:     //�ر�ָʾ�Ƶ�Դ
		{
			AL_BYTE Data[2] = {0x04,0x01};
			BSP_OUTPUT_Off(PWR_LED);
			
			//�ϴ�Ӧ���Դ�رս��   
			if(PacSend(cmd,Data,2) != AL_STS_OK)
			{
				Error_Handler();
			}
		}break;
		default:break;
	}		
	
}


/***��дһ������Դ��翪ʼ�ͳ������ĺ���
	* @brief
	* @param
	* @retval
***/
void Charging_Test(void)
{
	BSP_INPUT_Init(CHARGIN_EN,INPUT_MODE_GPIO);
	BSP_INPUT_Init(CHARGDON_EN,INPUT_MODE_GPIO);
	
	//����翪ʼ
	if(BSP_INPUT_GetState(CHARGIN_EN) == 0 && BSP_INPUT_GetState(CHARGDON_EN) == 1)
	{
		
		RealTime_BattValue = BatteryCheck();   //��ȡ��ǰʵʱ��������
		AssignTask(TASK_CHARGIN);   //ע����������
		TIM14_Start();  //������ʱ����
	}
	
	//��������
	else if(BSP_INPUT_GetState(CHARGIN_EN) == 1 && BSP_INPUT_GetState(CHARGDON_EN) == 0)
	{
		RealTime_BattValue = BatteryCheck();   //��ȡ��������
		AssignTask(TASK_CHARGDON);  //ע�����������
		TIM14_Stop();  //ֹͣ��ʱ����
	}
	
	else
	{
		return;
	}
	
}




 
 
