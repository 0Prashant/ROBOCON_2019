#include "leg.h"

float gravity_compensator_factor;
float omeg;
float ang;
float comp_omg;

void leg::set_omega(float omega){
	encoder__.calculate_omega();
	omeg = encoder__.get_omega();
	ang = encoder__.get_angle();
	if(this->is_raised()){
		gravity_compensator_factor = cos(encoder__.get_angle()) * LEG_WEIGHT * kl_ * (fabs(omeg-omega) / this->get_max_omega());
	}
	else{
		gravity_compensator_factor = cos(encoder__.get_angle()) * BODY_WEIGHT * kb_ * (fabs(omeg-omega) / this->get_max_omega());
	}

	float computed_omega = pid__.compute_pid(encoder__.get_omega(), omega);
	comp_omg = computed_omega;
	//float computed_omega = omega;
	//computed_omega += gravity_compensator_factor; 
	motor__[0].set_omega(computed_omega);
	motor__[1].set_omega(computed_omega);
}
float leg::get_omega(void){
	return encoder__.get_omega();
}
bool leg::is_raised(){
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
float leg::get_angle(void){
	return encoder__.get_angle();
}
int leg::get_steps(){
	return steps;
}

int16_t leg::get_encoder_count(){
	return encoder__.get_count();
}