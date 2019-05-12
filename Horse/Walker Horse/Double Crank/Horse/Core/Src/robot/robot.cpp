#include "robot.h"
extern leg leg[2];
extern steering steering;

Vec3<float> read_Orientation(uint32_t dt_millis);
Vec3<float> initial_angle;
Vec3<float> angle;

static const float robot_speed = 5.0;
static float leg_speed = robot_speed;    //17 is the maximum with safe zone
static const float steering_speed = 0.6; // 0.875 is the 100%
static const float steering_angle_limit = 9 * PI / 180;
float leg_omega0;
float leg_omega1;
float angle0;
float angle1;
// static float number_of_samples = 0;
// static bool orientation_flag = false;

bool sand_dune_crossed_flag = false;
float robot_angle = 0, temp_robot_angle = 0;
float tolerance = 1.1 * PI / 180;

float steps[7] = {10, 17, 23, 28, 34, 45, 70};
float angles[7] = {0, 50, 50, 55, 0, -10, 0};

bool go(int steps, float angle)
{
	angle *= PI / 180;
	move_leg(steps, angle);
	move_steering(steps, angle);

	leg_omega0 = leg[0].get_omega();
	leg_omega1 = leg[1].get_omega();
	angle0 = leg[0].get_angle();
	angle1 = leg[1].get_angle();

	calculate_robot_angle();
	printf(" count_1 = %d\t count_2 = %d\t leg_1 = %d\t leg_2 = %d\n", (int)(leg[0].get_encoder_count())
	, (int)(leg[1].get_encoder_count()), (int)(leg[0].get_actual_angle()*180/PI), (int)(leg[1].get_actual_angle()*180/PI));
	 printf("%d\t%d", (int)(leg[0].get_angle()*180/PI), (int)(leg[1].get_angle()*180/PI));

	if (((leg[0].get_steps() +leg[0].get_steps())/2 >= steps) && (fabs(angle - robot_angle) <= 0.1))
	{
		leg[0].set_omega(0);
		leg[1].set_omega(0);
		steering.set_omega(0);
		return true;
	}
	else{
		return false;
	}
}

void move_leg(int steps, float angle)
{
	float del_speed = 0;

	//Setting the slow speed in sand dune and tussok
	leg_speed = (steps == 23 || steps == 45) ? robot_speed / 1.5 : robot_speed;

	del_speed = (leg[0].get_actual_angle() - leg[1].get_actual_angle())*leg_speed;
	//del_speed = 0;
	leg[0].set_omega(leg_speed - del_speed);
	leg[1].set_omega(leg_speed + del_speed);
	// printf("\t%d \t%d \t", (int)(leg_speed + del_speed), (int)(leg_speed - del_speed));
}
void move_steering(int steps, float angle)
{
	if (leg[0].is_raised() == Leg_condition::RAISED)
	{
		printf("raised");
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
		printf ("fallen");
		if (steering.get_angle() > (tolerance)){
				steering.set_omega(-steering_speed);}

			else if (steering.get_angle() < (-tolerance)){
				steering.set_omega(steering_speed);}

			else{
				steering.set_omega(0);}
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
	else
	{
		printf("nothing");
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

	//!Alternate code to calculate robot_angle using IMU
	// robot_angle = -(initial_angle.getZ() - angle.getZ())*PI/180;

	robot_angle = -(initial_angle.getZ() - angle.getZ())*PI/180;
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
}

bool play()
{
	uint32_t dt = HAL_GetTick();
	initial_angle = read_Orientation(10);
		while (true)
		{
			if ((HAL_GetTick() - dt) >= (int)(SAMPLE_TIME))
			{
				dt = HAL_GetTick();

				calculate_datas();
				if (go(100,45) == true)
				break;
			}
		}
		// for (int i = 0; i < 6; i++)
		// {
		// 	// if (!sand_dune_crossed_flag && (i==3)){
		// 	// 	i--;
		// 	// 	go(steps[1]+1, 0);
		// 	// 	leg[0].steps = steps[3];
		// 	// 	break;
		// 	// }
		// 	while (true)
		// 	{
		// 		if ((HAL_GetTick() - dt) >= (int)(SAMPLE_TIME))
		// 		{
		// 			dt = HAL_GetTick();
		// 			// printf("\tstep=%d\t", i);
		// 			calculate_datas();
		// 			if (go(steps[i], angles[i]) == true)
		// 				break;
		// 		}
		// 	}
		// 	printf("\tstep=%d\t", i);
		// }
	return true;
}
