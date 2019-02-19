/*22-1-2019*/

#include <math.h>
#include "motor.h"

float floatAbs(float num)
{
    if (num < 0) 
        return (-1) * num;
    else 
        return num;
}

void calculateVelocity(struct motorStr *motor)
{
    motor->velocity = motor->omega * motor->radius;
}

void setDirection(struct motorStr *motor, enum motorDirection d)
{
    if (d == DIR_CLOCKWISE)       
    {
        HAL_GPIO_WritePin(motor->dir1_port, motor->dir1_pin, GPIO_PIN_SET);
        HAL_GPIO_WritePin(motor->dir2_port, motor->dir2_pin, GPIO_PIN_RESET);
    }
    else if (d == DIR_ANTICLOCKWISE)
    {
        HAL_GPIO_WritePin(motor->dir1_port, motor->dir1_pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(motor->dir2_port, motor->dir2_pin, GPIO_PIN_SET);
    }
    else if (d == DIR_HALT)
    {
        HAL_GPIO_WritePin(motor->dir1_port, motor->dir1_pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(motor->dir2_port, motor->dir2_pin, GPIO_PIN_RESET);
    }
    else if (d == DIR_BRAKE)
    {
        HAL_GPIO_WritePin(motor->dir1_port, motor->dir1_pin, GPIO_PIN_SET);
        HAL_GPIO_WritePin(motor->dir2_port, motor->dir2_pin, GPIO_PIN_SET);
    }
    
    else
    {
        // error
    }
}

void setDutyCycle(struct motorStr *motor, uint16_t dutyCycle)
{
    uint16_t mapped_value;
    mapped_value = (10499 * dutyCycle) / 65535;
    
    __HAL_TIM_SET_COMPARE(motor->htim, motor->channel, mapped_value); 
}

void setOmega(struct motorStr *motor, float omega)
{
    uint16_t new_omega = (65535.0 / MAX_OMEGA) * fabs(omega);
    if (omega > 0) 
    {
            setDirection(motor, DIR_CLOCKWISE);
            setDutyCycle(motor, new_omega);
    }
    
    else if (omega < 0) 
    {
            setDirection(motor, DIR_ANTICLOCKWISE);
            setDutyCycle(motor, new_omega);
    }
    
    else 
    {
            setDirection(motor, DIR_HALT);
            setDutyCycle(motor, 0);
    }
}

void getAngle(struct motorStr *motor)
{
        motor->angle = calculateAngle(motor->encoder);
}

void getOmega(struct motorStr *motor)
{
        motor->omega = calculateOmega(motor->encoder);
}

void resetAngle(struct motorStr *motor)
{
    resetEncoderAngle(motor->encoder);
}
