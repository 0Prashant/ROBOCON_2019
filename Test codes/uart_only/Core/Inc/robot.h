/*
 * robot.h
 *
 * Created : 10/3/2018
 */

#ifndef _ROBOT_H_
#define _ROBOT_H_

//#include "robo_init.h"
#include "wheel.h"
#include "drive.h"
#include "encoder.h"
#include <stdlib.h>


void run_CurrentConfig(void);

// This function contains an infinite loop
void play(void);
int goto_distance(float dx, float dy, float dth);
int goto_absolute_distance(float dx, float dy);


#endif // _ROBOT_H_
