#ifndef _STEERING_H_
#define _STEERING_H_

#include "motor.h"
#include "encoder.h"
#include "pid.h"
#include "limit_switch.h"

class steering
{
public:
	steering(){}
	steering(steering &&) = default;
	steering(const steering &) = default;
	steering &operator=(steering &&) = default;
	steering &operator=(const steering &) = default;
	~steering(){}
        void set_config(motor_config *motor, encoder_config *encoder) { 
		motor__.set_config(motor);
		encoder__.set_config(encoder);
	}

	void set_omega(float omega);
	float get_omega(void);
	void reset_angle(float angle_in_radians);
	float get_angle(void);
	void set_PID_constants(float kp, float ki, float kd, float lower_limit, float upper_limit) {
		dpid_.set_P(kp);
		dpid_.set_I(ki);
		dpid_.set_D(kd);
		dpid_.set_Limits(upper_limit, lower_limit);
	}
	limit_switch limit_switch__[1];
	

private:
	motor motor__;
	encoder encoder__;
	Discrete_PID dpid_;
};

#endif // !_STEERING_H_
