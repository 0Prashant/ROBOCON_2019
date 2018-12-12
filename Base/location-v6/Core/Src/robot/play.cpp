/*
 * play.cpp
 * 
 * Created : 11/9/2018
 *  Author : n-is
 *   email : 073bex422.nischal@pcampus.edu.np
 */

#include <math.h>
#include "robot/play.h"
#include "devs/sensor.h"
#include "devs/position_sensor.h"

#include "robot/robot.h"


#define MAX_POSSIBLE_OMEGA      (40)

float gFirst_Heading = 0;
bool gIs_First_Heading = true;


Encoder gXEncoder(&gXEnc, SensorName::XEncoder);
Encoder gYEncoder(&gYEnc, SensorName::YEncoder);
Lidar gXLidar(SensorName::XLidar);
PositionSensor gPSensor;

Robot gKhangaiRobot;

void setup(uint32_t dt_millis)
{
        gPSensor.add_Sensor(&gXEncoder);
        gPSensor.add_Sensor(&gYEncoder);
        gPSensor.add_Sensor(&gXLidar);

        gKhangaiRobot.set_PositionSensor(&gPSensor);
}

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
        gKhangaiRobot.run(dt_millis);
}
