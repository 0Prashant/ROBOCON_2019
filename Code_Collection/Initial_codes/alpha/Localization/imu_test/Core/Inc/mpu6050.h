/*
 * mpu6050.h
 * 
 * Created : 9/27/2018
 *  Author : n-is
 *   email : 073bex422.nischal@pcampus.edu.np
 */

#ifndef _MPU6050_H_
#define _MPU6050_H_

#include "i2c.h"
#include "mpu6050_reg.h"

struct Axis
{
        int16_t x,y,z;
};

enum Accel_Scale
{
        SCALE_2G = 0,
        SCALE_4G,
        SCALE_8G,
        SCALE_16G
};

enum Gyro_Scale
{
        SCALE_250 = 0,
        SCALE_500,
        SCALE_1000,
        SCALE_2000
};

struct MPU6050
{
        I2C_HandleTypeDef *hi2c;
        struct Axis gyro_axis, accel_axis;
        uint8_t address;
        float tmpr;
        enum Accel_Scale a_scale;
        enum Gyro_Scale g_scale;
};


void MPU6050_Init(struct MPU6050 *mpu);
void MPU6050_Read_Accel(struct MPU6050 *mpu);
void MPU6050_Read_Gyro(struct MPU6050 *mpu);
void MPU6050_Read_Temp(struct MPU6050 *mpu);
void MPU6050_Read_Axes(struct MPU6050 *mpu);


#endif // _MPU6050_H_
