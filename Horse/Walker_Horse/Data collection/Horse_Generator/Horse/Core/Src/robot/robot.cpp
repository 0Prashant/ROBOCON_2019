#include "robot.h"

static const float leg_speed = 15; 		//35 is the 100%
static const float steering_speed = 0.8;	// 1.3 is the 100%
extern leg leg[1];
extern steering steering;

bool go(int steps, int angle){
	while(  (leg[0].get_steps() < steps)  &&  (angle < steering.get_angle())  ){
		leg[0].set_omega(leg_speed);
		steering.set_omega(steering_speed);
	}
	return true;	
}

bool rewind(){
	while ( (fabs(steering.get_angle())) > 0  ){
		leg[0].set_omega(leg_speed);
		steering.set_omega(steering_speed*((steering.get_angle())/fabs(steering.get_angle())));
	}
	steering.reset_angle(0);
	return true;	
}