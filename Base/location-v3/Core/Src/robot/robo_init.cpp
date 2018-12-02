/*
 * robo_init.cpp
 * 
 * Created : 11/26/2018
 *  Author : n-is
 *   email : 073bex422.nischal@pcampus.edu.np
 */

#include "robot/robo_init.h"

#define ENCODER_WHEEL_RADIUS    (35)
#define ENCODER_PPR             (250)

Vec3<float> gOmega_Bias;

Complementary_Filter gHigh_Lambda(0.85);
Complementary_Filter2 gDefault_K(7);

Kalman_Vars gAccel_Gyro;
Kalman_Vars gLidar_Encoder;
// Kalman_Vars gMag_Gyro;

Kalman_Filter gRoll_Filter(&gAccel_Gyro);
Kalman_Filter gPitch_Filter(&gAccel_Gyro);
Kalman_Filter gYaw_Filter(&gAccel_Gyro);
Kalman_Filter gXDist_Filter(&gLidar_Encoder);
Filter Second_Pass;

static void KF_Init(uint32_t dt_millis);

void robo_Init(uint32_t dt_millis)
{
        KF_Init(dt_millis);

        IMU_Init();
        Stepper_Init();
        Encoders_Init();
        LIDAR_Init();

        Wheeler_Init();
        gOmega_Bias = calc_OmegaMean(&Body_IMU, 1000);
        
        // Compass Calibration Code
        // HMC5883_Calibrate(&Body_HMC, &IMU_Stepper, 200);
}

static void KF_Init(uint32_t dt_millis)
{
        float dt = (float)dt_millis / 1000.0;

        // For Orientation Purpose
        Mat state_model(2,2);
        state_model.at(0,0) = 1;
        state_model.at(0,1) = -dt;
        state_model.at(1,0) = 0;
        state_model.at(1,1) = 1;

        Mat control_model(2,1);
        control_model.at(0,0) = dt;
        control_model.at(1,0) = 0;

        Mat obs_model(1,2);
        obs_model.at(0,0) = 1;
        obs_model.at(0,1) = 0;

        Mat eye(Mat::ident(2));

        Mat priori_error(2,2);
        priori_error.at(0,0) = 10;
        priori_error.at(0,1) = 0;
        priori_error.at(1,0) = 0;
        priori_error.at(1,1) = 10;

        Mat process_error(2,2);
        process_error.at(0,0) = 0.001;
        process_error.at(0,1) = 0;
        process_error.at(1,0) = 0;
        process_error.at(1,1) = 0.003;
        
        Mat measure_error(1,1);
        measure_error.at(0,0) = 0.04;

        gAccel_Gyro.set_F(state_model);
        gAccel_Gyro.set_B(control_model);
        gAccel_Gyro.set_H(obs_model);
        gAccel_Gyro.set_I(eye);
        gAccel_Gyro.set_P(priori_error);
        gAccel_Gyro.set_Q(process_error);
        gAccel_Gyro.set_R(measure_error);

        // For Positioning Purpose
        float dpc = 2 * 3.1415 * (float)(ENCODER_WHEEL_RADIUS) / (float)(ENCODER_PPR);

        state_model.at(0,0) = 1;
        state_model.at(0,1) = dpc;
        state_model.at(1,0) = 0;
        state_model.at(1,1) = 1;

        control_model.at(0,0) = dpc;
        control_model.at(1,0) = 0;

        obs_model.at(0,0) = 1;
        obs_model.at(0,1) = 0;

        priori_error.at(0,0) = 1000;
        priori_error.at(0,1) = 0;
        priori_error.at(1,0) = 0;
        priori_error.at(1,1) = 1000;

        process_error.at(0,0) = 0.0001;
        process_error.at(0,1) = 0;
        process_error.at(1,0) = 0;
        process_error.at(1,1) = 0.0003;

        measure_error.at(0,0) = 50;

        gLidar_Encoder.set_F(state_model);
        gLidar_Encoder.set_B(control_model);
        gLidar_Encoder.set_H(obs_model);
        gLidar_Encoder.set_I(eye);
        gLidar_Encoder.set_P(priori_error);
        gLidar_Encoder.set_Q(process_error);
        gLidar_Encoder.set_R(measure_error);
}