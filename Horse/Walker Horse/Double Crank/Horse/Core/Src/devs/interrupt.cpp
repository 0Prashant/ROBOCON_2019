
#include "robo_init.h"

extern leg leg[2];
extern steering steering;
extern bool sand_dune_crossed_flag;
extern bool ROBOT_START_FLAG;
extern bool STEERING_FLAG;

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
        switch (GPIO_Pin) {
		case GPIO_PIN_0 : {
			ROBOT_START_FLAG = true;
			printf("\n\n\t\t\t !!start_pressed!!\n\n");
		}break;

                case GPIO_PIN_9 : {
                        leg[0].steps++;
                        leg[0].reset_angle(300*PI/180);
			printf("\n\n\t\t\tRobot interrupt0\t %d\n", leg[0].steps);
                } break;

                case GPIO_PIN_11: {
                        leg[1].steps++;
                        leg[1].reset_angle(20*PI/180);
			printf("\n\n\t\t\tRobot interrupt1\t %d\n", leg[1].steps);
                } break;

		case GPIO_PIN_15: {
			STEERING_FLAG = true;
			steering.reset_angle(0);
			 printf("\n\n\t\t\tsteering interrupt \t\n");
                } break;

		case GPIO_PIN_8: {
			sand_dune_crossed_flag = true;
			// printf("\nSand_Dune_Crossed \n");
		}

		case GPIO_PIN_10: {
			sand_dune_crossed_flag = true;
			// printf("\nSand_Dune_Crossed \n");
		}
        }
}