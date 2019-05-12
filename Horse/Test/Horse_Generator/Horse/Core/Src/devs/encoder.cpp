#include"encoder.h"

float encoder::get_omega(){
	float omega = (2 * PI * (int)encoder_->henc->Instance->CNT * 1000)/(encoder_->ppr * SAMPLE_TIME);	// omega = 2*pi*f; f = count_per_unittime/ppr;
	count_ = (int)encoder_->henc->Instance->CNT;
	encoder_->henc->Instance->CNT = 0;
	return omega;
}

float encoder::get_angle(){
	angle_ += ((2*PI)*(count_+(int)encoder_->henc->Instance->CNT)/(encoder_->ppr));
	int temp_angle = (int)(angle_*1000);
	temp_angle %=360;
	return angle_;
}

void encoder::reset_angle(float angle_in_radian){
	angle_ = angle_in_radian;
}