/*
 * robo_init.h
 *
 * Created : 10/1/2018
 */

#ifndef _ROBO_INIT_H_
#define _ROBO_INIT_H_

#include "tim.h"
#include "usart.h"
#include "gpio.h"
#include "adc.h"

#include "wheel.h"


extern struct Wheel Wheel_arr[4];

void wheel_init(void);

#endif // _ROBO_INIT_H_
