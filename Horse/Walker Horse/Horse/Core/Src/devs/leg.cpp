#include "leg.h"


void leg::set_omega(float omega){
	float gravity_compensator_factor;
	if(!is_raised()){
		gravity_compensator_factor = cos(encoder__.get_angle())  * kb_ * omega ;
	}
	else{
		gravity_compensator_factor = -cos(encoder__.get_angle())  * kl_ * omega ;
	}
	omega += gravity_compensator_factor;
	float computed_omega = dpid_.compute(omega - encoder__.get_omega(), SAMPLE_TIME);
	motor__[0].set_omega(computed_omega);
	motor__[1].set_omega(computed_omega);
}

Leg_condition leg::is_raised(){
	float dead_angle = 10*PI/180;
	if(((this->get_angle()>=(0+dead_angle)) && (this->get_angle()<=(PI-dead_angle)))  ||
	   ((this->get_angle()<=(-PI-dead_angle)) && (this->get_angle()>=(-2*PI+dead_angle)))){
		return RAISED;
	}
	else if(((this->get_angle()>=(PI+dead_angle)) && (this->get_angle()<=(2*PI-dead_angle)))  ||
		((this->get_angle()<=(-dead_angle)) && (this->get_angle()>=(-PI+dead_angle)))){
		return FALLEN;
	}
	else{
		return UNDEFINED;
	}
}

bool leg::is_raised_without_deadzone(){
	if(((this->get_angle()>=0) && (this->get_angle()<=(PI)))  ||  ((this->get_angle()<=(-PI)) && (this->get_angle()>=(-2*PI)))){
		return true;
	}
	else{
		return false;
	}

}

void leg::reset_angle(float angle_in_radians){
	encoder__.reset_angle(angle_in_radians);
}

void leg::calculate_omega(){
	encoder__.calculate_omega();
}
