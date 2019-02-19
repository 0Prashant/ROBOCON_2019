/*22-1-2019*/

#ifndef _ROBO_INIT_H_
#define _ROBO_INIT_H_

#include "tim.h"
#include "gpio.h"

#include "stm32f4xx_hal.h"
#include "defines.h"
#include "encoder.h"
#include "motor.h"
#include "pid.h"
#include "blnc_motor.h"

void roboInit(void);

#endif /*robo_init.h*/
