#include "encoder.h"

struct encoder e[4];
float robotx, roboty;


void calculate_distance_of_encoder(struct encoder *encoder)
{
	
	switch (encoder->id)
	{
		case 0:
		{
			encoder->distancex = (encoder->count/(pow(2,0.5))*(CIRCUMFERENCE_OF_WHEEL/PPR));
			encoder->distancey = (encoder->count/(pow(2,0.5))*(CIRCUMFERENCE_OF_WHEEL/PPR));
		}
		case 1:
		{
			encoder->distancex = -(encoder->count/(pow(2,0.5))*(CIRCUMFERENCE_OF_WHEEL/PPR));
			encoder->distancey = (encoder->count/(pow(2,0.5))*(CIRCUMFERENCE_OF_WHEEL/PPR));
		}
		case 2:
		{
			encoder->distancex = -(encoder->count/(pow(2,0.5))*(CIRCUMFERENCE_OF_WHEEL/PPR));
			encoder->distancey = (encoder->count/(pow(2,0.5))*(CIRCUMFERENCE_OF_WHEEL/PPR));
		}
		case 3:
		{
			encoder->distancex = (encoder->count/(pow(2,0.5))*(CIRCUMFERENCE_OF_WHEEL/PPR));
			encoder->distancey = (encoder->count/(pow(2,0.5))*(CIRCUMFERENCE_OF_WHEEL/PPR));
		}
	}	
	encoder->distancex = (encoder->count);
	
}

void calculate_distance_of_robot(void)
{
	for(int i=0;i<4;i++)
	{
		calculate_distance_of_encoder(&e[i]);
		robotx += e[i].distancex;
		roboty += e[i].distancey;
	}	
	robotx/=4;
	roboty/=4;
	
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
