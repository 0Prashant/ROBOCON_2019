/*
 * wheel.h
 *
 * Created : 10/1/2018
 */

#ifndef _Wheel_H_
#define _Wheel_H_

#include "stm32f4xx_hal.h"
#include "tim.h"

#include "defines.h"

enum Direction
{
        DIR_AHEAD,
        DIR_BACK,
        DIR_HALT
};

struct Wheel
{
        uint8_t id;
        float radius;
        float velcoity;

        float omega;

        GPIO_TypeDef *in1_port;
        uint16_t in1_pin;
        GPIO_TypeDef *in2_port;
        uint16_t in2_pin;

        TIM_HandleTypeDef *htim;
        uint32_t channel;
};

float float_abs(float x);
void calculate_omega(struct Wheel *w);
void set_Direction(struct Wheel *w, enum Direction d);
void set_DutyCycle(struct Wheel *w, uint16_t dutyCycle);
void set_Omega(struct Wheel *w, float omega);

#endif // _WHEEL_H_
