/*
 * robot.c
 *
 * Created : 10/1/2018
 */

#include "robo_init.h"

#define HUMAN_RESPONSE          (0.07)

// Constants that need to be modified according to robot's config
const float Speed_Multiplier = 1.5;
const float time = 1;

const int Coupling_Matrix[4][3] = {	{    1,   1,    1},
					{    1,  -1,    1},
					{   -1,  -1,    1},
					{   -1,   1,    1}};

// Latest delta_x and delta_y as received
//static float final_delta_x, final_delta_y;
//static float curr_delta_x, curr_delta_y;



void calculate_robot_velocity(float dx, float dy)
{
        float x,y;
        x = dx * Speed_Multiplier;
        y = dy * Speed_Multiplier;
        float velocity[3] = {x / time, y / time, 0};

        for (int i = 0; i < 4; i++)
        {
                Wheel_arr[i].velcoity = 0;
                for (int j = 0; j < 3; j++)
                {
                        Wheel_arr[i].velcoity += velocity[j] * Coupling_Matrix[i][j];
                }
                calculate_omega(&Wheel_arr[i]);
        }
}

void run_CurrentConfig(void)
{
        for (uint8_t i = 0; i < 4; ++i) {
                set_Omega(&Wheel_arr[i], Wheel_arr[i].omega);
        }
}

void play(void)
{
       uint16_t speed=0;
	int downcount=0;
        uint8_t buffer[1];
        while (1) {
                if (HAL_UART_Receive(&huart1, buffer, 1, 2) == HAL_OK)
			printf("%c",buffer[0]);
		if(speed<58000)
			speed += 500;
		robot_speed(speed);
		if(buffer[0]!='S')
			downcount=20;
		switch (buffer[0])
		{
			
			case 'F':
			{
				robot_direction(FORWARD);
				break;
			}
			case 'B':
			{
				set_Direction(&Wheel_arr[0], DIR_CLOCKWISE);
				set_Direction(&Wheel_arr[1], DIR_ANTICLOCKWISE);
				set_Direction(&Wheel_arr[2], DIR_ANTICLOCKWISE);
				set_Direction(&Wheel_arr[3], DIR_CLOCKWISE);
				break;
			}
			case 'L':
			{
				set_Direction(&Wheel_arr[0], DIR_CLOCKWISE);
				set_Direction(&Wheel_arr[1], DIR_CLOCKWISE);
				set_Direction(&Wheel_arr[2], DIR_ANTICLOCKWISE);
				set_Direction(&Wheel_arr[3], DIR_ANTICLOCKWISE);
				break;
			}
			case 'R':
			{
				set_Direction(&Wheel_arr[0], DIR_ANTICLOCKWISE);
				set_Direction(&Wheel_arr[1], DIR_ANTICLOCKWISE);
				set_Direction(&Wheel_arr[2], DIR_CLOCKWISE);
				set_Direction(&Wheel_arr[3], DIR_CLOCKWISE);
				break;
			}
			case 'G':
			{
				set_Direction(&Wheel_arr[0], DIR_HALT);
				set_Direction(&Wheel_arr[1], DIR_CLOCKWISE);
				set_Direction(&Wheel_arr[2], DIR_HALT);
				set_Direction(&Wheel_arr[3], DIR_ANTICLOCKWISE);
				break;
			}
			case 'I':
			{
				set_Direction(&Wheel_arr[0], DIR_ANTICLOCKWISE);
				set_Direction(&Wheel_arr[1], DIR_HALT);
				set_Direction(&Wheel_arr[2], DIR_CLOCKWISE);
				set_Direction(&Wheel_arr[3], DIR_HALT);
				break;
			}
			case 'H':
			{
				set_Direction(&Wheel_arr[0], DIR_CLOCKWISE);
				set_Direction(&Wheel_arr[1], DIR_CLOCKWISE);
				set_Direction(&Wheel_arr[2], DIR_CLOCKWISE);
				set_Direction(&Wheel_arr[3], DIR_CLOCKWISE);
				break;
			}
			case 'J':
			{
				set_Direction(&Wheel_arr[0], DIR_ANTICLOCKWISE);
				set_Direction(&Wheel_arr[1], DIR_ANTICLOCKWISE);
				set_Direction(&Wheel_arr[2], DIR_ANTICLOCKWISE);
				set_Direction(&Wheel_arr[3], DIR_ANTICLOCKWISE);
				break;
			}
			case 'S':
			{
				
				if(downcount>0)
				{
					speed -=2000;
					downcount--;
					break;
				}
				set_Direction(&Wheel_arr[0], DIR_HALT);
				set_Direction(&Wheel_arr[1], DIR_HALT);
				set_Direction(&Wheel_arr[2], DIR_HALT);
				set_Direction(&Wheel_arr[3], DIR_HALT);
				break;

			}
			default:
			{
			}
				
		}
		/*
                calculate_robot_velocity(curr_delta_x, curr_delta_y);
                run_CurrentConfig();*/
        }
}
