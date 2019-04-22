#ifndef _LEG_H_
#define _LEG_H_

#include "motor.h"
#include "encoder.h"
#include "pid.h"
#include "limit_switch.h"
#include "defines.h"

class leg
{
public:
	leg(){}
	leg(leg &&) = default;
	leg(const leg &) = default;
	leg &operator=(leg &&) = default;
	leg &operator=(const leg &) = default;
	~leg(){}
        void set_config(motor_config *motor0, motor_config *motor1, encoder_config *encoder) { 
		motor__[0].set_config(motor0);
		motor__[1].set_config(motor1);
		encoder__.set_config(encoder);
	}
	int get_steps();
	void set_omega(float omega);
	float get_omega(void);
	int16_t get_encoder_count(void);
	bool is_raised();
	void reset_angle(float angle_in_radians);
	float get_angle(void);
	float get_max_omega(){return motor__[0].get_max_omega();}
	void set_pid_constants(float kp, float ki, float kd){pid__.set_pid_constants(kp, ki, kd);}
	void set_gravity_compensator_constant(float kbody, float kleg){kb_ = kbody; kl_ = kleg;}
	limit_switch limit_switch__[2];
	int steps;

private:
	motor motor__[2];
	encoder encoder__;
	pid pid__;
	float kb_ = 0, kl_ = 0;
};

#endif // !_LEG_H_
