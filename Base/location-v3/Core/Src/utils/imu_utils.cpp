/*
 * imu_utils.cpp
 * 
 * Created : 11/11/2018
 *  Author : n-is
 *   email : 073bex422.nischal@pcampus.edu.np
 */

#include <math.h>
#include "utils/imu_utils.h"
#include "utils/smoothie.h"

Vec3<float> calc_OmegaMean(struct MPU6050 *mpu, uint32_t n)
{
        Vec3<float> mean;
        float item_num;
        
        Exp_Smooth alpha35(0.35);
        Smoothie s(&alpha35);

        for (uint32_t i = 0; i < n; ++i) {
                MPU6050_Read_NormGyro(mpu);
                item_num = 1.0 / (float)(i+1);
                mean += (s.smooth(mpu->norm_g_axis) - mean).mult_EW(item_num);
                HAL_Delay(1);
        }
        return mean;
}

template <uint32_t N>
float avg(float (&arr)[N], uint32_t n)
{
        float mean = 0;

        for (uint32_t i = 0; i < n; ++i) {
                mean += (arr[i] - mean) / (float)(i+1);
        }

        return mean;
}

template <uint32_t N>
float var(float (&arr)[N], uint32_t n)
{
        float mean = avg(arr, n);
        float diff;
        float sum_diff_sq = 0;
        for (uint32_t i = 0; i < n; ++i) {
                diff = arr[i] - mean;
                sum_diff_sq += diff * diff;
        }
        return sum_diff_sq / (float)n;
}

template <uint32_t N>
Vec3<float> avg(Vec3<float> (&arr)[N], uint32_t n)
{
        Vec3<float> mean;

        for (uint32_t i = 0; i < n; ++i) {
                mean += (arr[i] - mean).mult_EW(1.0 / (float)(i+1));
        }

        return mean;
}

template <uint32_t N>
Vec3<float> var(Vec3<float> (&arr)[N], uint32_t n)
{
        Vec3<float> mean = avg(arr, n);
        Vec3<float> diff;
        Vec3<float> sum_diff_sq(0,0,0);
        for (uint32_t i = 0; i < n; ++i) {
                diff = arr[i] - mean;
                sum_diff_sq += diff.mult_EW(diff);
        }
        return sum_diff_sq.mult_EW(1.0 / (float)n);
}

template <uint32_t N>
Vec3<float> stdDev(Vec3<float> (&arr)[N], uint32_t n)
{
        Vec3<float> std_dev;
        Vec3<float> variance = var(arr, n);
        std_dev.setX(sqrtf(variance.getX()));
        std_dev.setY(sqrtf(variance.getY()));
        std_dev.setZ(sqrtf(variance.getZ()));

        return std_dev;
}

template <uint32_t N>
Vec3<float> max(Vec3<float> (&arr)[N], uint32_t n)
{
        Vec3<float> mx = arr[0];

        for (uint32_t i = 0; i < n; ++i) {
                if (mx.getX() < arr[i].getX()) {
                        mx.setX(arr[i].getX());
                }
                if (mx.getY() < arr[i].getY()) {
                        mx.setY(arr[i].getY());
                }
                if (mx.getZ() < arr[i].getZ()) {
                        mx.setZ(arr[i].getZ());
                }
        }
        
        return mx;
}

template <uint32_t N>
Vec3<float> min(Vec3<float> (&arr)[N], uint32_t n)
{
        Vec3<float> mn = arr[0];

        for (uint32_t i = 0; i < n; ++i) {
                if (mn.getX() > arr[i].getX()) {
                        mn.setX(arr[i].getX());
                }
                if (mn.getY() > arr[i].getY()) {
                        mn.setY(arr[i].getY());
                }
                if (mn.getZ() > arr[i].getZ()) {
                        mn.setZ(arr[i].getZ());
                }
        }
        
        return mn;
}

Vec3<float> calc_VarAccel(struct MPU6050 *mpu, uint32_t n)
{
        Exp_Smooth alpha35(0.35);
        Smoothie s(&alpha35);

        const uint32_t data_num = 100;
        Vec3<float> accel_initial_data[data_num];
        for (uint32_t i = 0; i < data_num; ++i) {
                MPU6050_Read_NormAccel(mpu);
                accel_initial_data[i] = s.smooth(mpu->norm_a_axis);
        }
        return var(accel_initial_data, data_num);
}

Vec3<float> calc_VarGyro(struct MPU6050 *mpu, uint32_t n)
{
        Exp_Smooth alpha35(0.35);
        Smoothie s(&alpha35);

        const uint32_t data_num = 100;
        Vec3<float> gyro_initial_data[data_num];
        for (uint32_t i = 0; i < data_num; ++i) {
                MPU6050_Read_NormGyro(mpu);
                gyro_initial_data[i] = s.smooth(mpu->norm_g_axis);
        }
        return var(gyro_initial_data, data_num);
}

Vec3<float> calc_StdDevAccel(struct MPU6050 *mpu, uint32_t n)
{
        Exp_Smooth alpha35(0.35);
        Smoothie s(&alpha35);

        const uint32_t data_num = 100;
        Vec3<float> accel_initial_data[data_num];
        for (uint32_t i = 0; i < data_num; ++i) {
                MPU6050_Read_NormAccel(mpu);
                accel_initial_data[i] = s.smooth(mpu->norm_a_axis);
        }
        return stdDev(accel_initial_data, data_num);
}

Vec3<float> calc_StdDevGyro(struct MPU6050 *mpu, uint32_t n)
{
        Exp_Smooth alpha35(0.35);
        Smoothie s(&alpha35);

        const uint32_t data_num = 100;
        Vec3<float> gyro_initial_data[data_num];
        for (uint32_t i = 0; i < data_num; ++i) {
                MPU6050_Read_NormGyro(mpu);
                gyro_initial_data[i] = s.smooth(mpu->norm_g_axis);
        }
        return stdDev(gyro_initial_data, data_num);
}
