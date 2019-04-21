#ifndef __MOTOR_H_
#define __MOTOR_H_

#include "tim.h"

enum Direction
{
        CLOCKWISE,
        ANTICLOCKWISE,
        STOP
};

struct motor_config
{
        uint8_t id;

        GPIO_TypeDef *in1_port;
        uint16_t in1_pin;
        GPIO_TypeDef *in2_port;
        uint16_t in2_pin;

        TIM_HandleTypeDef *htim;        // PWM Generating Timer
        uint32_t channel;               // PWM Channel
        TIM_HandleTypeDef *henc;

        float max_omega;                // Max Omega Attained by the wheel(motor)
};

class motor
{
public:
	motor();
	motor(motor &&) = default;
	motor(const motor &) = default;
	motor &operator=(motor &&) = default;
	motor &operator=(const motor &) = default;
	~motor();

	void set_Config(Wheel_Config *wheel) { wheel_ = wheel; }
        void set_Direction(enum Direction d) { dir_ = d; }
        void set_Omega(float omega);
        void update() const;

        float get_Omega(uint32_t dt_millis);
        float get_MaxOmega() { return wheel_->max_omega; }


private:
       motor_config *motor_;
       enum Direction dir_;
       float omega_;
};
#endif //!__MOTOR_H_