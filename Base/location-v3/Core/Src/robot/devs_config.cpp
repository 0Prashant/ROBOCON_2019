/*
 * devs_config.cpp
 * 
 * Created : 11/9/2018
 *  Author : n-is
 *   email : 073bex422.nischal@pcampus.edu.np
 */

#include "robot/devs_config.h"

struct MPU6050 Body_IMU;
struct HMC5883 Body_HMC;
struct A4988 IMU_Stepper;

struct Encoder gXEncoder;
struct Encoder gYEncoder;

void IMU_Init()
{
        Body_IMU.hi2c = &hi2c1;
        Body_IMU.address = 0xD0;
        Body_IMU.a_scale = Accel_Scale::SCALE_2G;
        Body_IMU.g_scale = Gyro_Scale::SCALE_250;

        Body_HMC.hi2c = &hi2c1;
        Body_HMC.address = 0x3C;

        MPU6050_Init(&Body_IMU);
        HMC5883_Init(&Body_HMC);
}

void Stepper_Init()
{
        IMU_Stepper.htim = &htim2;
        IMU_Stepper.steps_per_rev = 200;
        IMU_Stepper.dir_port = GPIOC;
        IMU_Stepper.dir_pin = GPIO_PIN_0;
        IMU_Stepper.step_port = GPIOA;
        IMU_Stepper.step_pin = GPIO_PIN_2;

        A4988_setDirection(&IMU_Stepper, 0);
}

void Encoders_Init()
{
        gXEncoder.chB_port = GPIOE;
        gXEncoder.chB_pin = GPIO_PIN_11;
        gXEncoder.count = 0;
        
        gYEncoder.chB_port = GPIOD;
        gYEncoder.chB_pin = GPIO_PIN_11;
        gYEncoder.count = 0;
}
