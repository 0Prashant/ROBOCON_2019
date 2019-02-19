/*1-27-2019*/

#include "interrupt.h"
#include "robo_init.h"
#include "blnc_motor.h"

extern struct Blnc balance;

extern State gRobo_State;


// gRobo_State.get_ID() -> giv

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_PIN)
{       if(GPIO_PIN == GPIO_PIN_0)	//blnc_motor_chA_Pin
        {
                if(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0) == 0)	//blnc_motor_chB_Port-pin
                {
                        balance.motor.encoder->count--;
                }
                else
                {
                        balance.motor.encoder->count++;
                }

        } 
	if (GPIO_PIN == balance.interrupt[0].int_pin)
	{
		if(gRobo_State.get_ID() == State_ID::WS3)
		{
			setDutyCycle(&balance.motor, 0);
			setDirection(&balance.motor, DIR_BRAKE);
		}    
        	printf("A\n");
    	}
   	if (GPIO_PIN == balance.interrupt[1].int_pin)
   	{
   		if(gRobo_State.get_ID() == State_ID::WS4)	
		{	
			setDutyCycle(&balance.motor, 0);	
			setDirection(&balance.motor, DIR_BRAKE);
		}	
		printf("B\n");
   	}
   	if (GPIO_PIN == balance.interrupt[2].int_pin)
   	{
   		if((gRobo_State.get_ID()==State_ID::HOME) || (gRobo_State.get_ID()==State_ID::WS1))	
		{	
			setDutyCycle(&balance.motor, 0);
			setDirection(&balance.motor, DIR_BRAKE);	
		}	
		printf("C\n");
   	}
	
   	if (GPIO_PIN == balance.interrupt[3].int_pin)
   	{	
   		printf("D\n");	
		if(gRobo_State.get_ID() == State_ID::WS2)	
		{
			setDutyCycle(&balance.motor, 0);
			setDirection(&balance.motor, DIR_BRAKE);
		}
    	}     
}
