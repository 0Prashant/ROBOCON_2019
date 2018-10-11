/*
 * wheel.c
 *
 * Created : 10/1/2018
 */

#include "wheel.h"


uint16_t time_period(uint16_t PWM_frequency)
{
        if (PWM_frequency < 2000) { //MIN PWM_FREQUENCY = 1281.738 for time period to be 16 bit
                _Error_Handler(__FILE__, __LINE__);
        }
        return ((PWM_TIMER_FREQUENCY / 2) / PWM_frequency) - 1; // In Center aligned mode period doubles hence we divide by 2
}

/* SET DUTYCYCLE primary function */
void set_DutyCycle_Primary(TIM_HandleTypeDef *htim, uint32_t Channel, uint16_t dutyCycle)
{
        uint16_t mapped_value;
        mapped_value = (time_period(MOTOR_DRIVER_FREQUENCY) * dutyCycle) / 65535;
        __HAL_TIM_SET_COMPARE(htim, Channel, mapped_value);
}


double float_abs(double x)
{
        if (x < 0)
        {
                return (-1 * x);
        }
        return x;
}



void set_Direction(struct Wheel *w, enum Direction d)
{
        if (d == DIR_ANTICLOCKWISE)       // AHEAD
        {
                HAL_GPIO_WritePin(w->in1_port, w->in1_pin, GPIO_PIN_SET);
                HAL_GPIO_WritePin(w->in2_port, w->in2_pin, GPIO_PIN_RESET);
        }
        else if (d == DIR_CLOCKWISE)  // BACK
        {
                HAL_GPIO_WritePin(w->in1_port, w->in1_pin, GPIO_PIN_RESET);
                HAL_GPIO_WritePin(w->in2_port, w->in2_pin, GPIO_PIN_SET);
        }
        else                    // HALT
        {
                HAL_GPIO_WritePin(w->in1_port, w->in1_pin, GPIO_PIN_SET);
                HAL_GPIO_WritePin(w->in2_port, w->in2_pin, GPIO_PIN_SET);
        }
}

void set_DutyCycle(struct Wheel *w, uint16_t dutyCycle)
{
        set_DutyCycle_Primary(w->htim, w->channel, dutyCycle);
}

void set_ocr(struct Wheel *w, double velocity)
{
	float max_velocity = MAX_VELOCITY*0.99999999999;
        uint16_t ocr = (uint16_t)((65535.0*float_abs(velocity)) / max_velocity)  ;
        if (velocity > 0) {
                set_Direction(w, DIR_ANTICLOCKWISE);
                set_DutyCycle(w, ocr);
        }
        else if (velocity < 0) {
                set_Direction(w, DIR_CLOCKWISE);
                set_DutyCycle(w, ocr);
        }
        else {
                set_Direction(w, DIR_HALT);
                set_DutyCycle(w, 0);
        }
}
