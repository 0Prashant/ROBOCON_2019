#include "pid.h"
#include <math.h>

void init_PID(struct PID* spid)
{
    spid->kp = 0;
	spid->ki = 0;
	spid->kd = 0;	
	
	spid->last_input = 0;
	spid->last_error = 0;
	spid->previous_error = 0;
	spid->last_output = 0;
	
	spid->output = 0;
	spid->max_output = 0;
	spid->min_output = 0;

}

void reset_Params(struct PID* spid)
{
	spid->last_output = 0;
	spid->previous_error = 0;
	spid->last_error = 0;
}
void set_Gains(struct PID* spid, float kp, float ki, float kd)
{
	spid->kp = fabs(kp);
	spid->ki = fabs(ki);
	spid->kd = fabs(kd);
}

void set_OutputLimit(struct PID* spid, float max_output, float min_output)
{
	spid->max_output = max_output;
	spid->min_output  = min_output;
}

float pid_Compute(struct PID* spid,float set_point, float input, uint16_t dt)
{
	float error = set_point - input;
	float p_term = error-spid->last_error;

	float i_term = dt*error/1000;
	float d_term = (error - 2*spid->last_error + spid->previous_error)*1000/dt;	

	spid->output = spid->last_output + spid->kp*error + spid->ki*i_term + spid->kd*d_term; 

	spid->last_output = spid->output;
	spid->previous_error = spid->last_error;
	spid->last_error = error;

	if(spid->output > (spid->max_output))
	{
		spid->output = (spid->max_output);
	}

	if(spid->output < (spid->min_output))
	{
		spid->output = (spid->min_output);
	}

	return (spid->output*MOTOR_MAX_OMEGA)/12;
}


