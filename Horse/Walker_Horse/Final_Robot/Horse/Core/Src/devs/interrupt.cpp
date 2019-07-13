
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
			float tolerence = 40*PI/180;
			static int count1 = 0;
			if(((leg[0].get_angle()>(leg0_reset_angle-tolerence)) && (leg[0].get_angle()<(leg0_reset_angle+tolerence))) || (count1>2)){
                        	leg[0].reset_angle(leg0_reset_angle);
				int temp = round(leg[0].get_actual_angle()/ (360*PI/180));
				leg[0].reset_actual_angle((int)(temp)*2*PI + leg0_reset_angle);
				count1 = 0;
			}
			else{
				count1++;
			}
			// printf("\n\n\t\t\tRobot interrupt0\t %d \t %d\n", leg[0].steps, temp);
                } break;

                case GPIO_PIN_14: {
			float leg1_reset_angle = 90*PI/180;
			float tolerence = 40*PI/180;
			static int count2 = 0;
			if(((leg[1].get_angle()>(leg1_reset_angle-tolerence)) && (leg[1].get_angle()<(leg1_reset_angle+tolerence))) || (count2>2)){
                        	leg[1].reset_angle(leg1_reset_angle);
				int temp = round(leg[1].get_actual_angle() / (360*PI/180));
				leg[1].reset_actual_angle((int)(temp)*2*PI + leg1_reset_angle);
				count2 = 0;
			}
			else{
				count2++;
			}
			// printf("\n\n\t\t\tRobot interrupt1\t %d\n", leg[1].steps);
                } break;

		case GPIO_PIN_13: {
			STEERING_FLAG = true;
			float tolerence = 3*PI/180;
			static int count3 = 0;
			if(((steering.get_angle()>(-tolerence)) && (leg[1].get_angle()<(tolerence))) || (count3>2)){
				steering.reset_angle(0*PI/180);
				count3 = 0;
			}
			else{
				count3++;
			}
			//  printf("\n\n\t\t\tsteering interrupt \t\n");
                } break;
	
        }
}