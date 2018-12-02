/*
 * play.cpp
 * 
 * Created : 11/9/2018
 *  Author : n-is
 *   email : 073bex422.nischal@pcampus.edu.np
 */

#include <math.h>
#include "robot/play.h"

#define ROBOT_VELOCITY          (1000)
#define FIRST_ANGLE             (0 + 3.14159)

#define MAX_POSSIBLE_OMEGA      (40)


void read_State(Vec3<float> &euler_angles, Vec3<float> &position, uint32_t dt_millis);

float gFirst_Heading = 0;
bool gIs_First_Heading = true;


/*
 * Tasks to be performed by this part of robot
 TODO 1:  Read Orientation of Robot
 TODO 2:  Smooth the Orientation Reading and Filter It Using CF or KF
 TODO  :    -> Inclination : Gyroscope and Accelerometer
 TODO  :    -> Heading : Gyroscope and Magnetometer
 TODO 3:  Correct LIDAR Servo/Dynamixel if Used
 TODO 4:  Read Distaces from Fences Using LIDAR
 TODO 5:  Smooth the Distance Reading and Filter It using KF
 TODO  :    -> should probably use enncoder for second data
 TODO 6:  Calculate Four Corners of Robot using Distance and Orientation Data
 TODO 7:  Transfer the Calulated values
 TODO 8:  Repeat from step 1.
 */

void play(uint32_t dt_millis)
{
        Vec3<float> euler_angles;
        Vec3<float> position;

        read_State(euler_angles, position, dt_millis);

        if (gIs_First_Heading) {
                gIs_First_Heading = false;
                gFirst_Heading = euler_angles.getZ();
        }
        else {
                if (position.getX() < 120) {
                        calculate_robot_velocity(ROBOT_VELOCITY, FIRST_ANGLE, (gFirst_Heading - euler_angles.getZ()) / 57.3, dt_millis);
                }
                else {
                        calculate_robot_velocity(0, 0, 0, dt_millis);
                }
                int16_t wheel_omegas[4];
                for (uint8_t i = 0; i < 4; ++i) {
                        float tmp = gWheel_Omegas[i] * (32768.0 / (float)MAX_POSSIBLE_OMEGA);
                        wheel_omegas[i] = (int16_t)(tmp);
                }
                send_WheelerPack(wheel_omegas);
        }
}
