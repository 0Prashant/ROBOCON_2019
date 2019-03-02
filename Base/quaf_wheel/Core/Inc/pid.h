#ifndef __PID_H__
#define __PID_H__

#include "stm32f4xx_hal.h"
#define MOTOR_MAX_OMEGA 15.0

struct PID
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

void init_PID(struct PID* spid);
void reset_Params(struct PID* spid);
void set_OutputLimit(struct PID* spid, float max_output, float min_output);
void set_Gains(struct PID* spid, float kp, float ki, float kd);
float pid_Compute(struct PID* spid,float set_point, float process_value, uint16_t dt);

#endif
