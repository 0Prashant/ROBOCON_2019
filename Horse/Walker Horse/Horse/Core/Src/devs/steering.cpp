#include "steering.h"

void steering::set_omega(float omega){
	encoder__.calculate_omega();
	motor__.set_omega(omega);
	float computed_omega = pid__.compute_pid(encoder__.get_omega(), omega);
	motor__.set_omega(computed_omega);
}

float steering::get_omega(void){
	return encoder__.get_omega();
}

void steering::reset_angle(float angle_in_radians){
	encoder__.reset_angle(angle_in_radians);
}

float steering::get_angle(void){
	return encoder__.get_angle();
}