/*
 * leg.c
 *
 * Created : 12/13/2018
 */

#include "leg.h"

/* SET DUTYCYCLE primary function */
void set_DutyCycle_Primary(TIM_HandleTypeDef *htim, uint32_t Channel, uint16_t dutyCycle)
{
        uint16_t mapped_value;
        mapped_value = (10499 * dutyCycle) / 65535;
        __HAL_TIM_SET_COMPARE(htim, Channel, mapped_value);
}


float float_abs(float x)
{
        if (x < 0)
        {
                return (-1 * x);
        }
        return x;
}

void calculate_omega(struct Leg_str *leg)
{
        // v = wr
        leg->omega = leg->velcoity / leg->radius;
}

void set_MotorDirection(struct Leg_str *leg, enum motor_Direction d)
{
        if (d == DIR_FORWARD)       
        {
                HAL_GPIO_WritePin(leg->dir1_port, leg->dir1_pin, GPIO_PIN_SET);
                HAL_GPIO_WritePin(leg->dir2_port, leg->dir2_pin, GPIO_PIN_RESET);
        }
        else if (d == DIR_BACKWARD)
        {
                HAL_GPIO_WritePin(leg->dir1_port, leg->dir1_pin, GPIO_PIN_RESET);
                HAL_GPIO_WritePin(leg->dir2_port, leg->dir2_pin, GPIO_PIN_SET);
        }
        else if (d == DIR_HALT)
        {
                HAL_GPIO_WritePin(leg->dir1_port, leg->dir1_pin, GPIO_PIN_RESET);
                HAL_GPIO_WritePin(leg->dir2_port, leg->dir2_pin, GPIO_PIN_RESET);
        }
        else if(d == DIR_BRAKE)
        {
             HAL_GPIO_WritePin(leg->dir1_port, leg->dir1_pin, GPIO_PIN_SET);
             HAL_GPIO_WritePin(leg->dir2_port, leg->dir2_pin, GPIO_PIN_SET);
        }
}

void set_PneuPosition(struct Leg_str *leg, enum pneu_Position p)
{
	if(p == ACTUATE)
	{
		HAL_GPIO_WritePin(leg->pneu_port, leg->pneu_pin,GPIO_PIN_SET);
	}
	else if(p == DEACTUATE)
	{
		HAL_GPIO_WritePin(leg->pneu_port,leg->pneu_pin,GPIO_PIN_RESET);
	}
}

void set_DutyCycle(struct Leg_str *leg, uint16_t dutyCycle)
{
        set_DutyCycle_Primary(leg->htim, leg->channel, dutyCycle);
}

void set_Omega(struct Leg_str *leg, float omega)
{
        uint16_t new_omega = (65535.0 / MAX_OMEGA) * float_abs(omega);
        if (omega > 0) {
                set_MotorDirection(leg, DIR_FORWARD);
                set_DutyCycle(leg, new_omega);
        }
        else if (omega < 0) {
                set_MotorDirection(leg, DIR_BACKWARD);
                set_DutyCycle(leg, new_omega);
        }
        else {
                set_MotorDirection(leg, DIR_HALT);
                set_DutyCycle(leg, 0);
        }
        
        leg->omega = omega;
}

