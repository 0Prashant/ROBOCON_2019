/*12-12-2018*/

#ifndef __PID_H__
#define __PID_H__

#include "stm32f4xx_hal.h"

/*Discrete velocity algorithm*/

/* 
 * Integral TF   : ((z + 1) * dt ) / (2 * (z - 1))
 * Derivative Tf : (z - 1) / z * dt
 *
 *
 * u(k) = u(t-1) + a * e(t) + b * e(t-1) + c * e(t - 2)
 * 
 * a = ( Kp + Ki * dt / 2 + Kd / dt )
 * b = ( -Kp + Ki * dt / 2 - 2 * Kd / dt  )
 * c = Kd / dt
 *
 */

struct pidStr
{
	double kp;
	double ki;
	double kd;	
	
	float iterm;
	float output;
	float max_output;
	float min_output;
  float last_output;
    
	float last_error;
  float previous_error;
		
};

void  PID_Init(struct pidStr* pid);
void  PID_resetParams(struct pidStr* pid);
void  PID_setOutputLimit(struct pidStr* pid, float max_output, float min_output);
void  PID_setGains(struct pidStr* pid, float kp, float ki, float kd);
float PID_Compute(struct pidStr* pid,float set_point, float process_value, uint16_t dt);

#endif
