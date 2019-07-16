/**
 * robot_functions.h
 *This header includes all the functions incorporated with the function play().The functions included in this header are related with the operations 
 *of robot such as to move the legs, move steering to change the orientation of robot. 
*/
#ifndef _ROBOT_FUNCTIONS_H_
#define _ROBOT_FUNCTIONS_H_ 

#include "robo_init.h"


Vec3<float> read_Orientation(uint32_t dt_millis);

void initialize_position(void);
bool initialize_leg_position(void);
bool initialize_steering_position(void);
bool getup_n_run(void);

bool go(int step, float angle);
void move_leg(int steps, float angle);
void move_steering(int steps, float angle);

void set_angle(float angle);
void correct_steering_angle(float angle);

void calculate_datas(void);
void calculate_robot_angle(void);
float motion_profile(float angle_in_degrees, float min_speed, float max_speed);

#endif //!_ROBOT_FUNCTIONS_H_

