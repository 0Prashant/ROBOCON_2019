#include "steering.h"

void steering::set_omega(float omega){
	motor__.set_omega(omega);
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