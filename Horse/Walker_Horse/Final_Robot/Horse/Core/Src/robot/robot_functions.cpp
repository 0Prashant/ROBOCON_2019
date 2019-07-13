#include "robot_functions.h"

extern leg leg[2];
extern steering steering;
extern bool USE_IMU_FLAG;
Vec3<float> initial_angle;
Vec3<float> curr_angle;

static const float robot_speed = 6;      //17 is the maximum with safe zone
static const float steering_speed = 0.8; // 0.875 is the 100%
static const float steering_angle_limit = 8.5 * PI / 180;

extern int steps[7];
extern float angles[7];
float summation_angle = 0;
uint32_t start_time;

bool STEERING_FLAG = false;

float robot_angle = 0, temp_robot_angle = 0;
float tolerance = 3 * PI / 180;

/*
 * \brief This function makes the robot to reach the provided steps and angle.
 * \brief this function needs to be called every 10 ms unti it reaches the desired position.
 * \params: number of steps and angle in degree the robot has to reach.
 * \returns true if robot reaches to the provided steps and angle.
 * The angles that are caalibrated in degrees needs to be converted to radian.
 * The function move_leg() is called at the beginning which moves the leg upto the desired point.
 * There are two parameters inside this function move_leg(), the steps parameter is used whereas the angle parameter is not used till now and just kept so that it can be used in future for some purpose
 * The condition is if statement is to ensure that the robot is not in sand dune and it has not reached the game_end state. If the robot is in these regions the speed needs to be slowed down. And the steering should not work as well as corrections should not be done.Otherwise the steering should reach the desired angle.
 * Whenever the robot reaches the sand dune the number of steps is declared to 100, so the step != 100 denotes that it is not in the sand dune region.
 * The function calculate_robot_angle() calculates its relative position from the initial state and returns the angle that needs to be given to the robot to rotate.
 * This function operates until the desired steps and desired angles is reached. This is ensured by the if() condition at the end of this function. Whenever the desired steps and angle is reached, the speed is set to 0 for both the legs and steering.
 * After the desired step and angle is reached, the function returns True.
*/
bool go(int step, float angle)
{
	angle *= PI / 180;
	move_leg(step, angle);
	if ((step == 100))
	{
		
		USE_IMU_FLAG = false;
		steering.set_angle(0);
	}
	else
	{
		if(HAL_GPIO_ReadPin(IMU_Detect_GPIO_Port, IMU_Detect_Pin)==GPIO_PIN_RESET){
			USE_IMU_FLAG = true;
		}
		else{
			USE_IMU_FLAG = false;
		}
		move_steering(step, angle);
	}
	// printf(" steps = %d\t robot_angle = %d\t curr_angle = %d\t", (int)(leg[0].get_steps()), (int)(robot_angle * 180 / PI), (int)(curr_angle.getZ()));
	printf(" steps = %d\t robot_angle = %d\t leg_1_angle = %d\t leg_2_angle = %d", (int)(leg[0].get_steps()),
	       (int)(robot_angle * 180 / PI), (int)(leg[0].get_actual_angle() * 180 / PI), (int)(leg[1].get_actual_angle() * 180 / PI));

	// if ((leg[0].get_steps()   >= step) && (fabs(angle - robot_angle) <= 0.1))
	if (leg[0].get_steps() >= step)
	{
		leg[0].set_omega(0);
		leg[1].set_omega(0);
		steering.set_omega(0);
		return true;
	}
	else
	{
		return false;
	}
}
/*
 *The function move_leg() is the basic function to give speed to the motor.
 *\return params: void
 *\input parameter: steps, angle
 *This function takes two input parameters: step and angle. Although the parameter angle is not used in this function, it is only kept if it can be used for future purpose.
 *The variable leg_speed stores the value of speed. When the robot is in sand_dune region, the speed needs to be slowed down. This is ensured by the condition.
 *The variable del_speed stores the value of difference in speed in case where the legs are not in sync. The basic theory here is the two legs must be 180 degrees out of phase.
 *If the legs are not in 180 degrees phase difference, some calculations need to be made in order to keeps these two legs in sync.
 *This is done by evaluating the angles every time. Whenever the returned angle is not in the desired 180 degrees difference, the variable del_speed stores the value of 
 *difference is the two leg angles multiplied by leg_speed which is again multiplied by the factor 2 which only denotes the correction factor.
 *Following the above reference, the leg_speed is calculated in closed loop basis.
*/
void move_leg(int step, float angle)
{
	float del_speed = 0;
	float leg_speed = robot_speed;

	//Setting the slow speed in sand dune and tussok
	leg_speed = ((step == 100) || (step == 30)) ? robot_speed / 1.3	 : robot_speed;

	leg_speed = motion_profile(leg[0].get_angle() * 180 / PI, 1, leg_speed);
	del_speed = 3 * (leg[0].get_actual_angle() - leg[1].get_actual_angle()) * leg_speed;
	if (fabs(del_speed) >= leg_speed)
	{
		del_speed /= fabs(del_speed);
		del_speed *= leg_speed;
	}
	leg[0].set_omega(leg_speed - del_speed);
	leg[1].set_omega(leg_speed + del_speed);
}

