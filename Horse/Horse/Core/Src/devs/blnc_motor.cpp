#include "blnc_motor.h"
#include "robo_states.h"

extern State gHorse_State;
extern State_ID gCurrent_Position;

struct Blnc gBallet;
uint16_t bvel = 45000;

void balance_Init(void)
{
	/******Initializing Interrupts******/
	gBallet.interrupt[0].int_port = GPIOC;
	gBallet.interrupt[0].int_pin = GPIO_PIN_14;
	gBallet.interrupt[1].int_port = GPIOC;
        gBallet.interrupt[1].int_pin = GPIO_PIN_15;
	gBallet.interrupt[2].int_port = GPIOC;
        gBallet.interrupt[2].int_pin = GPIO_PIN_7;
	gBallet.interrupt[3].int_port = GPIOB;
        gBallet.interrupt[3].int_pin = GPIO_PIN_12;

	/******Initializing Motor******/
	gBallet.motor.dir1_port = GPIOA;
	gBallet.motor.dir1_pin = GPIO_PIN_3;
	gBallet.motor.dir2_port = GPIOA;
	gBallet.motor.dir2_pin = GPIO_PIN_4;
	gBallet.motor.channel = TIM_CHANNEL_3;
	gBallet.motor.update_omega = 0;
	gBallet.motor.htim = &htim5;
	HAL_TIM_Base_Start(gBallet.motor.htim);
        HAL_TIM_PWM_Start(gBallet.motor.htim, gBallet.motor.channel);

	/******Initializing Encoder******/
	gBallet.motor.encoder->count = 0;
	gBallet.motor.encoder->ref_count = 0;
	gBallet.motor.encoder->last_count = 0;
	gBallet.motor.encoder->ppr = 79;

	/******Initializing PID******/
	PID_Init(&gBallet.pid);
	gBallet.pid.max_output = 12;
	gBallet.pid.min_output = -12;
	gBallet.pid.kp = 0.3;
	gBallet.pid.ki = 0.02;
	gBallet.pid.last_error = 0;
	gBallet.pid.output = 0;
	gBallet.pid.previous_error = 0;
	gBallet.pid.last_output = 0;
}

void balance_Loop(void)
{
	// printf("%ld\n", (gBallet.motor.encoder->count));
	if(gCurrent_Position != gHorse_State.get_ID())
	{
		switch (gHorse_State.get_ID())
		{
			case State_ID::HOME:
			{
				setDutyCycle(&gBallet.motor, bvel);	
				setDirection(&gBallet.motor, DIR_CLOCKWISE);
				break;
			}
			case State_ID::WS1:
			{
				setDutyCycle(&gBallet.motor, bvel);	
				setDirection(&gBallet.motor, DIR_CLOCKWISE);
				break;
			}
			case State_ID::WS2:
			{
				setDutyCycle(&gBallet.motor, bvel);	
				setDirection(&gBallet.motor, DIR_CLOCKWISE);
				break;
			}
			case State_ID::WS3:
			{
				setDutyCycle(&gBallet.motor, bvel);	
				setDirection(&gBallet.motor, DIR_CLOCKWISE);
				break;
			}
			case State_ID::WS4:
			{
				setDutyCycle(&gBallet.motor, bvel);	
				setDirection(&gBallet.motor, DIR_CLOCKWISE);
				break;
			}
		}
		//printf("%d\n", bvel);
	}	
}