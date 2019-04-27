#ifndef _PID_H_
#define _PID_H_

#include "defines.h"
class pid
{
public:
	pid(){}
	pid(pid &&) = default;
	pid(const pid &) = default;
	pid &operator=(pid &&) = default;
	pid &operator=(const pid &) = default;
	~pid(){}
	void set_pid_constants(float kp, float ki, float kd);
	float compute_pid(float current_value, float set_point);

private:
	float kp_=0, ki_=0, kd_=0;
	float last_output_ = 0;
	float prev_error_ = 0;
	float last_error_ = 0;
};


#endif // !_PID_H_