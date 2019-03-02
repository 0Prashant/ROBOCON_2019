#ifndef __PID_H__
#define __PID_H__

#include "stm32f4xx_hal.h"
#define MOTOR_MAX_OMEGA 15.0

struct Str_pid
{
	double kp;
	double ki;
	double kd;	
	
	float last_input;
	float last_error;
	float previous_error;
	float last_output;
	
	float output;
	float max_output;
	float min_output;	
};

void init_PID(struct Str_pid* spid);
void reset_Params(struct Str_pid* spid);
void set_OutputLimit(struct Str_pid* spid, float max_output, float min_output);
void set_Gains(struct Str_pid* spid, float kp, float ki, float kd);
float pid_Compute(struct Str_pid* spid,float set_point, float process_value, uint16_t dt);

#endif
