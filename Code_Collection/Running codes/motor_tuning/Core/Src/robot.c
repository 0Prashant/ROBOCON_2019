/*
 * robot.c
 *
 * Created : 10/1/2018
 */

#include "robo_init.h"
#include "drive.h"
#include "encoder.h"
#define HUMAN_RESPONSE          (0.07)
#define NUM 25
extern struct encoder e[4];
extern float robotx, roboty;
extern float velocity[3];
static uint8_t index = 0;
float time ;
const uint8_t Speed_Multiplier = 1;
//float ROBOT_VELOCITY = 1.0000;

// Constants that need to be modified according to robot's config



void play(void)
{
	//uint16_t speed=0;
	//int downcount=0;
        uint8_t buffer[1];
        while (1) 
	{
                if (HAL_UART_Receive(&huart1, buffer, 1, 2) == HAL_OK)
			printf("%c",buffer[0]);
		/*if(speed<58000)
			speed += 500;
		//robot_speed(speed);
		if(buffer[0]!='S')
			downcount=20;*/
		switch (buffer[0])
		{
			
			case 'F':
			{
				velocity[0] = 0;
				velocity[1] = ROBOT_VELOCITY;
				velocity[2] = 0;
				break;
			}
			case 'B':
			{
				velocity[0] = 0;
				velocity[1] = -ROBOT_VELOCITY;
				velocity[2] = 0;
				break;
			}
			case 'L':
			{
				velocity[0] = -ROBOT_VELOCITY;
				velocity[1] = 0;
				velocity[2] = 0;
				break;
			}
			case 'R':
			{
				velocity[0] = ROBOT_VELOCITY;
				velocity[1] = 0;
				velocity[2] = 0;
				break;
			}
			case 'G':
			{	
				velocity[0] = -ROBOT_VELOCITY/2;
				velocity[1] = ROBOT_VELOCITY/2;
				velocity[2] = 0;
				break;
			}
			case 'I':
			{
				velocity[0] = ROBOT_VELOCITY/2;
				velocity[1] = ROBOT_VELOCITY/2;
				velocity[2] = 0;
				break;
			}
			case 'H':
			{
				velocity[0] = 0;
				velocity[1] = 0;
				velocity[2] = ROBOT_VELOCITY;
				break;
			}
			case 'J':
			{
				velocity[0] = 0;
				velocity[1] = 0;
				velocity[2] = ROBOT_VELOCITY;
				break;
			}
			case 'S':
			{
				
				/*if(downcount>0)
				{
					speed -=2000;
					downcount--;
					break;
				}*/
				velocity[0] = 0;
				velocity[1] = 0;
				velocity[2] = 0;;
				break;

			}
			default:
			{
			}
				
		}
		
                calculate_robot_velocity();
                //run_CurrentConfig();
        }
}

//float deltas[NUM][2] = {{0.01,0},{0.01,0},{0.02,0},{0.03,0},{0.04,0},{0.05,0},{0.06,0},{0.07,0},{0.08,0},{0.09,0},{0.1,0},{0.1,0},{0.1,0},{0.1,0},{0.09,0},{0.08,0},{0.07,0},{0.06,0},{0.05,0},{0.04,0},{0.03,0},{0.02,0},{0.01,0},{0,0}
//};
int goto_distance(float dx, float dy)
{
	
	//if(((float_abs(robotx)-float_abs(dx))>=0)&&((float_abs(roboty)-float_abs(dy))>=0))
	while(1)
	{	
		double distance;
		distance = pow((dx*dx+dy*dy),0.5);
		time = distance/ROBOT_VELOCITY;			
		velocity[0] = dx / time;
		velocity[1] = dy / time;
		velocity[2] = 0;
		calculate_robot_velocity();
		calculate_robot_distance();
		printf("%d \t%d \t%d \t%d \t%f \t%f \t%f \t%f \t%f \t%f\n", e[0].count, e[1].count, e[2].count, e[3].count,e[0].distancex, e[1].distancex, e[2].distancex, e[3].distancex, robotx, roboty);
		
		if(((float_abs(robotx)-float_abs(dx))>=0)&&((float_abs(roboty)-float_abs(dy))>=0))
		{
			velocity[0]=0;
			velocity[1]=0;
			velocity[2]=0;
			calculate_robot_velocity();
			return 1;
			
		}
		
			
	}

	//++index;		
}