/*
 *The function move_steering() is concerned with moving the steering to certain angle with certain speed.
 *Return patameter: void
 *Input parameters: step, angle
 *During initialization, the summation_angle is set to 0. After the robot starts to move, there is drift in the robot angle due to various errors.
 * This error of course needs to be minimized. This minimization is done by  summing all the drifts in the robot that the IMU has detected. This summation angle is added or subtracted from the angle variable
 * that it gives to the robot. Whenever the steering sets the angle for robot, this error is also added to it.
 * The robot changes the angle whenever the outside leg is raised. For the robot and leg to be in the straight position, for this the steering needs to be corrected whenever
 * the outside leg is in fallen state.
*/
void move_steering(int step, float angle)
{
	if (step == steps[0])
	{
		summation_angle += 1.3*robot_angle;
		angle = -summation_angle;
		angle /= 300;
	}
	if (leg[0].is_raised() == Leg_condition::RAISED)
	{
		// printf("  raised  ");
		set_angle(angle);
	}
	else if (leg[0].is_raised() == Leg_condition::FALLEN)
	{
		// printf("  fallen  ");
		correct_steering_angle(angle);
	}
	else
	{
		// printf("  nothing  ");
		steering.set_omega(0);
	}
}
/*
 * The function set_angle() is concerned with setting the angle for steering.
 * Return params : void
 * Input patams : angle
 * The variable del_angle stores the differnece in given input angle and the current angle returned from the sensors.
 * If this del_angle returns positive value, it depicts that the robot has already reached the angle that is greater than the desired angle.
 * TO correct this to the desired angle the angle needs to be set.
 * If this del_angle returns a negative value, it depicts that the robot has not reached the desired state and hence the angle needs to be added in the negative direction.
*/
void set_angle(float angle)
{
	float del_angle = (angle - robot_angle) * 180 / PI;
	// printf("\tdel_angle = %d\t", (int)(del_angle));
	if (del_angle >= 0)
	{
		steering.set_angle((del_angle > (steering_angle_limit * 180 / PI)) ? (steering_angle_limit * 180 / PI) : del_angle);
	}
	else
	{
		steering.set_angle((del_angle < -(steering_angle_limit * 180 / PI)) ? -(steering_angle_limit * 180 / PI) : del_angle);
	}
	//*/
}
/*
 * This function correct_steering_angle() is called whenever the outside leg is fallen and the steerring needs to be oriented to the robot direction.
 * return param: void
 * input params: angle
 * same as set_angle function. the difference is this function is called whenever the leg is not raised whereas the set_angle() function is called whenever 
 * the leg is raised.
*/
void correct_steering_angle(float angle)
{
	float del_angle = (angle - robot_angle) * 180 / PI;
	if (fabs(del_angle) <= (steering_angle_limit * 180 / PI))
	{
		steering.set_angle(0);
		// printf("center_alligned\t");
	}
	else
	{
		steering.set_angle(del_angle > 0 ? -(steering_angle_limit * 180 / PI) : (steering_angle_limit * 180 / PI));
		// printf("peak_alligned\t");
	}
	//*/
}
/**
 * This function calculate_robot_angle() calculates the relative robot angle from the initial position. The angle detected at the beginning is initialized
 * to zero and all other angles are taken with reference to this initial angle.
 * return param : void
 * /params input parameters : void
 * The variable robot_angle stores the realtive value of robot angle from the original position
 * The temp_robot_angle is just the temporary variable.
*/
void calculate_robot_angle()
{
	if (USE_IMU_FLAG)
	{
		if (!leg[0].is_raised_without_deadzone()){
		robot_angle = (initial_angle.getZ() - curr_angle.getZ()) * PI / 180;
		temp_robot_angle = robot_angle;
		}
		// printf("\tusing IMU\t");
	}
	else
	{
		if (leg[0].is_raised_without_deadzone())
		{
			robot_angle = steering.get_angle() + temp_robot_angle;
			printf("\t%d\t%d", (int)(steering.get_angle()*180/PI), (int)(temp_robot_angle*180/PI));
		}
		else
		{
			temp_robot_angle = robot_angle - steering.get_angle();
			printf("\t%d\t%d", (int)(steering.get_angle()*180/PI), (int)(temp_robot_angle*180/PI));
		}
		// printf("\tusing ENCODER\t");
	}
}

