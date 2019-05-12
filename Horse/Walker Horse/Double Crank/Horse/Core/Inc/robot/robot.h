#ifndef _ROBOT_H_
#define _ROBOT_H_

#include "robo_init.h"

bool go(int steps, float angle);
void move_leg(int steps, float angle);
void move_steering(int steps, float angle);
void calculate_datas(void);
void calculate_robot_angle(void);
bool rewind(void);
bool play(void);

#endif // !_ROBOT_H_
