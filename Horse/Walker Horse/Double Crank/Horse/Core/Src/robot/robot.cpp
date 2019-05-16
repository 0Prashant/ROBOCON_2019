#include "robot.h"
extern leg leg[2];
extern steering steering;

Vec3<float> read_Orientation(uint32_t dt_millis);
Vec3<float> initial_angle;
Vec3<float> angle;
float robotx = 0;

bool ROBOT_START_FLAG = false;
bool STEERING_FLAG = false;

static const float robot_speed = 5.0;
static float leg_speed = robot_speed;    //17 is the maximum with safe zone
static const float steering_speed = 0.8; // 0.875 is the 100%
static const float steering_angle_limit = 9 * PI / 180;

// static float number_of_samples = 0;
// static bool orientation_flag = false;

bool sand_dune_crossed_flag = false;
float robot_angle = 0, temp_robot_angle = 0;
float tolerance = 1.1 * PI / 180;
float xdistance = 40;

float steps[7] = {7, 12, 17, 19, 25, 30, 50};
float angles[7] = {0, 50, 50, 60, 0, 0, 0};

void initialize_position(void)
{
	uint32_t dt = HAL_GetTick();
	while (true)
	{
		if ((HAL_GetTick() - dt) >= (int)(SAMPLE_TIME))
		{
			dt = HAL_GetTick();
			printf("\n\tInitializing_Leg_Orientation\t");
			calculate_datas();
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
			calculate_datas();
			leg[0].set_omega(0);
			leg[1].set_omega(0);
			steering.set_omega(0);
		}
	}
	while (true)
	{
		if ((HAL_GetTick() - dt) >= (int)(SAMPLE_TIME))
		{
			dt = HAL_GetTick();
			printf("\n\tInitializing_Steering_Orientation\t");
			leg[0].set_omega(0);
			leg[1].set_omega(0);
			calculate_datas();
			if (initialize_steering_position() == true)
				break;
		}
	}
}

bool initialize_leg_position(void)
{
	float initial_tolerance = 4;
	float leg_initial_position = 90;
	static bool leg0_flag = false;
	static bool leg1_flag = false;

	//  printf("\n\tsteps0 = %d, angle0 = %d\tsteps0 = %d, angle0 = %d", leg[0].get_steps(),(int)(leg[0].get_angle()*180/PI)
	//  , leg[1].get_steps(),(int)(leg[1].get_angle()*180/PI) );
	if ((leg[0].get_angle() > ((leg_initial_position - initial_tolerance) * PI / 180)) &&
	    (leg[0].get_angle() < ((leg_initial_position + initial_tolerance) * PI / 180) && (leg[0].get_steps() != 0)))
	{
		leg[0].set_omega(0);
		leg0_flag = true;
	}
	else
	{
		leg[0].set_omega(5);
		leg0_flag = false;
	}

	if ((leg[1].get_angle() > ((leg_initial_position - initial_tolerance) * PI / 180)) &&
	    (leg[1].get_angle() < ((leg_initial_position + initial_tolerance) * PI / 180)) && (leg[0].get_steps() != 0))
	{
		leg[1].set_omega(0);
		leg1_flag = true;
	}
	else
	{
		leg[1].set_omega(5);
		leg1_flag = false;
	}

	if (leg0_flag && leg1_flag)
	{
		leg[0].steps = 0;
		leg[1].steps = 0;
		leg[0].reset_angle(90 * PI / 180);
		leg[1].reset_angle(90 * PI / 180);
		leg[0].reset_actual_angle(90 * PI / 180);
		leg[1].reset_actual_angle(90 * PI / 180);
		return true;
	}
	else
	{
		return false;
	}
}

bool initialize_steering_position(void)
{
	// printf("\nangle = %d\t",(int)(steering.get_angle()*180/PI) );
	// printf("\tsteps0 = %d, angle0 = %d\tsteps0 = %d, angle0 = %d", leg[0].get_steps(),(int)(leg[0].get_angle()*180/PI)
	// , leg[1].get_steps(),(int)(leg[1].get_angle()*180/PI) );
	if (STEERING_FLAG)
	{
		steering.set_omega(0);
		steering.reset_angle(0);
		return true;
	}
	else
	{
		steering.set_omega(0.5);
		return false;
	}
}

