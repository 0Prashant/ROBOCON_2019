/*
 * imu_utils.h
 * 
 * Created : 11/9/2018
 *  Author : n-is
 *   email : 073bex422.nischal@pcampus.edu.np
 */

#ifndef _IMU_UTILS_H_
#define _IMU_UTILS_H_

#include "devs/mpu6050.h"

Vec3<float> calc_OmegaMean(struct MPU6050 *mpu, uint32_t n);
Vec3<float> calc_VarAccel(struct MPU6050 *mpu, uint32_t n);
Vec3<float> calc_VarGyro(struct MPU6050 *mpu, uint32_t n);

Vec3<float> calc_StdDevAccel(struct MPU6050 *mpu, uint32_t n);
Vec3<float> calc_StdDevGyro(struct MPU6050 *mpu, uint32_t n);

#endif // !_IMU_UTILS_H_