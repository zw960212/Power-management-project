#ifndef __BATTERY_H
#define __BATTERY_H

#ifdef __cplusplus
extern "C" {
#endif
#include "adc.h"
	
#define BATTERY_FULL				2.64   //满压（4节电池经过分压的结果）
#define	BATTERY_DEPLETION		1.70   //放电终止电压（4节电池经过分压的结果）

	
	
#define LOW_PWR_VALUE       370   //低电压（3.7V * 100）
	
	
extern uint8_t BatteryInit(void);
extern void BatteryDeInit(void);
extern uint16_t BatteryGetValue(void);
extern uint8_t BatteryCheck(void);


#ifdef __cplusplus
}
#endif

#endif		// #ifndef __BATTERY__
