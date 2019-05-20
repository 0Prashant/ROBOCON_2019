#include "robot_functions.h"

extern leg leg[2];
extern steering steering;
extern bool USE_IMU_FLAG;
Vec3<float> initial_angle;
Vec3<float> curr_angle;

static const float robot_speed = 5;
static float leg_speed = robot_speed;     //17 is the maximum with safe zone
static const float steering_speed = 0.85; // 0.875 is the 100%
static const float steering_angle_limit = 6 * PI / 180;

extern int steps[7];
extern float angles[7];
float robotx = 0;
uint32_t start_time;

bool STEERING_FLAG = false;

float robot_angle = 0, temp_robot_angle = 0;
float tolerance = 3 * PI / 180;


bool go(int step, float angle)
{
	angle *= PI / 180;
	move_leg(step, angle);
	if((step != steps[2]) && (step != steps[6])){
		move_steering(step, angle);
	}
	else{
		steering.set_angle(0);
	}
	calculate_robot_angle();
	//printf(" \nsteps = %d\t robot_angle = %d\t steering_angle = %d\t", (int)(leg[0].get_steps()), (int)(robot_angle * 180 / PI), (int)(steering.get_angle() * 180 / PI));
	printf(" steps = %d\t robot_angle = %d\t leg_1_angle = %d\t leg_2_angle = %d\n", (int)(leg[0].get_steps()),
	 (int)(robot_angle * 180 / PI), (int)(leg[0].get_actual_angle() * 180 / PI), (int)(leg[1].get_actual_angle() * 180 / PI));

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

	del_speed = 2*(leg[0].get_actual_angle() - leg[1].get_actual_angle()) * leg_speed;
	if(fabs(del_speed) >= leg_speed){
		del_speed /= fabs(del_speed);
		del_speed *= leg_speed;
	}
	leg[0].set_omega(leg_speed - del_speed);
	leg[1].set_omega(leg_speed + del_speed);
	// printf("\t%d \t%d \t", (int)(leg_speed + del_speed), (int)(leg_speed - del_speed));
}
void move_steering(int step, float angle)
{
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

void calculate_robot_angle()
{
	if (USE_IMU_FLAG)
	{
		robot_angle = (initial_angle.getZ() - curr_angle.getZ()) * PI / 180;
		temp_robot_angle = robot_angle;
	}
	else
	{
		if (leg[0].is_raised_without_deadzone())
		{
			robot_angle = steering.get_angle() + temp_robot_angle;
		}
		else
		{
			temp_robot_angle = robot_angle - steering.get_angle();
		}
	}
}

void initialize_position(void)
{
	uint32_t dt = HAL_GetTick();
	while (true)
	{
		if ((HAL_GetTick() - dt) >= (int)(SAMPLE_TIME))
		{
			dt = HAL_GetTick();
			// printf("\n\tInitializing_Leg_Orientation\t");
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
			leg[0].steps = 0;
			leg[1].steps = 0;
			leg[0].reset_angle(90 * PI / 180);
			leg[1].reset_angle(90 * PI / 180);
			leg[0].reset_actual_angle(90 * PI / 180);
			leg[1].reset_actual_angle(90 * PI / 180);
			leg[0].set_omega(0);
			leg[1].set_omega(0);
			steering.set_omega(0);
		}
	}
	start_time = HAL_GetTick();
	while (true)
	{
		if ((HAL_GetTick() - dt) >= (int)(SAMPLE_TIME))
		{
			dt = HAL_GetTick();
			printf("\n\tInitializing_Steering_Orientation\t %d \t %d", (int)HAL_GetTick(), (int)start_time);
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

	printf("\n\tsteps0 = %d, angle0 = %d\tsteps0 = %d, angle0 = %d, \trobot_angle = %d"
	, leg[0].get_steps(), (int)(leg[0].get_angle() * 180 / PI), leg[1].get_steps(), (int)(leg[1].get_angle() * 180 / PI), (int)(robot_angle * 180 /PI));
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
	if(((HAL_GetTick()-start_time) >= 2000)){
		steering.set_omega(0);
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, GPIO_PIN_SET);
		return true;	
	}
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




void calculate_datas()
{
	leg[0].calculate_omega();
	leg[1].calculate_omega();
	steering.calculate_omega();
	curr_angle = read_Orientation(10);
	if(leg[0].is_raised()){
		calculate_robot_angle();
	}
	// printf("\tangleZ = %d\t", (int)curr_angle.getZ());
	HAL_ADC_Start(&hadc1);
	if (HAL_ADC_PollForConversion(&hadc1, 5) == HAL_OK)
	{
		robotx = HAL_ADC_GetValue(&hadc1);
		robotx = 62.17 * pow(((robotx * 3.3) / 4096), -1.0893);
	}
}