#ifndef _LEG_H_
#define _LEG_H_

#include "motor.h"
#include "encoder.h"
#include "pid.h"
#include "limit_switch.h"
#include "defines.h"
#include "pid_algorithms.h"

enum Leg_condition{	RAISED, 
			FALLEN, 
			UNDEFINED};

class leg
{
public:
	leg() {}
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
	void set_PID_constants(float kp, float ki, float kd, float lower_limit, float upper_limit) {
		dpid_.set_P(kp);
		dpid_.set_I(ki);
		dpid_.set_D(kd);
		dpid_.set_Limits(upper_limit, lower_limit);
	}

	
	void set_omega(float omega);
	void set_gravity_compensator_constant(float kbody, float kleg){kb_ = kbody; kl_ = kleg;}
	void reset_angle(float angle_in_radians);
	void calculate_omega();
	Leg_condition is_raised();
	bool is_raised_without_deadzone();

	int get_steps(){return steps;}
	float get_omega(void){return encoder__.get_omega();}
	float get_angle(void){return encoder__.get_angle();}
	float get_max_omega(){return motor__[0].get_max_omega();}
	int16_t get_encoder_count(void){return encoder__.get_count();}

	limit_switch limit_switch__[2];
	int steps = 0;

private:
	Discrete_PID dpid_;
	motor motor__[2];
	encoder encoder__;
	float kb_ = 0, kl_ = 0;
};

#endif // !_LEG_H_
