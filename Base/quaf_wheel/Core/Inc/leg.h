/*
 * leg.h
 *
 
 * Created : 12/13/2018
 */

#ifndef _LEG_H_
#define _LEG_H_

#define MAX_OMEGA 12
#include "stm32f4xx_hal.h"
#include "tim.h"
#include "robo_init.h"
#include <math.h>

/*To use this library,
 * This robot consists of 4 legs, each leg consists of two parts,the part above the knee and the part below the knee.
 * The part above the knee is represented by pneumatic and the part below the knee is represented by motors.
 * The pneumatic can be actuated and deactuated.
 * When the pneumatic is deactuated, the leg steps on the ground and when the pneumatic is actuated, the leg steps above the ground.
 * The motor has incremental encoders in it. It can count from 1 to 250, but as the encoder is used in TIM_ENCODER_MODE, the resolution will increase and hence the encoder counts from 1 to 1000
 * The  
 * 1. Initialize the motor of all legs, for this leg_Init() need to be initialized.
*/


enum motor_Status
{
	FORWARD,
	BACKWARD,
	HALT,
	BRAKE
};

enum leg_Status
{
	UP,
	DOWN
};

struct Motor
{
	uint32_t previous_cnt;
	float angle;

	GPIO_TypeDef *dir1_port;
	uint16_t dir1_pin;	
	GPIO_TypeDef *dir2_port;
	uint16_t dir2_pin;
	TIM_HandleTypeDef *htim;
	uint32_t channel;
	TIM_HandleTypeDef *henc;
};
struct Leg
{
	uint8_t id;
	uint8_t home_position;
	
	struct Motor Motor;
	enum motor_Status motor_Status;
	enum leg_Status Status;

	GPIO_TypeDef *pneu_port;
	uint16_t pneu_pin;    
	GPIO_TypeDef *sw_port;
	uint16_t sw_pin;       
};

uint16_t time_period(uint16_t PWM_frequency);
void set_MotorDirection(struct Leg *l, enum motor_Status d);
void set_DutyCycle_Primary(TIM_HandleTypeDef *htim, uint32_t Channel, uint16_t dutyCycle);
void set_DutyCycle(struct Leg *l, uint16_t dutyCycle);
void set_Omega(struct Leg *l, float omega);
float calculate_omega(struct Leg *l);
float calculate_Angle(struct Leg *l);
void set_PneuPosition(struct Leg *l,enum leg_Status);

#endif // _LEG_H_
