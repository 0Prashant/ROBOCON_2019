#include "robot.h"
extern leg leg[2];
extern steering steering;

extern Vec3<float> initial_angle;
extern Vec3<float> curr_angle;
extern float robot_angle ;
Robot_States robo_state;

bool ROBOT_START_FLAG = false;
bool GEREGE_FLAG = false;
bool USE_IMU_FLAG = true;
bool FRONT_PROXIMITY_FLAG = false;
bool BACK_PROXIMITY_FLAG = false;

float steps[7] = {7, 13, 17, 20, 24, 29, 41};
float angles[7] = {0, 45, 45, 60, 0, -10, -10};



void start_Robot(enum Robot_States *state_)
{
	switch (*state_)
	{
	case HOME:
	{
		/**
		 * vitra ko khutta tala teknaparcha
		 * baira ko khutta mathi teknaparcha
		 * steering IR navetinjel ghumaunaparcha
		 * tespachi stop vaera bascha
		 * user button thichesi this state ends 
		 *  state_A flag set garnaparcha 
			*/
		printf("\n\tWaiting_to_start\t");
		// printf("steering_angle = %d \t", (int)(steering.get_angle()*1800/PI));
		leg[0].set_omega(0);
		leg[1].set_omega(0);
		steering.set_angle(0);
		leg[0].reset_actual_angle(90 * PI / 180);
		leg[1].reset_actual_angle(90 * PI / 180);
		if(!ROBOT_START_FLAG){initial_angle = curr_angle;}
		printf("leg2_angle = %d\tleg1_angle = %d\tsteering_angle = %d\t\trobot_angle = %d\t", (int)(leg[0].get_actual_angle()*180/PI),
		  (int)(leg[1].get_actual_angle()*180/PI), (int)(steering.get_angle()*180/PI), (int)(robot_angle*180/PI));
		if (ROBOT_START_FLAG && GEREGE_FLAG)
		{
			*state_ = MARCH;
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_15, GPIO_PIN_RESET);
		}
	}
	break;

	case MARCH:
	{
			/*
			  user button thichesi state_A starts when state A flag is set
			  10 steps agadi gayesi state_A flag clear garnaparcha
			  state_B flag set garnaparcha
			*/
		printf("March");
		if (go(steps[0], angles[0]) == true)
		{
			*state_ =TURN_45;
			HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_12);
			HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_13);
			HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_14);
			HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_15);
		}
	}
	break;

	case TURN_45:
	{
		/*state _B flag set vayesi angle 45 degree change garnaparcha
			  2 steps jati gayesi sand dune aucha
			  sand dune kateko thapauna proximity sensor use garnaparcha ki
			  ta IMU bata roll value hernaparcha 
			  sand dune kateko thapaesi state_B flag clear garnaparcha
			  state_C flag set garnaparcha			   
			*/
		printf("Turn 45");
		
		/*/
		if (go(steps[1], angles[1]) == true)
		{
			*state_ = SAND_DUNE;
		}
		/*/
		go(99, angles[1]);
		if((HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_10)==GPIO_PIN_RESET) || leg[0].get_steps() > steps[1]){
		// if(HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_10) == GPIO_PIN_RESET){
			*state_ = SAND_DUNE;
			leg[0].steps = steps[1];
			leg[1].steps = steps[1];
			BACK_PROXIMITY_FLAG = false;
			FRONT_PROXIMITY_FLAG = false;
			
			HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_12);
			HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_13);
			HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_14);
			HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_15);
		}
		//*/
	}
	break;
	case SAND_DUNE:
	{
		/*state _C flag set vayesi 55 degree jati ghumna parcha 1 to 2 steps
			after this seate_C flag needs to be cleared and state_D flag needs to be set
			sand dune pachi ko ek dui steps wala region
			*/
		printf("Sand dune");
		/*/
		if (go(steps[2], angles[2]) == true)
		{
			*state_ = STATE_D; 
		}
		/*/
		
		go(100, angles[2]);
		if((HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_8)==GPIO_PIN_RESET) || leg[0].get_steps() > steps[2]){
		// if(HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_8) == GPIO_PIN_RESET){			
			*state_ = STATE_D;
			leg[0].steps = steps[2];
			leg[1].steps = steps[2];
			
			HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_12);
			HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_13);
			HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_14);
			HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_15);
		}
		//*/
	}
	break;
	case STATE_D:
	{
		/*
			state_D start vayesi ek dui stpes sidha janaparcha around 90 degrees from the original position 
			teti steps pugesi 0 angle ma agadi janaparcha 
			tespachi state d flag clear huncha ani state E flag set huncha*/
			printf("State D");
		if (go(steps[3], angles[3]) == true)
		{
			*state_ = TURN_90;
			
			HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_12);
			HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_13);
			HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_14);
			HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_15);
		}
	}
	break;

	case TURN_90:
	{
		/*tussock cross garnaparcha
			no of steps = 
			no of steps le state_E end vako thahuncha 
			*/
		printf("Turn 90");
		if (go(steps[4], angles[4]) == true)
		{
			*state_ = TUSSOCK;
			
			HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_12);
			HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_13);
			HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_14);
			HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_15);
		}
	}
	break;

	case TUSSOCK:
	{
		/*state_F flag set vayesi
			robot stops 
			user button thichesi state_F end huncha
			*/
		printf("tussock");
		if (go(steps[5], angles[5]) == true)
		{
			*state_ = BASE_CAMP;
			
			HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_12);
			HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_13);
			HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_14);
			HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_15);
			initialize_position();
		}
	}
	break;

	case BASE_CAMP:
	{
		/*state_G flag user button thichesi start huncha
			uukhai zone ma n no of steps gayesi state_G end huncha
			*/
		printf("Base camp");
		leg[0].set_omega(0);
		leg[1].set_omega(0);
		steering.set_angle(0);
		if((HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_8)==GPIO_PIN_RESET)){
			*state_ = MOUNTAIN;
			ROBOT_START_FLAG = false;
			
			HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_12);
			HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_13);
			HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_14);
			HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_15);
		}
	}
	break;
	case MOUNTAIN:
	{
		/*state_G flag user button thichesi start huncha
			uukhai zone ma n no of steps gayesi state_G end huncha
			*/
		printf("MOUNTAIN");
		if (ROBOT_START_FLAG)
		{
			if (go(steps[6], angles[6]) == true)
			{
				*state_ = UUKHAI;
				
			HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_12);
			HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_13);
			HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_14);
			HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_15);
			}
		}
		else{
			leg[0].set_omega(0);
			leg[1].set_omega(0);
			steering.set_angle(0);
		}
	}

	break;
	case UUKHAI:
	{
		/*state_G flag user button thichesi start huncha
			uukhai zone ma n no of steps gayesi state_G end huncha
			*/
		initialize_leg_position();
		printf("\nUUKHAI\n");
		HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_12);
		HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_13);
		HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_14);
		HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_15);
	}
	break;
	case TUNE:
	{
		leg[0].set_omega(10);
		leg[1].set_omega(10);
		steering.set_omega(0);
	}
	}
}

