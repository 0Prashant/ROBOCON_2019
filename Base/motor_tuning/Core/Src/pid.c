
/*
 * PID.c
 *
 * Created: 10/4/2018 6:12:32 PM
 * Author : Nirdesh
 */ 
#include "pid.h"
#include "defines.h"

uint32_t sample_time = 15L;
uint16_t dt = 15;  
uint32_t break_tick ;

float omega[4] = {0,0,0,0};
float new_omega[4] = {0,0,0,0};
int counts[4] = {0,0,0,0};			
uint32_t update_tick;
float req_omega[4];
int counts_per_st[4] = {0,0,0,0};
struct Str_pid pid_1;
struct Str_pid pid_2;
struct Str_pid pid_3;
struct Str_pid pid_4;

void set_Gains(struct Str_pid* spid, double kp, double ki, double kd)
{
	spid->kp = kp;
	spid->ki = ki;
	spid->kd = kd;
}

void set_OutputLimit(struct Str_pid* spid, float max_output, float min_output)
{
	spid->max_output = max_output;
	spid->min_output  = min_output;
}

float pid_Compute(struct Str_pid* spid,float set_point, float input, uint16_t dt)
{ 
	
	float alpha = (1 - (spid->ki*dt/spid->kp)/1000);
	
	spid->output = spid->kp*(set_point - alpha*input) + spid->last_output;
	
	
	spid->last_output = spid->output;
	
	if(spid->output > (spid->max_output))
	{
		spid->output = (spid->max_output);
	}
	
	if(spid->output < (spid->min_output))
	{
		spid->output = (spid->min_output);
	}
		
	return (12*spid->output)/MAX_OMEGA;
		
}

void calculate_velocity_with_pid(void)
{
	calculate_robot_velocity();
	if ((HAL_GetTick()- update_tick)> sample_time)
	{  
		
		update_tick = HAL_GetTick();
		
		set_Gains(&pid_1,0.4,0,0 );
		set_Gains(&pid_2,0.4,0,0 );	
		set_Gains(&pid_3,0.4,0,0 );	
		set_Gains(&pid_4,0.4,0,0 );

		
		//printf(" %d\t %d \t %d \t %d  \n",counts[0],counts[1],counts[2],counts[3]);
	      
		counts_per_st[0] = (counts[0]); 
		counts[0] = 0;
		
		
		counts_per_st[1] = (counts[1]); 
		counts[1] = 0;
		
		counts_per_st[2] = (counts[2]); 
		counts[2] = 0;
		
		counts_per_st[3] = (counts[3]); 
		counts[3] = 0;
	
	
		omega[0] = 25.173 * counts_per_st[0]/dt;	//				
		new_omega[0] = pid_Compute(&pid_1,req_omega[0],omega[0],dt);
		
		omega[1] = 25.173 * counts_per_st[1]/dt;					
		new_omega[1] = pid_Compute(&pid_2,req_omega[1],omega[1],dt);
		
		omega[2] = 25.173 * counts_per_st[2]/dt;					
		new_omega[2] = pid_Compute(&pid_3,req_omega[2],omega[2],dt);
		
		omega[3] = 25.173 * counts_per_st[3]/dt;					
		new_omega[3] = pid_Compute(&pid_4,req_omega[3],omega[3],dt);
		 
		//printf(" %f\t %f \t %f \t %f  \n", req_omega[0], req_omega[1], req_omega[2], req_omega[3]);
		
		//printf(" %f\t %f \t %f \t %f  \n", omega[0], omega[1], omega[2], omega[3]);
		
		
		set_Omega(&Wheel_arr[0],new_omega[0]);
		set_Omega(&Wheel_arr[1],new_omega[1]);
		set_Omega(&Wheel_arr[2],new_omega[2]);
		set_Omega(&Wheel_arr[3],new_omega[3]);	
	}
}


