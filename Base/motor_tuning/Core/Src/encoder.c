#include "encoder.h"

struct encoder e[4];
float robotx = 0, roboty = 0;
float distance[2][3];		//set1, set2 (distancex, distancey, yaw)
//int16_t counts[4] =  {0,0,0,0};

void init_encoder()
{
	for(int i=0; i<4; i++)
	{
		e[i].id = i;
	}
}	

void calculate_encoder_distance(struct encoder *e)
{
	//Assuming that encoder count increases in anticlockwise direction
	if(e->id==0)
	{
		e->distance = -(e->count)*(CIRCUMFERENCE_OF_WHEEL/PPR);
	}
	else if(e->id==1)
	{
		e->distance = -(e->count)*(CIRCUMFERENCE_OF_WHEEL/PPR);
	}
	else if(e->id==2)
	{
		e->distance = (e->count)*(CIRCUMFERENCE_OF_WHEEL/PPR);	
	}
	else if(e->id==3)
	{
		e->distance = (e->count)*(CIRCUMFERENCE_OF_WHEEL/PPR);
	}
	else{}
	/*if(e->id==0)
	{
		e->distancex = (e->count*0.70710678)*(CIRCUMFERENCE_OF_WHEEL/PPR);
		e->distancey = (e->count*0.70710678)*(CIRCUMFERENCE_OF_WHEEL/PPR);
	}
	else if(e->id==1)
	{
		e->distancex = (e->count*0.70710678)*(CIRCUMFERENCE_OF_WHEEL/PPR);
		e->distancey = -(e->count*0.70710678)*(CIRCUMFERENCE_OF_WHEEL/PPR);
	}
	else if(e->id==2)
	{
		e->distancex = -(e->count*0.70710678)*(CIRCUMFERENCE_OF_WHEEL/PPR);
		e->distancey = -(e->count*0.70710678)*(CIRCUMFERENCE_OF_WHEEL/PPR);
	}
	else if(e->id==3)
	{
		e->distancex = -(e->count*0.70710678)*(CIRCUMFERENCE_OF_WHEEL/PPR);
		e->distancey = (e->count*0.70710678)*(CIRCUMFERENCE_OF_WHEEL/PPR);
	}
	else{}
	*/
}
void calculate_robot_distance()
{
	float d13, d24;
	static float theta =0;
	for(int i=0;i<4;i++)
	{
		calculate_encoder_distance(&e[i]);
	}
	d13 = (e[1].distance + e[3].distance)/2;
	d24 = (e[2].distance + e[4].distance)/2;
	robotx += d24 * sin(theta + PI/4) - d13 * sin(theta - PI/4);	//this is to be modified according to robot motor direction
	roboty += d13 * sin(theta+PI/4) + d24 * sin(theta-PI/4);
	theta += ((e[3].distance - e[1].distance + e[4].distance - e[2].distance)/WIDTH);//*(180/3.14159265359) //here the theta is in radian
	
	/*
	for(int i=0;i<4;i++)
	{
		calculate_encoder_distance(&e[i]);
		robotx += e[i].distancex;
		roboty += e[i].distancey;
	}
	robotx /= 4;
	roboty /= 4;*/

}


void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
        switch (GPIO_Pin) {
                case GPIO_PIN_1 : {             // PA1
                        if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0))
                        {
                                ++e[0].count;
                        }
                        else
                        {
                                --e[0].count;
                        }
                } break;

                case GPIO_PIN_7 : {             // PA7
                        if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_6))
                        {
                                ++e[1].count;
                        }
                        else
                        {
                                --e[1].count;
                        }
                } break;

                case GPIO_PIN_11 : {            // PE11
                        if (HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_9))
                        {
                                ++e[2].count;
                        }
                        else 
                        {
                                --e[2].count;
                        }
                } break;

                case GPIO_PIN_13 : {            // PD13
                        if (HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_12))
                        {
                                ++e[3].count; 
                        }
                        else
                        {
                                --e[3].count;
                        }
                } break;

                default : {

                }
        }
}

