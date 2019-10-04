// #include "pid.h"

// void pid::set_pid_constants(float kp, float ki, float kd){
// 	kp_ = kp;
// 	ki_ = ki; 
// 	kd_ = kd;
// }
	
// float pid::compute_pid(float current_value, float set_point){
	
// 	float error = set_point - current_value;

// 	float i_term = SAMPLE_TIME * error / 1000 ;
// 	float d_term = (error - 2*last_error_ + prev_error_) * 1000.0 /(float)SAMPLE_TIME;

// 	float output = last_output_  + kp_*error + ki_*i_term + kd_*d_term ;

// 	last_output_  = output ;
// 	prev_error_   = last_error_;
// 	last_error_   = error;
	
	
// 	//printf("  iterm = %d\n", (int)(i_term*100));

// 	if(output > 70 ) 
// 	{
// 		output = 70;
// 	}

// 	if(output < -70)
// 	{
// 		output = -70; 	
	
// 	}
// 	return output;
// }