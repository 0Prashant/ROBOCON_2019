#include "leg.h"

void leg::set_omega(float omega){
	motor__[0].set_omega(omega);
	motor__[1].set_omega(omega);
}
float leg::get_omega(void){
	return encoder__.get_omega();
}
bool leg::is_raised(){
	return raised_flag;
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