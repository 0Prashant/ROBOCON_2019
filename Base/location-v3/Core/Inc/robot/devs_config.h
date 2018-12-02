
/*
 * sensor_config.h
 * 
 * Created : 11/9/2018
 *  Author : n-is
 *   email : 073bex422.nischal@pcampus.edu.np
 */

#ifndef _SENSOR_CONFIG_H_
#define _SENSOR_CONFIG_H_

#include "periphs/i2c.h"
#include "periphs/tim.h"
#include "devs/mpu6050.h"
#include "devs/hmc5883.h"
#include "devs/a4988.h"
#include "devs/encoder.h"
#include "devs/lidar.h"
#include "thinker.h"
#include "wheeler.h"

// These are the main IMU and the Compass Of the Robot
extern struct MPU6050 Body_IMU;
extern struct HMC5883 Body_HMC;
extern struct A4988 IMU_Stepper;

extern struct Encoder gXEncoder;
extern struct Encoder gYEncoder;


void IMU_Init();
void Stepper_Init();
void Encoders_Init();

#endif // !_SENSOR_CONFIG_H_
