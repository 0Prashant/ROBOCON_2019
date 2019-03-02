#include "robo_init.h"

struct Leg Leg[4];
struct Trap_struct Motion_Profile[4];
struct PID PID[4];

void robot_init()
{
	leg_init();
	pid_init();
}
void leg_init()
{
	for (int i=0 ; i<4; i++)
        {
            Leg[i].id = i;
			Leg[i].home_position = 0;
			Leg[i].Status = DOWN;
        }
	motor_init();
	encoder_init();
	
        Leg[0].pneu_port = GPIOC;
        Leg[0].pneu_pin = GPIO_PIN_3;
        Leg[1].pneu_port = GPIOE;
        Leg[1].pneu_pin = GPIO_PIN_8;
        Leg[2].pneu_port = GPIOB;
        Leg[2].pneu_pin = GPIO_PIN_2;
        Leg[3].pneu_port = GPIOC ;
        Leg[3].pneu_pin = GPIO_PIN_1;

	Leg[0].sw_port = GPIOC;
        Leg[0].sw_pin = GPIO_PIN_13;
        Leg[1].sw_port = GPIOE;
        Leg[1].sw_pin = GPIO_PIN_5;
        Leg[2].sw_port = GPIOE;
        Leg[2].sw_pin = GPIO_PIN_3;
        Leg[3].sw_port = GPIOC ;
        Leg[3].sw_pin = GPIO_PIN_1;
}
void motor_init()
{
	for (int i=0 ; i<4; i++)
    {     
			Leg[i].Motor.angle = 0;
			Leg[i].Motor.previous_cnt = 0;
	}

	HAL_TIM_Base_Start(&htim8);
  	HAL_TIM_PWM_Start(&htim8, TIM_CHANNEL_1);
  	HAL_TIM_PWM_Start(&htim8, TIM_CHANNEL_2);
  	HAL_TIM_PWM_Start(&htim8, TIM_CHANNEL_3);    
  	HAL_TIM_PWM_Start(&htim8, TIM_CHANNEL_4);

         /*leg1*/
	Leg[0].Motor.dir1_port = GPIOA;
        Leg[0].Motor.dir1_pin = GPIO_PIN_8;
        Leg[0].Motor.dir2_port = GPIOA;
        Leg[0].Motor.dir2_pin = GPIO_PIN_15;
        Leg[0].Motor.channel = TIM_CHANNEL_1;
        /*leg2*/
        Leg[1].Motor.dir1_port = GPIOC;
        Leg[1].Motor.dir1_pin = GPIO_PIN_11;
        Leg[1].Motor.dir2_port = GPIOD;
        Leg[1].Motor.dir2_pin = GPIO_PIN_3;
        Leg[1].Motor.channel = TIM_CHANNEL_2;
        /*leg3*/
        Leg[2].Motor.dir1_port = GPIOD;
        Leg[2].Motor.dir1_pin = GPIO_PIN_4;
        Leg[2].Motor.dir2_port = GPIOD;
        Leg[2].Motor.dir2_pin = GPIO_PIN_5;
        Leg[2].Motor.channel = TIM_CHANNEL_3;
        /*leg4*/
        Leg[3].Motor.dir1_port = GPIOD;
        Leg[3].Motor.dir1_pin = GPIO_PIN_7;
        Leg[3].Motor.dir2_port = GPIOB;
        Leg[3].Motor.dir2_pin = GPIO_PIN_5;
        Leg[3].Motor.channel = TIM_CHANNEL_4;
}
void pid_init()
{
	set_OutputLimit(&PID[0],12,-12);
    	set_OutputLimit(&PID[1],12,-12);
    	set_OutputLimit(&PID[2],12,-12);
    	set_OutputLimit(&PID[3],12,-12);
	set_Gains(&PID[0], 0.56, 0, 0);
	set_Gains(&PID[1], 0.56, 0, 0);
	set_Gains(&PID[2], 0.56, 0, 0);
	set_Gains(&PID[3], 0.56, 0, 0);
}
void encoder_init()
{
	HAL_TIM_Encoder_Start(&htim1, TIM_CHANNEL_ALL);
	HAL_TIM_Encoder_Start(&htim2, TIM_CHANNEL_ALL);
	HAL_TIM_Encoder_Start(&htim3, TIM_CHANNEL_ALL);
	HAL_TIM_Encoder_Start(&htim4, TIM_CHANNEL_ALL);
	
	Leg[0].Motor.henc = &htim1;
	Leg[1].Motor.henc = &htim2;
	Leg[2].Motor.henc = &htim3;
	Leg[3].Motor.henc = &htim4;
}
void motion_Profile_init()
{
	//motionProfile_Init(&Motion_Profile[0], 5, 60, 50, dt_secs);
	//motionProfile_Init(&Motion_Profile[1], 5, 60, 50, dt_secs);
	//motionProfile_Init(&Motion_Profile[2], 5, 60, 50, dt_secs);
	//motionProfile_Init(&Motion_Profile[3], 5, 60, 50, dt_secs);
}