bool play()
{	
	start_Robot(&robo_state);
	return true;
}

void zone_select(void){

	if(HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_8) == GPIO_PIN_RESET){
		robo_state = HOME;
	}
	else if(HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_8) == GPIO_PIN_RESET){
		robo_state = SAND_DUNE;
		leg[0].steps = steps[1];
		leg[1].steps = steps[1];
		robot_angle = angles[1];
		initial_angle.set_Values(initial_angle.getX(),initial_angle.getY(),initial_angle.getZ() + angles[1]);
	}
	else if(HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_8) == GPIO_PIN_RESET){
		robo_state = STATE_D;
		leg[0].steps = steps[2];
		leg[1].steps = steps[2];
		robot_angle = angles[2];
		initial_angle.set_Values(initial_angle.getX(),initial_angle.getY(),initial_angle.getZ() + angles[2]);
	}
	else if(HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_8) == GPIO_PIN_RESET){
		robo_state = BASE_CAMP;
		leg[0].steps = steps[5];
		leg[1].steps = steps[5];
		robot_angle = angles[5];
	}
	else {
		robo_state = HOME;
		initialize_position();
	}

	if (HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_8) == GPIO_PIN_RESET){
		for(int i=0; i<7; i++){
			angles[i] *= -1;
		}
	}
	else if (HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_8) == GPIO_PIN_RESET){

	}
}