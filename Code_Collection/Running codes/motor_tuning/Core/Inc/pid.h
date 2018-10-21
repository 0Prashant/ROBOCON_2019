/*
 * PID.h
 *
 * Created: 10/4/2018 6:12:32 PM
 * Author: Nirdesh
 */ 

#ifndef __PID_H__
#define __PID_H__



struct Str_pid
{
	float kp;
	float ki;
	float kd;	
	
	float input;
	float output;
	float set_point; 
	float last_input;
	float dt;

	float max_output;
	float min_output;
	
	
};

void tunePID(struct Str_pid* spid, float kp, float ki, float kd);
void set_OutputLimit(struct Str_pid* spid, float max_output, float min_output);
void set_Gains(struct Str_pid* spid, float kp, float ki, float kd);
float pid_Compute(struct Str_pid* spid,float set_point, float process_value, float dt);

#endif
