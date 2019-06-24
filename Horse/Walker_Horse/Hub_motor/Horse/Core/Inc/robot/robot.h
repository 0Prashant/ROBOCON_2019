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
	TUNE
};

class Robot
{
public:
	Robot(){};
	Robot(Robot &&) = default;
	Robot(const Robot &) = default;
	Robot &operator=(Robot &&) = default;
	Robot &operator=(const Robot &) = default;
	~Robot(){};

	// initialize_position();
	// move(float step, float angle);
	// calculate_datas();

	// bool START_FLAG;

private:
	leg leg_0_, leg_1_;
	steering steering_;
};


void start_Robot(enum Robot_States *state_);
bool play(void);
void zone_select(void);

#endif // !_ROBOT_H_
