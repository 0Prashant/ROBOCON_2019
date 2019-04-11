/*
 * robot.c
 *
 * Created : 10/1/2018
 */

#include "robo_init.h"
extern struct Wheel Wheel_arr[4];
uint16_t max_front_velocity = 30000;
uint16_t max_rotation_velocity = 40000;
uint16_t front_velocity = 0;
uint16_t rotation_velocity = 0;

void play(void)
{
        uint8_t buffer[1];
        while (1) {
                if (HAL_UART_Receive(&huart3, buffer, 1, 10) == HAL_OK){
			if((buffer[0] == 'S') || (buffer[0] == 's'))
				HAL_GPIO_WritePin(GPIOD,GPIO_PIN_14, GPIO_PIN_SET);
			else{
				HAL_GPIO_WritePin(GPIOD,GPIO_PIN_14, GPIO_PIN_RESET);
				HAL_GPIO_TogglePin(GPIOD,GPIO_PIN_12);
			}
			printf("%c",buffer[0]);
		}
		//HAL_UART_Transmit(&huart3, buffer, 1, 10);
		//printf("Data = %c",buffer[0]);	
		//buffer[0] = 'F';
		switch (buffer[0])
		{
			case 'F':
			{
				robot_direction(FORWARD);
				if(front_velocity<max_front_velocity)
					front_velocity++;
				if(rotation_velocity>0)
					rotation_velocity--;
				break;
			}
			case 'B':
			{
				robot_direction(BACKWARD);
				if(front_velocity<max_front_velocity)
					front_velocity++;
				if(rotation_velocity>0)
					rotation_velocity--;
				break;
			}
			case 'L':
			{
				robot_direction(ROTATE_LEFT);
				if(front_velocity>0)
					front_velocity--;
				if(rotation_velocity<max_rotation_velocity)
					rotation_velocity++;
				break;
			}
			case 'R':
			{
				robot_direction(ROTATE_RIGHT);
				if(front_velocity>0)
					front_velocity--;
				if(rotation_velocity<max_rotation_velocity)
					rotation_velocity++;
				robot_speed(front_velocity);
				robot_yaw(rotation_velocity);
				break;
			}
			case 'S':
			{
				robot_direction(STOP);
				if(front_velocity>0)
					front_velocity--;
				if(rotation_velocity>0)
					rotation_velocity--;
				break; 
			}
			default:
			{
			}		
		}
		robot_speed(front_velocity);
		robot_yaw(rotation_velocity);
	}
}
