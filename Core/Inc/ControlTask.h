#ifndef _ControlTask_H
#define _ControlTask_H

#include "stm32f4xx_hal.h"

#define zhMotorEncoder 0
#define zqMotorEncoder 1
#define yhMotorEncoder 3
#define yqMotorEncoder 2

extern int32_t lockFlag;
extern int32_t AngleAndPositionTIM;

extern uint16_t Time1_ms;
extern uint16_t Time2_ms;
extern uint16_t Time3_ms;

#endif