bool go(int step, float angle)
{
	angle *= PI / 180;
	move_leg(step, angle);
	move_steering(step, angle);

	calculate_robot_angle();
	// printf(" steps = %d\t robot_angle = %d\t steering_angle = %d\t leg_1_angle = %d\t leg_2_angle = %d\n", (int)(leg[0].get_steps()), (int)(robot_angle * 180 / PI), (int)(steering.get_angle() * 180 / PI), (int)(leg[0].get_angle() * 180 / PI), (int)(leg[1].get_angle() * 180 / PI));

	if (((leg[0].get_steps() + leg[1].get_steps()) / 2 >= step) && (fabs(angle - robot_angle) <= 0.1))
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

void move_leg(int step, float angle)
{
	float del_speed = 0;

	//Setting the slow speed in sand dune and tussok
	leg_speed = (step == steps[2] || step == steps[4]) ? robot_speed / 1.35 : robot_speed;

	del_speed = (leg[0].get_actual_angle() - leg[1].get_actual_angle()) * leg_speed;
	//del_speed = 0;
	leg[0].set_omega(leg_speed - del_speed);
	leg[1].set_omega(leg_speed + del_speed);
	// printf("\t%d \t%d \t", (int)(leg_speed + del_speed), (int)(leg_speed - del_speed));
}
void move_steering(int step, float angle)
{
	if (leg[0].is_raised() == Leg_condition::RAISED)
	{
		printf("raised");
		// float dd = cos(robot_angle)*robotx - xdistance;
		set_angle(angle);
	}
	else if (leg[0].is_raised() == Leg_condition::FALLEN)
	{
		printf("fallen");
		correct_steering_angle();
		
	}
	else
	{
		printf("nothing");
		steering.set_omega(0);
	}
}

void set_angle(float angle)
{
	if ((steering.get_angle() > -steering_angle_limit) && (steering.get_angle() < steering_angle_limit))
		{
			if (angle > (robot_angle + tolerance))
				steering.set_omega(steering_speed);

			else if (angle < (robot_angle - tolerance))
				steering.set_omega(-steering_speed);

			else
				steering.set_omega(0);
		}
		else
		{
			steering.set_omega((steering.get_angle() > 0) ? -steering_speed : steering_speed); //for protection of overturning
		}
}

void correct_steering_angle(void)
{
	if (steering.get_angle() > (tolerance))
	{
		steering.set_omega(-steering_speed);
	}

	else if (steering.get_angle() < (-tolerance))
	{
		steering.set_omega(steering_speed);
	}

	else
	{
		steering.set_omega(0);
	}
	// if (fabs(angle - robot_angle) <= steering_angle_limit)
	// {
	// 	if (steering.get_angle() > (tolerance)){
	// 		steering.set_omega(-steering_speed);}

	// 	else if (steering.get_angle() < (-tolerance)){
	// 		steering.set_omega(steering_speed);}

	// 	else{
	// 		steering.set_omega(0);}
	// 	printf("center_alligned\t");
	// }
	// else{

	// 	if(fabs(steering.get_angle()) < steering_angle_limit){
	// 		steering.set_omega((angle>0)? (-steering_speed):steering_speed);
	// 	}
	// 	else{
	// 		steering.set_omega(0);
	// 	}
	// 	printf("peak_alligned\t");
	// }
}

void calculate_robot_angle()
{
	//*/
	if (leg[0].is_raised_without_deadzone())
	{
		robot_angle = steering.get_angle() + temp_robot_angle;
	}
	else
	{
		temp_robot_angle = robot_angle - steering.get_angle();
	}
	/*/

	//!Alternate code to calculate robot_angle using IMU
	// robot_angle = -(initial_angle.getZ() - angle.getZ())*PI/180;

	robot_angle = -(initial_angle.getZ() - angle.getZ()) * PI / 180;
	// if(leg[0].is_raised() == Leg_condition::FALLEN){
	// 	temp_robot_angle += -(initial_angle.getZ() - angle.getZ())*PI/180;
	// 	number_of_samples++;
	// 	orientation_flag = true;
	// 	// printf("\tcalculating");
	// }
	// else{
	// 	if(orientation_flag){
	// 		robot_angle = temp_robot_angle / number_of_samples;
	// 		temp_robot_angle = 0;
	// 		number_of_samples = 0;
	// 		orientation_flag = false;
	// 		// printf("displayed");
	// 	}
	// 	// printf("still");
	// }

	//*/
}

void calculate_datas()
{
	leg[0].calculate_omega();
	leg[1].calculate_omega();
	steering.calculate_omega();
	angle = read_Orientation(10);
	HAL_ADC_Start(&hadc1);
	if (HAL_ADC_PollForConversion(&hadc1, 5) == HAL_OK)
	{
		robotx = HAL_ADC_GetValue(&hadc1);
		robotx = 62.17 * pow(((robotx * 3.3) / 4096), -1.0893);
		// printf("\n\tDistance = %d", (int)robotx);
	}
}

bool play()
{
	uint32_t dt = HAL_GetTick();
	initial_angle = read_Orientation(10);


	// initialize_position();

	while (true)
	{
		if ((HAL_GetTick() - dt) >= (int)(SAMPLE_TIME))
		{
			dt = HAL_GetTick();
			printf("\n\tWaiting_to_start\t");
			calculate_datas();
			leg[0].set_omega(0);
			leg[1].set_omega(0);
			steering.set_omega(0);
			if (ROBOT_START_FLAG)
				break;
		}
	}
	for (int i = 0; i < 6; i++)
	{
		// if (!sand_dune_crossed_flag && (i==3)){
		// 	i--;
		// 	go(steps[1]+1, 0);
		// 	leg[0].steps = steps[3];
		// 	break;
		// }
		while (true)
		{
			if ((HAL_GetTick() - dt) >= (int)(SAMPLE_TIME))
			{
				dt = HAL_GetTick();
				// printf("\tstep=%d\t", i);
				calculate_datas();
				if (go(steps[i], -angles[i]) == true)
					break;
			}
		}
		printf("\tstep=%d\t", i);
	}
	return true;
}
