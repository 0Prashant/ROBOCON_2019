/*
 * globl_objects.cpp
 * 
 * Created : 11/30/2018
 *  Author : n-is
 *   email : 073bex422.nischal@pcampus.edu.np
 */

#include "devs/mpu6050.h"
#include "devs/hmc5883.h"
#include "devs/a4988.h"
#include "devs/encoder.h"

struct MPU6050 Body_IMU;
struct HMC5883 Body_HMC;
struct A4988 IMU_Stepper;

struct Enc gXEnc;
struct Enc gYEnc;
