#include "drive.h"

void robot_direction(enum robot_direction dir)
{
	
	if(dir == FORWARD)
	{
		set_Direction(&Wheel_arr[0], DIR_ANTICLOCKWISE);
		set_Direction(&Wheel_arr[1], DIR_CLOCKWISE);
	}
	if(dir == BACKWARD)
	{
		set_Direction(&Wheel_arr[0], DIR_CLOCKWISE);
		set_Direction(&Wheel_arr[1], DIR_ANTICLOCKWISE);
	}
	if(dir == ROTATE_LEFT)
	{
		set_Direction(&Wheel_arr[2], DIR_ANTICLOCKWISE);
	}
	if(dir == ROTATE_RIGHT)
	{
		set_Direction(&Wheel_arr[2], DIR_CLOCKWISE);
	}
	if(dir == STOP)
	{
		set_Direction(&Wheel_arr[0], DIR_HALT);
		set_Direction(&Wheel_arr[1], DIR_HALT);
		set_Direction(&Wheel_arr[2], DIR_HALT);
	}
	
	
}

void robot_speed(int16_t speed)
{
	for(int i=0; i<2; i++)
	{
	set_DutyCycle(&Wheel_arr[i], speed);
	}
}
void robot_yaw(int16_t speed)
{
	set_DutyCycle(&Wheel_arr[2], speed);
}


