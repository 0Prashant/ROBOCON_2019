#include"encoder.h"
#include "leg.h"

extern leg leg[1];
void encoder::calculate_omega(){
	omega_ = (2 * PI * (int16_t)encoder_->henc->Instance->CNT * 1000)/(encoder_->ppr * SAMPLE_TIME);	// omega = 2*pi*f; f = count_per_unittime/ppr;
	count_ += (int16_t)encoder_->henc->Instance->CNT;
	actual_count_ += (int16_t)encoder_->henc->Instance->CNT;
	encoder_->henc->Instance->CNT = 0;
}

float encoder::get_omega(){
	return omega_;
}

float encoder::get_angle(){
	
	angle_ = ((2*PI)*(count_+(int16_t)encoder_->henc->Instance->CNT)/(encoder_->ppr));
	if(angle_>(2*PI)){
		leg[encoder_->id].steps++;
		reset_angle(angle_-(2*PI));
	}
	if(angle_<(-2*PI)){
		leg[encoder_->id].steps++;
		reset_angle(angle_+(2*PI));
	}
	return angle_;
}

float encoder::get_actual_angle(void){
	actual_angle_ = ((2*PI)*(actual_count_+(int16_t)encoder_->henc->Instance->CNT)/(encoder_->ppr));
	return actual_angle_;
}

void encoder::reset_angle(float angle_in_radian){
	angle_ = angle_in_radian;
	count_ = (angle_*encoder_->ppr)/(2*PI);
}
void encoder::reset_actual_angle(float angle_in_radian){
	actual_angle_ = angle_in_radian;
	actual_count_ = (actual_angle_*encoder_->ppr)/(2*PI);
}

int32_t encoder::get_count(){
	return count_;
}