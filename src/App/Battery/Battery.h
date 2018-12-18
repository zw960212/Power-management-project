#ifndef __BATTERY_H
#define __BATTERY_H

#ifdef __cplusplus
extern "C" {
#endif
#include "adc.h"
	
#define BATTERY_FULL				2.64   //��ѹ��4�ڵ�ؾ�����ѹ�Ľ����
#define	BATTERY_DEPLETION		1.70   //�ŵ���ֹ��ѹ��4�ڵ�ؾ�����ѹ�Ľ����

	
	
#define LOW_PWR_VALUE       370   //�͵�ѹ��3.7V * 100��
	
	
extern uint8_t BatteryInit(void);
extern void BatteryDeInit(void);
extern uint16_t BatteryGetValue(void);
extern uint8_t BatteryCheck(void);


#ifdef __cplusplus
}
#endif

#endif		// #ifndef __BATTERY__
