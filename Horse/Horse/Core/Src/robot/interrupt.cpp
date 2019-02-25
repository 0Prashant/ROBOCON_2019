/*1-27-2019*/

#include "interrupt.h"
#include "robo_init.h"
#include "blnc_motor.h"
#include "robo_states.h"

// void Balance_Handle_Interrupt(uint16_t GPIO_Pin);

extern State gHorse_State;
extern Blnc gBallet;
extern bool gReady_To_Go;

//! This is dangerous
State_ID gCurrent_Position = State_ID::WS3;

static bool gFirst_Home_Interrupt = true;

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_PIN)
{
        if (GPIO_PIN == int_arr[0].int_pin)
        {
                printf("Motor_1 \n");
                if (int_arr[0].intState == STATE_B)
                {
                        int_arr[0].intState = STATE_A;
                }
                else if (int_arr[0].intState == STATE_HOME)
                {
                        int_arr[0].fhome_pos = 1;
                }
        }

        if (GPIO_PIN == int_arr[1].int_pin)
        {
                printf("Motor_2 \n");
                if (int_arr[1].intState == STATE_B)
                {
                        int_arr[1].intState = STATE_A;
                }
                else if (int_arr[1].intState == STATE_HOME)
                {
                        int_arr[1].fhome_pos = 1;
                }
        }

        if (GPIO_PIN == int_arr[2].int_pin)
        {
                printf("Motor_3 \n");
                if (int_arr[2].intState == STATE_B)
                {
                        int_arr[2].intState = STATE_A;
                }
                else if (int_arr[2].intState == STATE_HOME)
                {
                        int_arr[2].fhome_pos = 1;
                }
        }

        if (GPIO_PIN == int_arr[3].int_pin)
        {
                printf("Motor_4 \n");
                if (int_arr[3].intState == STATE_B)
                {
                        int_arr[3].intState = STATE_A;
                }
                else if (int_arr[3].intState == STATE_HOME)
                {
                        int_arr[3].fhome_pos = 1;
                }
        }

        //! This zombie code is here for position pid purpose
        // if (GPIO_PIN == GPIO_PIN_12) //blnc_motor_chA_Pin
        // {
        //         printf("int\n");
        //         if (HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_8) != GPIO_PIN_RESET) //blnc_motor_chB_Port-pin
        //         {
        //                 ++gCounter;
        //         }
        //         else
        //         {
        //                 --gCounter;
        //         }
        //         // balance.motor.encoder->count = gCounter;
        // }
	if(!gReady_To_Go)
	{
        	if (GPIO_PIN == gBallet.interrupt[0].int_pin)
        	{
        	        if (gHorse_State.get_ID() == State_ID::WS3)
        	        {
        	                gCurrent_Position = State_ID::WS3;
        	                setDutyCycle(&gBallet.motor, 0);
        	                setDirection(&gBallet.motor, DIR_BRAKE);
				gReady_To_Go = true;
        	        }
			printf("A\n");
        	}
        	if (GPIO_PIN == gBallet.interrupt[1].int_pin)
        	{
        	        if (gHorse_State.get_ID() == State_ID::WS4)
        	        {
        	                gCurrent_Position = State_ID::WS4;
        	                setDutyCycle(&gBallet.motor, 0);
        	                setDirection(&gBallet.motor, DIR_BRAKE);
				gReady_To_Go = true;
        	        }
			printf("B\n");
        	}
        	if (GPIO_PIN == gBallet.interrupt[2].int_pin)
        	{
        	        if ((gHorse_State.get_ID() == State_ID::HOME) || (gHorse_State.get_ID() == State_ID::WS1))
        	        {
        	                if (gFirst_Home_Interrupt)
        	                {
        	                        gFirst_Home_Interrupt = false;
        	                        gCurrent_Position = State_ID::HOME;
        	                }
        	                else
        	                {
        	                        gCurrent_Position = State_ID::WS1;
        	                }
        	                setDutyCycle(&gBallet.motor, 0);
        	                setDirection(&gBallet.motor, DIR_BRAKE);
				gReady_To_Go = true;
        	        }
        	        printf("C\n");
        	}

        	if (GPIO_PIN == gBallet.interrupt[3].int_pin)
        	{
        	        if (gHorse_State.get_ID() == State_ID::WS2)
        	        {
        	                gCurrent_Position = State_ID::WS2;
        	                setDutyCycle(&gBallet.motor, 0);
        	                setDirection(&gBallet.motor, DIR_BRAKE);
				gReady_To_Go = true;
        	        }
        	        printf("D\n");
        	}
	}
}
