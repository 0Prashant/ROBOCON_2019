/*
 * mpu6050.c
 * 
 * Created : 9/27/2018
 *  Author : n-is
 *   email : 073bex422.nischal@pcampus.edu.np
 */

#include "mpu6050.h"

#define I2C_TIMEOUT     (10)

static uint8_t i2c_buffer[8];

void MPU6050_Init(struct MPU6050 *mpu)
{
        i2c_buffer[0] = PWR_MGMT_1;        // PWR_MGMT_1
        i2c_buffer[1] = (0x00);
        HAL_I2C_Master_Transmit(mpu->hi2c, mpu->address, i2c_buffer, 2, 10);
        
        i2c_buffer[0] = SMPLRT_DIV;        // SMPLRT_DIV
        i2c_buffer[1] = 0x19;
        HAL_I2C_Master_Transmit(mpu->hi2c, mpu->address, i2c_buffer, 2, 10);
        
        i2c_buffer[0] = CONFIG;            // CONFIG
        i2c_buffer[1] = 0x00;
        HAL_I2C_Master_Transmit(mpu->hi2c, mpu->address, i2c_buffer, 2, 10);

        i2c_buffer[0] = GYRO_CONFIG;
        i2c_buffer[1] = (mpu->g_scale) << 3;
        HAL_I2C_Master_Transmit(mpu->hi2c, mpu->address, i2c_buffer, 2, I2C_TIMEOUT);

        i2c_buffer[0] = ACCEL_CONFIG;
        i2c_buffer[1] = (mpu->a_scale) << 3;
        HAL_I2C_Master_Transmit(mpu->hi2c, mpu->address, i2c_buffer, 2, I2C_TIMEOUT);

}

void MPU6050_Read_Accel(struct MPU6050 *mpu)
{
        i2c_buffer[0] = ACCEL_XOUT_H;   // Starting Address for Accelerometer data
        HAL_I2C_Master_Transmit(mpu->hi2c, mpu->address, i2c_buffer, 1, I2C_TIMEOUT);
        HAL_I2C_Master_Receive(mpu->hi2c, mpu->address, i2c_buffer, 6, I2C_TIMEOUT);
        mpu->accel_axis.x = (i2c_buffer[0] << 8) | i2c_buffer[1];
        mpu->accel_axis.y = (i2c_buffer[2] << 8) | i2c_buffer[3];
        mpu->accel_axis.z = (i2c_buffer[4] << 8) | i2c_buffer[5];
}

void MPU6050_Read_Gyro(struct MPU6050 *mpu)
{
        i2c_buffer[0] = GYRO_XOUT_H;   // Starting Address for gyro data
        HAL_I2C_Master_Transmit(mpu->hi2c, mpu->address, i2c_buffer, 1, I2C_TIMEOUT);
        HAL_I2C_Master_Receive(mpu->hi2c, mpu->address, i2c_buffer, 6, I2C_TIMEOUT);
        mpu->gyro_axis.x = (i2c_buffer[0] << 8) | i2c_buffer[1];
        mpu->gyro_axis.y = (i2c_buffer[2] << 8) | i2c_buffer[3];
        mpu->gyro_axis.z = (i2c_buffer[4] << 8) | i2c_buffer[5];
}

void MPU6050_Read_Temp(struct MPU6050 *mpu)
{
        int16_t tmpr;
        i2c_buffer[0] = TEMP_OUT_H;   // Starting Address for temperature data
        HAL_I2C_Master_Transmit(mpu->hi2c, mpu->address, i2c_buffer, 1, I2C_TIMEOUT);
        HAL_I2C_Master_Receive(mpu->hi2c, mpu->address, i2c_buffer, 2, I2C_TIMEOUT);
        
        tmpr = (i2c_buffer[0] << 8) | i2c_buffer[1];
        mpu->tmpr = (tmpr / 340.0) + 36.53;
}

void MPU6050_Read_Axes(struct MPU6050 *mpu)
{
        MPU6050_Read_Accel(mpu);
        MPU6050_Read_Gyro(mpu);
}
