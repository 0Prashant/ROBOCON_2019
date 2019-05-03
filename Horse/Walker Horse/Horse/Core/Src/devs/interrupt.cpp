
#include "robo_init.h"

extern leg leg[1];
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
        switch (GPIO_Pin) {
                case GPIO_PIN_0 : {		
                        leg[0].steps++;		//leg[0].limit_switch__[0].int_pin
                        leg[0].reset_angle(116*PI/180);
			// printf("\tRobot interrupt 0\t %d\n", leg[0].steps);
                } break;

                case GPIO_PIN_2: {	//leg[0].limit_switch__[1].int_pin
			//printf("\tRobot interrupt 1 \t");
                } break;
        }
}