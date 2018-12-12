/*
 * play.cpp
 * 
 * Created : 11/9/2018
 *  Author : n-is
 *   email : 073bex422.nischal@pcampus.edu.np
 */

#include "devs/mpu6050.h"
#include "devs/hmc5883.h"
#include "devs/a4988.h"

#include "devs/lidar.h"
#include "devs/encoder.h"

void mpu_Test(struct MPU6050 *mpu)
{
        if (HAL_I2C_IsDeviceReady(mpu->hi2c, mpu->address, 1, 5)) {
                printf("MPU6050 is Ready..\n");
        }
}

void hmc_Test(struct HMC5883 *hmc)
{
        if (HAL_I2C_IsDeviceReady(hmc->hi2c, hmc->address, 1, 5)) {
                printf("HMC5883 is Ready..\n");
        }
}


