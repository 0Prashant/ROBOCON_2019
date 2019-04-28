#include "steering.h"

void steering::set_omega(float omega){
	motor__.set_omega(omega);
	float computed_omega = dpid_.compute(omega-encoder__.get_omega(), 10);
	motor__.set_omega(computed_omega);
	printf("\tsetting %d \t", (int)(omega*100));
}

void steering::reset_angle(float angle_in_radians){
	encoder__.reset_angle(angle_in_radians);
}

void steering::calculate_omega(){
	encoder__.calculate_omega();
}

