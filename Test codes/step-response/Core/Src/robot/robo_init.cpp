/*
 * robo_init.c
 *
 * Created : 10/1/2018
 */

#include "robo_init.h"

void pid_Init();

static Wheel_Config gWheel_Configurations[4];
Wheel gWheels[4];

static void wheel_init(void);

void robo_Init()
{
        wheel_init();
        Thinker_Init();
        pid_Init();
}

static void wheel_init(void)
{
        int i;
        for (i = 0; i < 4; i++)
        {
                gWheel_Configurations[i].id = i;
                gWheel_Configurations[i].radius = 0.067;
                // All motors are connected to same timer : TIM8
                gWheel_Configurations[i].htim = &htim8;
        }
        gWheel_Configurations[0].in1_port = GPIOD;
        gWheel_Configurations[0].in1_pin = GPIO_PIN_3; 
        gWheel_Configurations[0].in2_port = GPIOD;
        gWheel_Configurations[0].in2_pin = GPIO_PIN_1;
        gWheel_Configurations[0].channel = TIM_CHANNEL_1;
        gWheel_Configurations[0].henc = &htim1;

        gWheel_Configurations[1].in1_port = GPIOD;
        gWheel_Configurations[1].in1_pin = GPIO_PIN_14;
        gWheel_Configurations[1].in2_port = GPIOD;
        gWheel_Configurations[1].in2_pin = GPIO_PIN_5;
        gWheel_Configurations[1].channel = TIM_CHANNEL_2;
        gWheel_Configurations[1].henc = &htim3;
        
        gWheel_Configurations[2].in1_port = GPIOD;
        gWheel_Configurations[2].in1_pin = GPIO_PIN_7;
        gWheel_Configurations[2].in2_port = GPIOD;
        gWheel_Configurations[2].in2_pin = GPIO_PIN_15;
        gWheel_Configurations[2].channel = TIM_CHANNEL_3;
        gWheel_Configurations[2].henc = &htim2;

        gWheel_Configurations[3].in1_port = GPIOC;
        gWheel_Configurations[3].in1_pin = GPIO_PIN_12;
        gWheel_Configurations[3].in2_port = GPIOC;
        gWheel_Configurations[3].in2_pin = GPIO_PIN_10;
        gWheel_Configurations[3].channel = TIM_CHANNEL_4;
        gWheel_Configurations[3].henc = &htim4;

        for (uint8_t i = 0; i < 4; ++i) {
                gWheels[i].set_Config(&gWheel_Configurations[i]);
        }
}

