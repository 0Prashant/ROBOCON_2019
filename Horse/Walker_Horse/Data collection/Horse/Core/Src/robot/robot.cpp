/**
 * up proximity only detects the shagai thrown 
 * while outer leg touches, speed increases by 1.3 
 */
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
bool USE_IMU_FLAG = false;
float angle_offset1 = 1.2;
float angle_offset2 = 6;

float steps[7] = {6, 10, 15, 17, 22, 25, 32};
float angles[7] = {0 , 45, 45, 68, 0, 0, -90};

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

		static bool INITIAL_ANGLE_FLAG = true;
		printf("\nHOME");
		if (HAL_GPIO_ReadPin(IMU_Detect_GPIO_Port, IMU_Detect_Pin) == GPIO_PIN_RESET)
		{
			USE_IMU_FLAG = true;
		}
		else
		{
			USE_IMU_FLAG = false;
			robot_angle = 0;
		}
		// printf("steering_angle = %d \t", (int)(steering.get_angle()*1800/PI));

		if (!ROBOT_START_FLAG && INITIAL_ANGLE_FLAG)
		{
			initial_angle = curr_angle;
			printf("  not set  ");
			// initial_angle.setZ(initial_angle.getZ());
			// printf("\t%d\t", (int)initial_angle.getZ());
		}
		else if (ROBOT_START_FLAG)
		{
			INITIAL_ANGLE_FLAG = false;
			printf("\n\nset\n\n");
			HAL_GPIO_WritePin(Grip_Pneumatic_GPIO_Port, Grip_Pneumatic_Pin, GPIO_PIN_SET);
		}
		printf("  leg0_angle = %d\tleg1_angle = %d\tsteering_angle = %d\trobot_angle = %d\trobot_angle = %d\t", (int)(leg[0].get_angle() * 180 / PI),
		       (int)(leg[1].get_actual_angle() * 180 / PI), (int)(steering.get_angle() * 180 / PI), (int)(robot_angle*180/PI), (int)(curr_angle.getZ()));
		if ((HAL_GPIO_ReadPin(ZONE_INIT_GPIO_Port, ZONE_INIT_Pin) == GPIO_PIN_RESET) ||
		    (HAL_GPIO_ReadPin(ZONE_45_GPIO_Port, ZONE_45_Pin) == GPIO_PIN_RESET) ||
		    (HAL_GPIO_ReadPin(ZONE_SAND_GPIO_Port, ZONE_SAND_Pin) == GPIO_PIN_RESET) ||
		    (HAL_GPIO_ReadPin(ZONE_MOUNTAIN_GPIO_Port, ZONE_MOUNTAIN_Pin) == GPIO_PIN_RESET))
		{
			HAL_GPIO_WritePin(Grip_Pneumatic_GPIO_Port, Grip_Pneumatic_Pin, GPIO_PIN_RESET);
			ROBOT_START_FLAG = true;
			INITIAL_ANGLE_FLAG = false;
			initial_angle = curr_angle;
			if (getup_n_run() == true)
			{
				zone_select();
			}
		}
		else if ((HAL_GPIO_ReadPin(GEREGE_SWITCH_GPIO_Port, GEREGE_SWITCH_Pin) == GPIO_PIN_RESET))
		{

			HAL_GPIO_WritePin(GreenLED_GPIO_Port, GreenLED_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(OrangeLED_GPIO_Port, OrangeLED_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(RedLED_GPIO_Port, RedLED_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(BlueLED_GPIO_Port, BlueLED_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(Grip_Pneumatic_GPIO_Port, Grip_Pneumatic_Pin, GPIO_PIN_RESET);
			ROBOT_START_FLAG = false;

			static uint32_t init_time = HAL_GetTick();
			if ((HAL_GetTick() - init_time) >= (uint32_t)(500))
			{
				if (getup_n_run() == true)
				{
					zone_select();
				}
			}
		}

		else
		{
			leg[0].set_omega(0);
			leg[1].set_omega(0);
			steering.set_angle(0);
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
		printf("\nMarch");
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
		printf("\nTurn 45");

		go(99, angles[1]);
		if ((HAL_GPIO_ReadPin(Proximity_Front_GPIO_Port, Proximity_Front_Pin) == GPIO_PIN_RESET) || leg[0].get_steps() > steps[1])
		{
			// if(HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_10) == GPIO_PIN_RESET){
			*state_ = SAND_DUNE;
			leg[0].steps = steps[1];
			leg[1].steps = steps[1];

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
		printf("\nSand dune");

		go(100, angles[2]);
		if (((HAL_GPIO_ReadPin(Proximity_Back_GPIO_Port, Proximity_Back_Pin) == GPIO_PIN_RESET) && (leg[0].get_steps() > (steps[2] - 2))) || leg[0].get_steps() >= steps[2])
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
		printf("\nState D");
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
		printf("\nTurn 90");
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
		static bool INITIALIZATION_FLAG = false;
		printf("\nTussock");
		if (!INITIALIZATION_FLAG)
		{
			if (go(steps[5], angles[5]) == true) 
			{
				INITIALIZATION_FLAG = true;
			}
		}
		else
		{
			if (initialize_leg_position() == true)
			{
				*state_ = BASE_CAMP;
				ROBOT_START_FLAG = false;
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
		if (((HAL_GPIO_ReadPin(Proximity_Up_GPIO_Port, Proximity_Up_Pin) == GPIO_PIN_RESET)) &&
		    ROBOT_START_FLAG)
		{
			*state_ = MOUNTAIN;
			angles[6] = robot_angle * 180 / PI;
			ROBOT_START_FLAG = false;
			leg[0].steps = steps[5];
			leg[1].steps = steps[5];
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

		static bool INITIALIZATION_FLAG_2 = false;
		printf("\nMOUNTAIN");
		if (!INITIALIZATION_FLAG_2)
		{
			if (go(steps[6], angles[6]) == true)
			{
				INITIALIZATION_FLAG_2 = true;
				leg[0].steps = steps[5];
				leg[1].steps = steps[5];
			}
		}
		else
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
	return true;
}

void zone_select(void)
{

	if (HAL_GPIO_ReadPin(ZONE_INIT_GPIO_Port, ZONE_INIT_Pin) == GPIO_PIN_RESET)
	{
		robo_state = MARCH;
		ROBOT_START_FLAG = true;
		printf("\n\n\tZone MARCH");
	}
	else if (HAL_GPIO_ReadPin(ZONE_45_GPIO_Port, ZONE_45_Pin) == GPIO_PIN_RESET)
	{
		printf("\n\n\tZone 45");
		robo_state = TURN_45;
		leg[0].steps = steps[1] - 2;
		leg[1].steps = steps[1] - 2;
		float temp_angle_ = angles[1];
		for (int i = 0; i < 7; i++)
		{
			angles[i] -= temp_angle_;
		}
		// initial_angle.set_Values(initial_angle.getX(), initial_angle.getY(), initial_angle.getZ() + angles[1]);
	}
	else if (HAL_GPIO_ReadPin(ZONE_SAND_GPIO_Port, ZONE_SAND_Pin) == GPIO_PIN_RESET)
	{
		printf("\n\n\tZone SAND_DUNE");
		robo_state = STATE_D;
		leg[0].steps = steps[2];
		leg[1].steps = steps[2];
		float temp_angle_ = angles[2];
		for (int i = 0; i < 7; i++)
		{
			angles[i] -= temp_angle_;
		}
		// initial_angle.set_Values(initial_angle.getX(), initial_angle.getY(), initial_angle.getZ() + angles[2]);
	}
	else if (HAL_GPIO_ReadPin(ZONE_MOUNTAIN_GPIO_Port, ZONE_MOUNTAIN_Pin) == GPIO_PIN_RESET)
	{
		printf("\n\n\tZone MOUNTAIN");
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
		printf("\n\n\tZone Default");
		robo_state = MARCH;
	}

	if (HAL_GPIO_ReadPin(ZONE_RED_GPIO_Port, ZONE_RED_Pin) == GPIO_PIN_RESET)
	{
		printf("\tZone RED");
		for (int i = 0; i < 7; i++)
		{
			angles[i] *= -1;
			// angles[i] -= 7.5;
			// printf("\n\t angles= %d",(int)(angles[i]));
		}
		angles[0] -= angle_offset1;
		angles[1] -= angle_offset2;
		angles[2] -= angle_offset2;
	}
	else if (HAL_GPIO_ReadPin(ZONE_BLUE_GPIO_Port, ZONE_BLUE_Pin) == GPIO_PIN_RESET)
	{
		printf("\tZone BLUE");for (int i = 0; i < 7; i++)
		{
			// angles[i] -= 7.5;
			//printf("\n\t angles= %d",(int)(angles[i]));
		}
		angles[0] -= angle_offset1;
		angles[1] -= angle_offset2;
		angles[2] -= angle_offset2;
	}
	else
	{
		printf("\tZone Default_RED");
		for (int i = 0; i < 7; i++)
		{
			angles[i] *= -1;
			// angles[i] -= 7.5;
			//printf("\n\t angles= %d",(int)(angles[i]));
		}
		
		angles[0] -= angle_offset1;
		angles[1] -= angle_offset2;
		angles[2] -= angle_offset2;
	}
}

void test(void)
{
	if(HAL_GPIO_ReadPin(Proximity_Front_GPIO_Port, Proximity_Front_Pin) == GPIO_PIN_RESET){
		printf("\t FRONT");
	}
	if(HAL_GPIO_ReadPin(Proximity_Back_GPIO_Port, Proximity_Back_Pin) == GPIO_PIN_RESET){
		printf("\t BACK");
	}
	if(HAL_GPIO_ReadPin(Proximity_Up_GPIO_Port, Proximity_Up_Pin) == GPIO_PIN_RESET){
		printf("\t UP");
	}
	if(HAL_GPIO_ReadPin(GEREGE_SWITCH_GPIO_Port, GEREGE_SWITCH_Pin) == GPIO_PIN_RESET){
		printf("\t GEREGE");
	}
	leg[0].set_omega(0);
	leg[1].set_omega(0);
	steering.set_angle(0);
	printf("\tleg0_angle = %d\tleg1_angle = %d\tsteering_angle = %d\trobot_angle = %d\t curr_angle = %d", (int)(leg[0].get_actual_angle() * 180 / PI),
	       (int)(leg[1].get_actual_angle() * 180 / PI), (int)(steering.get_angle() * 180 / PI), (int)(robot_angle*180/PI), (int)(curr_angle.getZ()));
	zone_select();
}