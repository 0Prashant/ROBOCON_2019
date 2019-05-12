#include "leg.h"
static float ramped_value = 0;

void leg::set_omega(float omega){
	float gravity_compensator_factor;
	if(!is_raised()){
		gravity_compensator_factor = cos(encoder__.get_angle())  * kb_ * omega ;
	}
	else{
		gravity_compensator_factor = -cos(encoder__.get_angle())  * kl_ * omega ;
	}
	omega += gravity_compensator_factor;
	
	 omega = ramp(omega);
	float computed_omega = dpid_.compute(omega - encoder__.get_omega(), SAMPLE_TIME);
	// printf("omega = %d\t, computed_omega = %d\n", (int)(omega*100), (int)(computed_omega*100));
	motor__[0].set_omega(computed_omega);
	motor__[1].set_omega(computed_omega);
}

float leg::ramp(float omega){
	int factor = 2;

	if (omega>motor__[0].get_max_omega())
	{
		omega = motor__[0].get_max_omega();
	}
	else if(omega < -motor__[0].get_max_omega())
	{
		omega = -motor__[0].get_max_omega();
	}

	if(ramped_value < (omega-factor)){
		ramped_value += factor;
	}
	else if(ramped_value > (omega+factor)){
		ramped_value -= factor;
	}
	else{
		ramped_value = omega;
	}
	return ramped_value;
}

Leg_condition leg::is_raised(){
	float dead_angle = 15*PI/180;
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
