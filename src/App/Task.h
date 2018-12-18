#ifndef __TASK__
#define __TASK__

#ifdef __cplusplus
extern "C" {
#endif

#include "stdint.h"

	

extern void AssignTask(uint32_t taskMask);
extern void ClearTask(uint32_t taskMask);
extern uint32_t GetTask(uint32_t taskMask);


#ifdef __cplusplus
}
#endif

#endif		// #ifndef __TASK__
