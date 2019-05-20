#ifndef _ROBOT_H_
#define _ROBOT_H_

#include "robo_init.h"
#include "robot_functions.h"

enum Robot_States{
	HOME,
	MARCH,
	TURN_45,
	SAND_DUNE,
	STATE_D,
	TURN_90,
	TUSSOCK,
	BASE_CAMP,
	MOUNTAIN,
	UUKHAI,
};

bool go(int step, float angle);

void start_Robot(enum Robot_States *state_);
bool play(void);

#endif // !_ROBOT_H_
