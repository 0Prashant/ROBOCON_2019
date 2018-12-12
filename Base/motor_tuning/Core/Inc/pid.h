/*
 * PID.h
 *
 * Created: 10/4/2018 6:12:32 PM
 * Author: Nirdesh
 */ 

#ifndef __PID_H__
#define __PID_H__
#include "stm32f4xx_hal.h"
#include <stdlib.h>
#include "drive.h"


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


struct Str_pid_distance{
	double kpp;
	double kii;
	double kdd;

	float prev_error;
	float error;
	float iterm;
	
	double output;

};
void init_PID(struct Str_pid* spid);
void reset_Params(struct Str_pid* spid);
void set_OutputLimit(struct Str_pid* spid, float max_output, float min_output);
void set_Gains(struct Str_pid* spid, float kp, float ki, float kd);
float pid_Compute(struct Str_pid* spid,float set_point, float process_value, uint16_t dt);
void calculate_velocity_with_pid(void);
void pid_distance(void);
double compute_pid_distance(struct Str_pid_distance* dis_pid,float req_distance, float curr_distance );
void set_Gains_distance(struct Str_pid_distance* dis_pid,double kpp, double kii, double kdd);

#endif
