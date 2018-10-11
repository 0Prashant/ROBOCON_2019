/*
 * motor.c
 *
 * Created : 10/1/2018
 */

#include "motor.h"


uint16_t time_period(uint16_t PWM_frequency)
{
        if (PWM_frequency < 2000)  //MIN PWM_FREQUENCY = 1281.738 for time period to be 16 bit
	{	
                //_Error_Handler(__FILE__, __LINE__);
        }
        return ((PWM_TIMER_FREQUENCY / 2) / PWM_frequency) - 1; // In Center aligned mode period doubles hence we divide by 2
}

/* SET DUTYCYCLE primary function */
static void set_DutyCycle_Primary(TIM_HandleTypeDef *htim, uint32_t Channel, uint16_t dutyCycle)
{
	uint16_t mapped_value;
	mapped_value = (time_period(MOTOR_DRIVER_FREQUENCY) * dutyCycle) / 65535;
	__HAL_TIM_SET_COMPARE(htim, Channel, mapped_value);
}

void initialize_all_motor(void)
{
	int i;
	for (i = 0; i < 4; i++)
	{
		motor[i].id = i;
		motor[i].radius = 0.067;
		// All motors are connected to same timer : TIM8
		motor[i].htim = &htim8;
		// Initialize velocity and omega to 0 at the first epoch
		motor[i].velcoity = 0;
		motor[i].omega = 0;
	}
	motor[0].in1_port = GPIOD;
	motor[0].in1_pin = GPIO_PIN_1;
	motor[0].in2_port = GPIOD;
	motor[0].in2_pin = GPIO_PIN_3;
	motor[0].channel = TIM_CHANNEL_1;
	
	motor[1].in1_port = GPIOD;
	motor[1].in1_pin = GPIO_PIN_5;
	motor[1].in2_port = GPIOD;
	motor[1].in2_pin = GPIO_PIN_14;
	motor[1].channel = TIM_CHANNEL_2;
	
	motor[2].in1_port = GPIOD;
	motor[2].in1_pin = GPIO_PIN_7;
	motor[2].in2_port = GPIOD;
	motor[2].in2_pin = GPIO_PIN_15;
	motor[2].channel = TIM_CHANNEL_3;
	
	motor[3].in1_port = GPIOC;
	motor[3].in1_pin = GPIO_PIN_12;
	motor[3].in2_port = GPIOC;
	motor[3].in2_pin = GPIO_PIN_10;
	motor[3].channel = TIM_CHANNEL_4;	
}	
	
float float_abs(float x)
{
	if (x < 0)
	{
		return (-1 * x);
	}
	return x;
}

void motor::calculate_omega (void)
{
	omega = velcoity / radius;
}

void motor::set_Direction(enum Direction d)
{
	if (d == FRONT)       // AHEAD
	{
		HAL_GPIO_WritePin(in1_port, in1_pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(in2_port, in2_pin, GPIO_PIN_RESET);
	}
	else if (d == BACK)  // BACK
	{
		HAL_GPIO_WritePin(in1_port, in1_pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(in2_port, in2_pin, GPIO_PIN_SET);
	}
	else                    // HALT
	{
		HAL_GPIO_WritePin(in1_port, in1_pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(in2_port, in2_pin, GPIO_PIN_SET);
	}
}

void motor::set_DutyCycle(uint16_t dutyCycle)
{
        set_DutyCycle_Primary(htim, channel, dutyCycle);
}

void motor::set_Omega(float omega1)
{
	uint16_t new_omega = (65535.0 / MAX_OMEGA) * float_abs(omega);
	if (omega1 > 0) {
		set_Direction(FRONT);
		set_DutyCycle(new_omega);
	}
	else if (omega1 < 0) {
		set_Direction(BACK);
		set_DutyCycle(new_omega);
	}
	else {
		set_Direction(STOP);
		set_DutyCycle(0);
	}
	omega = omega1;
}


