#include "steering.h"

void steering::set_omega(float omega){
	motor__.set_omega(omega);
	float computed_omega = dpid_.compute(omega-encoder__.get_omega(), 10);
	motor__.set_omega(computed_omega);
	//printf("\tsettingg %d \t", (int)(omega*100));
}


