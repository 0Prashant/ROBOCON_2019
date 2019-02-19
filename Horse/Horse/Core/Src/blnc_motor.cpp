#include "blnc_motor.h"

struct Blnc balance;
State gRobo_State;
State_ID gcurrent_position = State_ID::WS1;
uint16_t bvel = 45000;

void balance_init(void)
{
	/******Initializing Interrupts******/
	balance.interrupt[0].int_port = GPIOA;
	balance.interrupt[0].int_pin = GPIO_PIN_14;
	balance.interrupt[1].int_port = GPIOB;
        balance.interrupt[1].int_pin = GPIO_PIN_15;
	balance.interrupt[2].int_port = GPIOC;
        balance.interrupt[2].int_pin = GPIO_PIN_7;
	balance.interrupt[3].int_port = GPIOE;
        balance.interrupt[3].int_pin = GPIO_PIN_3;

	/******Initializing Motor******/
	balance.motor.dir1_port = GPIOA;
	balance.motor.dir1_pin = GPIO_PIN_3;
	balance.motor.dir2_port = GPIOA;
	balance.motor.dir2_pin = GPIO_PIN_4;
	balance.motor.channel = TIM_CHANNEL_3;
	balance.motor.update_omega = 0;
	balance.motor.htim = &htim5;
	HAL_TIM_Base_Start(balance.motor.htim);
        HAL_TIM_PWM_Start(balance.motor.htim, balance.motor.channel);

	/******Initializing Encoder******/
	balance.motor.encoder->ref_count = 0;
	balance.motor.encoder->last_count = 0;
	balance.motor.encoder->ppr = 79;

	/******Initializing PID******/
	PID_Init(&balance.pid);
	balance.pid.max_output = 12;
	balance.pid.min_output = -12;
	balance.pid.kp = 0.3;
	balance.pid.ki = 0.02;
	balance.pid.last_error = 0;
	balance.pid.output = 0;
	balance.pid.previous_error = 0;
	balance.pid.last_output = 0;
}

void check_N_run(void)
{
	if(gcurrent_position != gRobo_State.get_ID())
	{
		switch (gRobo_State.get_ID())
		{
			case State_ID::HOME:
			{
				setDutyCycle(&balance.motor, bvel);	
				setDirection(&balance.motor, DIR_CLOCKWISE);
				break;
			}
			case State_ID::WS1:
			{
				setDutyCycle(&balance.motor, bvel);	
				setDirection(&balance.motor, DIR_CLOCKWISE);
				break;
			}
			case State_ID::WS2:
			{
				setDutyCycle(&balance.motor, bvel);	
				setDirection(&balance.motor, DIR_CLOCKWISE);
				break;
			}
			case State_ID::WS3:
			{
				setDutyCycle(&balance.motor, bvel);	
				setDirection(&balance.motor, DIR_CLOCKWISE);
				break;
			}
			case State_ID::WS4:
			{
				setDutyCycle(&balance.motor, bvel);	
				setDirection(&balance.motor, DIR_CLOCKWISE);
				break;
			}
		}
		printf("%d\n", bvel);
	}	
}