#include "steering.h"

void steering::set_omega(float omega){
	//motor__.set_omega(omega);
	float computed_omega = dpid_.compute(omega-encoder__.get_omega(), 10);
	motor__.set_omega(computed_omega);
	//printf("\tsettingg %d \t", (int)(omega*100));
}

void steering::set_angle(float angle){
	angle *= 180/PI; 
	float computed_omega = apid_.compute(angle-(encoder__.get_angle()*180/PI), 10);
	computed_omega *= (motor__.get_max_omega()/dpid_.get_Upper());
	motor__.set_omega(computed_omega);
}