/**
 * \brief This function initializes the robot in erected position at the start of the game.
 * \bug remember to face towards anticlockwise direction from the 0 position at the start of the game.
 * \params void
 * \returns void
 */

void initialize_position(void)
{
	uint32_t dt = HAL_GetTick();
	while (true)
	{
		if ((HAL_GetTick() - dt) >= (int)(SAMPLE_TIME))
		{
			dt = HAL_GetTick();
			printf("\nInitializing_Leg_Orientation\t");
			if (initialize_leg_position() == true)
				break;
		}
	}
	for (int32_t i = 0; i < 3000000; i++)
	{
		if ((HAL_GetTick() - dt) >= (int)(SAMPLE_TIME))
		{
			dt = HAL_GetTick();
			printf("\n\tWaiting_The_Leg \t");
			leg[0].steps = 0;
			leg[1].steps = 0;
			leg[0].reset_angle(90 * PI / 180);
			leg[1].reset_angle(90 * PI / 180);
			leg[0].reset_actual_angle(90 * PI / 180);
			leg[1].reset_actual_angle(90 * PI / 180);
			leg[0].set_omega(0);
			leg[1].set_omega(0);
			steering.set_omega(0);
			STEERING_FLAG = false;
		}
	}
	start_time = HAL_GetTick();
	while (true)
	{
		if ((HAL_GetTick() - dt) >= (int)(SAMPLE_TIME))
		{
			dt = HAL_GetTick();
			printf("\tInitializing_Steering_Orientation\t %d \t %d", (int)HAL_GetTick(), (int)start_time);
			leg[0].set_omega(0);
			leg[1].set_omega(0);
			if (initialize_steering_position() == true)
				break;
		}
	}
}

/**
 * \brief This function initializes the leg in erected position at the start of the game.
 * \params void
 * \returns true if leg is erected.
 */
bool initialize_leg_position(void)
{
	float initial_tolerance = 4;
	float leg_initial_position = 90;
	static bool leg0_flag = false;
	static bool leg1_flag = false;

	// printf("\n\tsteps0 = %d, angle0 = %d\tsteps0 = %d, angle0 = %d, \trobot_angle = %d"
	// , leg[0].get_steps(), (int)(leg[0].get_angle() * 180 / PI), leg[1].get_steps(), (int)(leg[1].get_angle() * 180 / PI), (int)(robot_angle * 180 /PI));
	if ((leg[0].get_angle() > ((leg_initial_position - initial_tolerance) * PI / 180)) &&
	    (leg[0].get_angle() < ((leg_initial_position + initial_tolerance) * PI / 180) && (leg[0].get_steps() != 0)))
	{
		leg[0].set_omega(0);
		leg0_flag = true;
	}
	else
	{
		leg[0].set_omega(3);
		leg0_flag = false;
	}

	if ((leg[1].get_angle() > ((leg_initial_position - initial_tolerance) * PI / 180)) &&
	    (leg[1].get_angle() < ((leg_initial_position + initial_tolerance) * PI / 180)) && (leg[1].get_steps() != 0))
	{
		leg[1].set_omega(0);
		leg1_flag = true;
	}
	else
	{
		leg[1].set_omega(3);
		leg1_flag = false;
	}

	if (leg0_flag && leg1_flag)
	{

		return true;
	}
	else
	{
		return false;
	}
}

