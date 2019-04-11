#include "drive.h"
/*
void robot_direction(enum robot_direction dir)
{
	
	if(dir == FORWARD)
	{
		set_Direction(&Wheel_arr[0], DIR_ANTICLOCKWISE);
		set_Direction(&Wheel_arr[1], DIR_CLOCKWISE);
		set_Direction(&Wheel_arr[2], DIR_CLOCKWISE);
		set_Direction(&Wheel_arr[3], DIR_ANTICLOCKWISE);
	}
	if(dir == BACKWARD)
	{
		set_Direction(&Wheel_arr[0], DIR_CLOCKWISE);
		set_Direction(&Wheel_arr[1], DIR_ANTICLOCKWISE);
		set_Direction(&Wheel_arr[2], DIR_ANTICLOCKWISE);
		set_Direction(&Wheel_arr[3], DIR_CLOCKWISE);
	}
	if(dir == ROTATE)
	{
		set_Direction(&Wheel_arr[0], DIR_ANTICLOCKWISE);
		set_Direction(&Wheel_arr[1], DIR_ANTICLOCKWISE);
		set_Direction(&Wheel_arr[2], DIR_ANTICLOCKWISE);
		set_Direction(&Wheel_arr[3], DIR_ANTICLOCKWISE);
	}
	if(dir == STOP)
	{
		set_Direction(&Wheel_arr[0], DIR_HALT);
		set_Direction(&Wheel_arr[1], DIR_HALT);
		set_Direction(&Wheel_arr[2], DIR_HALT);
		set_Direction(&Wheel_arr[3], DIR_HALT);
	}
	
	
}*/

void robot_speed(int16_t speed)
{
	for(int i=0; i<4; i++)
	{
	set_DutyCycle(&Wheel_arr[i], speed);
	}
}

