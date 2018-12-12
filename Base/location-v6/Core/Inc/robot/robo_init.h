/*
 * robo_init.cpp
 * 
 * Created : 11/26/2018
 *  Author : n-is
 *   email : 073bex422.nischal@pcampus.edu.np
 */

#ifndef _ROBO_INIT_H_
#define _ROBO_INIT_H_

#include "utils/imu_utils.h"
#include "utils/mat.h"
#include "utils/kalman.h"
#include "utils/filter.h"
#include "robot/devs_config.h"
#include "devs/position_sensor.h"

#define PULSE_PER_REVOLUTION    (200)
#define FREEWHEEL_RADIUS        (29)

extern Kalman_Filter gRoll_Filter;
extern Kalman_Filter gPitch_Filter;
extern Kalman_Filter gYaw_Filter;
extern Kalman_Filter gXDist_Filter;
extern Kalman_Filter gYDist_Filter;
extern Filter Second_Pass;

void robo_Init(uint32_t);

#endif // !_ROBO_INIT_H_
