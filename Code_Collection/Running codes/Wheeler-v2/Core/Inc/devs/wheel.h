/*
 * wheel.h
 *
 * Created : 10/1/2018
 */

#ifndef _WHEEL_H_
#define _WHEEL_H_

#include "stm32f4xx_hal.h"
#include "tim.h"

#include "defines.h"

enum Direction
{
        AHEAD,
        BACK,
        HALT
};

struct Wheel_Config
{
        uint8_t id;
        float radius;

        GPIO_TypeDef *in1_port;
        uint16_t in1_pin;
        GPIO_TypeDef *in2_port;
        uint16_t in2_pin;

        TIM_HandleTypeDef *htim;        // PWM Generating Timer
        uint32_t channel;               // PWM Channel

        TIM_HandleTypeDef *henc;
        
        float tolerance;
};

class Wheel
{
public:
        Wheel() { wheel_ = 0; }
        Wheel(Wheel_Config *wheel) { wheel_ = wheel; }
        Wheel(Wheel &&) = default;
        Wheel(const Wheel &) = default;
        Wheel &operator=(Wheel &&) = default;
        Wheel &operator=(const Wheel &) = default;
        ~Wheel() { }
        
        void set_Config(Wheel_Config *wheel) { wheel_ = wheel; }
        void set_Direction(enum Direction d) { dir_ = d; }
        void set_Omega(float omega);
        void update() const;

        float get_Omega(uint32_t dt_millis);
	float get_Angle();

private:
       Wheel_Config *wheel_;
       enum Direction dir_;
       float omega_;    // This value should always be positive
       int16_t wheel_encoder_count;
};


#endif // _WHEEL_H_
