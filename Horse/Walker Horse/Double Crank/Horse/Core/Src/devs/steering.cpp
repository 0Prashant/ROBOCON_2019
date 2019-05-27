#include "steering.h"

float comp_omega = 0;

void steering::set_omega(float omega){
	//motor__.set_omega(omega);
	float computed_omega = dpid_.compute(omega-encoder__.get_omega(), 10);
	motor__.set_omega(computed_omega);
	//printf("\tsettingg %d \t", (int)(omega*100));
}

void steering::set_angle(float angle){ 
	float computed_omega = apid_.compute(angle-(encoder__.get_angle()*180/PI), 10);
	comp_omega = computed_omega*5;
	computed_omega = dpid_.compute(computed_omega-encoder__.get_omega(), 10);
	motor__.set_omega(computed_omega);
	// printf("\n computed = %d \t angle = %d \t ",(int)(computed_omega*100),(int)(encoder__.get_angle()*180/PI));
}