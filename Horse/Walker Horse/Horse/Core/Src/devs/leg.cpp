#include "leg.h"

float gravity_compensator_factor;
float omeg;
float ang;
float input_omega;
float comp_omg;

void leg::set_omega(float omega){
	encoder__.calculate_omega();
	//printf("setting");
	omeg = encoder__.get_omega();
	ang = encoder__.get_angle();
	input_omega = omega;
	if(!is_raised()){
		gravity_compensator_factor = -cos(encoder__.get_angle())  * kl_ * omega ;
	}
	else{
		gravity_compensator_factor = cos(encoder__.get_angle())  * kb_ * omega ;
	}
	omega += gravity_compensator_factor;
	float computed_omega = dpid_.compute(omega - encoder__.get_omega(), SAMPLE_TIME);
	comp_omg = computed_omega;
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