#include "cmsis_os.h"
#include "robot.h"

extern leg leg[2];
extern steering steering;

extern Vec3<float> initial_angle;
extern Vec3<float> curr_angle;
extern float robot_angle;
Robot_States robo_state;

bool ROBOT_START_FLAG = false;
bool GEREGE_FLAG = false;
bool USE_IMU_FLAG = true;
bool FRONT_PROXIMITY_FLAG = false;
bool BACK_PROXIMITY_FLAG = false;
float omega0 = 0;
float omega1 = 0;

// float steps[7] = {7, 13, 17, 19, 24, 27, 36};        //for 7 rad/sec
float steps[7] = {6, 13, 17, 19, 24, 27, 36};		//for 8 rad/sec
float angles[7] = {0, 45, 45, 45, 0, 0, -90};


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
		static bool WAIT_FLAG = true;
		printf("\n\tWaiting_to_start\t");
		// printf("steering_angle = %d \t", (int)(steering.get_angle()*1800/PI));
		if(WAIT_FLAG){
			leg[0].set_omega(0);
			leg[1].set_omega(0);
			steering.set_angle(0);
		}
		if (!ROBOT_START_FLAG)
		{
			initial_angle = curr_angle;
		}
		else
		{
			HAL_GPIO_WritePin(Grip_Pneumatic_GPIO_Port, Grip_Pneumatic_Pin, GPIO_PIN_SET);
		}
		printf("leg0_angle = %d\tleg1_angle = %d\tsteering_angle = %d\t\trobot_angle = %d\t", (int)(leg[0].get_angle() * 180 / PI),
		       (int)(leg[1].get_actual_angle() * 180 / PI), (int)(steering.get_angle() * 180 / PI), (int)(robot_angle * 180 / PI));
		if ((HAL_GPIO_ReadPin(GEREGE_SWITCH_GPIO_Port, GEREGE_SWITCH_Pin)==GPIO_PIN_RESET) && ROBOT_START_FLAG)
		{

			HAL_GPIO_WritePin(GreenLED_GPIO_Port, GreenLED_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(OrangeLED_GPIO_Port, OrangeLED_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(RedLED_GPIO_Port, RedLED_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(BlueLED_GPIO_Port, BlueLED_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(Grip_Pneumatic_GPIO_Port, Grip_Pneumatic_Pin, GPIO_PIN_RESET);
			
			static uint32_t init_time = HAL_GetTick();
			if ((HAL_GetTick() - init_time) >= (uint32_t)(1000))
			{
				WAIT_FLAG = false;
				if(getup_n_run() == true){
					printf("\n\n\n\t\tMARCHMARCHMARCH\n\n\n");
				*state_ = MARCH;
				}
			}
		}
	}
	break;

	case MARCH:
	{
		/*
			  Gerege switch thichesi state_A starts when state A flag is set
			  10 steps agadi gayesi state_A flag clear garnaparcha
			  state_B flag set garnaparcha
		*/
		printf("March");
		if (go(steps[0], angles[0]) == true)
		{
			*state_ = TURN_45;
			HAL_GPIO_TogglePin(GreenLED_GPIO_Port, GreenLED_Pin);
			HAL_GPIO_TogglePin(OrangeLED_GPIO_Port, OrangeLED_Pin);
			HAL_GPIO_TogglePin(RedLED_GPIO_Port, RedLED_Pin);
			HAL_GPIO_TogglePin(BlueLED_GPIO_Port, BlueLED_Pin);
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

		go(99, angles[1]);
		if ((HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_10) == GPIO_PIN_RESET) || leg[0].get_steps() > steps[1])
		{
			// if(HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_10) == GPIO_PIN_RESET){
			*state_ = SAND_DUNE;
			leg[0].steps = steps[1];
			leg[1].steps = steps[1];
			BACK_PROXIMITY_FLAG = false;
			FRONT_PROXIMITY_FLAG = false;

			HAL_GPIO_TogglePin(GreenLED_GPIO_Port, GreenLED_Pin);
			HAL_GPIO_TogglePin(OrangeLED_GPIO_Port, OrangeLED_Pin);
			HAL_GPIO_TogglePin(RedLED_GPIO_Port, RedLED_Pin);
			HAL_GPIO_TogglePin(BlueLED_GPIO_Port, BlueLED_Pin);
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

		go(100, angles[2]);
		if (((HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_8) == GPIO_PIN_RESET) && (leg[0].get_steps() > (steps[2] - 1))) || leg[0].get_steps() > steps[2])
		{
			// if(HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_8) == GPIO_PIN_RESET){
			*state_ = STATE_D;
			leg[0].steps = steps[2];
			leg[1].steps = steps[2];

			HAL_GPIO_TogglePin(GreenLED_GPIO_Port, GreenLED_Pin);
			HAL_GPIO_TogglePin(OrangeLED_GPIO_Port, OrangeLED_Pin);
			HAL_GPIO_TogglePin(RedLED_GPIO_Port, RedLED_Pin);
			HAL_GPIO_TogglePin(BlueLED_GPIO_Port, BlueLED_Pin);
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

			HAL_GPIO_TogglePin(GreenLED_GPIO_Port, GreenLED_Pin);
			HAL_GPIO_TogglePin(OrangeLED_GPIO_Port, OrangeLED_Pin);
			HAL_GPIO_TogglePin(RedLED_GPIO_Port, RedLED_Pin);
			HAL_GPIO_TogglePin(BlueLED_GPIO_Port, BlueLED_Pin);
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

			HAL_GPIO_TogglePin(GreenLED_GPIO_Port, GreenLED_Pin);
			HAL_GPIO_TogglePin(OrangeLED_GPIO_Port, OrangeLED_Pin);
			HAL_GPIO_TogglePin(RedLED_GPIO_Port, RedLED_Pin);
			HAL_GPIO_TogglePin(BlueLED_GPIO_Port, BlueLED_Pin);
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
			if (initialize_leg_position() == true)
			{
				*state_ = BASE_CAMP;
				
				HAL_GPIO_TogglePin(GreenLED_GPIO_Port, GreenLED_Pin);
				HAL_GPIO_TogglePin(OrangeLED_GPIO_Port, OrangeLED_Pin);
				HAL_GPIO_TogglePin(RedLED_GPIO_Port, RedLED_Pin);
				HAL_GPIO_TogglePin(BlueLED_GPIO_Port, BlueLED_Pin);
			}
		}
	}
	break;

	case BASE_CAMP:
	{
		/*state_G flag user button thichesi start huncha
			uukhai zone ma n no of steps gayesi state_G end huncha
			*/
		printf("\nBase camp");
		leg[0].set_omega(0);
		leg[1].set_omega(0);
		steering.set_angle(0);
		if ((HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_10) == GPIO_PIN_RESET))
		{
			*state_ = MOUNTAIN;
			angles[6] = robot_angle * 180 / PI;
			ROBOT_START_FLAG = false;
			
			HAL_GPIO_TogglePin(GreenLED_GPIO_Port, GreenLED_Pin);
			HAL_GPIO_TogglePin(OrangeLED_GPIO_Port, OrangeLED_Pin);
			HAL_GPIO_TogglePin(RedLED_GPIO_Port, RedLED_Pin);
			HAL_GPIO_TogglePin(BlueLED_GPIO_Port, BlueLED_Pin);
		}
	}
	break;
	case MOUNTAIN:
	{
		/*state_G flag user button thichesi start huncha
			uukhai zone ma n no of steps gayesi state_G end huncha
			*/
		printf("MOUNTAIN");
		if (go(steps[6], angles[6]) == true)
		{
			if (initialize_leg_position() == true)
			{
				*state_ = UUKHAI;

				HAL_GPIO_TogglePin(GreenLED_GPIO_Port, GreenLED_Pin);
				HAL_GPIO_TogglePin(OrangeLED_GPIO_Port, OrangeLED_Pin);
				HAL_GPIO_TogglePin(RedLED_GPIO_Port, RedLED_Pin);
				HAL_GPIO_TogglePin(BlueLED_GPIO_Port, BlueLED_Pin);
			}
		}
	}

	break;
	case UUKHAI:
	{
		/*state_G flag user button thichesi start huncha
			uukhai zone ma n no of steps gayesi state_G end huncha
			*/
		printf("\nUUKHAI\n");
		leg[0].set_omega(0);  
		leg[1].set_omega(0);
		steering.set_omega(0);
		
		HAL_GPIO_TogglePin(GreenLED_GPIO_Port, GreenLED_Pin);
		HAL_GPIO_TogglePin(OrangeLED_GPIO_Port, OrangeLED_Pin);
		HAL_GPIO_TogglePin(RedLED_GPIO_Port, RedLED_Pin);
		HAL_GPIO_TogglePin(BlueLED_GPIO_Port, BlueLED_Pin);
		HAL_GPIO_WritePin(Hand_Pneumatic_GPIO_Port, Hand_Pneumatic_Pin, GPIO_PIN_SET);
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
	omega0 = leg[0].get_omega();
	omega1 = leg[1].get_omega();
	return true;
}

void zone_select(void)
{

	if (HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_8) == GPIO_PIN_RESET)
	{
		robo_state = HOME;
		GEREGE_FLAG = true;
		ROBOT_START_FLAG = true;
	}
	else if (HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_8) == GPIO_PIN_RESET)
	{
		robo_state = SAND_DUNE;
		leg[0].steps = steps[1];
		leg[1].steps = steps[1];
		float temp_angle_ = angles[1];
		for (int i = 0; i < 7; i++)
		{
			angles[i] -= temp_angle_;
		}
		initial_angle.set_Values(initial_angle.getX(), initial_angle.getY(), initial_angle.getZ() + angles[1]);
	}
	else if (HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_8) == GPIO_PIN_RESET)
	{
		robo_state = STATE_D;
		leg[0].steps = steps[2];
		leg[1].steps = steps[2];
		float temp_angle_ = angles[2];
		for (int i = 0; i < 7; i++)
		{
			angles[i] -= temp_angle_;
		}
		initial_angle.set_Values(initial_angle.getX(), initial_angle.getY(), initial_angle.getZ() + angles[2]);
	}
	else if (HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_8) == GPIO_PIN_RESET)
	{
		robo_state = BASE_CAMP;
		leg[0].steps = steps[5];
		leg[1].steps = steps[5];
		float temp_angle_ = angles[5];
		for (int i = 0; i < 7; i++)
		{
			angles[i] -= temp_angle_;
		}
	}
	else
	{
		robo_state = HOME;
	}

	if (HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_8) == GPIO_PIN_RESET)
	{
		for (int i = 0; i < 7; i++)
		{
			angles[i] *= -1;
		}
	}
	else if (HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_8) == GPIO_PIN_RESET)
	{
	}
}