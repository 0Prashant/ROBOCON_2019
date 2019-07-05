
#include "robo_init.h"

extern leg leg[2];
extern steering steering;

extern bool ROBOT_START_FLAG;
extern bool STEERING_FLAG;
extern bool FRONT_PROXIMITY_FLAG;
extern bool BACK_PROXIMITY_FLAG;
extern bool GEREGE_FLAG;
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
        switch (GPIO_Pin) {
		case GPIO_PIN_0 : {
			ROBOT_START_FLAG = true;
			// printf("\n\n\t\t\t !!start_pressed!!\n\n");
		}break;

                case GPIO_PIN_15 : {
			float leg0_reset_angle = 90*PI/180;
                        leg[0].reset_angle(leg0_reset_angle);
			int temp = round(leg[0].get_actual_angle()/ (360*PI/180));
			leg[0].reset_actual_angle((int)(temp)*2*PI + leg0_reset_angle);
			// printf("\n\n\t\t\tRobot interrupt0\t %d \t %d\n", leg[0].steps, temp);
                } break;

                case GPIO_PIN_14: {
			float leg1_reset_angle = 90*PI/180;
                        leg[1].reset_angle(leg1_reset_angle);
			int temp = round(leg[1].get_actual_angle() / (360*PI/180));
			leg[1].reset_actual_angle((int)(temp)*2*PI + leg1_reset_angle);
			// printf("\n\n\t\t\tRobot interrupt1\t %d\n", leg[1].steps);
                } break;

		case GPIO_PIN_13: {
			STEERING_FLAG = true;
			steering.reset_angle(0*PI/180);
			 printf("\n\n\t\t\tsteering interrupt \t\n");
                } break;
	
        }
}