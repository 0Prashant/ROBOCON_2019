/*
 * robo_init.c
 *
 * Created : 10/1/2018
 */

#include "robo_init.h"

struct Wheel Wheel_arr[4];

void wheel_init(void)
{
        int i;
        for (i = 0; i < 4; i++)
        {
                Wheel_arr[i].id = i;
                Wheel_arr[i].radius = 0.067;
                // All motors are connected to same timer : TIM8
                Wheel_arr[i].htim = &htim8;
                // Initialize velocity = 0 at the first epoch
                Wheel_arr[i].velocity = 0.000000;
		Wheel_arr[i].next_velocity = 0.000000;
        }
        Wheel_arr[0].in1_port = GPIOD;
        Wheel_arr[0].in1_pin = GPIO_PIN_3;
        Wheel_arr[0].in2_port = GPIOD;
        Wheel_arr[0].in2_pin = GPIO_PIN_1;
        Wheel_arr[0].channel = TIM_CHANNEL_1;

        Wheel_arr[1].in1_port = GPIOD;
        Wheel_arr[1].in1_pin = GPIO_PIN_14;
        Wheel_arr[1].in2_port = GPIOD;
        Wheel_arr[1].in2_pin = GPIO_PIN_5 ;
        Wheel_arr[1].channel = TIM_CHANNEL_2;
        
        Wheel_arr[2].in1_port = GPIOD;
        Wheel_arr[2].in1_pin = GPIO_PIN_7;
        Wheel_arr[2].in2_port = GPIOD;
        Wheel_arr[2].in2_pin = GPIO_PIN_15;
        Wheel_arr[2].channel = TIM_CHANNEL_3;

        Wheel_arr[3].in1_port = GPIOC;
        Wheel_arr[3].in1_pin = GPIO_PIN_12;
        Wheel_arr[3].in2_port = GPIOC;
        Wheel_arr[3].in2_pin = GPIO_PIN_10;
        Wheel_arr[3].channel = TIM_CHANNEL_4;
}

