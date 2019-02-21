/*22-1-2019*/

#ifndef _MOTOR_H_
#define _MOTOR_H_

#define MAX_OMEGA 17
#include "stm32f4xx_hal.h"
#include "tim.h"
#include "encoder.h" 

enum motorDirection
{
        DIR_CLOCKWISE,
        DIR_ANTICLOCKWISE,
        DIR_HALT,
        DIR_BRAKE
};

struct motorStr
{
    float omega;
    float angle;
    float radius;  
    float velocity;
    float update_omega; 
    float angle_home_pos;
    
    /*Should be initialized*/
    GPIO_TypeDef *dir1_port;
    uint16_t dir1_pin;
    
    GPIO_TypeDef *dir2_port;
    uint16_t dir2_pin;
    
    TIM_HandleTypeDef *htim;
    uint32_t channel;

    struct encoderStr *encoder;
};

void setDirection(struct motorStr *motor, enum motorDirection d);
void setDutyCycle(struct motorStr *motor, uint16_t dutyCycle);
void setOmega(struct motorStr *motor, float omega);

void resetAngle(struct motorStr *motor);
void getAngle(struct motorStr *motor);
void getOmega(struct motorStr *motor);
 
uint16_t time_period(uint16_t PWM_frequency);
float float_abs(float x);

#endif /*motor.h*/
