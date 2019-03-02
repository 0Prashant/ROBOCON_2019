
#ifndef _ROBO_INIT_H_
#define _ROBO_INIT_H_

#include "tim.h"
#include "gpio.h"

#include "motionProfile.h"
#include "horse.h"
#include "leg.h"
#include "pid.h"

extern struct Leg_str      leg_arr[4];
extern struct Trap_struct  profile_arr[4];
extern struct Leg_index    present_leg;
extern struct Str_pid      pid[4];

void leg_init(void);

#endif // _ROBO_INIT_H_
