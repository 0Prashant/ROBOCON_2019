#ifndef _DRIVE_H_
#define _DRIVE_H_

#include "wheel.h"
#include "robo_init.h"

enum robot_direction
{
	FORWARD,
	BACKWARD,
	ROTATE,
	STOP
};

//void robot_direction(enum robot_direction);
void robot_speed(int16_t speed);

#endif //_DRIVE_H_
