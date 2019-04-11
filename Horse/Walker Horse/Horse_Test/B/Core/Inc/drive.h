#ifndef _DRIVE_H_
#define _DRIVE_H_

#include "wheel.h"
#include "robo_init.h"

enum robot_direction
{
	FORWARD,
	BACKWARD,
	ROTATE_LEFT,
	ROTATE_RIGHT,
	STOP
};

void robot_direction(enum robot_direction);
void robot_speed(int16_t speed);
void robot_yaw(int16_t speed);

#endif //_DRIVE_H_
