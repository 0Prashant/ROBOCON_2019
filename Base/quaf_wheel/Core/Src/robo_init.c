#include "robo_init.h"

struct Leg_str leg_arr[4];

void leg_init(void)
{
        
    
        int i;
        for (i = 0; i < 4; i++)
        {
                leg_arr[i].id = i;
                
                // All motors are connected to same timer : TIM8
                leg_arr[i].htim = &htim8;
                
                leg_arr[i].velcoity = 0;
                leg_arr[i].omega = 0;
        }
        
         /*leg1*/
		leg_arr[0].dir1_port = GPIOA;
        leg_arr[0].dir1_pin = GPIO_PIN_8;
        leg_arr[0].dir2_port = GPIOA;
        leg_arr[0].dir2_pin = GPIO_PIN_15;
        leg_arr[0].channel = TIM_CHANNEL_1;
        
        
        leg_arr[0].pneu_port = GPIOC;
        leg_arr[0].pneu_pin = GPIO_PIN_3;
        
        /*leg2*/
        leg_arr[1].dir1_port = GPIOC;
        leg_arr[1].dir1_pin = GPIO_PIN_11;
        leg_arr[1].dir2_port = GPIOD;
        leg_arr[1].dir2_pin = GPIO_PIN_3;
        leg_arr[1].channel = TIM_CHANNEL_2;
        
        leg_arr[1].pneu_port = GPIOE;
        leg_arr[1].pneu_pin = GPIO_PIN_8;
        
        /*leg3*/
        leg_arr[2].dir1_port = GPIOD;
        leg_arr[2].dir1_pin = GPIO_PIN_4;
        leg_arr[2].dir2_port = GPIOD;
        leg_arr[2].dir2_pin = GPIO_PIN_5;
        leg_arr[2].channel = TIM_CHANNEL_3;
       
        leg_arr[2].pneu_port = GPIOB;
        leg_arr[2].pneu_pin = GPIO_PIN_2;
        
        /*leg4*/
        leg_arr[3].dir1_port = GPIOD;
        leg_arr[3].dir1_pin = GPIO_PIN_7;
        leg_arr[3].dir2_port = GPIOB;
        leg_arr[3].dir2_pin = GPIO_PIN_5;
        leg_arr[3].channel = TIM_CHANNEL_4;
        
        leg_arr[3].pneu_port = GPIOC ;
        leg_arr[3].pneu_pin = GPIO_PIN_1;
        
       
}

