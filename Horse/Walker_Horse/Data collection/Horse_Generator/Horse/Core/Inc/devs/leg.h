#ifndef _LEG_H_
#define _LEG_H_

#include "motor.h"
#include "encoder.h"
#include "limit_switch.h"

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
	bool is_raised();
	void reset_angle(float angle_in_radians);
	float get_angle(void);
	limit_switch limit_switch__[2];
	bool raised_flag = true;
	int steps;

private:
	motor motor__[2];
	encoder encoder__;
};

#endif // !_LEG_H_
