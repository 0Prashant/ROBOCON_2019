#include "robot.h"
extern leg leg[1];
extern steering steering;

Vec3<float> read_Orientation(uint32_t dt_millis);
Vec3<float> initial_angle;
Vec3<float> angle;

static const float robot_speed = 20;
static float leg_speed = robot_speed;    //35 is the maximum with safe zone
static const float steering_speed = 0.7; // 1.35 is the 100%
static const float steering_angle_limit = 9 * PI / 180;
static float number_of_samples = 0;
static bool orientation_flag = false;

float robot_angle = 0, temp_robot_angle = 0;
float tolerance = 0.5 * PI / 180;

// float steps[6] = {12, 17, 23, 30, 45, 80};
// float angles[6] = {0, 45, 45, 55, -15, 0};

float steps[6] = {12, 17, 21, 24, 45, 80};
float angles[6] = {0, 45, 45, 50, -20, 0};

bool go(int steps, float angle)
{
	angle *= PI / 180;
	move_leg(steps, angle);
	move_steering(steps, angle);

	calculate_robot_angle();
	//printf("steps = %d \trobot_angle = %d, leg_omega = %d \t steering_angle = %d, st_omega = %d\n",
	//leg[0].steps,  (int)(robot_angle*180/PI), (int)(leg[0].get_omega()*100), (int)(steering.get_angle()*180/PI), (int)(steering.get_omega()*100));
	
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
	leg_speed = (steps == 23 || steps == 40) ? robot_speed / 1.35 : robot_speed;
	leg[0].set_omega(leg_speed);
}
void move_steering(int steps, float angle)
{
	if (leg[0].is_raised() == Leg_condition::RAISED)
	{
		//printf("raised");
		if ((steering.get_angle() > -steering_angle_limit) && (steering.get_angle() < steering_angle_limit))
		{
			if (angle > (robot_angle + tolerance))
				steering.set_omega(steering_speed);

			else if (angle < (robot_angle - tolerance))
				steering.set_omega(-steering_speed);

			else
				steering.set_omega(0);
		}
		else{
			steering.set_omega((steering.get_angle() > 0) ? -steering_speed : steering_speed);  //for protection of overturning
		}
	}
	else if (leg[0].is_raised() == Leg_condition::FALLEN)
	{
		//printf ("fallen");
		if (fabs(angle - robot_angle) <= steering_angle_limit)
		{
			if (steering.get_angle() > (tolerance)){
				steering.set_omega(-steering_speed);}

			else if (steering.get_angle() < (-tolerance)){
				steering.set_omega(steering_speed);}

			else{
				steering.set_omega(0);}
			// printf("center_alligned\t");
		}
		else{
			
			if(fabs(steering.get_angle()) < steering_angle_limit){
				steering.set_omega((angle>0)? (-steering_speed):steering_speed);
			}
			else{
				steering.set_omega(0);
			}			
			//printf("peak_alligned\t");
		}
	}
	else
	{
		// printf("nothing");
		steering.set_omega(0);
	}
}

void calculate_robot_angle()
{
	/*/
	if (leg[0].is_raised_without_deadzone())
	{
		robot_angle = steering.get_angle() + temp_robot_angle;
	}
	else
	{
		temp_robot_angle = robot_angle - steering.get_angle();
	}
	/*/

	//Alternate code to calculate robot_angle using IMU
	// robot_angle = -(initial_angle.getZ() - angle.getZ())*PI/180;

	if(leg[0].is_raised() == Leg_condition::RAISED){
		temp_robot_angle += -(initial_angle.getZ() - angle.getZ())*PI/180;
		number_of_samples++;
		orientation_flag = true;
		printf("\tcalculating");
	}
	else{
		if(orientation_flag){
			robot_angle = temp_robot_angle / number_of_samples;
			temp_robot_angle = 0;
			number_of_samples = 0;
			orientation_flag = false;
			printf("displayed");
		}
		printf("still");
	}

	//*/
}

void calculate_datas()
{
	leg[0].calculate_omega();
	steering.calculate_omega();
	angle = read_Orientation(10);
	printf("angleX = %d \t angleY = %d \t angleZ = %d \t robotangle = %d \n",
	 (int)(angle.getX()), (int)(angle.getY()), (int)(angle.getZ()), (int)(robot_angle*180/PI));
}

bool play()
{
	uint32_t dt = HAL_GetTick();
	initial_angle = read_Orientation(10);
		// while (true)
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
