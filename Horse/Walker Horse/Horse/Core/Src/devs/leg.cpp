#include "leg.h"

float gravity_compensator_factor;
float omeg;
float ang;

void leg::set_omega(float omega){
	encoder__.calculate_omega();
	omeg = encoder__.get_omega();
	ang = encoder__.get_angle();
	// if(this->is_raised()){
	// 	gravity_compensator_factor = cos(encoder__.get_angle()) * BODY_WEIGHT * kg_;
	// }
	// else{
	// 	gravity_compensator_factor = cos(encoder__.get_angle()) * LEG_WEIGHT * kg_;
	// }

	float computed_omega = pid__.compute_pid(encoder__.get_omega(), omega);
	//computed_omega += gravity_compensator_factor; 
	motor__[0].set_omega(computed_omega);
	motor__[1].set_omega(computed_omega);

	// printf ("%d \t %d \t %d \t %d \t ", (int)(omega*100), (int)(computed_omega), (int)steps, (int)(this->get_angle()*180/PI));
	// if(is_raised()){
	// 	printf("Raised\n");
	// }
	// else{
	// 	printf("Fallen\n");
	// }
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