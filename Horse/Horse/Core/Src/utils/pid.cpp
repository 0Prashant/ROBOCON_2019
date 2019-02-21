/*2-11-2018*/

#include "pid.h"

void PID_setGains(struct pidStr *pid, float kp, float ki, float kd)
{
	pid->kp = kp;
	pid->ki = ki;
	pid->kd = kd;
}

void PID_Init(struct pidStr *pid)
{
	pid->output = 0;
	pid->last_error = 0;
	pid->max_output = 0;
	pid->min_output = 0;
}

float PID_Compute(struct pidStr *pid, float set_point, float input, uint16_t dt)
{
	float error = set_point - input;

	float i_term = dt * error / 1000;
	float d_term = (error - 2 * pid->last_error + pid->previous_error) * 1000.0 / (float)dt;

	pid->output = pid->last_output + pid->kp * error + pid->ki * i_term + pid->kd * d_term;

	pid->last_output = pid->output;
	pid->previous_error = pid->last_error;
	pid->last_error = error;

	if (pid->output > (pid->max_output))
	{
		pid->output = (pid->max_output);
	}

	if (pid->output < (pid->min_output))
	{
		pid->output = (pid->min_output);
	}

	return (pid->output * 17) / 24;
}

void PID_resetParams(struct pidStr *pid)
{
	pid->last_error = 0;
	pid->output = 0;
	pid->previous_error = 0;
	pid->last_output = 0;
}

void PID_setOutputLimit(struct pidStr *pid, float max_output, float min_output)
{
	pid->max_output = max_output;
	pid->min_output = min_output;
}
