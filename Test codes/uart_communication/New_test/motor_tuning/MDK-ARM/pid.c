/*
 * PID.c
 *
 * Created: 10/4/2018 6:12:32 PM
 * Author : Nirdesh
 */ 
#include "pid.h"

void set_Gains(struct Str_pid* spid, float kp, float ki, float kd)
{
	spid->kp = kp;
	spid->ki = ki;
	spid->kd = kd;
}

void set_OutputLimit(struct Str_pid* spid, float max_output, float min_output)
{
	spid->max_output = max_output;
	spid->max_output  = max_output;
}

float pid_Compute(struct Str_pid* spid,float sp, float pv, float dt)
{ 

	float pterm;
	float iterm;
	float dterm;
	float errorsum;
	float derivative;
	float error;
	
	spid->set_point = sp;
	spid->input = pv;
	
  error = (spid->set_point) - spid->input;
	errorsum += error;
	
	pterm = error * (spid->kp);
	iterm = errorsum * (spid->ki)*dt;
  derivative = spid->input - (spid->last_input);
	dterm = (spid->kd*derivative)*dt;
	
	spid->output = pterm + iterm - dterm;
	
	spid->last_input = spid->input;
	
	if(spid->output > (spid->max_output))
	{
		spid->output = (spid->max_output);
	}
	
	if(spid->output < (spid->min_output))
	{
		spid->output = (spid->min_output);
	}
	
	if(spid->output > (spid->max_output))
	{
		spid->output = (spid->max_output);
	}
	
	if(spid->output < (spid->min_output))
	{
		spid->output = (spid->min_output);
	}
	
	return spid->output;
	
}
