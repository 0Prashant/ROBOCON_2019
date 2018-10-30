#include "drive.h"

//const float Speed_Multiplier = 1;
//const float time = 1;
const int coupling_matrix[4][3] = {	{    1,    1,    1   },
					{    1,   -1,    1   },
					{   -1,   -1,    1   },
					{   -1,    1,    1   }	  };
float velocity[3];
uint32_t ramp_counter;

void calculate_robot_velocity()
{
        for (int i = 0; i < 4; i++)
        {
                Wheel_arr[i].next_velocity = 0;
                for (int j = 0; j < 3; j++)
                {
                        Wheel_arr[i].next_velocity += velocity[j] * coupling_matrix[i][j];
                }
//		Wheel_arr[i].next_velocity *= 1.4142135623 ;
		if(Wheel_arr[i].next_velocity >MAX_VELOCITY)
			Wheel_arr[i].next_velocity = MAX_VELOCITY;
		ramp(Wheel_arr[i].id);
		//set_ocr(&Wheel_arr[i], Wheel_arr[i].velocity);
		//printf("%f\t%f\n",Wheel_arr[i].next_velocity,Wheel_arr[i].velocity );
        }
}

void ramp(uint8_t wheel_no)
{
	if(Wheel_arr[wheel_no].velocity != Wheel_arr[wheel_no].next_velocity)
	{
		if( (Wheel_arr[wheel_no].velocity + RAMPING_FACTOR) < Wheel_arr[wheel_no].next_velocity)
			Wheel_arr[wheel_no].velocity += RAMPING_FACTOR;
		else if( (Wheel_arr[wheel_no].velocity - RAMPING_FACTOR) > Wheel_arr[wheel_no].next_velocity)
			Wheel_arr[wheel_no].velocity -= RAMPING_FACTOR;
		else
		{
			Wheel_arr[wheel_no].velocity= Wheel_arr[wheel_no].next_velocity;
		}
	
		set_ocr(&Wheel_arr[wheel_no], Wheel_arr[wheel_no].velocity);
	}	
		//printf("%f\t%f\n",Wheel_arr[wheel_no].next_velocity,Wheel_arr[wheel_no].velocity );
}