/**
 * \brief Called by initialize_position();
 * \brief This function initializes the steering in center position at the start of the game.
 * \bug remember to towards anticlockwise direction from the 0 position at the start of the game.
 * \params void
 * \returns true if reached to 0 angle.
 */
bool initialize_steering_position(void)
{
	printf("\nangle = %d\t", (int)(steering.get_angle() * 180 / PI));
	// printf("\tsteps0 = %d, angle0 = %d\tsteps0 = %d, angle0 = %d", leg[0].get_steps(),(int)(leg[0].get_angle()*180/PI)
	// , leg[1].get_steps(),(int)(leg[1].get_angle()*180/PI) );
	if (((HAL_GetTick() - start_time) >= 500))
	{
		steering.set_omega(0);
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, GPIO_PIN_SET);
		return true;
	}
	if (STEERING_FLAG)
	{
		steering.set_omega(0);
		return true;
	}
	else
	{
		steering.set_omega(0.5);
		return false;
	}
}

bool getup_n_run()
{
	float initial_tolerance = 4;
	float leg_initial_position = 90;

	// printf("\tInitializing_Leg_Orientation_GETUP\t");
	leg[0].set_omega(0);
	// printf(" \t%d \t%d", (int)(leg[1].get_angle()), (int)(leg_initial_position));
	if ((leg[1].get_angle() > ((leg_initial_position - initial_tolerance) * PI / 180)) &&
	    (leg[1].get_angle() < ((leg_initial_position + initial_tolerance) * PI / 180)))
	{
		leg[1].set_omega(0);
		leg[0].reset_actual_angle(90 * PI / 180);
		leg[1].reset_actual_angle(90 * PI / 180);
		leg[0].reset_angle(90 * PI / 180);
		leg[1].reset_angle(90 * PI / 180);
		// printf("\tIF\t");
		return true;
	}
	else
	{
		leg[1].set_omega(3);
		// printf("\tELSE\t");
		return false;
	}

}

/**
 * \brief Called by initialize_position();
 * \brief This function calculates all the datas including the motor omegas, orientation and angles.
 * \remarks remember to call this function only once in every 10 ms.
 * \params void
 * \returns void
 */
void calculate_datas()
{
	leg[0].calculate_omega();
	leg[1].calculate_omega();
	steering.calculate_omega();
	curr_angle = read_Orientation(10);
	calculate_robot_angle();
	// printf("\tangleZ = %d\t", (int)curr_angle.getZ());
	// printf("\trobot_angle = %d\t", (int)(robot_angle*180/PI));
}

float motion_profile(float angle_in_degrees, float min_speed, float max_speed)
{
	float damping_angle = 30;
	float speed = 0;
	min_speed = 1.5;
	if (angle_in_degrees > 180)
	{
		angle_in_degrees -= 180;
		// max_speed = 10;
	}
	if ((angle_in_degrees >= 0) && (angle_in_degrees <= damping_angle))
	{
		speed = ((max_speed - min_speed) / (damping_angle - 0)) * (angle_in_degrees - 0) + min_speed;
		// printf("condition1");
	}

	else if ((angle_in_degrees >= (180 - damping_angle)) && (angle_in_degrees <= 180))
	{
		speed = ((min_speed - max_speed) / (180 - (180 - damping_angle))) * (angle_in_degrees - (180 - damping_angle)) + max_speed;
		// printf("conditionelseif");
	}

	else
	{
		speed = max_speed;
		// printf("else");
	}
	// printf("\tSpeed = %d", (int)(speed*100));

	return speed;
}