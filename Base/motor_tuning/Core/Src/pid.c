
/*
 * PID.c
 *
 * Created: 10/4/2018 6:12:32 PM
 * Author : Nirdesh
 */ 
#include "pid.h"
#include "defines.h"

uint32_t sample_time = 15L;
uint16_t dt = 10;  
uint32_t break_tick ;

float omega[4] = {0,0,0,0};
double new_omega[4] = {0,0,0,0};
int counts[4] = {0,0,0,0};			
uint32_t update_tick;
float req_omega[4];
int counts_per_st[4] = {0,0,0,0};
struct Str_pid pid[4];

extern double required_distance;
extern double time;
extern float velocity[3];
extern float required_dx,required_dy,required_theta;
extern float robotx, roboty,theta;
float distance_pid[3] = {100,100,100};
float Previous_distance_pid[3] = {100,100,100};
float del_distance_pid[3] = {1000,1000,1000};
struct Str_pid_distance pid_dis_1;
struct Str_pid_distance pid_dis_2;
struct Str_pid_distance pid_dis_3;

#include "pid.h"
#include <math.h>

void init_PID(struct Str_pid* spid)
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

void reset_Params(struct Str_pid* spid)
{
	spid->last_output = 0;
	spid->previous_error = 0;
	spid->last_error = 0;
}
void set_Gains(struct Str_pid* spid, float kp, float ki, float kd)
{
	spid->kp = fabs(kp);
	spid->ki = fabs(ki);
	spid->kd = fabs(kd);
}

void set_OutputLimit(struct Str_pid* spid, float max_output, float min_output)
{
	spid->max_output = max_output;
	spid->min_output  = min_output;
}

float pid_Compute(struct Str_pid* spid,float set_point, float input, uint16_t dt)
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
    	
	return (spid->output*MAX_OMEGA)/12;
	
	
}


void calculate_velocity_with_pid(void)
{


	
	calculate_robot_velocity();
	if ((HAL_GetTick()- update_tick)> sample_time)
	{  
		update_tick = HAL_GetTick();
	
		counts_per_st[0] = (counts[0]); 
		counts[0] = 0;
				
		counts_per_st[1] = (counts[1]); 
		counts[1] = 0;
		
		counts_per_st[2] = (counts[2]); 
		counts[2] = 0;
		
		counts_per_st[3] = (counts[3]); 
		counts[3] = 0;
	
	
		omega[0] = 25.173 * counts_per_st[0]/dt;				
		new_omega[0] = pid_Compute(&pid[0],req_omega[0],omega[0],dt);
		
		omega[1] = 25.173 * counts_per_st[1]/dt;					
		new_omega[1] = pid_Compute(&pid[1],req_omega[1],omega[1],dt);
		
		omega[2] = 25.173 * counts_per_st[2]/dt;					
		new_omega[2] = pid_Compute(&pid[2],req_omega[2],omega[2],dt);
		
		omega[3] = 25.173 * counts_per_st[3]/dt;					
		new_omega[3] = pid_Compute(&pid[3],req_omega[3],omega[3],dt);
	
		
		set_Omega(&Wheel_arr[0],new_omega[0]);
		set_Omega(&Wheel_arr[1],new_omega[1]);
		set_Omega(&Wheel_arr[2],new_omega[2]);
		set_Omega(&Wheel_arr[3],new_omega[3]);	
		//printf("%f\t%f\t%f\t%f\n", omega[0], omega[1], omega[2], omega[3]);
	}
}

void set_Gains_distance(struct Str_pid_distance* dis_pid,double kpp, double kii, double kdd)
{
	dis_pid->kpp = kpp;
	dis_pid->kii = kii;
	dis_pid->kdd = kdd;
	dis_pid->prev_error = 0;
}
double compute_pid_distance(struct Str_pid_distance* dis_pid,float req_distance, float curr_distance )
{
	dis_pid->error = req_distance - curr_distance;
	float temp_error = dis_pid->error - dis_pid->prev_error;
	dis_pid->iterm +=dis_pid->error*dis_pid->kii;
	dis_pid->prev_error = dis_pid->error;
	dis_pid->output = (dis_pid->kpp*dis_pid->error + dis_pid->iterm + dis_pid->kdd*(temp_error));
	return dis_pid->output;
}
void pid_distance()
{
	distance_pid[0] = compute_pid_distance(&pid_dis_1,required_dx,robotx);
	distance_pid[1] = compute_pid_distance(&pid_dis_2,required_dy,roboty);
	distance_pid[2] = compute_pid_distance(&pid_dis_3,required_theta,theta);
	
	del_distance_pid[0] = Previous_distance_pid[0] - distance_pid[0];
	del_distance_pid[1] = Previous_distance_pid[1] - distance_pid[1];
	del_distance_pid[2] = Previous_distance_pid[2] - distance_pid[2];
	
	Previous_distance_pid[0] = distance_pid[0];
	Previous_distance_pid[1] = distance_pid[1];
	Previous_distance_pid[2] = distance_pid[2];
	
	velocity[0] = distance_pid[0] / time;
	velocity[1] = distance_pid[1] / time;
	velocity[2] = (distance_pid[2] * required_distance )/ (time*5);
	
	//printf("%f\t %f\t %f\t %f\t %f \t%f \t%f \t%f \t%f\n", required_dx,robotx,distance_pid[0],required_dy,roboty,distance_pid[1],required_theta, theta,distance_pid[2] );
}


