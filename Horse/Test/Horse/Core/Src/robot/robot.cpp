#include "robot.h"
extern leg leg[2];
extern steering steering;

extern Vec3<float> initial_angle;

Robot_States robo_state;

bool ROBOT_START_FLAG = false;
bool USE_IMU_FLAG = true;
bool FRONT_PROXIMITY_FLAG = false;
bool BACK_PROXIMITY_FLAG = false;

int steps[7] = {9, 14, 18, 22, 27, 35, 51};
float angles[7] = {0, 45, 45, 75, 0, 0, 0};

void start_Robot(enum Robot_States *state_)
{
	calculate_datas();
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
		initial_angle = read_Orientation(10);
		printf("leg2_angle = %d\tleg1_angle = %d\t", (int)(leg[0].get_actual_angle()*180/PI), (int)(leg[1].get_actual_angle()*180/PI));
		if (ROBOT_START_FLAG)
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
		go(100, angles[1]);
		if(FRONT_PROXIMITY_FLAG){
			*state_ = SAND_DUNE;
			leg[0].steps = steps[1];
			leg[1].steps = steps[1];
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
		go(100, angles[1]);
		if(BACK_PROXIMITY_FLAG){			
			*state_ = SAND_DUNE;
			leg[0].steps = steps[2];
			leg[1].steps = steps[2];
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
		}
	}
	break;

	case BASE_CAMP:
	{
		/*state_G flag user button thichesi start huncha
			uukhai zone ma n no of steps gayesi state_G end huncha
			*/
		printf("Base camp");
		if(initialize_leg_position() == true){
			*state_ = MOUNTAIN;
			ROBOT_START_FLAG = false;
		}
	}
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
	}
}

bool play()
{
	uint32_t dt = HAL_GetTick();
	initial_angle = read_Orientation(10);
	initialize_position();

	ROBOT_START_FLAG = false;
	robo_state = HOME;
	
	for(int i=0; i<7; i++){
		angles[i] *= -1;
	}
	
	while(true){
		if ((HAL_GetTick() - dt) >= (int)(SAMPLE_TIME))
		{
			dt = HAL_GetTick();
			start_Robot(&robo_state);
		}
	}
	return true;
}











// while (true)
	// {
	// 	if ((HAL_GetTick() - dt) >= (int)(SAMPLE_TIME))
	// 	{
	// 		dt = HAL_GetTick();
	// 		//printf("\n\tWaiting_to_start\t");
	// 		// printf("steering_angle = %d \t", (int)(steering.get_angle()*1800/PI));
	// 		calculate_datas();
	// 		leg[0].set_omega(0);
	// 		leg[1].set_omega(0);
	// 		steering.set_angle(0);
	// 		if (ROBOT_START_FLAG)
	// 			break;
	// 	}
	// }
	// dt = HAL_GetTick();
	// for (int32_t i = 0; i < 1000000; i++)
	// while(true)	
	// {
	// 	if ((HAL_GetTick() - dt) >= (int)(SAMPLE_TIME))
	// 	{
	// 		dt = HAL_GetTick();
	// 		steering.set_angle(-5);
			
	// 		printf("steering_angle = %d \t", (int)(steering.get_angle()*180/PI));
	// 		initial_angle = read_Orientation(10);
	// 	}
	// }
