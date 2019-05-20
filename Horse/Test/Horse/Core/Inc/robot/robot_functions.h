#ifndef _ROBOT_FUNCTIONS_H_
#define _ROBOT_FUNCTIONS_H_ 

#include "robo_init.h"

void move_leg(int steps, float angle);
Vec3<float> read_Orientation(uint32_t dt_millis);
bool go(int step, float angle);
void move_steering(int steps, float angle);
void correct_steering_angle(float angle);
void set_angle(float angle);
void calculate_datas(void);
void initialize_position(void);
bool initialize_leg_position(void);
bool initialize_steering_position(void);
void calculate_robot_angle(void);


#endif //!_ROBOT_FUNCTIONS_H_

