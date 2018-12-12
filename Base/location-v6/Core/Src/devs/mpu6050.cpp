/*
 * mpu6050.c
 * 
 * Created : 9/27/2018
 *  Author : n-is
 *   email : 073bex422.nischal@pcampus.edu.np
 */

// TODO: Test code in this file in actual hardware.


#include "mpu6050.h"

#define I2C_TIMEOUT     (10)

static uint8_t i2c_buffer[8];

static const uint16_t lsb_per_g[4] = { 16384, 8192, 4096, 2048 };
static const float lsb_per_dsp[4] = { 131.0, 65.5, 32.8, 16.4 };

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

void MPU6050_Read_RawAccel(struct MPU6050 *mpu)
{
        i2c_buffer[0] = ACCEL_XOUT_H;   // Starting Address for Accelerometer data
        HAL_I2C_Master_Transmit(mpu->hi2c, mpu->address, i2c_buffer, 1, I2C_TIMEOUT);
        HAL_I2C_Master_Receive(mpu->hi2c, mpu->address, i2c_buffer, 6, I2C_TIMEOUT);
        int16_t x = (i2c_buffer[0] << 8) | i2c_buffer[1];
        int16_t y = (i2c_buffer[2] << 8) | i2c_buffer[3];
        int16_t z = (i2c_buffer[4] << 8) | i2c_buffer[5];

        mpu->raw_a_axis.set_Values(x,y,z);
}

void MPU6050_Read_RawGyro(struct MPU6050 *mpu)
{
        i2c_buffer[0] = GYRO_XOUT_H;   // Starting Address for gyro data
        HAL_I2C_Master_Transmit(mpu->hi2c, mpu->address, i2c_buffer, 1, I2C_TIMEOUT);
        HAL_I2C_Master_Receive(mpu->hi2c, mpu->address, i2c_buffer, 6, I2C_TIMEOUT);
        int16_t x = (i2c_buffer[0] << 8) | i2c_buffer[1];
        int16_t y = (i2c_buffer[2] << 8) | i2c_buffer[3];
        int16_t z = (i2c_buffer[4] << 8) | i2c_buffer[5];
        
        mpu->raw_g_axis.set_Values(x,y,z);
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

void MPU6050_Read_RawAxes(struct MPU6050 *mpu)
{
        MPU6050_Read_RawAccel(mpu);
        MPU6050_Read_RawGyro(mpu);
}

void MPU6050_Read_NormAccel(struct MPU6050 *mpu)
{
        MPU6050_Read_RawAccel(mpu);
        (mpu->norm_a_axis).divide(mpu->raw_a_axis, (float)lsb_per_g[mpu->a_scale]);
}


void MPU6050_Read_NormGyro(struct MPU6050 *mpu)
{
        MPU6050_Read_RawGyro(mpu);
        (mpu->norm_g_axis).divide(mpu->raw_g_axis, (float)lsb_per_dsp[mpu->g_scale]);
}

void MPU6050_Read_NormAxes(struct MPU6050 *mpu)
{
        MPU6050_Read_NormAccel(mpu);
        MPU6050_Read_NormGyro(mpu);
}
