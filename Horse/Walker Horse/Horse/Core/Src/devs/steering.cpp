#include "steering.h"

void steering::set_omega(float omega){
	encoder__.calculate_omega();
	motor__.set_omega(omega);
	float computed_omega = dpid_.compute(omega-encoder__.get_omega(), 10);
	motor__.set_omega(computed_omega);
	printf("\tsetting %d", (int)(omega*100));
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