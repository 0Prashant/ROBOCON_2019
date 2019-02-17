/*22-1-2019*/

#ifndef _ROBO_INIT_H_
#define _ROBO_INIT_H_

#include "tim.h"
#include "gpio.h"

#include "stm32f4xx_hal.h"
#include "feed_forward.h"
#include "pneumatic.h"
#include "interrupt.h"
#include "defines.h"
#include "encoder.h"
#include "motor.h"
#include "pid.h"
#include "fsm.h"

extern struct interruptStr blnc_int_arr[4];
extern struct encoderStr   encoder_arr[4];
extern struct motorStr     balance_motor;
extern struct motorStr     motor_arr[4];
extern struct pneumaticStr pneu_arr[4];
extern struct interruptStr int_arr[4];
extern struct pidStr       pid_arr[4];
extern struct feedStr      ff_arr[4];
extern struct fsmStr       fsm;

void feedForwardInit(void);
void pneumaticInit(void);
void interruptInit(void);
void encoderInit(void);
void motorInit(void);
void pidInit(void);

void roboInit(void);

#endif /*robo_init.h*/
