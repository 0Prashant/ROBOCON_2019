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
        DIR_CLOCKWISE,
        DIR_ANTICLOCKWISE,
        DIR_HALT
};

struct Wheel
{
        uint8_t id;
        float radius;
	double next_velocity;
        double velocity;
        GPIO_TypeDef *in1_port;
        uint16_t in1_pin;
        GPIO_TypeDef *in2_port;
        uint16_t in2_pin;

        TIM_HandleTypeDef *htim;
        uint32_t channel;
};

double float_abs(double x);
void set_Direction(struct Wheel *w, enum Direction d);
void set_DutyCycle_Primary(TIM_HandleTypeDef *htim, uint32_t Channel, uint16_t dutyCycle);
void set_DutyCycle(struct Wheel *w, uint16_t dutyCycle);
void set_ocr(struct Wheel *w, double velocity);
void set_Omega(struct Wheel *w,double omega);
uint16_t time_period(uint16_t PWM_frequency);

#endif // _WHEEL_H_
