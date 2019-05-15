/**
 * steering.h
 *
 * \brief Provide control over steering of the horse
 *
 * This file is meant as an wrapper to the elements in the encoder. 
 * It uses the functions of other headers (ie motor.h, encoder.h, 
 * limit_switch.h and pid.h tocontrol all the functions available 
 * in the steering.
 *
 * \author 	$Author 	: Prashant_Bhatta
 * \date 	$Date		: 2005/04/14 14:16:20 $
 * \contact	$Contact	: 073bex425.prashant@pcampus.edu.np
 *
 */
#ifndef _STEERING_H_
#define _STEERING_H_

#include "motor.h"
#include "encoder.h"
#include "pid.h"
#include "limit_switch.h"

class steering
{
public:
	/// These lines of codes are just for protection of random defination of the objects.
	steering(){}
	steering(steering &&) = default;
	steering(const steering &) = default;
	steering &operator=(steering &&) = default;
	steering &operator=(const steering &) = default;
	~steering(){}

	/**
	 * \brief This function setups the configurations for the steering
	 * \param motor: pointer to motor_config which contains all the 
	 * 		configurations for a motor
	 * \param encoder: pointer to encoder_config which contains all the 
	 * 		configurations for a motor  
	 * \return void
	 */
        void set_config(motor_config *motor, encoder_config *encoder) { 
		motor__.set_config(motor);
		encoder__.set_config(encoder);
	}

	/**
	 * \brief Sets constants for PID algorithm
	 * \param kp,ki,kd: constants for PID
	 * \param lower_limit, upper_limit: These are the maximum output a motor 
	 * 	   can give with gear reduction(in omegas)
	 * \return void
	 */
	void set_PID_constants(float kp, float ki, float kd, float lower_limit, float upper_limit) {
		dpid_.set_P(kp);
		dpid_.set_I(ki);
		dpid_.set_D(kd);
		dpid_.set_Limits(upper_limit, lower_limit);
	}

	/** 
	 * \brief sets omega for the steering
	 * 
	 */
	void set_omega(float omega);

	/**
	 * \brief resets the angle provided by the encoder
	 * \param angle_in_radian at which it is to be reset
	 * \return void
	 */
	void reset_angle(float angle_in_radians) {encoder__.reset_angle(angle_in_radians);}

	/**
	 * \brief calculates omega of the steering motor
	 * \remark Dont forget to call this function every 10 ms
	 * \param void
	 * \return void
	 */
	void calculate_omega(){	encoder__.calculate_omega();}

	/**
	 * \brief returns the current omega calculated by the encoder
	 */
	float get_omega(void){return encoder__.get_omega();}

	/**
	 * \brief returns the current angle calculated by the encoder
	 */
	float get_angle(void){return encoder__.get_angle();}

	/**
	 * \brief returns the current count calculated by the encoder
	 */
	int16_t get_encoder_count(void){return encoder__.get_count();}
	limit_switch limit_switch__[1];
	

private:
	motor motor__;
	encoder encoder__;
	Discrete_PID dpid_;
};

#endif // !_STEERING_H_
