#include "robot.h"

static const float robot_speed = 9;
static float leg_speed = robot_speed;	 //38 is the 100%
static const float steering_speed = 0.35; // 1.35 is the 100%
static const float steering_angle_limit = 0.2;
extern leg leg[1];
extern steering steering;
float robot_angle = 0, temp_robot_angle = 0;
bool robo_angle_flag = true;

//float steps[6] = {15, 28, 33, 38, 48, 78};
//float angles[6] = {0, 45, 75, 35, -20, -20};

// float steps[6] = {15, 20, 30, 33, 55, 80};
// float angles[6] = {0, 45, 45, 60, 0, 0};

float steps[6] = {10, 15, 24, 28, 55, 80};
float angles[6] = {0, 45, 45, 60, 0, 0};

bool go(int steps, float angle)
{
	angle *= PI / 180;
	if(steps == 38 || steps == 80) {
		leg_speed = robot_speed/1.5;
	}
	else
	{
		leg_speed = robot_speed;
	}
	
	if((leg[0].get_angle()>((3*PI)/4))&&(leg[0].get_angle()<PI)){
		leg_speed = robot_speed/2;
	}
	else if((leg[0].get_angle()>((6*PI)/4))&&(leg[0].get_angle()<(2*PI-(PI/6)))){
		//leg_speed = -robot_speed/3;
		leg_speed = 1;
	}
	if (leg[0].get_steps() < steps)
	{
		leg[0].set_omega(leg_speed);
	}

	//printf("%d \t %d \t", (int)(100 * angle), (int)(100 * robot_angle));
	printf("%d \t ", (int)(100 * leg[0].get_angle()));
	if (fabs(angle) != fabs(robot_angle))
	{
		if (leg[0].is_raised())
		{
			if ((steering.get_angle() > -steering_angle_limit) && (steering.get_angle() < steering_angle_limit))
			{
				if (angle > (robot_angle + 2 * PI / 180))
				{
					steering.set_omega(steering_speed);
				}
				else if (angle < (robot_angle - 2 * PI / 180))
				{
					steering.set_omega(-steering_speed);
				}
				else
				{
					steering.set_omega(0);
				}
				// steering.set_omega(angle>robot_angle?steering_speed:-steering_speed);
				printf("setting steering\t");
			}
			else
			{
				steering.set_omega(0);
			}
		}
		else
		{
			if (steering.get_angle() > (2 * PI / 180))
			{
				steering.set_omega(-steering_speed);
			}
			else if (steering.get_angle() < (-2 * PI / 180))
			{
				steering.set_omega(steering_speed);
			}
			else
			{
				steering.set_omega(0);
			}
			printf("resetting steering\t");
			//steering.set_omega(steering_speed * ((steering.get_angle() > 0) ? (-1) : (1)));
		}
	}
	else
	{
		steering.set_omega(0);
	}

	// printf("%d \t %d \t %d\n", (int)(steering.get_omega() * 100), (int)(steering.get_angle() * 100), (int)(robot_angle * 100));
	printf("%d\n", (int)(leg[0].get_omega() * 100));

	if (leg[0].is_raised())
	{
		robot_angle = steering.get_angle() + temp_robot_angle;
	}
	else
	{
		temp_robot_angle = robot_angle - steering.get_angle();
	}

	if ((leg[0].get_steps() >= steps) && (fabs(angle-robot_angle) <= 0.05 ))
	{
		leg[0].set_omega(0);
		steering.set_omega(0);
		//printf("Completed\n");
		return true;
	}
	else
	{
		//printf("Not Completed\n");
		return false;
	}
}

bool play()
{
	uint32_t dt = HAL_GetTick();
	while (true)
		{
			if ((HAL_GetTick() - dt) >= (int)(SAMPLE_TIME))
			{
				dt = HAL_GetTick();
				if (go(10, 0) == true)
					break;
			}
		}
	// for (int i = 0; i < 6; i++)
	// {
	// 	while (true)
	// 	{
	// 		if ((HAL_GetTick() - dt) >= (int)(SAMPLE_TIME))
	// 		{
	// 			dt = HAL_GetTick();
	// 			if (go(steps[i], angles[i]) == true)
	// 				break;
	// 		}
	// 	}
	// 	printf ("\tstep=%d\t", i);
	// }
	return true;
}
