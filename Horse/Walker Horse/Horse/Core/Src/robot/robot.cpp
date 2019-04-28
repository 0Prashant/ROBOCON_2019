#include "robot.h"
extern leg leg[1];
extern steering steering;

static const float robot_speed = 8;
static float leg_speed = robot_speed;    //38 is the 100%
static const float steering_speed = 0.6; // 1.35 is the 100%
static const float steering_angle_limit = 8*PI/180;

float robot_angle = 0, temp_robot_angle = 0;
bool robo_angle_flag = true;
float tolerance = 1 * PI / 180;

//float steps[6] = {15, 28, 33, 38, 48, 78};
//float angles[6] = {0, 45, 75, 35, -20, -20};

// float steps[6] = {15, 20, 30, 33, 55, 80};
// float angles[6] = {0, 45, 45, 60, 0, 0};

float steps[6] = {10, 15, 23, 26, 40, 80};
float angles[6] = {0, 45, 45, 50, 0, 0};

bool go(int steps, float angle)
{
	angle *= PI / 180;
	move_leg(steps, angle);
	if (fabs(angle - robot_angle) >= tolerance)
		move_steering(steps, angle);
	else
		steering.set_omega(0);

	calculate_robot_angle();
	printf("steps = %d \trobot angle = %d \t steering angle = %d\n",steps,  (int)(robot_angle*180/PI), (int)(steering.get_angle()*180/PI));	

	if ((leg[0].get_steps() >= steps) && (fabs(angle - robot_angle) <= 0.04))
	{
		leg[0].set_omega(0);
		steering.set_omega(0);
		return true;
	}
	else
	{
		leg[0].set_omega(leg_speed);
		return false;
	}
}

void move_leg(int steps, float angle)
{
	//Setting the slow speed in sand dune and tussok
	leg_speed = (steps == 23 || steps == 40) ? robot_speed / 1.5 : robot_speed;
}
void move_steering(int steps, float angle)
{
	if (leg[0].is_raised() == Leg_condition::RAISED)
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
			steering.set_omega((steering.get_angle() > 0) ? -steering_speed : steering_speed);
	}
	else if (leg[0].is_raised() == Leg_condition::FALLEN)
	{
		if (steering.get_angle() > (tolerance))
			steering.set_omega(-steering_speed);
		
		else if (steering.get_angle() < (-tolerance))
			steering.set_omega(steering_speed);

		else
			steering.set_omega(0);
	}
	else
	{
		steering.set_omega(0);
	}
}

void calculate_robot_angle()
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

void calculate_datas()
{
	leg[0].calculate_omega();
	steering.calculate_omega();
}

bool play()
{
	uint32_t dt = HAL_GetTick();
	while (true)
		// {
		// 	if ((HAL_GetTick() - dt) >= (int)(SAMPLE_TIME))
		// 	{
		// 		dt = HAL_GetTick();
				
		// 		calculate_datas();
		// 		if (go(8,45) == true)
		// 			break;
		// 	}
		// }
		for (int i = 0; i < 6; i++)
		{
			while (true)
			{
				if ((HAL_GetTick() - dt) >= (int)(SAMPLE_TIME))
				{
					dt = HAL_GetTick();
					calculate_datas();
					if (go(steps[i], angles[i]) == true)
						break;
				}
			}
			printf("\tstep=%d\t", i);
		}
	return true;
}
