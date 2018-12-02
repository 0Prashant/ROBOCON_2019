/*
 * i2c_utils.cpp
 * 
 * Created : 11/10/2018
 *  Author : n-is
 *   email : 073bex422.nischal@pcampus.edu.np
 */

#include "utils/i2c_utils.h"

//* This function prints all the available addresses in the specified I2C Bus.
void print_I2C_Addresses(I2C_HandleTypeDef * hi2c)
{
        uint8_t devs_found = 0;
        printf("Scanning...\n");
        for (uint8_t i = 0; i < 255; ++i) {
                if (HAL_I2C_IsDeviceReady(hi2c, i, 2, 10) == HAL_OK) {
                        ++devs_found;
                        printf("Address : 0x%X\n", i);
                        ++i;
                }
                HAL_Delay(10);
        }
        if (devs_found) {
                printf("%d devices detected", devs_found);
        }
        else {
                printf("No devices found");
        }
        printf(" on given I2C Bus\n");
}
